<?php
/**
 * @file
 * Theming functions for the si_field_collection module.
 *
 * Preprocessor functions fill variables for templates and helper
 * functions to make theming easier.
 */

/**
 * Template preprocess handler for 'si_flexslider' theme.
 */
function template_process_si_flexslider(&$vars) {

  // Reference configuration variables
  $optionset = &$vars['settings']['optionset'];
  $settings = &$vars['settings'];
  $items = &$vars['items'];
	$titles = array();
	$vars['titles'] = '';
	if ($settings['direction'] == 'vertical') {
		foreach($items as $delta => $item) {
			if (isset($item['caption'])) {
				$titles[$delta] = $item['caption'];
				unset($items[$delta]['caption']);
			}
		}
	}
	$vars['titles'] = theme('item_list', array(
													'items' => $titles, 
													'attributes' => array(
														'class' => array('flex-custom-nav')
														)));
  // Set the default container type
  if (empty($settings['type'])) {
    $settings['type'] = 'ul';
  }

  // Load the selected optionset
  if (!empty($optionset)) {
    $optionset = flexslider_optionset_load($optionset);
  }

  // Check if an optionset was loaded
  if (empty($optionset)) {
    // Fall back to 'default' option set
    $optionset = flexslider_optionset_load('default');
    watchdog('flexslider', 'Fallback to default optionset.', array(), WATCHDOG_WARNING);
  }

  // Configure attributes for containing elements
  $attributes = array();
  // Merge with defined attributes
  if (isset($settings['attributes']) and is_array($settings['attributes'])) {
    $attributes += $settings['attributes'];
  }

  // Set the ID for each flexslider instance if none is provided
  if (empty($attributes['id'])) {
    $flexslider_id = &drupal_static('flexslider_id', 0);
    $attributes['id'] = 'flexslider-' . ++$flexslider_id;
  }

  // Add the namespace to any classes
  // @todo figure out what this is supposed to do
  if (!empty($attributes['class']) && !empty($optionset->options['namespace'])) {
    foreach ($attributes['class'] as $key => $value) {
      $attributes['class'][$key] = $optionset->options['namespace'] . $value;
    }
  }
	$direction = empty($settings['direction']) ? 'horizontal' : $settings['direction'];
  // Add the flexslider class to be namespaced
  $attributes['class'][] = 'flexslider';
  $attributes['class'][] = empty($settings['caption_position']) ? '' :  drupal_clean_css_identifier($settings['caption_position']);
  $vars['container_class'] = 'flexslider-container ' . drupal_clean_css_identifier($direction);

  // Add the attributes to the settings array.
  $settings['attributes'] = $attributes;

  // Finally, add the configuration to the page
  si_flexslider_add($vars['settings']['attributes']['id'], $vars['settings']['optionset'], $direction);
}

function si_flexslider_add($id = NULL, $optionset = NULL, $direction) {
	$js_settings = array();
  // Check optionset value
  if (is_string($optionset)) {
    $name = $optionset;
    $optionset = flexslider_optionset_load($name);
  }

  // Static array to remember which scripts are already attached.
  // @todo not currently in use
  $cache = &drupal_static(__FUNCTION__, array());

  libraries_load('flexslider');

  // If the ID or optionset aren't set, it is assumed the settings will be set
  // manually via the calling module/theme
  if (!empty($optionset) && !in_array($optionset->name, $cache)) {
    $cache[] = $optionset->name;
	  $js_settings['optionsets'] = array(
      $optionset->name => $optionset->options,
    );
  }
  if ($direction == 'vertical' && !in_array('default', $cache)) {
		 $defaultset = flexslider_optionset_load('default');
		$js_settings['optionsets']['default'] = $defaultset->options;
		$cache[] = 'default';
	}
  if (!empty($id)) {
    // JavaScript settings
    $js_settings['instances'] = array(
        $id => $optionset->name,
    
    );
		$js_settings['directions'] = array(
        $id => $direction,
    );
    // @todo add alter hook for optionset
    drupal_add_js(array('si_flexslider' => $js_settings), 'setting');
  }
  // Loader JavaScript
  drupal_add_js(drupal_get_path('module', 'si_field_collection') . '/js/si_flexslider.load.js', array('type' => 'file', 'scope' => 'footer'));
}

/**
 * Default implementation of theme_si_field_collection_tabs().
 */
function theme_si_field_collection_accordion($variables) {
	libraries_load('bootstrap');
  $tabs = array();
	$tab = '';
  foreach ($variables['titles'] as $delta => $title) {
		$expand = $delta == 0 && !empty($variables['open_tab']) ? TRUE : FALSE; 
    $tab_id = drupal_clean_css_identifier($variables['tabID'] . '-' . $delta);
		$tab = '<div class="panel" id="accordion-'. $tab_id .'">';

    $tab .= theme('si_field_collection_tabs_tab_title', array(
      'title' => $title,
      'tab_id' => $tab_id,
			'tabMode' => 'accordion_tab',
			'open_tab' => $expand ? 'true' : 'false',
    ));
		$tab .= theme('si_field_collection_tabs_tab', array(
      'tab' => $variables['tabs'][$delta],
      'tab_id' => $tab_id,
			'tabMode' => 'accordion_tab',
			'open_tab' => $expand ? 'in' : '',

    ));
		$tab .='</div>';
    $tabs[$delta] = $tab; 
  }
  return  implode('', $tabs);
}

function theme_si_field_collection_tabs($variables) {
  $titles = $tabs = array();
	$out = '';
  foreach ($variables['titles'] as $delta => $title) {
		
    $tab_id = drupal_clean_css_identifier($variables['tabID'] . '-' . $delta);
		$item = array(
			'data' => theme('si_field_collection_tabs_tab_title', array(
      'title' => $title,
      'tab_id' => $tab_id)),
			'tabMode' => $variables['tabMode'],
			'class' => array('tab-title'),
		);
    $titles[] = $item;
    $tabs[] = theme('si_field_collection_tabs_tab', array(
      'tab' => $variables['tabs'][$delta],
      'tab_id' => $tab_id,
      'title' => $title,
			'tabMode' => $variables['tabMode'],
    ));
  }
	$out .= theme('item_list', array('items' => $titles));
	$out .= '<div class="tab-wrapper">';
	$out .= implode('', $tabs);
	$out .= '</div>';
  return  $out;
}

/**
 * Default implementation of theme_si_field_collection_tabs_tab_title().
 */
function theme_si_field_collection_tabs_tab_title($variables) {
	switch($variables['tabMode']) {
		case 'accordion_tab':
	
			//$expanded = empty($variables['open_tab']) ? 'false' : 'true';
			$link = '<a href="#' . $variables['tab_id'] . '" role="button" data-toggle="collapse" data-parent="#accordion-'. $variables['tab_id'] .'"
							aria-expanded="'. $variables['open_tab'] .'" aria-controls="'.  $variables['tab_id'] .'">' . t($variables['title']) . '</a>';
			 $div = array(
				'element' => array(
				'#tag' => 'h3',
				'#value' => $link,
				'#attributes' => array(
					'class' => array('panel-heading'),
					'role' => 'tab',
					'id' => 'header-'. $variables['tab_id']
					),
				)
			);
			return theme('html_tag', $div);
 	
		break;
		default:
			return '<a href="#' . $variables['tab_id'] . '">' . t($variables['title']) . '</a>';
		break;
	}
	
}

/**
 * Default implementation of theme_si_field_collection_tabs_tab().
 */
function theme_si_field_collection_tabs_tab($variables) {
	switch($variables['tabMode']) {
		case 'menu_tab':	
			$attributes = array(
	 			'id' => $variables['tab_id'],
			 	'class' => array('tab-pane'),
			);	
			$content = '<h2 class="tab-content-title"><a name="'. $variables['tab_id'] .'">'. $variables['title'] .'</a></h2>' . $variables['tab'];
		break;
		case 'responsive_tab':
			$attributes = array(
	 			'id' => $variables['tab_id'],
			 	'class' => array('tab-pane'),
			);	
			//$content = '<a name="'. $variables['tab_id'] .'">'. $variables['title'] .'</a></h2>' . $variables['tab'];
			$content = $variables['tab'];
		break;
		case 'accordion_tab':
			$attributes = array(
	 			'id' => $variables['tab_id'],
				'class' => array('tab-pane','panel-collapse', 'collapse'),
			);
			$attributes['class'][] = $variables['open_tab'];
			$content = $variables['tab'];
		break;
	}
	$div = array(
		'element' => array(
			'#tag' => 'div',
			'#value' => $content,
			'#attributes' => $attributes,
		)
	);
	return theme('html_tag', $div);	

}
