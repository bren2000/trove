<?php
/**
* @file
*   Views query plugin for Trove Query.
*/

/**
* Views query plugin for the Trove Query results.
*/
class trove_plugin_query extends views_plugin_query {

  // Our argument array. Anything that needs to be passed to the API gets
  // added to this array through the argumet and filter handlers. This array
  // then gets passed to url() along with $api_url and becomes the query
  // string.
  var $args;
  var $required_fields;
  var $request;
  var $where;
  protected $errors;
  protected $abort = FALSE;

  /**
   * Constructor; Create the basic query object and fill with default values.
   */
  function init($base_table, $base_field) {
    //parent::init($base_table, $base_field, $options);
    // Required fields are fields that may not be empty in rows of the
    // resultset. Any rows with empty required fields are dropped *prior* to
    // paging.
    $this->required_fields = array();
    $this->ensure_table($base_table);
    $where = array();
  }

  function ensure_table($table) {
    if (!isset($this->request)) {
      $this->request = troveAPI::factory($table);
    }
  }

  /**
   * Get the arguments attached to the WHERE and HAVING clauses of this query.
   */
  function get_where_args() {

    $args = array();
    foreach ($this->where as $group => $where) {
      $args = array_merge($args, $where['args']);
    }
    foreach ($this->having as $group => $having) {
      $args = array_merge($args, $having['args']);
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
   * @param $where
   *   'where' or 'having'.
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
          // query and the count query, so clone the subquery to have two instances
          // of the same object. - http://drupal.org/node/1112854
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

  function add_filter($field, $value, $operator = '=') {
    //$this->ensure_table($table);
    $this->request->setFilter($field, $value);
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
            //dpm($condition);
            $this->add_filter($condition['field'], $condition['value']);
          }
        }
      }
    }
  }

  /**
   * Executes the query and fills the associated view object with according
   * values.
   *
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   */
  function execute(&$view) {

    if ($this->errors || $this->abort ){
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

      // foreach ($this->args as $k => $v) {
      //   $this->request->setFilter($k, $v);
      // }
      $this->request->query();
      $view->result = $this->request->parse();

      // Setup pager
      $view->init_pager();
      if ($this->pager->use_pager()) {
        $this->pager->set_current_page($view->current_page);
      }
      $this->pager->total_items = $view->result['total_results'];
      //dpm($view->result['total_results']);
      $this->pager->update_page_info();

      // Slice the resultset.
      //if (isset($this->offset, $this->limit)) {
      //  $end = $this->offset + $this->limit;
      //  $end = count($view->result) >= $end ? $end : count($view->result);
      //  $view->result = array_slice($view->result, $this->offset, $end);
      //}
      //dpm($this->pager);
      //
      //dpm($this);

    } catch (Exception $e) {
      $this->errors[] = $e->getMessage();
      // Recursion to get the same error behaviour as above.
      return $this->execute($view);
    }
  }
}
