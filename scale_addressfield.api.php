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
 *   An array, matching the format of config/address-formats.json. It contains,
 *   roughly, the following elements:
 *   - label: The label used for the "Country" field, upon which all dynamic
 *     behavior is based.
 *   - options: An associative array mapping field configurations to their
 *     respective ISO country codes. Each field configuration can consist of any
 *     number of associative arrays, representing fields, whose keys are the
 *     xNAL field names used by the address field module.
 */
function hook_addressfield_config_alter(&$config) {
  // You might want to update the label for the country field itself.
  // Note: Do not run these through t(), they are translated via i18n
  $config['label'] = 'Country/Region';

  // Perhaps you don't want to display a certain field for a certain country.
  unset($config['options']['CA']['locality']['postalcode']);

  // For full details on structure, see the default address configuration file,
  // located at config/address-formats.json.
}


/**
 * @}
 */
