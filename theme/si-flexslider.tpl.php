<?php
/**
 * @file
 * Default output for a FlexSlider object.
*/
?>
<div class="<?php print($container_class) ?>">
  <div <?php print drupal_attributes($settings['attributes'])?>>
    <?php print theme('flexslider_list', array('items' => $items, 'settings' => $settings)); ?>
      
  </div>
  <?php if ($titles): ?>
    <?php print($titles) ?>
	<?php endif; ?>
  <div class="flex-control-nav-container">
  </div>
</div>