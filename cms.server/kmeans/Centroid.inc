<?php
/**
 *  File: Centroid.in
 *  This class represents the Centroid for a Cluster. The initial centroid 
 *  is calculated using an equation which divides the sample space for each
 *  dimension into equal parts depending upon the value of `k'.
 */
require_once 'Debugger.inc';
require_once 'KMeans.inc';

class Centroid {
  public $handle;
  public $debugger;
  private $attrs;     //an associative array holding clustering attributes
  private $cluster;

  public function __construct($attrs) {
    $this->handle = fopen('log/Centroid.log', 'w');
    $this->debugger = new Debugger($this->handle);
    //$this->debugger->append("Centroid contructed with: " . var_export($attrs, true));
    $this->attrs = $attrs;

    foreach ($attrs as $k=>$v) {
      $this->$k = $v;
    }
  }

  /** Calculate the centroid value of current instance. Presently, only called
      by KMeans instance */
  public function calcCentroid() {
    $numDP = $this->cluster->getNumDataPoints();

    $tempAttrs = KMeans::initClusteringAttrs(array_keys($this->attrs));
    //$this->debugger->append("debug calcCentroid tempAttrs: " .
    //    var_export($tempAttrs, true));

    //$this->debugger->append("\n\nKMeans::initClusteringAttrs: " .
    //    var_export($tempAttrs, true) . "\n\n");
 
    //calculating the new Centroid
    for ($i = 0; $i < $numDP; ++$i) {
      foreach ($tempAttrs as $k=>&$v) {
        $this->debugger->append("DEBUG: tempAttrs against datapoint attrs: " .
            $this->cluster->getDataPoint($i)->get($k) . "\n");
        $v += $this->cluster->getDataPoint($i)->get($k);
      }
    }
    //$this->debugger->append("with reference: " . var_export($tempAttrs, true) . "\n");
    //$this->debugger->append("calculating the new Centroid, tempAttrs: " . var_export($tempAttrs, true));

    //divide by zero
    //$this->lat = $tempAttrs['lat'] / $numDP;
    //$this->lon = $tempAttrs['lon'] / $numDP;

    /*
    foreach ($this->attrs as $k=>$v) {
      $this->$k = $tempAttrs[$k] / $numDP;
      $this->debugger->append($this->$k . "\n");
      //$this->debugger->append("$tempAttrs[$k] / $numDP: " . ($tempAttrs[$k] / $numDP) . "\n");
      //$this->debugger->append('$this->'.$k.' = '.'$tempAttrs['.$k.'] / $numDP: '.$this->$k."\n");
    }*/
    foreach ($tempAttrs as $k=>&$v) {
      $this->$k = ($v / $numDP);
    }

    //calculating the new Euclidean Distance for each Crime Data Point
    $tempAttrs = KMeans::initClusteringAttrs($this->attrs);

    for ($i = 0; $i < $numDP; ++$i) {
      $this->cluster->getDataPoint($i)->calcEuclideanDistance();
    }

    //calculate the new Sum of Squares for the Cluster
    $this->cluster->calcSumOfSquares();
  }

  public function setCluster($c) {
    $this->cluster = $c;
  }

  public function get($attr) {
    return $this->$attr;
  }

  public function getCluster() {
    return $this->cluster;
  }

  public function __clone() {
  }
}
?>
