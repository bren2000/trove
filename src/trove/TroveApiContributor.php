<?php
/**
 * @file
 * TroveApiContributor class for represnting Trope API contributor operations.
 */

namespace  Drupal\trove;

/**
 * TroveAPIContributor class.
 *
 * Use TroveApi::factory(operation) to get a request object. All
 * public methods return $this and can be chained together.
 */
class TroveApiContributor extends TroveApi {

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->call($this->params);
    if (!isset($this->params['id'])) {
      $this->setTotalResults();
    }
    return $this->response ? $this->response : FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function parse() {
    return $this->response;
  }

  /**
   * Setter method for $totalResults.
   */
  public function setTotalResults() {
    $this->totalResults = count($this->response['response']['contributor']);
  }

  /**
   * Getter method for $totalResults.
   */
  public function getTotalResults() {
    return $this->totalResults;
  }

}
