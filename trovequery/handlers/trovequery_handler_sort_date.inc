<?php

class trovequery_handler_sort_date extends views_handler_sort {

  /**
   * Determine if a sort can be exposed.
   */
  function can_expose() {
    return TRUE;
  }

  /**
   * Provide a list of options for the default sort form.
   *
   * Should be overridden by classes that don't override sort_form.
   */
  function sort_options() {
    return array(
      'asc' => t('Sort ascending'),
      'desc' => t('Sort descending'),
    );
  }

  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->field_alias = $this->real_field;
    if (isset($this->definition['sort order'], $this->options['order'])) {
      $this->query->add_filter($this->definition['sort method'], $this->definition['sort order'] . $this->options['order']);
    }
  }

}
