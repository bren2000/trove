<?php
/**
 * @file
 * The admin settings for the trove module.
 */

use Drupal\trove\TroveApi;
use Drupal\trove\TroveApiContributor;

/**
 * Implements hook_settings_form().
 */
function trove_admin_settings() {

  $form['trove_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('trove_api_key', ''),
    '#description' => t('API Key from Trove (see the <a href="@trove_link">Trove API page</a> to
      apply for an API  key).', array('@trove_link' => url('http://trove.nla.gov.au/general/api'))),
  );


  $form['trove_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trove API operation details'),
    '#tree' => TRUE,
  );

  $times = array(
    900,
    1800,
    2700,
    3600,
    7200,
    10800,
    14400,
    18000,
    21600,
    43200,
    86400,
  );

  $ageoptions = drupal_map_assoc($times, 'format_interval');

  $form['trove_details']['trove_cache_duration'] = array(
    '#type' => 'select',
    '#title' => t('Cache update interval'),
    '#options' => $ageoptions,
    '#default_value' => variable_get('trove_cache_duration', 3600),
    '#description' => t('The Trove module caches calls to the API to improve
      performance. Select a refresh interval indicating how often you want to
      check cached trove API calls are up to date.'),
  );

  $timeout_raw_options = array(
    3.0,
    6.0,
    12.0,
    30.0,
    60.0,
  );

  $timeout_options = drupal_map_assoc($timeout_raw_options, 'format_interval');

  $form['trove_details']['trove_timeout'] = array(
    '#type' => 'select',
    '#title' => t('API calls timeout'),
    '#options' => $timeout_options,
    '#default_value' => variable_get('trove_timeout', 3.0),
    '#description' => t('Set the timeout for API calls.'),
  );

  $form['trove_details']['trove_cache_items_cron'] = array(
    '#type' => 'checkbox',
    '#title' =>  t('Cache Trove Contributors and Trove Newspaper titles during
      Drupal cron run. '),
    '#default_value' => variable_get('trove_cache_items_cron', TRUE),
    '#description' => t('Results that change less frequently can be requested and cached
          during cron runs.'),
  );

  return system_settings_form($form);
}

/**
 * Validate user input.
 */
function trove_admin_settings_validate($form, &$form_state) {
  $key = trim($form_state['values']['trove_api_key']);
  if ($key && (preg_match('/^[A-Za-z\d]{16}$/', $key) != 1)) {
    form_set_error('trove_api_key', t('This does not appear to be a trove API key.'));
  }
}

/**
 * Submit form data.
 */
function trove_admin_settings_submit($form, &$form_state) {
  // Clean up the data.
  $form_state['values']['trove_api_key'] = trim($form_state['values']['trove_api_key']);
  // Save the settings.
  system_settings_form_submit($form, $form_state);
}
