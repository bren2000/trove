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

  function query($get_count = FALSE) {  }

  /**
   * Executes the query and fills the associated view object with according
   * values.
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
      // pager
      if (empty($this->args['method'])) {
        return;
      }
      if (method_exists($view, 'init_pager')) {
        $view->init_pager();
      }

      if ($this->pager->use_pager()) {
        $this->pager->set_current_page($view->current_page);
        $this->pager->total_items = 100;
        $this->pager->options['total_pages'] = 10;
      }
      $this->pager->query();

      $this->request = troveAPI::factory($view->base_table);
      foreach ($this->args as $k => $v) {
        $this->request->setFilter($k, $v);
      }
      $this->request->query();
      $view->result = $this->request->parse();

      // Slice the resultset.
      if (isset($this->offset, $this->limit)) {
        $end = $this->offset + $this->limit;
        $end = count($view->result) >= $end ? $end : count($view->result);
        $view->result = array_slice($view->result, $this->offset, $end);
      }
      dpm($this->pager);

    } catch (Exception $e) {
      $this->errors[] = $e->getMessage();
      // Recursion to get the same error behaviour as above.
      return $this->execute($view);
    }
  }
}
