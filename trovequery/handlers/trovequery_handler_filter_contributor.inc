<?php

class trovequery_handler_filter_contributor
  extends views_handler_filter_in_operator {

  public $opt = array();

  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Index filter');
      $contributors = call_user_func($this->definition['trovequery']['index_method']);
      $this->value_options = $contributors;
    }
  }


  function query() {
    if (isset($this->value, $this->definition['trovequery'])) {
      $this->query->args['method'] = $this->definition['trovequery']['method'];
      $this->query->args[$this->definition['trovequery']['arg']] .=
        $this->definition['trovequery']['index'] . ':' .
        array_pop($this->value);
    }
  }

  function operators() {
    $operators = array(
      'in' => array(
        'title' => t('Is one of'),
        'short' => t('in'),
        'short_single' => t('='),
        'method' => 'op_simple',
        'values' => 1,
        ),
    );
    return $operators;
  }

}
