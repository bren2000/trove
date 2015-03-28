<?php
/**
 * @file
 * TroveApiContributor class for represnting Trope API contributor operations.
 */

/**
 * TroveAPIContributor class.
 *
 * Use TroveApi::factory(operation) to get a request object. All
 * public methods return $this and can be chained together.
 */

namespace  Drupal\trove;

class TroveApiContributor extends TroveApi {

  /**
   * Constructor.
   */
  public function __construct($method) {
    parent::__construct($method);
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->call($this->params);
    $this->setTotalResults();
    return $this->response ? $this->response : FALSE;
  }

  /**
  * {@inheritdoc}
  */
  public function parse() {
    return $this->response['response']['contributor'];
  }

  /**
   * Setter method for $totalResults
   */
  public function setTotalResults() {
    $this->totalResults = count($this->response['response']['contributor']);
  }

  /**
   * Getter method for $totalResults
   */
  public function getTotalResults() {
    return $this->totalResults;
  }

}
