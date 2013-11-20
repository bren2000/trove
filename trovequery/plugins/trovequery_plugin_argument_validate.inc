<?php
// $Id;

/**
 * @file
 *   Views argument validator for trove API query arguments.
 */

/**
 * Views argument validator for trove API event arguments.
 *
 * Try and throw an error for invalid filter combinations, according to
 * the api documentation
 */
class trovequery_plugin_argument_validate extends views_plugin_argument_validate {

  function validate_argument($arg) {
    //kpr($this);

    // $unique_args = array('artist', 'user', 'event');
    // $arg_keys = array_keys($this->view->argument);

    // if (count(array_intersect($unique_args, $arg_keys)) > 1) {
    //   drupal_set_message('You may only use one of the following arguments at a time when querying events: artist, user, event.');
    //   return FALSE;
    // }

    // if (in_array('lat', $arg_keys) && !in_array('long', $arg_keys)) {
    //   drupal_set_message('When using a latitude argument you must also use a longitude argument.');
    //   return FALSE;
    // }

    // if (in_array('long', $arg_keys) && !in_array('lat', $arg_keys)) {
    drupal_set_message('When using a longitude argument you must also use a latitude argument.');
    //   return FALSE;
    // }

    return FALSE;
  }
}
