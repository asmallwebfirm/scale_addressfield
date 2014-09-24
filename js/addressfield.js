(function($, Drupal) {
  var originalConvertToSelect = $.fn.addressfield.convertToSelect,
      originalConvertToText = $.fn.addressfield.convertToText;

  /**
   * Override the provided method to ensure Drupal's special text/select classes
   * are applied/replaced properly.
   */
  $.fn.addressfield.convertToSelect = function () {
    $(this).attr('class', $(this).attr('class').replace('form-text', 'form-select'));
    return originalConvertToSelect.call(this);
  };

  /**
   * Override the provided method to ensure Drupal's special text/select classes
   * are applied/replaced properly.
   */
  $.fn.addressfield.convertToText = function () {
    $(this).attr('class', $(this).attr('class').replace('form-select', 'form-text'));
    return originalConvertToText.call(this);
  };

  /**
   * Override the provided label update method to take Drupal's "required"
   * asterisks into account.
   */
  $.fn.addressfield.updateLabel = function (label) {
    var fieldId = $(this).attr('id'),
        $label = $('label[for="' + fieldId + '"]').not('.error'),
        old_label = $label.contents().filter(function() {
        return this.nodeType == 3;
      }).text(),
      old_markup = $label.html();

    if ($label.length) {
      $label.html(old_markup.replace(old_label, label + ' '));
    }
  };

  /**
   * Override the placeholder method to localize the placeholder text.
   */
  $.fn.addressfield.updateEg = function (example) {
    var text = example ? Drupal.t('e.g. @example', {'@example': example}) : '';
    $(this).attr('placeholder', text);
  };

  /**
   * On ready, asynchronously load the address field conig JSON and apply it.
   */
  $(document).ready(function() {
    var i18nMessage = Drupal.t("Please check your formatting."),
        messageField;

    // If jQuery.validate is installed, override jquery.addressfield's default
    // validation messages with a localized string.
    if (typeof $.validator !== 'undefined') {
      for (messageField in Drupal.settings.scale_addressfield.enabled) {
        $.validator.messages['isValid_' + messageField] = i18nMessage;
      }
    }

    // Return our configured JSON file and instantiate jQuery address field.
    $.getJSON(Drupal.settings.scale_addressfield.config_json, function (data) {
      var wrapper,
          country,
          position;

      // Set the data on the Drupal.settings object for later use.
      Drupal.settings.scale_addressfield.config = data;

      // Store a map of countries to their position in the config array.
      Drupal.settings.scale_addressfield.config_map = {};
      for (position in data.options) {
        Drupal.settings.scale_addressfield.config_map[data.options[position].iso] = position;
      }

      // Iterate through all enabled forms.
      for (wrapper in Drupal.settings.scale_addressfield.enabled) {
        // Attach listeners.
        $('#' + wrapper + ' .country').bind('change', function () {
          var config,
              fields;

          if (typeof Drupal.settings.scale_addressfield.hasOwnProperty('config')) {
            position = Drupal.settings.scale_addressfield.config_map[this.value];
            config = Drupal.settings.scale_addressfield.config.options[position];
            fields = Drupal.settings.scale_addressfield.enabled[wrapper];

            if (config.hasOwnProperty('fields')) {
              $('#' + wrapper).addressfield(config, fields);
            }
          }
        });

        // During the initial load/ready, also run an initial addressfield
        // country change in order to ensure proper configuration.
        country = $('#' + wrapper + ' .country').val();
        position = Drupal.settings.scale_addressfield.config_map[country];
        $('#' + wrapper).addressfield(Drupal.settings.scale_addressfield.config.options[position], Drupal.settings.scale_addressfield.enabled[wrapper]);
      }

      // Trigger an event, signaling addressfield functionality initialization.
      $(document).trigger('scale_addressfield:initialized');
    });
  });
})(jQuery, Drupal);
