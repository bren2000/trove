<?php
/**
 * @file
 * TroveApiNewspaperTitle class for represnting Trope API newspaper title operations.
 */

namespace  Drupal\trove;

/**
 * TroveAPINewspaperTitle class.
 *
 * Use TroveApi::factory(operation) to get a request object. All
 * public methods return $this and can be chained together.
 */
class TroveApiNewspaperTitle extends TroveApi {

  /**
   * {@inheritdoc}
   */
  public function query() {
    $this->call($this->params);
    if ($this->response && $this->params['method'] == 'newspaper/titles') {
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
    $this->totalResults = $this->response['response']['records']['total'];
  }

  /**
   * Getter method for $totalResultse.
   */
  public function getTotalResults() {
    return $this->totalResults;
  }

}
