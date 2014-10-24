<?php
/**
 * @file
 * Views field handler for basic Trove Query fields.
 */

class trovequery_handler_filter_holding extends views_handler_filter_in_operator {

  public $opt = array();

  /**
   * Sets the'value options'.
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Index filter');
      $contributors = call_user_func($this->definition['trovequery']['index_method']);
      $this->value_options = $contributors;
    }
  }

  /**
   * Called to add the field to a query.
   */
  function query() {
    if (isset($this->value, $this->definition['trovequery'])) {
      $this->query->args['method'] = $this->definition['trovequery']['method'];
      if (is_array($this->value)) {
        foreach ($this->value as $key => $value) {
          $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->definition['trovequery']['index'] . ':' . $value);
        }
      }
      else {
        $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->definition['trovequery']['index'] . ':' . $this->value);
      }
    }
  }

  /**
   * Overrides views_handler_filter_string::operators
   */
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
