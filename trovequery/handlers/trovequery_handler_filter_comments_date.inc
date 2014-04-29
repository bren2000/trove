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
class trovequery_handler_filter_comments_date
extends views_handler_filter_date {

  function query() {
    dpm(date("Y-m-d\TH:i:s\Z", strtotime($this->value['value'] . "-10 hours") ));

    $this->field_alias = $this->real_field;

    $info = $this->operators();
    $this->field_alias = $this->real_field;
    if (isset($this->value, $this->definition['trovequery']) && $this->value) {
      $this->query->args['method'] = $this->definition['trovequery']['method'];
      $this->query->args[$this->definition['trovequery']['arg']] .=
        $this->definition['trovequery']['index'] . ':[' .
        date(DATE_ATOM, strtotime($this->value['value']) ) . '] ';
    }
  }

  function op_between($field) {
    $a = intval(strtotime($this->value['min'], 0));
    $b = intval(strtotime($this->value['max'], 0));

    if ($this->value['type'] == 'offset') {
      $a = '***CURRENT_TIME***' . sprintf('%+d', $a); // keep sign
      $b = '***CURRENT_TIME***' . sprintf('%+d', $b); // keep sign
    }
    // This is safe because we are manually scrubbing the values.
    // It is necessary to do it this way because $a and $b are formulas when using an offset.
    $operator = strtoupper($this->operator);
    $this->query->add_where_expression($this->options['group'], "$field $operator $a AND $b");
  }

}
