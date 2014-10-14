<?php
/**
 * Implementation of hook_views_plugins().
 */
function trovequery_views_plugins() {
  $plugin = array();
  $plugin['query']['trove_plugin_query'] = array(
    'title' => t('Trove API Query'),
    'help' => t('Trove API query object.'),
    'handler' => 'trove_plugin_query',
    'use pager' => TRUE,
  );
  return $plugin;
}

/**
 * Implementation of hook_views_data().
 */
function trovequery_views_data() {
  $data = array();

  // Trove query base table
  $data['trovequery']['table']['group']  = t('Trove Results');
  $data['trovequery']['table']['base'] = array(
    'title' => t('Trove Results'),
    'help' => t('Query Trove.'),
    'field' => 'title',
    'query class' => 'trove_plugin_query',
  );

  // title
  $data['trovequery']['title'] = array(
    'title' => t('title'),
    'help' => t('The title of this result.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_title',
    ),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_field',
      'help' => t('Keyword filter on the title field.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'title',
      ),
    ),
  );

  // isbn
  $data['trovequery']['isbn'] = array(
    'title' => t('ISBN'),
    'help' => t('The ISBN for this result.'),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_include_string',
      'help' => t('Filter on ISBNs.'),
      'label' => t('The ISBN of the item.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'isbn',
      ),
    ),
  );

  // issn
  $data['trovequery']['issn'] = array(
    'title' => t('ISSN'),
    'help' => t('The ISSN for this result.'),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_include_string',
      'help' => t('Filter on ISSNs.'),
      'label' => t('The ISSN of the item.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'issn',
      ),
    ),
  );

  // id
  $data['trovequery']['id'] = array(
    'title' => t('id'),
    'help' => t('The work number of this item.'),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_include_string',
      'help' => t('Filter on work number (ID).'),
      'label' => t('The work number of the item.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'id',
      ),
    ),
  );



  // tags
  $data['trovequery']['tag'] = array(
    'title' => t('tag'),
    'help' => t('Tags associated with this item.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_include',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'tags',
      ),
    ),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_include',
      'help' => t('Limit results to tagged items.'),
      'label' => t('The item has tags?'),
      'use equal' => true,
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'has',
        'value' => 'tags',
      ),
    ),
  );

  //date
  $data['trovequery']['date'] = array(
    'title' => t('date'),
    'help' => t("Date of publication"),
    'field' => array(
      'handler' => 'trovequery_handler_field_date',
    ),
  );

  // comments date
  $data['trovequery']['comments_date'] = array(
    'title' => t('comments date'),
    'help' => t('Comments posted on this date.'),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_date',
      'help' => t('Limit results to items with comments posted in this date range.'),
      'label' => t('Comments date range'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'commentlastupdated',
      ),
    ),
  );

  // lastupdated
  $data['trovequery']['lastupdated'] = array(
    'title' => t('last updated'),
    'help' => t('The date the item was last updated.'),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_date',
      'help' => t('Limit results to items updated before, after or between certain dates'),
      'label' => t('Last updated date'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'lastupdated',
      ),
    ),
  );

  $data['trovequery']['relevance_sort'] = array(
    'title' => t('relevance'),
    'sort' => array(
      'handler' => 'trovequery_handler_sort_relevance',
      'help' => t('Sort results by relevance.'),
      'sort method' => 'sortby',
      'sort order' => 'relevance',
    ),
  );
  $data['trovequery']['date_sort'] = array(
    'title' => t('date sort'),
    'sort' => array(
      'handler' => 'trovequery_handler_sort_date',
      'help' => t('Sort results by relevance.'),
      'sort method' => 'sortby',
      'sort order' => 'date',
    ),
  );

  //comments
  $data['trovequery']['comment'] = array(
    'title' => t('comment'),
    'help' => t('Comments associated with this item.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_include',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'comments',
      ),
    ),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_include',
      'help' => t('Limit results to items with comments.'),
      'label' => t('The item has comments?'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'has',
        'value' => 'comments',
      ),
    ),
  );

  // trove url
  $data['trovequery']['trove_url'] = array(
    'title' => t('trove url'),
    'help' => t('The url of this result.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_url',
    ),
  );

  // query
  $data['trovequery']['query'] = array(
    'title' => t('query'),
    'help' => t('The query keywords'),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_query',
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
      ),
    ),
  );

  // facets - format
  $data['trovequery']['facet_article'] = array(
    'title' => t('article format'),
    'help' => t('The article format facet to filter on'),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_facet',
      'trovequery' => array(
        'arg' => 'l-format',
        'method' => 'result',
        'facets_method' => '_get_facets_format',
      ),
    ),
  );
  // facets - availability
  $data['trovequery']['facet_availaility'] = array(
    'title' => t('availability'),
    'help' => t('Filter on whether the item is online or not.'),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_facet',
      'trovequery' => array(
        'arg' => 'l-availability',
        'method' => 'result',
        'facets_method' => '_get_facets_availability',
      ),
    ),
  );
  // zone
  $data['trovequery']['zone'] = array(
    'title' => t('zone'),
    'help' => t('Filter results based on trove zones'),
    'field' => array(
      'handler' => 'trovequery_handler_field_title',
    ),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_zone',
      'trovequery' => array(
        'arg' => 'zone',
        'method' => 'result',
        'zones_method' => '_get_zones',
      ),
    ),
  );
  // contributor
  $data['trovequery']['contributor'] = array(
    'title' => t('contributor'),
    'help' => t('The contibutor of this work.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_contributor',
    ),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_contributor',
      'help' => t('Filter by contributor'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'nuc',
        'index_method' => '_get_index_contributors',
      ),
    ),
  );
  // id
  $data['trovequery']['tid'] = array(
    'title' => t('tid'),
    'help' => t('The Trove id of this work.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_tid',
    ),
  );
  // snippet
  $data['trovequery']['snippet'] = array(
    'title' => t('snippet'),
    'help' => t('A snippet of this work.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_snippet',
    ),
  );
  // category
  $data['trovequery']['category'] = array(
    'title' => t('category'),
    'help' => t('The category of this work.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_title',
    ),
    'filter' => array(
      'handler' => 'trovequery_handler_filter_facet',
      'trovequery' => array(
        'arg' => 'l-category',
        'method' => 'result',
        'facets_method' => '_get_facets_category',
      ),
    ),
  );
  // lists
  $data['trovequery']['lists'] = array(
    'title' => t('lists'),
    'help' => t('Lists associated with this item.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_include',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'lists',
      ),
    ),
  );
  // images
  $data['trovequery']['image'] = array(
    'title' => t('image'),
    'help' => t('The image for the Trove work - if available.'),
    'field' => array(
      'handler' => 'trovequery_handler_field_image',
    ),
  );
  // holdings: nuc
  $data['trovequery']['holdings_nuc'] = array(
    'title' => t('holdings:nuc'),
    'help' => t('The NUC symbol of an organisation with a copy of this work.'),
    'field'=> array(
      'handler' => 'trovequery_handler_field_string'
    ),
  );

  return $data;
}


