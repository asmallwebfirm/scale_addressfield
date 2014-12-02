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
   * On ready, asynchronously load the address field config JSON and apply it.
   */
  $(document).ready(function() {
    // Return our configured JSON file and instantiate jQuery address field.
    $.getJSON(Drupal.settings.scale_addressfield.config_json, function (data) {
      var validationInstalled = typeof $.validator !== 'undefined',
          i18nMessage = Drupal.t("Please check your formatting."),
          messagePos,
          wrapper;

      // Set the data on the Drupal.settings object for later use.
      Drupal.settings.scale_addressfield.config = data;

      // Iterate through all enabled forms.
      for (wrapper in Drupal.settings.scale_addressfield.enabled) {
        // Initialize jQuery.addressfield for this wrapper.
        $('#' + wrapper).addressfield({
          json: data,
          fields: Drupal.settings.scale_addressfield.enabled[wrapper]
        });

        // If jQuery.validate is installed, override jquery.addressfield's
        // default validation messages with a localized string.
        if (validationInstalled) {
          for (messagePos in Drupal.settings.scale_addressfield.enabled[wrapper]) {
            $.validator.messages['isValid_' + Drupal.settings.scale_addressfield.enabled[wrapper][messagePos]] = i18nMessage;
          }
        }
      }

      // Trigger an event, signaling addressfield functionality initialization.
      $(document).trigger('scale_addressfield:initialized');
    });
  });
})(jQuery, Drupal);
