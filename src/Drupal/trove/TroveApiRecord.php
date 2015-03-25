<?php
/**
 *  Represents a Trove metadata record
 *
 *  (see http://help.nla.gov.au/trove/building-with-trove/api-technical-guide#anchor-2)
 */

namespace  Drupal\trove\TroveApiRecord;

class TroveApiRecord extends TroveApi {

  /**
   * Make the request.
   */
  public function query() {
    return $this->call($this->params);
  }

  /**
   * Create the result object
   */
  protected function parse() {}
}
