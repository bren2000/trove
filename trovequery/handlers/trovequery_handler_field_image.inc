<?php

/**
 * @file
 * Views field handler for basic Trove Query fields.
 */

/**
 * Views field handler for basic Trove Query fields.
 *
 * The only thing we're doing here is making sure the field_alias
 * gets set properly, and that none of the sql-specific query functionality
 * gets called.
 */
class trovequery_handler_field_image extends views_handler_field {
  function query() {
    $this->field_alias = $this->real_field;
  }

  /**
   * Render the field.
   */
  function render($values) {
    if (isset($values->image)) {
      $variables = array(
        'path' => $values->{$this->field},
      );
      return theme('image', $variables);
    }
  }

}
