(function($, D, w) {

  // Define some globally relevant methods, namespaced on the Drupal object.
  D.scale_addressfield = {

    /**
     * Runs through and asynchronously pings all configured forms, triggering a
     * "pinged" event against the form. The pinged event takes a single
     * argument, representing the form_build_id that should be associated with
     * the form.
     */
    diagnose: function() {
      $.each(D.settings.scale_addressfield.enabled_forms, function(index, value) {
        var build_id = $('input[name="form_id"][value="' + value + '"]').siblings('input[name="form_build_id"]').val();
        var form_selector_id = $('input[name="form_id"][value="' + value + '"]').closest('form').attr('id');
        $.ajax(D.settings.scale_addressfield.callback + '?fid=' + form_selector_id + '&fbid=' + build_id, {
          success: function(response, status) {
            // Sanity check for browser support (object expected).
            // When using iFrame uploads, responses must be returned as a string.
            if (typeof response === 'string') {
              response = $.parseJSON(response);
            }

            // Run through all returned commands.
            for (var i in response) {
              // We only care about the "invoke" command.
              if (response.hasOwnProperty(i) && response[i]['command'] && response[i]['command'] === 'invoke') {
                var $element = $(response[i].selector);
                $element[response[i].method].apply($element, response[i].arguments);
              }
            }
          }
        });
      });
    },

    /**
     * Applies a new (theoretically valid) form_build_id over the old (probably
     * invalid) form_build_id.
     */
    heal: function(form_selector_id, form_build_id) {
      $('#' + form_selector_id + ' input[name="form_build_id"]').val(form_build_id);
    }
  };

  /**
   * Primary "application" logic.
   */
  $(document).ready(function() {
    // Iterate through all enabled forms, attach event handlers to each form.
    $.each(D.settings.scale_addressfield.enabled_forms, function(index, value) {
      var form_selector_id = $('input[name="form_id"][value="' + value + '"]').closest('form').attr('id');
      // Bind an event handler to the "pinged" event against this form.
      $('#' + form_selector_id).bind('pinged', function(event, form_build_id) {
        // Ensure that the form_build_id passed at least seems valid.
        if (typeof form_build_id === 'string') {
          // React on form staleness.
          D.scale_addressfield.heal(form_selector_id, form_build_id);
        }
      });
    });

    // Invoke the ping command once on load.
    D.scale_addressfield.diagnose();

    // Set the command to be invoked once per defined interval.
    w.setInterval(D.scale_addressfield.diagnose, D.settings.scale_addressfield.interval * 1000);
  });

})(jQuery, Drupal, window);
