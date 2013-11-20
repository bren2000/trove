<?php

/**
 * @file
 * Definition of views_handler_filter_date.
 */

/**
 * Filter to handle dates stored as a timestamp.
 *
 * @ingroup views_filter_handlers
 */
class trovequery_handler_filter_comments_date extends views_handler_filter_date {

  function query() {
    $this->field_alias = $this->real_field;
    dpm( date(DATE_ATOM, strtotime($this->value['value']) ));
    $info = $this->operators();
    $this->field_alias = $this->real_field;
    if (isset($this->value, $this->definition['trovequery']) && $this->value) {
      $this->query->args['method'] = $this->definition['trovequery']['method'];
      $this->query->args[$this->definition['trovequery']['arg']] .=  $this->definition['trovequery']['index'] . ':[' . date(DATE_ATOM, strtotime($this->value['value']) ) . '] ';
    }
  }

}
