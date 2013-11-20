<?php

class trovequery_handler_filter_zone extends views_handler_filter_in_operator {

  function get_value_options() {
    //dpm($this->definition['trovequery']);
    if (!isset($this->value_options)) {
      $this->value_title = t('Zone type');
      $zones = call_user_func($this->definition['trovequery']['zones_method']);
      foreach ($zones as $type => $info) {
        $options[$type] = $info;
      }

      $this->value_options = $options;
      //dpm($this->value_options);
    }
  }

  function query() {
    //dpm($this->definition['trovequery']['arg'] . ":" . $this->value);
    //dpm($this->value);
    if (isset($this->value, $this->definition['trovequery'])) {
      $this->query->args['method'] = $this->definition['trovequery']['method'];
      if(is_array($this->value)) {
        $this->query->args[$this->definition['trovequery']['arg']] = implode($this->value,',');
      } else {
        $this->query->args[$this->definition['trovequery']['arg']] = $this->value;
      }
    }
  }

}

