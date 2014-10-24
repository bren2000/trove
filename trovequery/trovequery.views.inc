<?php
/**
 * @file
 * Definition of trovequery view.
 */

/**
 * Implements hook_views_plugins().
 */
function trovequery_views_plugins() {
  $plugin = array();
  $plugin['query']['TrovePluginQuery'] = array(
    'title' => t('Trove API Query'),
    'help' => t('Trove API query object.'),
    'handler' => 'TrovePluginQuery',
    'use pager' => TRUE,
  );
  return $plugin;
}

/**
 * Implements hook_views_data().
 */
function trovequery_views_data() {
  $data = array();

  $data['trovequery']['table']['group']  = t('Trove Results');
  $data['trovequery']['table']['base'] = array(
    'title' => t('Trove Results'),
    'help' => t('Query Trove.'),
    'field' => 'title',
    'query class' => 'TrovePluginQuery',
  );

  $data['trovequery']['title'] = array(
    'title' => t('title'),
    'help' => t('The title of this result.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldTitle',
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterField',
      'help' => t('Keyword filter on the title field.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'title',
      ),
    ),
  );

  $data['trovequery']['isbn'] = array(
    'title' => t('ISBN'),
    'help' => t('The ISBN for this result.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterIncludeString',
      'help' => t('Filter on ISBNs.'),
      'label' => t('The ISBN of the item.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'isbn',
      ),
    ),
  );

  $data['trovequery']['issn'] = array(
    'title' => t('ISSN'),
    'help' => t('The ISSN for this result.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterIncludeString',
      'help' => t('Filter on ISSNs.'),
      'label' => t('The ISSN of the item.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'issn',
      ),
    ),
  );

  $data['trovequery']['id'] = array(
    'title' => t('id'),
    'help' => t('The work number of this item.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterIncludeString',
      'help' => t('Filter on work number (ID).'),
      'label' => t('The work number of the item.'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'id',
      ),
    ),
  );

  $data['trovequery']['tag'] = array(
    'title' => t('tag'),
    'help' => t('Tags associated with this item.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'tags',
      ),
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterInclude',
      'help' => t('Limit results to tagged items.'),
      'label' => t('The item has tags?'),
      'use equal' => TRUE,
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'has',
        'value' => 'tags',
      ),
    ),
  );

  $data['trovequery']['date'] = array(
    'title' => t('date'),
    'help' => t("Date of publication"),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldDate',
    ),
  );

  $data['trovequery']['comments_date'] = array(
    'title' => t('comments date'),
    'help' => t('Comments posted on this date.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterDate',
      'help' => t('Limit results to items with comments posted in this date range.'),
      'label' => t('Comments date range'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'commentlastupdated',
      ),
    ),
  );

  $data['trovequery']['lastupdated'] = array(
    'title' => t('last updated'),
    'help' => t('The date the item was last updated.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterDate',
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
      'handler' => 'TrovequeryHandlerSortRelevance',
      'help' => t('Sort results by relevance.'),
      'sort method' => 'sortby',
      'sort order' => 'relevance',
    ),
  );
  $data['trovequery']['date_sort'] = array(
    'title' => t('date sort'),
    'sort' => array(
      'handler' => 'TrovequeryHandlerSortDate',
      'help' => t('Sort results by relevance.'),
      'sort method' => 'sortby',
      'sort order' => 'date',
    ),
  );

  $data['trovequery']['comment'] = array(
    'title' => t('comment'),
    'help' => t('Comments associated with this item.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'comments',
      ),
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterInclude',
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

  $data['trovequery']['trove_url'] = array(
    'title' => t('trove url'),
    'help' => t('The url of this result.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldUrl',
    ),
  );

  $data['trovequery']['query'] = array(
    'title' => t('query'),
    'help' => t('The query keywords'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterQuery',
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
      ),
    ),
  );

  $data['trovequery']['facet_article'] = array(
    'title' => t('article format'),
    'help' => t('The article format facet to filter on'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterFacet',
      'trovequery' => array(
        'arg' => 'l-format',
        'method' => 'result',
        'facets_method' => 'trove_get_facets_format',
      ),
    ),
  );

  $data['trovequery']['facet_availaility'] = array(
    'title' => t('availability'),
    'help' => t('Filter on whether the item is online or not.'),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterFacet',
      'trovequery' => array(
        'arg' => 'l-availability',
        'method' => 'result',
        'facets_method' => 'trove_get_facets_availability',
      ),
    ),
  );

  $data['trovequery']['zone'] = array(
    'title' => t('zone'),
    'help' => t('The trove zone this work belongs to'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldTitle',
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterZone',
      'help' => t('Filter results based on trove zones'),
      'trovequery' => array(
        'arg' => 'zone',
        'method' => 'result',
        'zones_method' => 'trove_get_zones',
      ),
    ),
  );

  $data['trovequery']['contributor'] = array(
    'title' => t('contributor'),
    'help' => t('The contibutor of this work.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldContributor',
    ),
  );

  $data['trovequery']['tid'] = array(
    'title' => t('tid'),
    'help' => t('The Trove id of this work.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldTid',
    ),
  );

  $data['trovequery']['snippet'] = array(
    'title' => t('snippet'),
    'help' => t('A snippet of this work.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldSnippet',
    ),
  );

  $data['trovequery']['category'] = array(
    'title' => t('category'),
    'help' => t('The category of this work.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldTitle',
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterFacet',
      'trovequery' => array(
        'arg' => 'l-category',
        'method' => 'result',
        'facets_method' => 'trove_get_facets_category',
      ),
    ),
  );

  $data['trovequery']['lists'] = array(
    'title' => t('lists'),
    'help' => t('Lists associated with this item.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldInclude',
      'trovequery' => array(
        'arg' => 'include',
        'value' => 'lists',
      ),
    ),
  );

  $data['trovequery']['image'] = array(
    'title' => t('image'),
    'help' => t('The image for the Trove work - if available.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldImage',
    ),
  );

  $data['trovequery']['holdings_nuc'] = array(
    'title' => t('holdings:nuc'),
    'help' => t('The NUC symbol of an organisation with a copy of this work.'),
    'field' => array(
      'handler' => 'TrovequeryHandlerFieldString',
    ),
    'filter' => array(
      'handler' => 'TrovequeryHandlerFilterHolding',
      'help' => t('Filter by NUC (ID of organisation who has a copy of this work).'),
      'trovequery' => array(
        'arg' => 'q',
        'method' => 'result',
        'index' => 'nuc',
        'index_method' => 'trove_get_index_contributors',
      ),
    ),
  );

  return $data;
}
