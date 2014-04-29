<?php

function trovequery_views_default_views() {
  $view = new view();
  $view->name = t('Trovequery default view');
  $view->description = t('This is the default Trovequery module view');
  $view->tag = t('default');
  $view->base_table = 'trovequery';
  $view->human_name = t('Trovequery default view');
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'trovequery';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Trove Results: title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'trovequery';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Sort criterion: Trove Results: relevance */
  $handler->display->display_options['sorts']['relevance_sort']['id'] = 'relevance_sort';
  $handler->display->display_options['sorts']['relevance_sort']['table'] = 'trovequery';
  $handler->display->display_options['sorts']['relevance_sort']['field'] = 'relevance_sort';
  /* Filter criterion: Trove Results: zone */
  $handler->display->display_options['filters']['zone']['id'] = 'zone';
  $handler->display->display_options['filters']['zone']['table'] = 'trovequery';
  $handler->display->display_options['filters']['zone']['field'] = 'zone';
  $handler->display->display_options['filters']['zone']['value'] = array(
    'all' => 'all',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'trovequery';

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;
}
