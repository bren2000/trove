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
 *
 *  Some trove api response fields contain HTML, which is typically through
 * check_plain() and thus not rendered. Here we give the user the option to
 * override this behavior and render the HTML.
 */

class trovequery_handler_field_include extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['render_html'] = array('default' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['render_html'] = array(
      '#title' => t('Render HTML'),
      '#description' => t('By default, HTML returned from the Trove API is escaped and will not render. Checking this option will disable this behavior and allow the HTML to render. Note that this could be a security problem if Troves filtering of dangerous HTML has problems.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['render_html'],
      '#weight' => 1,
    );
    parent::options_form($form, $form_state);
  }

  function pre_query() {
    $this->query->add_where('', $this->definition['trovequery']['arg'], $this->definition['trovequery']['value']);
  }

  function query() {
    $this->field_alias = $this->real_field;
  }

  function render($values) {
    if (isset($values->{$this->field})) {
      if ($this->options['render_html']) {
        $value = $values->{$this->field};
      }
      else {
        $value = check_plain($values->{$this->field});
      }
      return $value;
    }
  }

}
