INTRODUCTION
------------
The Trove module provides an simple API around the National Library of
Australia Trove API service.
 * For a full description of the module, visit the project page:
   https://drupal.org/project/trove
 * For more information on Trove, visit http://trove.nla.gov.au

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
 * Visit https://trove.nla.gov.au/signup to sign up as a registered user. Once
   registered you can request a free API key.
 * Once the Trove module is enabled, visit admin/config/services/trove and
   enter your API key.
 * For more information on using the Trove API, visit
   http://help.nla.gov.au/trove/building-with-trove/api

USAGE
-----
Trove API methods are described on the Trove API help site. This Trove module
provides wrapper functions around those methods. These are defined in a set of
constants:
 * TROVE_RESULT
   Method for Trove API search.
 * TROVE_CONTRIBUTOR
   Method for searching and retrieving full list of trove contributors.
 * TROVE_RECORD_NEWSPAPER
   Method for rerieving information about a newspaper work.
 * TROVE_RECORD_WORK
   Method for rerieving information about a general work.
 * TROVE_RECORD_LIST
   Method for rerieving information about a list.
 * TROVE_NEWSPAPER_TITLE
   Method for rerieving information about a newspaper title.
 * TROVE_NEWSPAPER_TITLES
   Method for searching information about newspaper titles.

Examples

Getting a list of all Trove contributors:
<?php
use Drupal\trove\TroveApi;
use Drupal\trove\TroveApiContributor;

$contributors = TroveAPI::factory(TROVE_CONTRIBUTOR);
$contributors->query();
$list = $contributors->parse();
?>

Getting a result set for items in the Trove books zone with the keyword query "fish":
<?php
use Drupal\trove\TroveApi;
use Drupal\trove\TroveApiQuery;

$my_search = TroveAPI::factory(TROVE_RESULT);
$my_search->set_filter('zone', 'book')
  ->set_filter('q', 'fish')
  ->query();
$list = $my_search->parse();
?>

Get the list of years available for the <em>Sydney Morning Herald </em>and get the dates it was issued in 1926 and 1927:
<?php
use Drupal\trove\TroveApi;
use Drupal\trove\TroveApiQueryNewspaperTitle;

$title = TroveAPI::factory(TROVE_NEWSPAPER_TITLE);
$title->setId('35')
  ->setFilter('include', 'years')
  ->setFilter('range', '19260101-19271231')
  ->query();
$list = $title->parse();
?>
Get the full record for a digitised newspaper article:
<?php
use Drupal\trove\TroveApi;
use Drupal\trove\TroveApiQueryRecord;

$newspaper = TroveAPI::factory(TROVE_RECORD_NEWSPAPER);
$newspaper->setId('18342701')
  ->setFilter('include', 'articletext')
  ->setFilter('reclevel', 'full');
  ->query();
$list = $newspaper->parse();
?>
All public Trove module API methods can be chained.

Other utilities offered by the Trove module include:
<?php
// Get the trove search zones.
$zones = trove_get_zones();

// Get a list of trove format facets.
$formats = trove_get_facets_format();

// Get trove availability facets.
$availability = trove_get_facets_availability();

// Get the trove category facets.
$category = trove_get_facets_category();
?>

THANKS
------
 * Thanks to the National Library and all of the contributors for the amazing,
   vast and free Trove service. Be sure to acknowledge them if you use
   this module on your site.
   See http://trove.nla.gov.au/general/api-powered-by-trove
   for more information on acknowledging Trove.
 * Thanks to Greg Dunlap (https://www.drupal.org/user/128537) for his
   very helpful four part article on Building Views Query
   Plugins,
   see https://www.lullabot.com/blog/article/building-views-query-plugins
