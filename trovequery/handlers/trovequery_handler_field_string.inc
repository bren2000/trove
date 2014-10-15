<?php
/**
 * @file
 *   Views field handler for basic Trove Query fields.
 */

/**
 * Views field handler for basic Trove Query fields.
 *
 * The only thing we're doing here is making sure the field_alias
 * gets set properly, and that none of the sql-specific query functionality
 * gets called.
 */
class trovequery_handler_field_string extends views_handler_field {
  
  function query() {
    $this->field_alias = $this->real_field;
    $this->query->add_where('', 'include', 'holdings');
  }



}