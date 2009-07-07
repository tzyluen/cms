<?php 
/**
 *  File: Cluster.inc
 *  This class represents a Cluster in a KMeans instance.
 *  A Cluster is associated with ONLY ONE KMeans instance.
 *  A Cluster is related to MORE THAN ONE DataPoints(Crime) and
 *  + ONLY ONE centroid.
 */
class Cluster {
  private $name;
  private $centroid;
  private $sumSqr;
  private $dataPoints;

  public function __construct($name) {
    $this->name = $name;
    $this->centroid = null; //will be set by calling setCentroid()
    $this->dataPoints = array();
  }


  /** Called from KMeans::setInitialCentroids() */
  public function setCentroid($c) {
    $this->centroid = $c;
  }

  public function getCentroid() {
    return $this->centroid;
  }

  public function addDataPoint($dp) { //called from KMeans instance
    $dp->setCluster($this); //initiates a inner call to
    $dp->calcEuclideanDistance();
    array_push($this->dataPoints, $dp);
    $this->calcSumOfSquares();
  }

  public function removeDataPoint($dpa) {
    $key = array_search($dpa, $this->dataPoints); 
    unset ($this->dataPoints[$key]);
    //reindex
    $temp_array = array_values($this->dataPoints);
    $this->dataPoints = $temp_array;

    $this->calcSumOfSquares();
  }

  public function getNumDataPoints() {
    return count($this->dataPoints);
  }

  public  function getDataPoint($pos) {
    return $this->dataPoints[$pos];
  }

  public function calcSumOfSquares() { //called from Centroid
    $size = count($this->dataPoints);
    $temp = 0;

    for ($i = 0; $i < $size; $i++) {
      $temp = $temp + $this->dataPoints[$i]->getCurrentEuDt();
    }

    $this->sumSqr = $temp;
  }

  public function getSumSqr() {
    return $this->sumSqr;
  }

  public function getName() {
    return $this->name;
  }

  public function getDataPoints() {
    return $this->dataPoints;
  }
}
?>
