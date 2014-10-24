<?php
/**
 * @file
 * Views filter handler for basic Trove Query fields.
 */

/**
 * Views filter handler for basic Trove Query fields.
 */
class TrovequeryHandlerFilterQuery extends views_handler_filter_string {

  public $always_required = TRUE;

  /**
   * Add this filter to the query.
   */
  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field);
    }
  }

  /**
   * Add the Trove query options.
   */
  function operators() {
    $operators = array(
      'word' => array(
        'title' => t('Multi word search'),
        'short' => t('contains word'),
        'method' => 'op_multi',
        'values' => 1,
      ),
      'phrase' => array(
        'title' => t('Phrase search'),
        'short' => t('contains phrase'),
        'method' => 'op_phrase',
        'values' => 1,
      ),
      'not_word' => array(
        'title' => t('Does not contain these words'),
        'short' => t('not word'),
        'method' => 'op_not_multi',
        'values' => 1,
      ),
      'not_phrase' => array(
        'title' => t('Does not contain this phrase'),
        'short' => t('not phrase'),
        'method' => 'op_not_phrase',
        'values' => 1,
      ),
    );

    return $operators;
  }

  /**
   * Set the operator.
   */
  function operator() {
    $_operator = '';
    switch ($this->operator) {
      case 'word':
      case 'phrase':
        break;

      case 'not_word':
      case 'not_phrase':
        $_operator = ' NOT ';
        break;

    }
    return $_operator;
  }

  /**
   * Format a phrase query.
   */
  function op_phrase($field) {
    if (is_array($this->value)) {
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], '"' . implode($this->value, ',') . '"');
    }
    else {
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], '"' . $this->value . '"');
    }
  }

  /**
   * Format a multi-word query.
   */
  function op_multi($field) {
    if (is_array($this->value)) {
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], implode($this->value, ' ') . ' ');
    }
    else {
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->value . ' ');
    }
  }

  /**
   * Format a NOT phrase query.
   */
  function op_not_phrase($field) {
    if (is_array($this->value)) {
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], implode($this->value, $this->operator()));
    }
    else {
      $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->operator() . $this->value);
    }
  }

  /**
   * Format a NOT multi-word query.
   */
  function op_not_multi($field) {
    $this->query->add_where($this->options['group'], $this->definition['trovequery']['arg'], $this->operator() . str_replace(' ', $this->operator(), $this->value));
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
