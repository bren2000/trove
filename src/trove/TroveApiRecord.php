<?php
/**
 * @file
 * TroveApiRecord class for represnting Trope API record operations.
 */

namespace  Drupal\trove;

/**
 * TroveAPIRecord class.
 *
 * Use TroveApi::factory(operation) to get a request object. All
 * public methods return $this and can be chained together.
 */
class TroveApiRecord extends TroveApi {

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
    return $this->response['article'];
  }

}
