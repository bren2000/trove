<?php

/**
 * @file
 * Sort results by relevance.
 */

/**
 * Basic sort handler for relevance.
 *
 */
class trovequery_handler_sort_relevance extends views_handler_sort {

  /**
   * Determine if a sort can be exposed.
   */
  function can_expose() {
    return TRUE;
  }

  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->field_alias = $this->real_field;
    if (isset($this->definition['sort order'])) {
      $this->query->add_filter($this->definition['sort method'], $this->definition['sort order']);
    }
  }

}
