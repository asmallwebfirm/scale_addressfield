<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard Drupal
 * manner.
 */

/**
 * @defgroup scale_addressfield Scale Address Field hooks.
 */


/**
 * Allows you to modify address field format configurations before they are used
 * to dynamically render the address field, and before strings are localized.
 *
 * @param array $config
 *   An array, matching the format of json/addressfield.json. It contains,
 *   roughly, the following elements:
 *   - label: The label used for the "Country" field, upon which all dynamic
 *     behavior is based.
 *   - options: An array containing country configuration objects, including
 *     country names/labels, respective ISO country codes, and field configs.
 *     For full details, see the JSON configuration referenced above.
 */
function hook_addressfield_config_pre_alter(&$config) {
  // You might want to update the label for the country field itself.
  // Note: Do not run these through t(), they are translated via i18n
  $config['label'] = 'Country/Region';

  // For full details on structure, see the default address configuration file,
  // located at json/addressfield.json.
}

/**
 * Allows you to modify address field format configurations before they are used
 * to dynamically render the address field, but AFTER strings are localized and
 * processed.
 *
 * @param array $config
 *   @see hook_addressfield_config_pre_alter().
 */
function hook_addressfield_config_post_alter(&$config) {

}


/**
 * @}
 */
