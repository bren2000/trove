<?php

class trovequery_plugin_pager extends views_plugin_pager_full {

  function query() {
    global $pager_page_array, $pager_total_items;
    if ($this->view->base_table == 'trovequery') {
      //dpm($this->view);
      $pager_total_items = 10;
      $page = isset($_GET['page']) ? $_GET['page'] : '';
      $offset = 10;
      $start = 0;
      $end = 100;

      // $pager_total_items = pager_load_array($this->options['first_year'], $this->options['id'], $pager_total_items);
      // $page = isset($_GET['page']) ? $_GET['page'] : '';
      // $pager_page_array = explode(',', $page);

      // $offset = $pager_page_array[$this->options['id']] * $this->options['years_per_page'];
      // $start = variable_get('wbapi_current_year', '2008') - $offset;
      // $end = $start - $this->options['years_per_page'] + 1;
      // if ($end < $this->options['first_year']) {
      //   $end = $this->options['first_year'];
      // }
      // $this->view->query->requests['wbapi_data']->setFilter('date', "$end:$start");
    }
  }


}
