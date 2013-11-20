<?php

class trovequery_handler_filter_include extends views_handler_filter_boolean_operator {

  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->field_alias = $this->real_field;
    if (isset($this->value, $this->definition['trovequery']) && $this->value) {
      $this->query->args['method'] = $this->definition['trovequery']['method'];
      $this->query->args[$this->definition['trovequery']['arg']] .=  $this->definition['trovequery']['index'] . ':' . $this->definition['trovequery']['value'] . ' ';
    }
  }

  function get_value_options() {
    $this->value_options = array(1 => t('Yes'), 0 => t('No'));
  }

}

