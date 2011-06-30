<?php


/**
 * Implements hook_rules_event_info().
 * @ingroup rules
 */
function openlayers_proximity_rules_event_info() {
  return array(
    'proximity_notification' => array(
      'label' => t('Node is notified based on geographic proximity'),
      'module' => 'OpenLayers Proximity',
      'arguments' => array(
        'nodeA' => array(
          'type' => 'node',
          'label' => t('First node, which notifies the second.'),
        ),
        'nodeB' => array(
          'type' => 'node',
          'label' => t('Second node, which is notified by the first.'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */
function openlayers_proximity_rules_action_info() {
  return array(
    'openlayers_proximity_action_notify_nodes' => array(
      'label' => t('Notify nodes based on geographic proximity'),
      'arguments' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Content with locaiton'),
        ),
      ),
      'module' => 'OpenLayers Proximity',
    ),
  );
}

/**
 * Implementation of action callback.
 */
function openlayers_proximity_action_notify_nodes($nodeA, $settings) {
  list($view, $display_id, $filter_id) = explode('-', $settings['display']);

  if ($view = views_get_view($view)) {
    $view->build($display_id);
    $filter = $view->display_handler->get_handler('filter', $filter_id);
    $filter->options['location_as'] = 'node';
    $filter->value['node'] = $nodeA->nid;
    $view->display_handler->execute();

    if ($view->result) {
      foreach ($view->result as $row) {
        $nodeB = node_load((int) $row->nid);
        rules_invoke_event('proximity_notification', $nodeA, $nodeB);
      }
    }
  }
}

/**
 * Implementation of action form callback.
 */
function openlayers_proximity_action_notify_nodes_form($settings, &$form) {

  $settings += array(
    'message' => '',
    'error' => '',
  );
  $options = array();
  if ($views = views_get_all_views()) {
    foreach ($views as $view) {
      if ($view->base_table == 'node') {
        foreach ($view->display as $display_id => $display) {
          $view->build($display_id);
          foreach ($view->display_handler->get_handlers('filter') as $filter_id => $handler) {
            if ($handler->definition['handler'] == 'openlayers_proximity_handler_filter_circle' || $handler->definition['handler'] == 'openlayers_proximity_handler_filter_square') {
              $options[$view->name][$view->name . '-' . $display_id . '-' . $filter_id] = t('@display - @filter (@filter_id)', array('@display' => $display->display_title, '@filter' => $handler->definition['title'] . ' ' . $handler->admin_summary(), '@filter_id' => $filter_id));
            }
          }
        }
      }
    }
  }

  if (count($options) == 0) {
    drupal_set_message(t('In order to proceed you need to <a href="!url">create at least one Views display</a> using a proximity filter: it will be used to filter out which nodes will be notified when this action will be performed.', array('!url' => url('admin/structure/views/add'))), 'warning');
  }

  $form['settings']['display'] = array(
    '#type' => 'select',
    '#title' => t('Views display'),
    '#default_value' => $settings['display'],
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t('Choose which Views display will be in charge of retrieving the nodes that needs to be notified. The starting point node will be passed to the specified proximity filter.'),
  );
}

