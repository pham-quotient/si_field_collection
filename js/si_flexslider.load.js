(function($) {
	//var breakpoint = 'default';
	if ($('body[class*=breakpoint]').length == 0) {
		$(window).setBreakpoints({
			breakpoints: [
					320,
					720,
					980
			] 	
		});		
	}
	
			
  // Behavior to load FlexSlider
  Drupal.behaviors.si_flexslider = {
    attach: function(context, settings) {
			$(window).bind('enterBreakpoint320',function() {
					 _si_flexslider_init('mobile', context, settings);
				});
			$(window).bind('enterBreakpoint720',function() {
				_si_flexslider_init('mobile', context, settings);
			});	
			$(window).bind('enterBreakpoint980',function() {
				_si_flexslider_init('desk', context, settings);
			});	

    }
  };

  /**
   * Set appropriate settings for flexslider
   */
  function _si_flexslider_init(breakpoint, context, settings) {
		var sliders = [],
				optionset = '';
		if ($.type(settings.si_flexslider) !== 'undefined' && $.type(settings.si_flexslider.instances) !== 'undefined') {

			for (id in settings.si_flexslider.instances) {
				var direction = $.type(settings.si_flexslider.directions[id]) == 'undefined' ? 'horizontal' :  
												settings.si_flexslider.directions[id];
				if (settings.si_flexslider.optionsets[settings.si_flexslider.instances[id]] !== undefined) {
					if (settings.si_flexslider.optionsets[settings.si_flexslider.instances[id]].asNavFor !== '') {
						// We have to initialize all the sliders which are "asNavFor" first.
						optionset = breakpoint == 'mobile' && direction =='vertical' ? settings.si_flexslider.optionsets['default'] :  settings.si_flexslider.optionsets[settings.si_flexslider.instances[id]];
						_si_flexslider_start(id, optionset, context, settings);
					} else {
						// Everyone else is second
						sliders[id] = settings.si_flexslider.optionsets[settings.si_flexslider.instances[id]];
					}
				}
			}
		}
		// Slider set
		for (id in sliders) {
//			var direction = $.type(settings.si_flexslider.directions[id]) == 'undefined' ? 'horizontal' :  
//						settings.si_flexslider.directions[id];
			optionset = breakpoint == 'mobile' && direction =='vertical' ? settings.si_flexslider.optionsets['default'] :  settings.si_flexslider.optionsets[settings.si_flexslider.instances[id]];		
			_si_flexslider_start(id, optionset,  context, settings);
		}
		$('.flex-custom-nav .read-more').click(function(){
			var href = $(this).children('a').attr('href');
  		console.log(href);
			window.location = href;
		}); 	
	}
  /**
   * Initialize the flexslider instance
   */
  function _si_flexslider_start(id, optionset, context, settings) {
    $('#' + id, context).once('flexslider', function() {
      // Remove width/height attributes
      // @todo load the css path from the settings
      $(this).find('ul.slides > li > *').removeAttr('width').removeAttr('height');

      if (optionset) {
        // Add events that developers can use to interact.
        $(this).flexslider($.extend(optionset, {
          start: function(slider) {
            slider.trigger('start');
          },
          before: function(slider) {
            slider.trigger('before');
          },
          after: function(slider) {
            slider.trigger('after');
          },
          end: function(slider) {
            slider.trigger('end');
          },
          added: function(slider) {
            slider.trigger('added');
          },
          removed: function(slider) {
            slider.trigger('removed');
          }
        }));
      } else {
        $(this).flexslider();
      }
    });
  }

}(jQuery));
