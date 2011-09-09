<?php

class openlayers_proximity_handler_field extends views_handler_field_numeric {

  function option_definition() {
    $options = parent::option_definition();
    $options['location_provider'] = array('default' => '');
    $options['set_precision'] = array('default' => TRUE);
    $options['precision'] = array('default' => 2);
    $options['hide_empty'] = array('default' => TRUE);
    $this->definition['float'] = TRUE;
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
      $this->field_alias = $this->query->add_field($handler->table_formula_alias(), $this->real_field);
    }
  }

  function render($values) {
    $value = $this->get_value($values);
    $handler = $this->view->display_handler->get_handler('filter', $this->options['location_provider']);
    $unit = $handler->options['exposed'] ? $handler->value[$handler->options['expose']['unit']] : $handler->value['unit'];
    $value = openlayers_proximity_measurement_units_convert_back($unit, $value);

    if (!empty($this->options['set_precision'])) {
      $value = number_format($value, $this->options['precision'], $this->options['decimal'], $this->options['separator']);
    }
    else {
      $remainder = abs($value) - intval(abs($value));
      $value = $value > 0 ? floor($value) : ceil($value);
      $value = number_format($value, 0, '', $this->options['separator']);
      if ($remainder) {
        $value .= $this->options['decimal'] . drupal_substr($remainder, 2);
      }
    }

    // Check to see if hiding should happen before adding prefix and suffix.
    if ($this->options['hide_empty'] && empty($value) && ($value !== 0 || $this->options['empty_zero'])) {
      return '';
    }

    $units = openlayers_proximity_get_available_units();
    return check_plain($this->options['prefix'] . $value . ' ' . $units[$unit]['short'] . ' ' . $this->options['suffix']);
  }

}
