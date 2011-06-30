<?php


class openlayers_proximity_handler_filter_circle extends openlayers_proximity_handler_filter {

  /**
   * Process operator.
   */
  function op_process($op = 'simple', $field) {
    parent::op_process($op, $field);

    if (!is_null($this->value['lat']) && !is_null($this->value['lon'])) {

      $join = new views_join();
      $table = $this->table_formula_alias();
      $join->definition['table formula'] = $this->table_formula($this->value['lat'], $this->value['lon']);
      $join->construct($table, 'node', 'nid', 'nid', array(), 'RIGHT');
      $this->query->add_table($table, 'node', $join);
      $this->query->add_field($table, 'distance');

      if ($op == 'simple') {
        $radius = $this->value['value'];
        $operator = $this->operator;
        $this->query->add_where($this->options['group'], $table.'.distance', $radius, $operator);
      }
      else {
        if ($this->operator == 'between') {
          $this->query->add_where($this->options['group'], $table.'.distance', array($this->value['min'], $this->value['max']), 'BETWEEN');
        }
        else {
          $this->query->add_where($this->options['group'], db_or()->condition($table.'.distance', $this->value['min'], '<')->condition($table.'.distance', $this->value['max'], '>'));
        }      
      }
    }
    $this->query->add_groupby("nid");
  }

  /**
   * Build the subselect.
   */

  function table_formula($lat, $lon) {

    $args = array(
      ':lat2' => $lat,
      ':lon2' => $lon,
    );

    $query = db_select('openlayers_proximity', 'op');
    $query->fields('op', array('nid'));
    $query->fields('op', array('lat'));
    $query->fields('op', array('lon'));
    $query->addExpression(OPENLAYERS_PROXIMITY_SQL_GREAT_CIRCLE, 'distance', $args);
    return $query;

  }

  function table_formula_alias() {
    return $this->table_alias . '_filter_' . $this->options['id'];
  }
}
