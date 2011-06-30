<?php


class openlayers_proximity_handler_filter_square extends openlayers_proximity_handler_filter {

  /**
   * Process operator.
   */
  function op_process($op = 'simple', $field) {
    parent::op_process($op, $field);

    if ($this->response) {

      $table = $this->table_alias;

      $this->query->add_field($table, 'lat');
      $this->query->add_field($table, 'lon');

      $lat = $this->value['lat'];
      $lon = $this->value['lon'];
      
      if ($op == 'simple') {
        $radius = $this->value['value'];
        $bounds = $this->get_bounds($lat, $lon, $radius);
        $operator = $this->operator;
        $where = $this->build_where($this->operator, $bounds, $table);
        //$this->query->add_where_expression($this->options['group'], $where);
      }
      else {
        $radius = $this->value['max'];
        $bounds = $this->get_bounds($lat, $lon, $radius);
        $inner = $this->build_where('<', $bounds, $table);

        $radius = $this->value['min'];
        $bounds = $this->get_bounds($lat, $lon, $radius);
        $outer = $this->build_where('>', $bounds, $table);

        $where = ($this->operator == 'between') ? "($inner AND $outer)" : "NOT($inner AND $outer)";
      }

      $this->query->add_where_expression($this->options['group'], $where);
    }
    $this->query->add_groupby("nid");
  }


  /**
   * Calculate search bounds give lat/lon and distance in kilometers.
   * @see http://imaginerc.com/software/GeoCalc/
   */
  function get_bounds($latitude, $longitude, $radius) {

    // Calculate lat/lon shifts
    $latitude_shift = $radius / OPENLAYERS_PROXIMITY_KM_PER_LAT;
    $longitude_shift = (1 / (OPENLAYERS_PROXIMITY_KM_PER_LAT * cos($latitude * OPENLAYERS_PROXIMITY_DEGREE_TO_RADIANTS))) * $radius;

    // Calculate the boundaries
    $bounds = array();
    $bounds['north'] = $latitude + $latitude_shift;
    $bounds['south'] = $latitude - $latitude_shift;
    $bounds['west'] = $longitude - $longitude_shift;
    $bounds['east'] = $longitude + $longitude_shift;
    return $bounds;
  }


  /**
   * Build WHERE clause.
   */
  function build_where($operator, $bounds, $alias) {
    $where = vsprintf("($alias.lat <= %f AND $alias.lat >= %f AND $alias.lon >= %f AND $alias.lon <= %f)", $bounds);
    return ($operator == '<' || $operator == '<=') ? $where : 'NOT' . $where;
  }

}
