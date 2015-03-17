<?php
/**
 * @file
 * The admin settings for the trove module.
 */

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
  $form['trove_cache_duration'] = array(
    '#type' => 'select',
    '#title' => t('Update interval'),
    '#options' => $ageoptions,
    '#default_value' => variable_get('trove_cache_duration', 3600),
    '#description' => t('The refresh interval indicating how often you want to check cached trove API calls are up to date.'),
  );
  $form['trove_update_contributors'] = array(
    '#type' => 'button',
    '#value' => t('Update contributors'),
    '#title' => t('Update contributors list'),
    '#description' => t('This will update the list of Trove contributors, this list is used by views as filter values.'),
    '#suffix' => '<span id="update-cont-div"></span>',
    '#ajax' => array(
      'callback' => 'troveGetTroveContributors',
      'wrapper' => 'update-cont-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  return system_settings_form($form);
}

/**
 * Update the trove contributors table.
 */
function troveGetTroveContributors() {
  $contributors = TroveApi::factory('trovecontrib');
  $contributors->query();
  $list = $contributors->parse();
  return (troveDbAddTroveContributors($list) ? 'Contributors list updated with ' . count($list) . ' entries' : 'An error occured');
}

/**
 * Add trove contributors.
 */
function troveDbAddTroveContributors($contributors) {
  if (!empty($contributors)) {
    foreach ($contributors as $cont) {
      if ($cont->nuc) {
        $values[] = array(
          'nuc' => $cont->nuc,
          'name' => $cont->name,
        );
      }
    }
    db_delete('trove_contributors')->execute();
    $insert = db_insert('trove_contributors')->fields(array('nuc', 'name'));
    foreach ($values as $value) {
      $insert->values($value);
    }
    $insert->execute();
    return TRUE;
  }
  return FALSE;
}

/**
 * Validate user input.
 */
function troveAdminSettingsValidate($form, &$form_state) {
  $key = trim($form_state['values']['trove_api_key']);
  if ($key && (preg_match('/^[A-Za-z\d]{16}$/', $key) != 1)) {
    form_set_error('trove_api_key', t('This does not appear to be a trove API key.'));
  }
}

/**
 * Submit form data.
 */
function troveAdminSettingsSubmit($form, &$form_state) {
  // Clean up the data.
  $form_state['values']['trove_api_key'] = trim($form_state['values']['trove_api_key']);
  // Save the settings.
  system_settings_form_submit($form, $form_state);
}
