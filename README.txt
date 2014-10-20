INTRODUCTION
------------
The Trove module provides an simple API around the National Library of Asutralia 
Trove API service. It also provides a Views integration module - Trove Views 
Query - allowing the use of Trove as a views datasource.
 * For a full description of the module, visit the project page:
   https://drupal.org/project/trove
 * For more information on Trove, visit http://trove.nla.gov.au

REQUIREMENTS
------------
Trove Views Query module requires the following modules:
 * Views (https://drupal.org/project/views)

INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
 * Visit https://trove.nla.gov.au/signup to sign up as a registered user. Once
   registered you can request a free API key.
 * Once the Trove module is enabled, visit admin/config/services/trove and enter
   your API key.
 * To use Trove as a views datasource, you will need to enable the Trove Views 
   Query module:
   - Once Trove Views Query module is enabled, add a new view from the 
     admin/structure/views page.
   - From the 'show' dropdown, select 'Trove Results' and set anuy other config
     settings you require from this page.
   - There are two required views filters for a 'Trove Results' view to be valid:
     - Select 'Trove Results: zone' from the filter criteria and select one of 
       the options (this may include all zones). This determines the Trove zones 
       to search across.
     - Select 'Trove Results: query' from the filter criteria. This is the key 
       word/phrase filter.
   - Select one or more fields to display.
 * For more information on using the Trove API, visit 
   http://help.nla.gov.au/trove/building-with-trove/api

TROUBLESHOOTING
---------------
 * If your view does not return any results, check the following:
   - Have you added a zone to the filter parameters?
   - Have you added a query to the filter parameters?
   - Have you tried clearing the drupal cache?

THANKS
------
 * Thanks to the National Library and all of the contributors for the amazing, vast 
   and free Trove service. Be sure and acknowledge them if you use this module on 
   your site. See http://trove.nla.gov.au/general/api-powered-by-trove for more 
   information on acknowledging Trove.
 * Thanks to Greg Dunlap (https://www.drupal.org/user/128537) for his very helpful 
   four part article on Building Views Query 
   Plugins, see https://www.lullabot.com/blog/article/building-views-query-plugins
   