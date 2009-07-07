<?php
/** 
 *  File: CrimeStat.inc
 *  Instance of query result from table `Tcrimes`
 */
class CrimeStat {
  private $result;
  
  public function __construct($result) {
    $this->result = $result;
  }

  /** Return associative array of total number for each crime type */
  public function numberOfCrimesPerCategory() {
    $numOfCrimes = array();

    foreach ($this->result as $k=>$v) {
      $numOfCrimes[$v['UCRCategories']]++;
    }

    return $numOfCrimes;
  }

  public function percentageOfTotalCrime() {
    $percentageOfCrimes = array();
    $totalNumOfCrimes = $this->total();

    foreach ($this->numberOfCrimesPerCategory() as $k=>$v) {
      $percentageOfCrimes[$k] = ($v / $totalNumOfCrimes) * 100.0;
    }

    return $percentageOfCrimes;
  }

  public function crimeTrend() {
  }

  /** Return total of crimes in this instance of result */
  public function total() {
    return count($this->result);
  }
}
?>
