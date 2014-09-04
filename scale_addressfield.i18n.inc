<?php
/**
 * @file
 * Internationalization (i18n) hooks.
 */

/**
 * Implements hook_i18n_string_info()
 */
function scale_addressfield_i18n_string_info() {
  // These really belong in Address Field; let's declare on their behalf.
  $groups['addressfield'] = array(
    'title' => t('Address field'),
    'description' => t('Address field related strings.'),
    // This group doesn't have strings with format.
    'format' => FALSE,
    // This group can list all strings.
    'list' => FALSE,
  );
  return $groups;
}

/**
 * Implements hook_i18n_string_refresh().
 *
 * Refresh translations for all generated strings.
 */
function scale_addressfield_i18n_string_refresh($group) {
  if ($group == 'addressfield') {
    scale_addressfield_update_i18n_strings();
  }
  return TRUE;
}

/**
 * Parse and update all user-facing, translatable strings.
 *
 * @see scale_addressfield_i18n_string_refresh()
 */
function scale_addressfield_update_i18n_strings() {
  $configs = scale_addressfield_get_address_configuration('php', language_default('language'), TRUE);

  // Global, top-level configuration.
  $context = scale_addressfield_i18n_context(array('country', 'label'));
  i18n_string_update($context, $configs['label']);

  // Iterate through all countries and their configurations.
  foreach ($configs['options'] as $country) {
    // Translate the country name.
    $context = scale_addressfield_i18n_context(array('country', 'options', $country['iso'], 'label'));
    i18n_string_update($context, $country['label']);

    // Iterate through all fields, check for labels.
    foreach ($country['fields'] as $field) {
      $xnal = key($field);

      // Translate field name labels.
      if (isset($field[$xnal]['label'])) {
        // These may be duplicated significantly, so we try to cut down on them.
        $label = $field[$xnal]['label'];
        $context = scale_addressfield_i18n_context(array('country', 'options', $country['iso'], $xnal, 'label'));
        i18n_string_update($context, $label);
      }
      // In this case, we're dealing with sub-fields (e.g. within locality).
      else {
        foreach ($field[$xnal] as $subfield) {
          $subxnal = key($subfield);

          // Translate field name labels.
          if (isset($subfield[$subxnal]['label'])) {
            // These may be duplicated significantly, so we try to cut down on them.
            $label = $subfield[$subxnal]['label'];
            $context = scale_addressfield_i18n_context(array('country', 'options', $country['iso'], $xnal, $subxnal, 'label'));
            i18n_string_update($context, $label);
          }

          // If the field has options, translate their user-facing strings.
          if (isset($subfield[$subxnal]['options'])) {
            foreach ($subfield[$subxnal]['options'] as $option) {
              $key = key($option);

              if (!empty($key)) {
                $context = scale_addressfield_i18n_context(array('country', 'options', $country['iso'], $xnal, $subxnal, 'options', $key));
                i18n_string_update($context, $option[$key]);
              }
            }
          }
        }
      }
    }
  }
}
