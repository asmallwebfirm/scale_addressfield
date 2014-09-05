<?php

/**
 * @file
 * Functions specific to localization for the Scale Address Field module.
 */


/**
 * Helper function to get an i18n string context, given an array of strings to
 * append to the base context.
 *
 * @param array $append
 *   An array of additional context keys to append.
 *
 * @return string
 *   A string, suitable for use as a context in i18n_string_update().
 */
function scale_addressfield_i18n_context($append) {
  $i18n_context = array('addressfield', 'form');
  return implode(':', array_merge($i18n_context, $append));
}

/**
 * Returns a translated address field string, given the string and some context.
 *
 * @param string $context
 *   The "context" of a given address field string.
 *
 * @param string $string
 *   The string to be translated.
 *
 * @param string $langcode
 *   (Optional) The language code of the translation target language. If none is
 *   provided, it will default to the global scope language.
 *
 * @return string
 *   The translated version of $string, or just $string if none is available.
 */
function scale_addressfield_translate($context, $string, $langcode = NULL) {
  return function_exists('i18n_string') ? i18n_string($context, $string, array('langcode' => $langcode)) : $string;
}

/**
 * Localizes a given, individual country config into the specified language.
 *
 * @param array $config
 *   The PHP representation (an array) of a country configuration.
 *
 * @param string $language
 *   The language code representing the target language for localization.
 */
function scale_addressfield_localize_country_config(&$config, $language) {
  // No need to waste cycles localizing if i18n string isn't even available.
  if (function_exists('i18n_string')) {
    // Country label.
    $context = scale_addressfield_i18n_context(array('country', 'options', $config['iso'], 'label'));
    $config['label'] = scale_addressfield_translate($context, $config['label'], $language);

    // Iterate through all fields, check for labels.
    foreach ($config['fields'] as $key => &$field) {
      $context = array('country', 'options', $config['iso']);
      scale_addressfield_localize_field_config($field, $context, $language);
    }
  }
}

/**
 * Localizes a given, individual field config into the specified language, given
 * a specified i18n string context array.
 *
 * @param array $config
 *   The PHP representation (an array) of a field configuration.
 *
 * @param array $context
 *   An array representing the i18n string context for this field.
 *
 * @param string $language
 *   The language code representing the target language for localization.
 */
function scale_addressfield_localize_field_config(&$config, $context, $language) {
  $xnal = key($config);

  // Translate field name labels.
  if (isset($config[$xnal]['label'])) {
    // Prepare the i18n string context.
    $this_context = $context;
    array_push($this_context, $xnal, 'label');
    $this_context = scale_addressfield_i18n_context($this_context);

    // Localize the label with the given context.
    $config[$xnal]['label'] = scale_addressfield_translate($this_context, $config[$xnal]['label'], $language);
  }

  // Translate field option sets.
  if (isset($config[$xnal]['options'])) {
    // Prepare the i18n string context.
    $this_context = $context;
    array_push($this_context, $xnal, 'options');

    // Localize the options with the given context.
    scale_addressfield_localize_options_config($config[$xnal]['options'], $this_context, $language);
  }

  // Special case for the "locality" fieldset... Go one deeper.
  if ($xnal === 'locality') {
    // Prepare the i18n string context.
    $this_context = $context;
    array_push($this_context, $xnal);

    // Iterate through each locality subfield and localize its configuration.
    foreach ($config[$xnal] as &$field) {
      scale_addressfield_localize_field_config($field, $this_context, $language);
    }
  }
}

/**
 * Localizes a given, individual option config into the specified language,
 * given a specified i18n string context array.
 *
 * @param array $config
 *   The PHP representation (an array) of an option list.
 *
 * @param array $context
 *   An array representing the i18n string context for this option list.
 *
 * @param string $language
 *   The language code representing the target language for localization.
 */
function scale_addressfield_localize_options_config(&$config, $context, $language) {
  foreach ($config as &$option) {
    // Do not "translate" the empty select value. This is handled elsewhere.
    if ($code = key($option)) {
      // Prepare the i18n string context.
      $this_context = $context;
      array_push($this_context, $code);
      $this_context = scale_addressfield_i18n_context($this_context);

      // Translate the option configuration.
      $option[$code] = scale_addressfield_translate($this_context, $option[$code], $language);
    }
  }
}

/**
 * Sort comparison function used to sort countries in natural sort order for the
 * current language.
 *
 * @param array $a
 *   The first country configuration array for the comparator. Should contain a
 *   "label" key.
 *
 * @param array $b
 *   The second country configuration array for the comparator. Should contain a
 *   "label" key.
 *
 * @return int
 * @see scale_addressfield_process_country_configuration().
 */
function scale_addressfield_localize_country_sort($a, $b) {
  static $collator = NULL;

  if ($collator === NULL) {
    $collator = collator_create(scale_addressfield_current_locale());
  }

  return $collator->compare($a['label'], $b['label']);
}

/**
 * Gets/sets the current global locale state...  Just a static function that's
 * used to hold onto global scope locale...
 *
 * @param string $locale
 *   (Optional) A string representing the current locale to be set.
 *
 * @return string
 *   Returns a string representing the current configured locale.
 *
 * @see scale_addressfield_localize_country_sort()
 * @see scale_addressfield_process_country_configuration()
 */
function scale_addressfield_current_locale($locale = NULL) {
  $static_locale = &drupal_static(__FUNCTION__, $GLOBALS['language']->language);

  if ($locale !== NULL) {
    $static_locale = $locale;
  }

  return $static_locale;
}
