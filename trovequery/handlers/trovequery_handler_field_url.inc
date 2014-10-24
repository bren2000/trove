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
class trovequery_handler_field_url extends views_handler_field_url {

  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->field_alias = $this->real_field;
  }

  /**
   * Render output based on options settings.
   * 
   * @param object $values
   * 
   * @return the rendered field
   */
  function render($values) {
    $value = $values->{$this->field};
    if (!empty($this->options['display_as_link'])) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $value;
      $text = !empty($this->options['text']) ? $this->sanitize_value($this->options['text']) : $this->sanitize_value($value, 'url');
      return $text;
    }
    else {
      return $this->sanitize_value($value, 'url');
    }
  }

}
