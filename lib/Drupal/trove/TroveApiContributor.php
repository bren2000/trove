<?php

class TroveApiContributor extends TroveApi {

  /**
   * Constructor.
   */
  public function __construct($method) {
    parent::__construct($method);
  }

  /**
   * Make the request.
   */
  public function query() {
    return $this->call($this->params);
  }

  /**
   * Create the result object.
   *
   * @return stdClass[]
   *  An array of  stClass objetcs representing a contributor, including
   *  name, id, and an array of nuc identifiers
   */
  public function parse() {
    $contributors = array();
    foreach ($this->response['response']['contributor'] as $contributor) {
      $cont = new stdClass();
      $cont->name = $contributor['name'];
      $cont->id = $contributor['id'];
      if (isset($contributor['nuc'])) {
        $cont->nuc  = $contributor['nuc'];
      }
      $contributors[] = $cont;
    }
    dd($contributors);
    return $contributors;
  }

}
