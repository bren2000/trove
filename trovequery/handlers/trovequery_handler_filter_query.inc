<?php

class trovequery_handler_filter_query extends views_handler_filter_string {

  public $always_required = TRUE;

  function query() {
    //dpm($this->definition['trovequery']['arg'] . ":" . $this->value);
    //dpm($this->value);

    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field);
    }

    // if (isset($this->value, $this->definition['trovequery'])) {
    //   $this->query->args['method'] = $this->definition['trovequery']['method'];
    //   if(is_array($this->value)) {
    //     $this->query->args[$this->definition['trovequery']['arg']] .= implode($this->value,',') . ' ';
    //   } else {
    //     $this->query->args[$this->definition['trovequery']['arg']] .= $this->value . ' ';
    //   }
    // }
  }

  function operators() {
    $operators = array(
      'word' => array(
        'title' => t('Multi word search'),
        'short' => t('has word'),
        'method' => 'op_multi',
        'values' => 1,
        //'default' => 1,
      ),
      'phrase' => array(
        'title' => t('Phrase search'),
        'short' => t('contains the phrase'),
        'method' => 'op_phrase',
        'values' => 1,
      ),
      'not_word' => array(
        'title' => t('Does not contain these words'),
        'short' => t('not words'),
        'method' => 'op_not_multi',
        'values' => 1,
      ),
      'not_phrase' => array(
        'title' => t('Does not contain this phrase'),
        'short' => t('!has'),
        'method' => 'op_not_phrase',
        'values' => 1,
      ),
    );

    return $operators;
  }

  function op_phrase($field) {
    $this->value = "\"$this->value\"";
    if(is_array($this->value)) {
      //$this->query->args[$this->definition['trovequery']['arg']] .= implode($this->value,',') . ' ';
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], implode($this->value,','));
    } else {
      //$this->query->args[$this->definition['trovequery']['arg']] = $this->value . ' ';
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->value);
    }
  }

  function op_multi($field) {
    //dpm('www');
    if(is_array($this->value)) {
      $this->query->args[$this->definition['trovequery']['arg']] .= implode($this->value,',') . ' ';
    } else {
      $this->query->args[$this->definition['trovequery']['arg']] = $this->value . ' ';
    }
  }

  function op_not_phrase($field) {
    $this->value = 'NOT ' . "\"$this->value\"";
    if(is_array($this->value)) {
      $this->query->args[$this->definition['trovequery']['arg']] .= implode($this->value,',') . ' ';
    } else {
      $this->query->args[$this->definition['trovequery']['arg']] = $this->value . ' ';
    }
  }

  function op_not_multi($field) {
    $this->value = 'NOT ' . str_replace(' ', ' NOT ', $this->value);
  }


  /**
   * Handle the 'left' side of the exposed options form.
   *
   * We remove the operator option since we only have one operators available.
   */
  function expose_form_left(&$form, &$form_state) {
    parent::expose_form_left($form, $form_state);
    $form['expose']['use_operator']['#access'] = FALSE;
    $form['expose']['operator']['#access'] = FALSE;
  }

  /**
   * Remove the optional checkbox since it makes no sense in our context.
   */
  function expose_form_right(&$form, &$form_state) {
    parent::expose_form_right($form, $form_state);
    $form['expose']['optional']['#access'] = FALSE;
  }

  /**
   * Remove the case sensitive option, since it is irrelevant.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['case']['#access'] = FALSE;
  }
}
