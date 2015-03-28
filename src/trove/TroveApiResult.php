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
    if($this->response) {
      $this->setTotalResults();
    }
    return $this->response;
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
    foreach($this->response['response']['zone'] as $zone) {
      $totalResults += $zone['records']['total'];
    }
  }

  /**
   * Get the total results.
   */
  public function getTotalResults() {
    return $this->totalResults;
  }

}
