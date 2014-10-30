<?php
/**
 * @file
 * Definition of trovequery_handler_filter_facet.
 */

class TrovequeryHandlerFilterFacet extends views_handler_filter_in_operator {

  public $opt = array();

  /**
   * Make an options list with depth indication.
   */
  public function make_options($array, $level = 0) {
    $pre = '';
    for ($i = $level; $i > 0; $i--) {
      $pre .= '--';
    }
    foreach ($array as $k => $v) {
      if (is_array($v)) {
        $this->opt[$k] = $k;
        $this->make_options($v, $level + 1);
      }
      else {
        $this->opt[$k] = $pre . $v;
      }
    }
  }

  /**
   * Sets the'value options'.
   */
  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Facet filter');
      $facets = call_user_func($this->definition['trovequery']['facets_method']);
      $this->make_options($facets);
      $this->value_options = $this->opt;
    }
  }

  /**
   * Called to add the field to a query.
   */
  public function query() {
    if (isset($this->value, $this->definition['trovequery'])) {
      $this->query->args['method'] = $this->definition['trovequery']['method'];
      if (is_array($this->value)) {
        $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], implode($this->value, ','));
      }
      else {
        $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->value);
      }
    }
  }

}
