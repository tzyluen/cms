<?php
/**
 *  File: KMeans.inc
 *  This class is the entry point for constructing the K-Means clustering
 *  analysis object. Each instance of KMeans is associated with ONE or MORE 
 *  clusters(depends on input of `k'), and an array of DataPoint(Crime) objects. 
 */
require_once 'Debugger.inc';
require_once 'Cluster.inc';
require_once 'Centroid.inc';

class KMeans { 
  public $handle;
  public $debugger;
	private $clusters;
	private $iter;
	private $dataPoints;
	private $SWCSS;
  private $initialCentroids;
  private $clusteringAttrs;
  private $centroidLogger;
  private $initialCentroidLogger;

  public function __construct($k, $iter, $dataPoints, $clusteringAttrs) {
    $this->handle = fopen('log/KMeans.log', 'w');
    $this->debugger = new Debugger($this->handle);
    $this->centroidLogger = new Debugger(fopen('log/centroidValues.xml', 'w'));
    $this->initialCentroidLogger = new Debugger(fopen('log/initialCentroidValues.xml', 'w'));
    $this->clusteringAttrs = $clusteringAttrs;
    $clusters = array();

    for ($i = 0; $i < $k; $i++) {
      $clusters[$i] = new Cluster("Cluster" + $i);
    }

    $this->iter = $iter;
    $this->dataPoints = $dataPoints;
		$this->clusters = $clusters;
    $this->initialCentroids = array();
  }

  /** FIXME:
   * CURRENTLY UNUSED
   * Calculate sum of `cluster sum of square`
   */
  private function calcSWCSS() {
    $temp = 0;
    for ($i = 0; $i < count($this->clusters); $i++) {
      $temp = $temp + $this->clusters[$i]->getSumSqr();
    }
    $this->SWCSS = $temp;
  }

  public function start($rand) {    
    //set Starting centroid positions - Start of Step 1
		$this->setInitialCentroids($rand);
    $this->logInitialCentroidValue();
    
		$n = 0;
		$cn = count($this->dataPoints);
		
    while (count($this->dataPoints) > $n) {
      for ($l = 0; $l < count($this->clusters); $l++)  {
        $this->clusters[$l]->addDataPoint($this->dataPoints[$n]);
        $n++;
        if ($n >= count($this->dataPoints)) {
          break;
        }
      }
    }

    //calculate E for all the clusters
    $this->calcSWCSS();
     
    //recalculate Cluster centroids - Start of Step 2
    for ($i = 0; $i < count($this->clusters); $i++) {
		//print_r($this->clusters[$i]->getCentroid()); //works
      $this->clusters[$i]->getCentroid()->calcCentroid();
    }
        
    //recalculate E for all the clusters
    $this->calcSWCSS();

    $buf = "<iterations>\n";  //start logging centroid into buf

    for ($i = 0; $i < $this->iter; $i++) {
      //enter the loop for cluster j
      for ($j = 0; $j < count($this->clusters); $j++) {
        for ($k = 0; $k < $this->clusters[$j]->getNumDataPoints(); $k++) {
          //pick the first element of the first cluster
          //get the current Euclidean distance
          //double tempEuDt = clusters[j].getDataPoint(k).getCurrentEuDt();
          $tempEuDt = $this->clusters[$j]->getDataPoint($k)->getCurrentEuDt();
          // Cluster tempCluster = null;
          $tempCluster == "";
          $matchFoundFlag = false;

          //call testEuclidean distance for all clusters
          for ($l = 0; $l < count($this->clusters); $l++) {
            //if testEuclidean < currentEuclidean then
            if ($tempEuDt > $this->clusters[$j]->getDataPoint($k)->testEuclideanDistance(
                  $this->clusters[$l]->getCentroid()
                  ))
               {
                $tempEuDt = $this->clusters[$j]->getDataPoint($k)->testEuclideanDistance(
                  $this->clusters[$l]->getCentroid()
                );
                $tempCluster = $this->clusters[$l];
                $matchFoundFlag = true;
            } //if statement - Check whether the Last EuDt is > Present EuDt 
          } //for variable 'l' - Looping between different Clusters for matching a Data Point.

          //add DataPoint to the cluster and calcSWCSS
          if ($matchFoundFlag) {
            $tempCluster->addDataPoint($this->clusters[$j]->getDataPoint($k));
            $this->clusters[$j]->removeDataPoint($this->clusters[$j]->getDataPoint($k));

            for ($m = 0; $m < count($this->clusters); $m++) {
                $this->clusters[$m]->getCentroid()->calcCentroid();
            }//for variable 'm' - Recalculating centroids for all Clusters

            $this->calcSWCSS();
          }//match found
           //if statement - A Data Point is eligible for transfer between Clusters.
        }//for variable 'k' - Looping through all Data Points of the current Cluster.
      }//for variable 'j' - Looping through all the Clusters.

      //output each iteration centroid value:
      $kn = 0;
      $buf .= "  <iteration>\n";
      //$buf .= "    <count>" . $i . "</count>\n";
      $buf .= "    <count>" . ($i + 1) ."</count>\n";
      for ($kn = 0; $kn < count($this->clusters); ++$kn) {
        $buf .= "    <cluster>".$kn."</cluster>\n";
        foreach ($this->clusteringAttrs as $v) {
          $buf .= "    <$v>".$this->clusters[$kn]->getCentroid()->get($v)."</$v>\n";
        }
      }
      $buf .= "  </iteration>\n";
    }//for variable 'i' - Number of iterations.
    $buf .= "</iterations>\n";
    $this->centroidLogger->append($buf);  //end of logging centroid
  }//end of function
  
  
  private function logInitialCentroidValue() {
    $buf .= "<InitialCentroids>\n";
    foreach ($this->initialCentroids as $centroid) {
      $buf .= "  <centroid>\n";
      $buf .= "    <cluster>" . $centroid->getCluster()->getName() . "</cluster>\n";
      foreach ($this->clusteringAttrs as $k=>$v) {
        $buf .= "    <$v>" . $centroid->get($v) . "</$v>\n";
      }
      $buf .= "  </centroid>\n";
    }
    $buf .= "</InitialCentroids>\n";
    $this->initialCentroidLogger->append($buf);
  }

	
  /**
   *  Return `k' length index of array representing the clusters; each cluster
   *  contains the array of crime objects.
   *  Made up the return value is two dimensional array.
   */
  public function getClusterOutput() {
    $v = array();
    for ($i = 0; $i < count($this->clusters); $i++) {
      $v[$i] = $this->clusters[$i]->getDataPoints();
    }
    return $v;
  }

  //FIXME: implementation decision static of instance method?
  public static function initClusteringAttrs($clusteringAttrs) {
    $tempAttrs = array();
    foreach ($clusteringAttrs as $k=>$v) {
      $tempAttrs[$v] = 0;
    }

    return $tempAttrs;
  }

  private function setInitialCentroids($rand = false) {
    $tempAttrs = self::initClusteringAttrs($this->clusteringAttrs);
    //formula to initialize initial centtoids
    //kn = (round((max-min)/k)*n)+min where n is from 0 to (k-1).
    if (!$rand) {
      $this->debugger->append("FORMULA CENTROID:\n");
      $n = 1;
      foreach ($this->clusters as &$cluster) {
        foreach ($tempAttrs as $k=>$v) {
          $tempAttrs[$k] =
            (
                (
                  ($this->getMaxValue($k) - $this->getMinValue($k))
                    / (count($this->clusters) + 1)
                ) * $n++
            ) + $this->getMinValue($k);
        }

        $this->debugger->append("initial centroids: " . var_export($tempAttrs, true));

        $centroid = new Centroid($tempAttrs);

        $this->debugger->append(var_export($centroid, true));

        $centroid->setCluster($cluster);
        $cluster->setCentroid($centroid);
        
        $this->initialCentroids[] = clone $centroid;
      }
    } else {
      //random select number of `k' datapoints as initial centroids
      $this->debugger->append("RANDOM CENTROID:\n");
      $n = 1;
      $prevAttrs = array();
      for ($i = 0; $i < count($this->clusters); ) {
        //randomly generate index number for datapoints and get the values
        $dp = $this->dataPoints[rand(0, $this->getTotalDataPoints())];
        foreach ($tempAttrs as $k=>$v) {
          $tempAttrs[$k] = $dp->get($k);
        }
        $this->debugger->append("random initial centroids: " . var_export($tempAttrs, true));

        //compare previous selected initial centroid:
        $compResult = array();
        foreach ($tempAttrs as $k=>$v) {
          //if prev attr same as current
          $this->debugger->append("DEBUG: TEST same clustering attributes\n");
          if ($v == $prevAttrs[$k]) {
            $compResult[] = true;
          }
        }

        if (in_array(true, $compResult)) {
          $this->debugger->append("DEBUG: FOUND same clustering attributes\n");
          continue 1;
        }

        $centroid = new Centroid($tempAttrs);
        $prevAttrs = $tempAttrs;
        $this->debugger->append(var_export($centroid, true));

        $centroid->setCluster($this->clusters[$i]);
        $this->clusters[$i]->setCentroid($centroid);
 
        $this->initialCentroids[] = clone $centroid;
        $i++;
      }


      /*
      foreach ($this->clusters as &$cluster) {
        //randomly generate index number for datapoints and get the values
        $dp = $this->dataPoints[rand(0, $this->getTotalDataPoints())];
        foreach ($tempAttrs as $k=>$v) {
          $tempAttrs[$k] = $dp->get($k);
        }
        $this->debugger->append("random initial centroids: " . var_export($tempAttrs, true));

        //compare previous selected initial centroid:
        $compResult = array();
        foreach ($tempAttrs as $k=$v) {
          //if prev attr same as current
          if ($v == $prevAttrs[$k]) {
            $compResult[] = true;
          }
        }

        if (in_array(true, $compResult)) {
          continue 1;
        }

        $centroid = new Centroid($tempAttrs);
        $prevAttrs = $tempAttrs;
        $this->debugger->append(var_export($centroid, true));

        $centroid->setCluster($cluster);
        $cluster->setCentroid($centroid);
 
        $this->initialCentroids[] = clone $centroid;
      }
      */
    }
  }

  /**
   *  Return max value of parameter `attr` from the 0..i..length-1 elements array of
   *  datapoints, where each enumerate object is instance of DataPoint/Crime.
   */
  private function getMaxValue($attr) {
    $temp = 0;
    $temp = $this->dataPoints[0]->get($attr);
    for ($i = 0; $i < count($this->dataPoints); $i++) {
      $dp = $this->dataPoints[$i];
      $temp = ($dp->get($attr) > $temp) ? $dp->get($attr) : $temp;
    }

    return $temp;
  }

  private function getMinValue($attr) {
    $temp = 0;
    $temp = $this->dataPoints[0]->get($attr);
    for ($i = 0; $i < count($this->dataPoints); $i++) {
      $dp = $this->dataPoints[$i];
      $temp = ($dp->get($attr) < $temp) ? $dp->get($attr) : $temp;
    }
    return $temp;
  }

  public function getKValue() {
    return count($this->$clusters);
  }

  public function getIterations() {
    return $this->iter;
  }

  public function getTotalDataPoints() {
    return count($this->dataPoints);
  }

  public function getSWCSS() {
    return $this->SWCSS;
  }

  public function getCluster($pos) {
    return $this->clusters[$pos];
  }

  public function getInitialCentroids() {
    return $this->initialCentroids;
  }
}
?>
