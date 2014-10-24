<?php
/**
 * @file
 * Views field handler for basic Trove Query fields.
 */

/**
 * Views field handler for basic Trove Query fields.
 *
 * The only thing we're doing here is making sure the field_alias
 * gets set properly, and that none of the sql-specific query functionality
 * gets called.
 */
class TrovequeryHandlerFieldTitle extends views_handler_field {

  /**
   * Constructor to provide additional field to add.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['trove_url'] = 'trove_url';
  }

  /**
   * Called to add the field to a query.
   */
  public function query() {
    $this->field_alias = $this->real_field;
  }

  /**
   * Define the options available.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_trove'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    $form['link_to_trove'] = array(
      '#title' => t('Link the title to the record in Trove'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_trove']),
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the taxonomy.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  public function render_link($data, $values) {
    $trove_url = $values->trove_url;
    if (!empty($this->options['link_to_trove']) && !empty($trove_url) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $trove_url;
    }
    return $data;
  }

  /**
   * Render output based on options settings.
   */
  public function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
