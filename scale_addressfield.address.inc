<?php

/**
 * @file
 * Contains the address field formatter override that replaces the standard
 * country-specific address field formatter provided by Addressfield.
 */

/**
 * Takes over Addressfield's default formatter; replaces the existing AJAX
 * implementation with a client-side-only, JSON-configured plugin.
 */
function scale_addressfield_override_format(&$format, $address, $context = array()) {
  // Load and call the original plugin.
  module_load_include('inc', 'addressfield', '/plugins/format/address');
  addressfield_format_address_generate($format, $address, $context);

  // Our customizations.
  if ($context['mode'] == 'form') {
    // Ensure that a postal code and administrative area fields always exist. We
    // do this because the field removal/addition is taken care of client-side.
    $format['locality_block']['administrative_area'] = array(
      '#title' => t('State'),
      '#size' => 10,
      '#required' => TRUE,
      '#attributes' => array('class' => array('state')),
    );
    $format['locality_block']['postal_code'] = array(
      '#title' => t('Postal Code'),
      '#size' => 10,
      '#required' => TRUE,
      '#attributes' => array('class' => array('postal-code')),
    );

    // Remove the existing AJAX functionality.
    unset($format['country']['#ajax']);
    unset($format['country']['#limit_validation_errors']);
    $format['country']['#element_validate'] = array();

    // Load default configurations.
    $default_country = $address['country'];
    $configs = scale_addressfield_get_address_configuration('php');
    $options_list = scale_addressfield_get_country_options_list();

    // Update the list of countries based on our configuration.
    $format['country']['#options'] = $options_list;

    // Update default labels with configured labels.
    $format['country']['#title'] = $configs['label'];
    if ($label = scale_addressfield_get_field_label($default_country, 'thoroughfare')) {
      $format['street_block']['thoroughfare']['#title'] = $label;
    }
    if ($label = scale_addressfield_get_field_label($default_country, 'premise')) {
      $format['street_block']['premise']['#title'] = $label;
    }
    if ($label = scale_addressfield_get_field_label($default_country, 'localityname')) {
      $format['locality_block']['locality']['#title'] = $label;
    }
    if ($label = scale_addressfield_get_field_label($default_country, 'administrativearea')) {
      $format['locality_block']['administrative_area']['#title'] = $label;
    }
    if ($label = scale_addressfield_get_field_label($default_country, 'postalcode')) {
      $format['locality_block']['postal_code']['#title'] = $label;
    }

    // Add our shiny, non-Drupal Form API AJAX functionality!
    $path = drupal_get_path('module', 'scale_addressfield');
    if (!isset($format['country']['#attached']['js'])) {
      $format['country']['#attached']['js'] = array();
    }
    $format['country']['#attached']['js'][] = array(
      'data' => array(
        'scale_addressfield' => array(
          'config_json' => url('addressfield/config.json', array('absolute' => TRUE)),
          'enabled' => array($format['#wrapper_id'] => array()),
        ),
      ),
      'type' => 'setting',
    );
    $format['country']['#attached']['js'][] = $path  . '/js/addressfield.js';
    $format['country']['#attached']['library'] = array(array('scale_addressfield', 'jquery.addressfield'));

    // Attach settings for each xNAL field.
    foreach ($format as $container => $element) {
      if (isset($element['#type']) && $element['#type'] == 'addressfield_container') {
        $xnal_field = scale_addressfield_get_xnal_field_name($container);
        if (!isset($format[$container]['#attached']['js'])) {
          $format[$container]['#attached']['js'] = array();
        }
        $format[$container]['#attached']['js'][] = array(
          'data' => array(
            'scale_addressfield' => array(
              'enabled' => array(
                $format['#wrapper_id'] => array($xnal_field),
              ),
            ),
          ),
          'type' => 'setting',
        );
        $format[$container]['#attributes']['class'][] = $xnal_field;

        foreach ($element as $name => $sub_element) {
          if (is_array($sub_element) && isset($sub_element['#title'])) {
            $xnal_field = scale_addressfield_get_xnal_field_name($name);
            if (!isset($format[$container][$name]['#attached']['js'])) {
              $format[$container][$name]['#attached']['js'] = array();
            }
            $format[$container][$name]['#attached']['js'][] = array(
              'data' => array(
                'scale_addressfield' => array(
                  'enabled' => array(
                    $format['#wrapper_id'] => array($xnal_field),
                  ),
                ),
              ),
              'type' => 'setting',
            );
            // Special case for "localityname" because Address Field provides
            // "locality" as a class name by default. So, overwrite it entirely.
            if ($xnal_field == 'localityname') {
              $format[$container][$name]['#attributes']['class'] = array($xnal_field);
            }
            else {
              // Otherwise, just append our class.
              $format[$container][$name]['#attributes']['class'][] = $xnal_field;
            }
          }
        }
      }
    }

    // We have to have special validation handling in cases where countries
    // require or don't require certain fields.
    $format['locality_block']['administrative_area']['#element_validate'] = array(
      'scale_addressfield_validate_conditional_element',
    );
    $format['locality_block']['postal_code']['#element_validate'] = array(
      'scale_addressfield_validate_conditional_element',
      'scale_addressfield_validate_field_format',
    );

    // Force any administrative area fields to be text rather than a select.
    if (isset($format['locality_block']['administrative_area'])) {
      unset($format['locality_block']['administrative_area']['#options']);
      $format['locality_block']['administrative_area']['#type'] = 'textfield';
    }
  }
}
