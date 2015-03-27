<?php
/**
 *  Represents a Trove Search
 *
 *  (see http://help.nla.gov.au/trove/building-with-trove/api-technical-guide#anchor-1)
 */

namespace  Drupal\trove;


class TroveApiResult extends TroveApi {

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
   */
  public function parse() {
    return $this->response['response'];
  }

  /**
   * Set the total results.
   */
  public function setTotalResults() {
    $_total = 0;
    foreach($this->response['response']['zone'] as $zone) {
      $_total += $zone['records']['total'];
    }
    $this->totalResults = $_total;
  }

  /**
   * Get the total results.
   */
  public function getTotalResults() {
    return $this->totalResults;
  }

}
