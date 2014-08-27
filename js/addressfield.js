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
  }

  /**
   * Override the provided label update method to take Drupal's "required"
   * asterisks into account.
   */
  $.fn.addressfield.updateLabel = function (label) {
    var fieldId = $(this).attr('id'),
        $label = $('label[for="' + fieldId + '"]'),
        old_label = $label.contents().filter(function() {
        return this.nodeType == 3;
      }).text(),
      old_markup = $label.html();

    if ($label.length) {
      $label.html(old_markup.replace(old_label, label + ' '));
    }
  }

  /**
   * On ready, asynchronously load the address field conig JSON and apply it.
   */
  $(document).ready(function() {
    $.getJSON(Drupal.settings.scale_addressfield.config_json, function (data) {
      var wrapper,
          country;

      // Set the data on the Drupal.settings object for later use.
      Drupal.settings.scale_addressfield.config = data;

      // Iterate through all enabled forms.
      for (wrapper in Drupal.settings.scale_addressfield.enabled) {
        // Attach listeners.
        $('#' + wrapper + ' .country').bind('change', function () {
          var config,
              fields;

          if (typeof Drupal.settings.scale_addressfield.hasOwnProperty('config')) {
            config = Drupal.settings.scale_addressfield.config.options[this.value];
            fields = Drupal.settings.scale_addressfield.enabled[wrapper];

            $('#' + wrapper).addressfield(config, fields);
          }
        });

        // During the initial load/ready, also run an initial addressfield
        // country change in order to ensure proper configuration.
        country = $('#' + wrapper + ' .country').val();
        $('#' + wrapper).addressfield(Drupal.settings.scale_addressfield.config.options[country], Drupal.settings.scale_addressfield.enabled[wrapper]);
      }
    });
  });
})(jQuery, Drupal);
