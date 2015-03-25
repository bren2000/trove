<?php
/**
 *  Represents a Trove Search
 *
 *  (see http://help.nla.gov.au/trove/building-with-trove/api-technical-guide#anchor-1)
 */

namespace  Drupal\trove\TroveApiResult;


class TroveApiResult extends TroveApi {

  /**
   * Make the request.
   */
  public function query() {
    return $this->call($this->params);
  }

  /**
   * Create the result object.
   */
  public function parse() {
    $results = array();
    $total_results = 0;

    if (isset($this->response['response'])) {
      foreach ($this->response['response']['zone'] as $zones) {
        switch ($zones['name']) {
          case 'people':
            $k = 'people';
            break;

          case 'newspaper':
            $k = 'article';
            break;

          case 'list':
            $k = 'list';
            break;

          default:
            $k = 'work';
        }
        if ($zones['records']['total'] != '0') {
          $total_results += $zones['records']['total'];
          foreach ($zones['records'][$k] as $res) {
            $row = new stdClass();
            if (isset($res['title'])) {
              if (is_array($res['title'])) {
                $title = $res['title']['value'];
              }
              else {
                $title = $res['title'];
              }
            }
            else {
              $title = NULL;
            }
            $row->title = $title;
            if (isset($res['contributor'])) {
              foreach ($res['contributor'] as $contrib) {
                $contributor = $contrib;
              }
            }
            else {
              $contributor = NULL;
            }
            if (isset($res['id'])) {
              $tid = $res['id'];
            }
            else {
              $tid = NULL;
            }
            $row->tid = $tid;
            $row->contributor = $contributor;

            if (isset($res['identifier'])) {
              foreach ($res['identifier'] as $identifier) {
                if (isset($identifier['linktype']) && $identifier['linktype'] == 'thumbnail') {
                  $row->image = $identifier['value'];
                }
              }
            }
            else {
              $row->image = NULL;
            }

            if (isset($res['tag'])) {
              $row->tag = implode(', ', array_map(function ($entry) {
                return $entry['value'];
              }, $res['tag']));
            }

            if (isset($res['list'])) {
              $row->list = implode(', ', array_map(function ($entry) {
                return $entry['value'];
              }, $res['list']));
            }

            if (isset($res['issued'])) {
              $row->date = $res['issued'];
            }

            if (isset($res['comment'])) {
              $row->comment = implode(', ', array_map(function ($entry) {
                return $entry['value'];
              }, $res['comment']));
            }

            if (isset($res['snippet'])) {
              $row->snippet = $res['snippet'];
            }

            if (isset($res['category'])) {
              $row->category = $res['category'];
            }

            if (isset($res['troveUrl'])) {
              $row->trove_url = $res['troveUrl'];
            }

            if (isset($res['isbn'])) {
              $row->isbn = $res['isbn'];
            }

            if (isset($res['issn'])) {
              $row->issn = $res['issn'];
            }

            if (isset($res['id'])) {
              $row->id = $res['id'];
            }

            if (isset($res['holding'])) {
              $row->holdings_nuc = implode(', ', array_map(function ($entry) {
                if (isset($entry['nuc'])) {
                  return $entry['nuc'];
                }
              }, $res['holding']));
            }

            $row->zone = $zones['name'];
            $results[] = $row;
          }
        }
      }
    }
    $this->setTotalResults($total_results);
    return $results;
  }

  /**
   * Set the total results.
   */
  public function setTotalResults($results = 0) {
    $this->total_results = $results;
  }

  /**
   * Get the total results.
   */
  public function getTotalResults() {
    return (int) $this->total_results;
  }

}
