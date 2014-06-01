<?php

/**
 * Trove API class.
 *
 * Use troveAPI::factory(operation) to get a request object. All
 * public methods return $this and can be chained together.
 */

class troveAPI {

  public $response;

  protected $trove_base_url;

  protected $api_key;

  protected $method;

  // API parameter, use troveAPI->setFilter() to set.
  protected $params = array();

  // Flag to designate if the $response is from a cache.
  public $cache;

  // The cache table to use.
  protected $cache_table = 'cache';

  protected $total_results;

  /**
   * Factory method
   *
   * @param $method
   *   The Trove API operation to build a request for.
   *
   * @return a new Trove object
   */
  static function factory($base_table) {
    switch ($base_table) {
      case 'trovequery':
        return new troveAPIResult('result');
      case 'work':
      case 'newspaper':
      case 'troveitem':
        return new troveAPIRecord($base_table);
      case 'trovecontrib':
        return new troveAPIContributor('contributor');
    }
  }

  /**
   * Constructor, please use the factory.
   */
  function __construct($method) {
    $this->setFilter('method', $method);
    $this->trove_base_url = TROVE_BASE_URL;
    $this->api_key = variable_get('trove_api_key', '');
  }

  /**
   * Add a filter to the request. Classes that extend this one should declare
   * what $keys are accepted.
   *
   * @param $key
   *   The filter to set
   * @param $value
   *   The value to set for the $key.
   *
   * @return the troveAPI object
   */
  public function setFilter($key, $value) {
    if(array_key_exists($key, $this->params) && $key !== 'method') {
      //dpm('this key exists' . $key . ' : ' . $value);
      $this->params[$key] .= (' ' .$value);
    } else {
      //dpm($key . ' : ' . $value);
      $this->params[$key] = $value;
    }
    return $this;
  }


  function call($arguments) {
    //dpm($arguments);
    foreach ($arguments as $key => $value) {
      if (is_null($value))
        unset($arguments[$key]);
    }
    $this->request($this->params['method'], $arguments);
    return $this->response ? $this->response : false;
  }

  function execute($request_url) {
    $options = array(
      'timeout' => 5.0,
    );

    $response = drupal_http_request($request_url, $options);

    if ($response->code == '200') {
      $data = json_decode($response->data, TRUE);
      if (is_array($data)) {
        if (isset($data->error)) {
          watchdog('error', "Trove error !code recieved: %message", array('!code' => $data->error, '%mesage' => $data->message));
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

  function request($command, $args = array()) {
    //dpm($this->params);
    unset($args['method']);
    $args = array_merge(array("encoding" => "json", "key" => $this->api_key), $args);

    foreach ($args as $key => $data) {
      if ( is_null($data) ) {
        unset($args[$key]);
        continue;
      } else {
        $args[$key] = trim($data);
      }
    }

    $request_url = url($this->trove_base_url . $command , array('query' => $args, 'absolute' => TRUE));

    // Check if we have a cache hit or not.
    dpm($request_url);
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
   * The caching model implemented here is somewhat different than drupal's
   * normal model because the data in this cache cannot be regenerated locally.
   *
   * Additionally we wait to avoid making repeated failed requests to the API in
   * the case where it's either down, or a invalid query has been fomulated.
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
      $items[$cid] = cache_get($cid, $this->cache_table);
      if (cache_get($cid, $this->cache_table) == FALSE)
        return FALSE;
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
      cache_set($this->cache_id($url), FALSE, $this->cache_table, CACHE_TEMPORARY);
    }
    else {
      $ttl = (int)variable_get('trove_cache_duration', 900);
      $expire = time() + $ttl;
      cache_set($this->cache_id($url), $data, $this->cache_table, $expire);
    }
  }

  /**
   * Helper function to generate a cache id based on the class name and
   * hash of the url
   */
  protected function cache_id($request_url) {
    return get_class($this) .':'. md5($request_url);
  }

  function query() {}

  /**
   * Custom parser for the type of data we're retrieving.
   * Must be implemented by all subclasses. Must return an
   * indexed array of results, each result being an array keyed
   * by the field name.
   */
  protected function parse($joins) {}

}

class troveAPIResult extends troveAPI {

  function query() {
    return $this->call($this->params);
  }

  function parse() {
    $results = array();
    $total_results = 0;

    if (isset($this->response['response'])) {
      foreach($this->response['response']['zone'] as $zones) {
        switch($zones['name']) {
          case 'people':
            $k = 'people'; break;
          case 'newspaper':
            $k = 'article'; break;
          case 'list':
            $k = 'list'; break;
          default:
            $k = 'work';
        }
        if ($zones['records']['total'] != '0') {
          $total_results += $zones['records']['total'];
          foreach($zones['records'][$k] as $res) {
            $row = new stdClass;
            // title
            if (isset($res['title'])) {
              if(is_array($res['title'])) {
                $title = $res['title']['value'];
              } else {
                $title = $res['title'];
              }
            } else {
              $title = '[title not set]';
            }
            $row->title = $title;
            // contributor
            if (isset($res['contributor'])) {
              foreach($res['contributor'] as $contrib) {
                $contributor = $contrib;
              }
            } else {
              $contributor = '[contributor not set]';
            }
            if (isset($res['id'])) {
              $tid = $res['id'];
            } else {
              $tid = '[not set]';
            }
            $row->tid = $tid;
            $row->contributor = $contributor;

            // image
            if(isset($res['identifier'])) {
              foreach ($res['identifier'] as $identifier) {
                if($identifier['linktype'] == 'thumbnail') {
                  $row->image = $identifier['value'];
                }
              }
            } else {
              $row->image = NULL;
            }

            // tag
            if(isset($res['tag'])) {
              foreach($res['tag'] as $k) {
                $row->tag = $row->tag . $k['value'] . ', ';
              }
              $row->tag = chop($row->tag, ', ');
            }
            // issued (date)
            if (isset($res['issued'])) {
              //foreach($res['issued'] as $k) {
              //  $row->date = $row->date . $k['value'] . ', ';
             // }
              //$row->date = chop($row->date, ', ');
              $row->date = $res['issued'];
            }
            // comments
            if(isset($res['comment'])) {
              foreach($res['comment'] as $k) {
                $row->comment = $row->comment . $k['value'] . ', ';
              }
              $row->comment = chop($row->comment, ', ');
            }
            // snippets
            if(isset($res['snippet'])) {
                $row->snippet = $res['snippet'];
            }
            // category
            if(isset($res['category'])) {
                $row->category = $res['category'];
            }
            // trove url
            if(isset($res['troveUrl'])) {
              $row->trove_url = $res['troveUrl'];
            }
            // isbn
            if (isset($res['isbn'])) {
              $row->isbn = $res['isbn'];
            }
            // issn
            if (isset($res['issn'])) {
              $row->issn = $res['issn'];
            }
            // id
            if (isset($res['id'])) {
              $row->id = $res['id'];
            }
            $row->zone = $zones['name'];

            $results[] = $row;
          }
        }
      }
    }
    $this->set_total_results($total_results);
    return $results;
  }

  function set_total_results($results = 0) {
    $this->total_results = $results;
  }

  function get_total_results() {
    return (int) $this->total_results;
  }

}

class troveAPIRecord extends troveAPI {

  function query() {
    return $this->call($this->params);
  }
}

class troveAPIContributor extends troveAPI {

  function __construct($method) {
    parent::__construct($method);
  }

  function query() {
    return $this->call($this->params);
  }

  function parse() {
    $contributors = array();
    foreach($this->response['response']['contributor'] as $contributor) {
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
