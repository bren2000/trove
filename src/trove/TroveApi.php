<?php
/**
 * @file
 * Abstract TroveApi base class for sub-classes represnting API operations.
 */

namespace  Drupal\trove;

/**
 * TroveAPI class.
 *
 * Use TroveApi::factory(operation) to get a request object. All
 * public methods return $this and can be chained together.
 */
abstract class TroveApi {

  // The response object.
  protected $response;

  // Base URL of the trove api service.
  protected $troveBaseUrl;

  // Users API key.
  protected $apiKey;

  // Reference to the trove api service method, e.g 'contributors'.
  protected $method;

  // API parameter, use TroveApi->set_filter() to set.
  protected $params = array();

  // Flag to designate if the $response is from a cache.
  public $cache;

  // The cache table to use.
  protected $cacheTable = 'cache';

  // Property holding total results returned from api call.
  protected $totalResults;

  /**
   * Factory method.
   *
   * @param string $op
   *   The Trove API operation to build a request for.
   *
   * @return object TroveApi
   *   The TroveApi object.
   */
  public static function factory($op) {
    switch ($op) {
      case TROVE_RESULT:
        return new TroveApiResult('result');

      case TROVE_RECORD_WORK:
      case TROVE_RECORD_NEWSPAPER:
      case TROVE_RECORD_LIST:
        return new TroveApiRecord($op);

      case TROVE_NEWSPAPER_TITLE:
        return new TroveApiNewspaperTitle($op);

      case TROVE_NEWSPAPER_TITLES:
        return new TroveApiNewspaperTitle($op);

      case TROVE_CONTRIBUTOR:
        return new TroveApiContributor($op);

    }
  }
  /**
   * Constructor. Use the factory method TroveApi objects.
   */
  public function __construct($method) {
    $this->setFilter('method', $method);
    $this->troveBaseUrl = TROVE_BASE_URL;
    $this->apiKey = variable_get('trove_api_key', '');
  }

  /**
   * Add an ID to the request.
   *
   * @param string $id
   *   The filter to set.
   *
   * @return object TroveApi
   *    the TroveApi object
   */
  public function setId($id) {
    $this->params['id'] = $id;
    return $this;
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
  public function setFilter($key, $value) {
    if (array_key_exists($key, $this->params) && $key !== 'method') {
      switch ($key) {
        case 'include':
        case 'zone':
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
    if (isset($this->params['id'])) {
      $_command = $this->params['method'] . '/' . $this->params['id'];
    }
    else {
      $_command = $this->params['method'];
    }
    $this->request($_command, $arguments);
    return $this->response ? $this->response : FALSE;
  }

  /**
   * Make the request.
   *
   * The results will be set on the `response` attribute.
   *
   * @return object TroveApiRequest
   *   the TroveApiRequest object
   */
  protected function execute($request_url) {
    $options = array(
      'timeout' => variable_get('trove_timeout', 3.0),
    );

    $response = drupal_http_request($request_url, $options);

    if ($response->code == '200') {
      $data = json_decode($response->data, TRUE);
      if (is_array($data)) {
        return $data;
      }
      else {
        $this->troveSetError($response->code, $response->status_message);
      }
    }
    else {
      $this->troveSetError($response->code,
        isset($response->status_message) ? $response->status_message : $response->error);
    }
    return FALSE;
  }

  /**
   * Make the actual HTTP request and parse output.
   *
   * @param string $command
   *   The Trove API operation to build a request for.
   * @param array $args
   *   An array of request parameters.
   *
   * @return object TroveApi
   *   The TroveApi object.
   */
  public function request($command, $args = array()) {
    unset($args['method']);
    unset($args['id']);
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
    if ($result = $this->cacheGet($request_url)) {
      $this->response = $result->data;
      $this->cache = TRUE;
    }
    elseif ($this->response = $this->execute($request_url)) {
      // Set custom cache expire for contributor & titles calls with no params.
      if (($this->params['method'] = 'newspaper/titles' ||
           $this->params['method'] = 'contributor') &&
          count($this->params) == 1) {
        $this->cacheSet($request_url, $this->response, 86400);
      }
      else {
        $this->cacheSet($request_url, $this->response);
      }
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
  protected function cacheGet($request_url, $reset = FALSE) {
    static $items = array();
    $cid = $this->cacheId($request_url);
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
   *
   * @param string $url
   *   The API url that will be used for the cache id.
   * @param mixed $data
   *   The response data to cache.
   * @param int $expires
   *   A time in seconds to overide the default timeout.
   */
  protected function cacheSet($url, $data, $expires = NULL) {
    if ($data === FALSE) {
      // If we don't get a response we set a temporary cache to prevent hitting
      // the API frequently for no reason.
      cache_set($this->cacheId($url), FALSE, $this->cacheTable, CACHE_TEMPORARY);
    }
    else {
      if ($expires == NULL) {
        $ttl = (int) variable_get('trove_cache_duration', 900);
      }
      else {
        $ttl = (int) $expires;
      }
      $expire = time() + $ttl;
      cache_set($this->cacheId($url), $data, $this->cacheTable, $expire);
    }
  }

  /**
   * Display an error message to trove admins and write an error to watchdog.
   *
   * @param string $message
   *   Message or error response to display.
   */
  protected function troveSetError($code, $message) {
    $new_message = t('Trove error @error_id: %trove_error', array(
      '@error_id' => $code,
      '%trove_error' => $message,
    ));

    if (user_access('administer trove')) {
      drupal_set_message($new_message, 'error');
    }
    watchdog('trove', 'Trove error @code: %message', array('%message' => $message, '@code' => $code), WATCHDOG_WARNING);
  }

  /**
   * Get a cache id.
   *
   * Helper function to generate a cache id based on class name & hash of url.
   *
   * @param string $request_url
   *   The full trove API requets URL.
   */
  protected function cacheId($request_url) {
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
  abstract public function parse();

}
