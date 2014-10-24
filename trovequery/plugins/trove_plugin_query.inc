<?php
/**
 * @file
 * Views query plugin for Trove Query.
 */

/**
 * Views query plugin for the Trove Query results.
 */
class trove_plugin_query extends views_plugin_query {

  /*
   * An array of sections of the WHERE query. Each section is in itself
   * an array of pieces. All AND by default.
   */
  var $where = array();

  /*
   * Array of all encountered errors.
   *
   * Each of these is fatal.
   *
   * A non-empty $errors property will
   * result in an empty result being returned.
   *
   * @var array
   */
  var $errors = array();

  /*
   * Whether to abort the search instead of executing it.
   *
   * @var bool
   */
  var $abort = FALSE;

  /**
   * Constructor; Create the basic query object and fill with default values.
   */
  public function init($base_table, $base_field, $options) {
    // parent::init($base_table, $base_field, $options);
    // Required fields are fields that may not be empty in rows of the
    // resultset. Any rows with empty required fields are dropped *prior* to
    // paging.
    $this->required_fields = array();
    $this->ensure_table($base_table);
    $where = array();
  }
  /**
   * Ensure table.
   */
  function ensure_table($table) {
    if (!isset($this->request)) {
      $this->request = TroveAPI::factory($table);
    }
  }


  /**
   * Get the arguments attached to the WHERE clauses of this query.
   */
  function get_where_args() {

    $args = array();
    foreach ($this->where as $group => $where) {
      $args = array_merge($args, $where['args']);
    }

    return $args;
  }

  /**
   * Construct the "WHERE" or "HAVING" part of the query.
   *
   * As views has to wrap the conditions from arguments with AND, a special
   * group is wrapped around all conditions. This special group has the ID 0.
   * There is other code in filters which makes sure that the group IDs are
   * higher than zero.
   *
   * @param string $where
   */
  function build_condition($where = 'where') {
    $has_condition = FALSE;
    $has_arguments = FALSE;
    $has_filter = FALSE;

    $main_group = db_and();
    $filter_group = $this->group_operator == 'OR' ? db_or() : db_and();

    foreach ($this->$where as $group => $info) {

      if (!empty($info['conditions'])) {
        $sub_group = $info['type'] == 'OR' ? db_or() : db_and();
        foreach ($info['conditions'] as $key => $clause) {
          // DBTNG doesn't support to add the same subquery twice to the main
          // query and the count query, so clone the subquery to have two
          // instances of the same object. - http://drupal.org/node/1112854
          if (is_object($clause['value']) && $clause['value'] instanceof SelectQuery) {
            $clause['value'] = clone $clause['value'];
          }
          if ($clause['operator'] == 'formula') {
            $has_condition = TRUE;
            $sub_group->where($clause['field'], $clause['value']);
          }
          else {
            $has_condition = TRUE;
            $sub_group->condition($clause['field'], $clause['value'], $clause['operator']);
          }
        }

        // Add the item to the filter group.
        if ($group != 0) {
          $has_filter = TRUE;
          $filter_group->condition($sub_group);
        }
        else {
          $has_arguments = TRUE;
          $main_group->condition($sub_group);
        }
      }
    }

    if ($has_filter) {
      $main_group->condition($filter_group);
    }

    if (!$has_arguments && $has_condition) {
      return $filter_group;
    }
    if ($has_arguments && $has_condition) {
      return $main_group;
    }
  }

  /**
   * Add a simple WHERE clause to the query.
   *   @param integer $group
   *   @param string $field
   *   @param string $value
   *   @param string $operator
   */
  function add_where($group, $field, $value = NULL, $operator = NULL) {
    // Ensure all variants of 0 are actually 0. Thus '', 0 and NULL are all
    // the default group.
    if (empty($group)) {
      $group = 0;
    }

    // Check for a group.
    if (!isset($this->where[$group])) {
      $this->set_where_group('AND', $group);
    }

    $this->where[$group]['conditions'][] = array(
      'field' => $field,
      'value' => $value,
    );
  }
  /**
   * Set a filter on the query object.
   */
  function add_filter($field, $value, $operator = '=') {
    // $this->ensure_table($table);
    $this->request->set_filter($field, $value);
  }

  /**
   * Builds the necessary info to execute the query.
   */
  public function build(&$view) {
    $this->view = $view;

    if (!empty($this->where)) {
      foreach ($this->where as $group_id => $group) {
        if (!empty($group['conditions'])) {
          foreach ($group['conditions'] as $condition) {
            $this->add_filter($condition['field'], $condition['value']);
          }
        }
      }
    }

  }


  /**
   * Executes query and fill the associated view object with according values.
   *
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   */
  function execute(&$view) {

    if ($this->errors || $this->abort) {
      if (error_displayable()) {
        foreach ($this->errors as $msg) {
          drupal_set_message(check_plain($msg), 'error');
        }
      }
      return;
    }
    try {

      $start = microtime(TRUE);

      if (empty($this->args['method'])) {
        return;
      }

      // Setup pager.
      $view->init_pager();
      if ($this->pager->use_pager()) {
        $this->pager->set_current_page($view->current_page);
      }

      $this->add_filter('n', $this->pager->options['items_per_page']);
      $this->add_filter('s', $this->pager->current_page * $this->pager->options['items_per_page']);

      $this->request->query();
      $view->result = $this->request->parse(NULL);

      $this->pager->total_items = $this->request->getTotalResults();
      $this->pager->update_page_info();

    }
    catch (Exception $e) {
      $this->errors[] = $e->getMessage();
      // Recursion to get the same error behaviour as above.
      return $this->execute($view);
    }
    $view->execute_time = microtime(TRUE) - $start;
  }

}
