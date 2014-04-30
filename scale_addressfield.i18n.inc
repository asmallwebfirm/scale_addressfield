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
    'list' => TRUE,
  );
  return $groups;
}

/**
 * Implements hook_i18n_string_list().
 */
function scale_addressfield_i18n_string_list($group) {
  if ($group == 'addressfield' || $group == 'all') {
    $strings = array();

    $configs = scale_addressfield_get_address_configuration('php', NULL, TRUE);

    // Global, top-level configuration.
    $strings['addressfield']['form']['country']['label'] = $configs['label'];

    // Iterate through all countries and their configurations.
    foreach ($configs['options'] as $country => $config) {
      // Translate the country name.
      if (isset($config['label'])) {
        $strings['addressfield']['form'][$country]['label'] = $config['label'];
      }

      // Iterate through all fields, check for labels.
      foreach ($config as $field_name => $definition) {
        // Translate field name labels.
        if (is_array($definition) && isset($definition['label'])) {
          // These may be duplicated significantly, so we try to cut down on them.
          $label = $definition['label'];
          $strings['addressfield']['form'][$field_name][$label] = $label;
        }

        // If the field has options, translate their user-facing strings.
        if (isset($definition['options'])) {
          $strings['addressfield']['form'][$country][$field_name]['options'] = $definition['options'];
        }
      }
    }

    return $strings;
  }
}

/**
 * Implements hook_i18n_string_refresh().
 *
 * Refresh translations for all generated strings.
 */
function scale_addressfield_i18n_string_refresh($group) {
  if ($group == 'addressfield') {
    scale_addressfield_i18n_update_strings();
  }
  return TRUE;
}
