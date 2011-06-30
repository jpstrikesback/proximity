<?php

/**
 * @file: Drush integration for OpenLayers Proximity module.
 */

/**
 * Implements hook_drush_command().
 */
function openlayers_proximity_drush_command() {
  $items = array();

  $items['rebuild-proximity'] = array(
    'description' => 'Rebuild OpenLayers Proximity index.',
    //    'options' => array( //      'nodes' => 'Comma delimited list nodes NID.', //    ),
    'examples' => array(
      'drush rebuild-proximity' => 'Rebuild proximity index for all nodes.',
      //      'drush rebuild-proximity --nodes=1,2,3' => 'Rebuild proximity index for nodes 1, 2 and 3.',
    ),
    'aliases' => array('rp'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function openlayers_proximity_drush_help($section) {
  switch ($section) {
    case 'drush:rebuild-proximity':
      return dt('This command will rebuild the proximity index for the specified nodes.');
  }
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_openlayers_proximity_rebuild_proximity() {

//  $nodes = drush_get_option('nodes');
  $batch = array(
    'title' => dt('Rebuild proximity index'),
    'operations' => array(array('openlayers_proximity_rebuild_index', array())),
    'finished' => 'openlayers_proximity_rebuild_index_finished',
    'init_message' => dt('The proximity index rebuilding process is beginning.'),
    'progress_message' => dt('Index rebuilt for @current out of @total.'),
    'error_message' => dt('The proximity index rebuilding process encountered an error.'),
  );

  batch_set($batch);
  drush_backend_batch_process();
}
