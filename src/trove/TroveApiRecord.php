<?php
/**
 *  Represents a Trove metadata record
 *
 *  (see http://help.nla.gov.au/trove/building-with-trove/api-technical-guide#anchor-2)
 */

namespace  Drupal\trove;

class TroveApiRecord extends TroveApi {

  /**
   * Make the request.
   */
  public function query() {
    $this->call($this->params);
    return $this->response ? $this->response : FALSE;
  }

  /**
   * Create the result object
   */
  public function parse() {
    return $this->response['article'];
  }
}
