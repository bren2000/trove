<?php

namespace  Drupal\trove;


class TroveApiContributor extends TroveApi {

  /**
   * Constructor.
   */
  public function __construct($method) {
    parent::__construct($method);
  }

  /**
   * Make the request.
   */
  public function query() {
    $this->call($this->params);
    $this->setTotalResults();
    return $this->response ? $this->response : FALSE;
  }

  /**
   * Create the result object.
   *
   * @return array
   *  An array of  contributors
   */
  public function parse() {
    return $this->response['response']['contributor'];
  }

  /**
   * Set the total results.
   */
  public function setTotalResults() {
    $this->totalResults = count($this->response['response']['contributor']);
  }

  /**
   * Get the total results.
   */
  public function getTotalResults() {
    return $this->totalResults;
  }

}
