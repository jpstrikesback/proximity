

# OpenLayers Proximity

OpenLayers Proximity enables geographical proximity search for the OpenLayers 
module.


# Views integration

OpenLayers Proximity exposes:

 * Square filter: it gives locations contained within a square derived by a 
   simple latitude/longitude comparison. Less accurate, better performance
 * Great-circle filter: it uses the Great-circle distance formula to return 
   locations within a circular area. More accurate, lower performance.
 * Distance sort criteria: used in combination with "Great-circle" filters it
   allows to sort nodes by distance from a given location.
 * Distance field: used in combination with "Great-circle" filters it shows 
   distance from a given location.

Both filters can measure distance from a location or from another node.


# Configuration 

After enabling the module is necessary to build the proximity index table by
visiting "admin/build/openlayers/proximity". This has to be done only the 
first time since it will index all existing nodes, the module will 
automatically index newly created nodes.

If you plan to show proximity search results on a map using exposed filers 
remember to add them in both your OpenLayers Map and OpenLayers Data display.
More at http://drupal.org/node/789668.

# Rules integration

Rules integration (http://drupal.org/project/rules) gives the possibility to
notify nodes that something has happened "near" them.

OpenLayers Proximity exposes the "Notify nodes based on geographic proximity"
action which, in turn, triggers the "Node is notified based on geographic 
proximity" event, for all nodes falling into the action's proximity filter.

Possible use cases are:

 * Notify all authors that a new content has been posted close to their content.
 * Change node proprieties based on proximity.

 