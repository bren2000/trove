<?php

/**
 * Trove API class.
 *
 * Use TroveAPI::factory(operation) to get a request object. All
 * public methods return $this and can be chained together.
 */

class TroveAPI {

  public $response;

  protected $troveBaseUrl;

  protected $apiKey;

  protected $method;

  // API parameter, use troveAPI->set_filter() to set.
  protected $params = array();

  // Flag to designate if the $response is from a cache.
  public $cache;

  // The cache table to use.
  protected $cacheTable = 'cache';

  protected $totalResults;

  /**
   * Factory method.
   *
   * @param string $base_table
   *   The Trove API operation to build a request for.
   *
   * @return 
   *   a new Trove object
   */
  public static function factory($base_table) {
    switch ($base_table) {
      case 'trovequery':
        return new TroveAPIResult('result');

      case 'work':
      case 'newspaper':
      case 'troveitem':
        return new TroveAPIRecord($base_table);

      case 'trovecontrib':
        return new TroveAPIContributor('contributor');
    }
  }
  /**
   * Constructor. Use the factory method.
   */
  public function __construct($method) {
    $this->set_filter('method', $method);
    $this->troveBaseUrl = troveBaseUrl;
    $this->apiKey = variable_get('trove_apiKey', '');
  }

  /**
   * Add a filter to the request. 
   * Classes that extend this one should declare what $keys are accepted.
   *
   * @param string $key
   *   The filter to set
   * @param string $value
   *   The value to set for the $key.
   *
   * @return
   *    the TroveAPI object
   */
  public function set_filter($key, $value) {
    if (array_key_exists($key, $this->params) && $key !== 'method') {
      switch ($key) {
        case 'include':
          $this->params[$key] .= (',' . $value);
          break;
        default:
          $this->params[$key] .= (' ' . $value);
          break;
      }
    }
    else {
      $this->params[$key] = $value;
    }
    return $this;
  }


  function call($arguments) {
    foreach ($arguments as $key => $value) {
      if (is_null($value)) {
        unset($arguments[$key]);
      }
    }
    $this->request($this->params['method'], $arguments);
    return $this->response ? $this->response : false;
  }

  /**
   * Make the request. The results of the call will be set on the `response`
   * attribute.
   *
   * @return the TroveAPIRequest object
   */
  function execute($request_url) {
    dpm($request_url);
    $options = array(
      'timeout' => 5.0,
    );

    $response = drupal_http_request($request_url, $options);

    if ($response->code == '200') {
      $data = json_decode($response->data, TRUE);
      if (is_array($data)) {
        if (isset($data->error)) {
          watchdog('error', "Trove error !code received: %message", array('!code' => $data->error, '%mesage' => $data->message));
        }
        else {
          return $data;
        }
      }
      else {
        watchdog('error', "Didn't receive valid API response (invalid JSON).");
      }
    }
    else {
      watchdog('error', 'HTTP error !code received', array('!code' => $response->code));
    }
    return FALSE;
  }

  /**
   * Make the actual HTTP request and parse output
   */
  function request($command, $args = array()) {
    unset($args['method']);
    $args = array_merge(array("encoding" => "json", "key" => $this->apiKey), $args);

    foreach ($args as $key => $data) {
      if ( is_null($data) ) {
        unset($args[$key]);
        continue;
      }
      else {
        $args[$key] = trim($data);
      }
    }

    $request_url = url($this->troveBaseUrl . $command, array('query' => $args, 'absolute' => TRUE));

    // Check if we have a cache hit or not.
    if ($result = $this->cache_get($request_url)) {
      $this->response = $result->data;
      $this->cache = TRUE;
    }
    else {
      $this->response = $this->execute($request_url);
      $this->cache_set($request_url, $this->response);
      $this->cache = FALSE;
    }
    return $this;
  }

  /**
   * Populate the cache. Wrapper around Drupal's cache_get()
   *
   * @param $url
   *   The API url that would be used.
   * @param $reset
   *   Set to TRUE to force a retrieval from the database.
   */
  protected function cache_get($request_url, $reset = FALSE) {
    static $items = array();
    $cid = $this->cache_id($request_url);
    if (!isset($items[$cid]) || $reset) {
      $items[$cid] = cache_get($cid, $this->cacheTable);
      if (cache_get($cid, $this->cacheTable) == FALSE) {
        return FALSE;
      }
      // Don't return temporary items more that 5 minutes old.
      if ($items[$cid]->expire === CACHE_TEMPORARY && $items[$cid]->created > (time() + 300)) {
        return FALSE;
      }
    }
    return $items[$cid];
  }

  /**
   * Retrieve the cache. Wrapper around Drupal's cache_set()
   */
  protected function cache_set($url, $data) {
    if ($data === FALSE) {
      // If we don't get a response we set a temporary cache to prevent hitting
      // the API frequently for no reason.
      cache_set($this->cache_id($url), FALSE, $this->cacheTable, CACHE_TEMPORARY);
    }
    else {
      $ttl = (int) variable_get('trove_cache_duration', 900);
      $expire = time() + $ttl;
      cache_set($this->cache_id($url), $data, $this->cacheTable, $expire);
    }
  }

  /**
   * Helper function to generate a cache id based on the class name and
   * hash of the url
   */
  protected function cache_id($request_url) {
    return get_class($this) . ':' . md5($request_url);
  }

  function query() { }

  /**
   * Custom parser for the type of data we're retrieving.
   * Must be implemented by all subclasses. Must return an
   * indexed array of results, each result being an array keyed
   * by the field name.
   */
  protected function parse($joins) { }

}

class troveAPIResult extends troveAPI {

  function query() {
    return $this->call($this->params);
  }

  function parse($joins) {
    $results = array();
    $totalResults = 0;

    if (isset($this->response['response'])) {
      foreach ($this->response['response']['zone'] as $zones) {
        switch ($zones['name']) {
          case 'people':
            $k = 'people';
            break;
          case 'newspaper':
            $k = 'article';
            break;
          case 'list':
            $k = 'list';
            break;
          default:
            $k = 'work';
        }
        if ($zones['records']['total'] != '0') {
          $totalResults += $zones['records']['total'];
          foreach ($zones['records'][$k] as $res) {
            dpm($res);
            $row = new stdClass;
            if (isset($res['title'])) {
              if (is_array($res['title'])) {
                $title = $res['title']['value'];
              }
              else {
                $title = $res['title'];
              }
            }
            else {
              $title = NULL;
            }
            $row->title = $title;
            if (isset($res['contributor'])) {
              foreach ($res['contributor'] as $contrib) {
                $contributor = $contrib;
              }
            }
            else {
              $contributor = NULL;
            }
            if (isset($res['id'])) {
              $tid = $res['id'];
            }
            else {
              $tid = NULL;
            }
            $row->tid = $tid;
            $row->contributor = $contributor;

            if (isset($res['identifier'])) {
              foreach ($res['identifier'] as $identifier) {
                if (isset($identifier['linktype']) && $identifier['linktype'] == 'thumbnail') {
                  $row->image = $identifier['value'];
                }
              }
            }
            else {
              $row->image = NULL;
            }

            if (isset($res['tag'])) {
              $row->tag = implode(', ', array_map(function ($entry) {
                return $entry['value'];
              }, $res['tag'] ));
            }

            if (isset($res['list'])) {
              $row->list = implode(', ', array_map(function ($entry) {
                return $entry['value'];
              }, $res['list'] ));
            }

            if (isset($res['issued'])) {
              $row->date = $res['issued'];
            }

            if (isset($res['comment'])) {
              $row->comment = implode(', ', array_map(function ($entry) {
                return $entry['value'];
              }, $res['comment'] ));
            }

            if (isset($res['snippet'])) {
              $row->snippet = $res['snippet'];
            }

            if (isset($res['category'])) {
              $row->category = $res['category'];
            }

            if (isset($res['troveUrl'])) {
              $row->trove_url = $res['troveUrl'];
            }

            if (isset($res['isbn'])) {
              $row->isbn = $res['isbn'];
            }

            if (isset($res['issn'])) {
              $row->issn = $res['issn'];
            }

            if (isset($res['id'])) {
              $row->id = $res['id'];
            }

            if (isset($res['holding'])) {
              foreach ($res['holding'] as $k) {
                $row->holdings_nuc[] = $k['nuc'];         
              }
            }

            $row->zone = $zones['name'];

            $results[] = $row;
          }
        }
      }
    }
    $this->setTotalResults($totalResults);
    return $results;
  }

  function setTotalResults($results = 0) {
    $this->totalResults = $results;
  }

  public function getTotalResults() {
    return (int) $this->totalResults;
  }

}

class TroveAPIRecord extends troveAPI {

  function query() {
    return $this->call($this->params);
  }
}

class TroveAPIContributor extends troveAPI {

  function __construct($method) {
    parent::__construct($method);
  }

  function query() {
    return $this->call($this->params);
  }

  function parse($joins) {
    $contributors = array();
    foreach ($this->response['response']['contributor'] as $contributor) {
      $cont = new stdClass;
      $cont->name = $contributor['name'];
      $cont->id = $contributor['id'];
      if ($contributor['nuc'] != '') {
        $cont->nuc  = $contributor['nuc'];
      }
      $contributors[] = $cont;
    }
    return $contributors;
  }

}
