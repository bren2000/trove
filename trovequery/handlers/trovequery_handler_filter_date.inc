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
class trovequery_handler_filter_date
extends views_handler_filter_date {

  function query() {
    dpm($this->operator);
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
    //TROVE_FROM_DATE
    // ----------------------------------------
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

  function op_simple($field) {

    $value = intval(strtotime($this->value['value'], 0));
    dpm($this);
    if (!empty($this->value['type']) && $this->value['type'] == 'offset') {
      $value = '***CURRENT_TIME***' . sprintf('%+d', $value); // keep sign
    }
    // This is safe because we are manually scrubbing the value.
    // It is necessary to do it this way because $value is a formula when using an offset.
    $this->query->add_where_expression($this->options['group'], "$field $this->operator $value");
  }

  function operators() {
    $operators = array(
      '<' => array(
        'title' => t('Is less than'),
        'method' => 'op_simple',
        'short' => t('<'),
        'values' => 1,
      ),
      '<=' => array(
        'title' => t('Is less than or equal to'),
        'method' => 'op_simple',
        'short' => t('<='),
        'values' => 1,
      ),
      '=' => array(
        'title' => t('Is equal to'),
        'method' => 'op_simple',
        'short' => t('='),
        'values' => 1,
      ),
      '!=' => array(
        'title' => t('Is not equal to'),
        'method' => 'op_simple',
        'short' => t('!='),
        'values' => 1,
      ),
      '>=' => array(
        'title' => t('Is greater than or equal to'),
        'method' => 'op_simple',
        'short' => t('>='),
        'values' => 1,
      ),
      '>' => array(
        'title' => t('Is greater than'),
        'method' => 'op_simple',
        'short' => t('>'),
        'values' => 1,
      ),
      'between' => array(
        'title' => t('Is between'),
        'method' => 'op_between',
        'short' => t('between'),
        'values' => 2,
      ),
      'not between' => array(
        'title' => t('Is not between'),
        'method' => 'op_between',
        'short' => t('not between'),
        'values' => 2,
      ),
    );

    return $operators;
  }


}
