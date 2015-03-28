<?php
/**
 * @file
 * TroveApiRecord class for represnting Trope API search operations.
 */

namespace  Drupal\trove;

/**
 * TroveAPIResult class.
 *
 * Use TroveApi::factory(operation) to get a request object. All
 * public methods return $this and can be chained together.
 */
class TroveApiResult extends TroveApi {

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->call($this->params);
    if ($this->response) {
      $this->setTotalResults();
    }
    return $this->response;
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
    foreach ($this->response['response']['zone'] as $zone) {
      $this->totalResults += $zone['records']['total'];
    }
  }

  /**
   * Getter method for $totalResultse.
   */
  public function getTotalResults() {
    return $this->totalResults;
  }

}
