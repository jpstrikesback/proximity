<?php


class openlayers_proximity_handler_sort extends views_handler_sort {

  function option_definition() {
    $options = parent::option_definition();
    $options['location_provider'] = array('default' => '');
    return $options;
  }

  /**
   * Basic options for all sort criteria
   */
  function options_form(&$form, &$form_state) {

    if ($handlers = $this->view->display_handler->get_handlers('filter')) {
      $options = array();
      foreach ($handlers as $name => $handler) {
        if ($handler->definition['handler'] == 'openlayers_proximity_handler_filter_circle') {
          $options[$name] = $handler->definition['group'] . ': ' . $handler->definition['title'] . ' (' . $handler->admin_summary() . ')';
        }
      }
      $form['location_provider'] = array(
        '#title' => t('Location provider'),
        '#type' => 'select',
        '#options' => $options,
      );
    }
    parent::options_form($form, $form_state);
  }

  function query() {
    $handler = $this->view->display_handler->get_handler('filter', $this->options['location_provider']);
    if ($handler && $handler->value['location'] && isset($this->query->table_queue[$handler->table_formula_alias()])) {
      $this->query->add_orderby($handler->table_formula_alias(), $this->real_field, $this->options['order']);
    }

  }

}
