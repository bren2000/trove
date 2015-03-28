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
    return $this->response ? $this->response : FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function parse() {
    return $this->response['newspaper'];
  }

}
