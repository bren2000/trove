<?php
/**
 * Trove API class.
 *
 * Use TroveApi::factory(operation) to get a request object. All
 * public methods return $this and can be chained together.
 */
namespace  Drupal\trove;

abstract class TroveApi {

  protected $response;

  protected $troveBaseUrl;

  protected $apiKey;

  protected $method;

  // API parameter, use TroveApi->set_filter() to set.
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
   * @return object TroveApi
   *   The TroveApi object.
   */
  public static function factory($base_table) {
    switch ($base_table) {
      case 'trovequery':
        return new TroveApiResult('result');

      case 'work':
      case 'newspaper':
      case 'troveitem':
        return new TroveApiRecord($base_table);

      case 'trovecontrib':
        return new TroveApiContributor('contributor');
    }
  }
  /**
   * Constructor. Use the factory method.
   */
  public function __construct($method) {
    $this->set_filter('method', $method);
    $this->troveBaseUrl = TROVE_BASE_URL;
    $this->apiKey = variable_get('trove_api_key', '');
  }

  /**
   * Add a filter to the request.
   *
   * Classes that extend this one should declare what $keys are accepted.
   *
   * @param string $key
   *   The filter to set.
   * @param string $value
   *   The value to set for the $key.
   *
   * @return object TroveApi
   *    the TroveApi object
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

  /**
   * Check the arguments then make the request.
   *
   * @return object TroveApiRequest
   *   the TroveApiRequest object
   */
  protected function call($arguments) {
    foreach ($arguments as $key => $value) {
      if (is_null($value)) {
        unset($arguments[$key]);
      }
    }
    $this->request($this->params['method'], $arguments);
    return $this->response ? $this->response : FALSE;
  }

  /**
   * Make the request. The results will be set on the `response` attribute.
   *
   * @return object TroveApiRequest
   *   the TroveApiRequest object
   */
  protected function execute($request_url) {
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
   * Make the actual HTTP request and parse output.
   */
  public function request($command, $args = array()) {
    unset($args['method']);
    $args = array_merge(array("encoding" => "json", "key" => $this->apiKey), $args);

    foreach ($args as $key => $data) {
      if (is_null($data)) {
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
   * Populate the cache. Wrapper around Drupal's cache_get().
   *
   * @param string $request_url
   *   The API url that will be used.
   * @param bool $reset
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
   * Retrieve the cache. Wrapper around Drupal's cache_set().
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
   * Helper function to generate a cache id based on class name & hash of url.
   */
  protected function cache_id($request_url) {
    return get_class($this) . ':' . md5($request_url);
  }

  /**
   * Abstract method query. Implement in sub-classes.
   */
  abstract public function query();

  /**
   * Abstract custom parser for the type of data we're retrieving.
   *
   * Must be implemented by all subclasses. Returns an indexed array
   * of results, each result being an array keyed by the field name.
   */
  abstract protected function parse();

}
