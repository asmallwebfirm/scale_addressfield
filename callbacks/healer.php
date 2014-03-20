<?php

/**
 * @file
 * The Scale Addressfield module's form freshness AJAX callback.
 *
 * The purpose of this callback is to check if a given form still has a
 * corresponding form cache entry. If it doesn't, it attempts to pass back the
 * form build ID of a valid form (by rebuilding the form from the referring
 * page and passing along its build ID).
 *
 * We do this here, rather than via a Drupal menu callback, for performance and
 * scalability.
 */

// Try and load Drupal's bootstrap.inc.
define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/common.inc';

// Bootstrap Drupal to the configuration stage.
if (function_exists('drupal_bootstrap')) {
  drupal_bootstrap(DRUPAL_BOOTSTRAP_CONFIGURATION);

  // Only respond if valid parameters were provided.
  if (isset($_GET['fid']) && isset($_GET['fbid']) && isset($_SERVER['HTTP_REFERER'])) {
    // Load form.inc and database.inc to check form cache staleness.
    require_once DRUPAL_ROOT . '/includes/form.inc';
    require_once DRUPAL_ROOT . '/includes/database/database.inc';

    // You shouldn't store your cache_form data in memory, but just in case you
    // do, here's the logic to ensure that's accounted for.
    require_once DRUPAL_ROOT . '/includes/cache.inc';
    foreach (variable_get('cache_backends', array()) as $include) {
      require_once DRUPAL_ROOT . '/' . $include;
    }

    // Check if a valid form cache entry still exists. If it does not, we'll
    // return a new, valid form_build_id.
    if (!cache_get('form_' . $_GET['fbid'], 'cache_form')) {
      $response = $_GET['fbid'];
      $delimit = strpos($_SERVER['HTTP_REFERER'], '?') !== FALSE ? '&' : '?';
      $get = $_SERVER['HTTP_REFERER'] . $delimit . 'healer=' . md5(mt_rand(0, 100000));

      // The referer could be spoofed; let's make sure we're pointing inbound.
      if (strpos($get, $GLOBALS['base_url']) === 0) {
        $new_form_page = drupal_http_request($get, array(
          'headers' => array('referer' => $_SERVER['HTTP_REFERER']),
        ));

        // Ensure that our request returned a response.
        if (isset($new_form_page->data) && !empty($new_form_page->data)) {
          // Attempt to parse the data into a useful format.
          $htmldom = new DOMDocument();
          @$htmldom->loadHTML($new_form_page->data);

          // Ensure successful conversion from DOMDocument to SimpleXMLElement.
          if ($pagexml = simplexml_import_dom($htmldom)) {
            // Attempt to pull the form_build_id from the rendered html.
            if ($form_build = $pagexml->xpath('//form[@id="' . $_GET['fid'] . '"]//input[@name="form_build_id"]')) {
              $response = (string) $form_build[0]->attributes()->value;
            }
          }
        }
      }
    }
    else {
      // Respond with the form_build_id passed if the cache entry exists.
      $response = $_GET['fbid'];
    }

    // Return an AJAX invoke command, including the cache form status.
    drupal_json_output(array(
      array(
        'command' => 'invoke',
        // Trigger an event named "pinged" against the given form ID, passing
        // along a true/false on whether or not the form is fresh.
        'method' => 'trigger',
        'selector' => '#' . check_plain($_REQUEST['fid']),
        'arguments' => array(
          'pinged',
          array(check_plain($response)),
        ),
      ),
    ));
    exit();
  }
}

// Fallback to returning nothing.
drupal_json_output(array());
