/**
 * @file
 * Provides js for tabs
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands larowlan at previousnext dot com dot au
 */


(function ($) {
/**
 * JS related to the tabs.
 */
Drupal.behaviors.siResponsiveTabs = {
  attach: function (context, settings) {
		$.each(settings.siResponsiveTabs.tabIDs, function( index, value ) {
    		var tab = $('#' + index, context);
				if (value !== 'accordion_tab') {
					tab.responsiveTabs({
							rotate: false,
	            startCollapsed: 'accordion',
              collapsible: 'accordion',
							//animation: 'fade'
					});
				}
				tab.once('tabs-processed');
		});
  }
};

})(jQuery);


