<?php
require_once 'model/Tcrimes.inc';
require_once 'DatasetFactory.inc';
require_once 'KMeans/Kmeans.inc';


//$clusteringAttrs = array('UCRCategoriesId'=>'', 'addressId'=>'', 'zip'=>'', 'lat'=>'', 'lon'=>'');
//$clusteringAttrs = array('lat'=>'', 'lon'=>'');
$clusteringAttrs = array('UCRCategoriesId'=>'');
$k = 3; //there has to be more clusters than points ;)
$iterations = 10; //about the number of datapoints

$crimes = new Tcrimes();
$result = $crimes->get(array('county'=>'Falls Church'));

$crimeRecords = array();
foreach ($result as $row) {
  $crimeRecords[] = DatasetFactory::create($row['UCRCategories'], $row, $clusteringAttrs);
}

//var_dump($crimeRecords);

$kmeans = new KMeans($k, $iterations, $crimeRecords, $clusteringAttrs);
$kmeans->start(false);
$clusters = $kmeans->getClusterOutput();  //2D array: cluster[k]datapoint[i];
$largestClust = 0;
$largestClustSize = 0;
         
$d = getDate();
printf("<strong>%s-%s-%s %s:%s:%s<br/>", $d['mday'], $d['mon'], $d['year'], $d['hours'], $d['minutes'], $d['seconds']);
echo "<table align='center' valign='top' width='150%' border='1' cellspacing=0 cellpadding=0><tr>";

echo "<table>";
echo "<tr bgcolor='lightblue'><td>Initial Centroid's Info:</td></tr>";
echo "<tr bgcolor='lightblue'><th>UCRCategories<th>crimeId<th>addressId<th>zip<th>lat<th>lon";
//print each cluster's centroid info:
$initialCentroids = $kmeans->getInitialCentroids();
foreach ($initialCentroids as $centroid) {
  echo "<tr bgcolor='pink'>";
  printf("<td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td>",
      $centroid->get('UCRCategoriesId'),
      $centroid->get('crimeId'),
      $centroid->get('addressId'),
      $centroid->get('zip'),
      $centroid->get('lat'),
      $centroid->get('lon'));
  echo "</tr>";
}
echo "</table>";

echo "<table>"; 
echo "<tr bgcolor='lightblue'><td>Result Centroid's Info:</td></tr>";
echo "<tr bgcolor='lightblue'><th>UCRCategories<th>crimeId<th>addressId<th>zip<th>lat<th>lon";
//print each cluster's centroid info:
for ($i = 0; $i < count($clusters); $i++) {
	$UCRCategoriesId = (float)$kmeans->getCluster($i)->getCentroid()->get('UCRCategoriesId');
	$crimeId = (float)$kmeans->getCluster($i)->getCentroid()->get('crimeId');
	$addressId = (float)$kmeans->getCluster($i)->getCentroid()->get('addressId');
	$zip = (float)$kmeans->getCluster($i)->getCentroid()->get('zip');
	$lat = (float)$kmeans->getCluster($i)->getCentroid()->get('lat');
	$lon = (float)$kmeans->getCluster($i)->getCentroid()->get('lon');

  printf("<tr bgcolor='pink'><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>\n", $UCRCategoriesId, $crimeId, $addressId, $zip, $lat, $lon);
}
echo '</table>';
echo '</table>';


//print each cluster output of i,
echo "<table align='center' valign='top' width='150%' border='1' cellspacing=0 cellpadding=0><tr>";
for ($i = 0; $i < count($clusters); $i++) {
  $tempCluster = $clusters[$i]; //get `i' clusters; contains array of dp

  //determine largest cluster
  if (count($tempCluster) > $largestClustSize) {
    $largestClustSize = count($tempCluster);
    $largestClust = $i;
  }

  echo "<td valign='top'><table border='1'>";
  echo "<tr bgcolor='lightgreen'><th>Cluster ".$i."<th>Size: ".count($tempCluster)."</tr>";
  echo "<tr bgcolor='lightgrey'><th>Crime<th>UCRCategoriesId<th>crimeId<th>addressId<th>zip<th>lat<th>lon";
   
  foreach ($tempCluster as $clusterObj){
    echo "<tr>";
    echo "<td>".$clusterObj->get('crimeCategory') . "</td><td>" . $clusterObj->get('UCRCategoriesId')
      . "</td><td>" . $clusterObj->get('crimeId')
      . "</td><td>" . $clusterObj->get('addressId')
      . "</td><td>" . $clusterObj->get('zip')
      . "</td><td>" . $clusterObj->get('lat')
      . "</td><td>" . $clusterObj->get('lon');
    echo "</tr>";
  }
  echo "</table></td>";
}
echo '</table>';
       

$UCRCategoriesId = $kmeans->getCluster($largestClust)->getCentroid()->get('UCRCategoriesId');
$crimeId = $kmeans->getCluster($largestClust)->getCentroid()->get('crimeId');
$addressId = $kmeans->getCluster($largestClust)->getCentroid()->get('addressId');
$zip = $kmeans->getCluster($largestClust)->getCentroid()->get('zip');
$lat = $kmeans->getCluster($largestClust)->getCentroid()->get('lat');
$lon = $kmeans->getCluster($largestClust)->getCentroid()->get('lon');
       
echo "<hr><strong>Summary:</strong><br/>";
echo("Cluster ".$largestClust . " with ". $largestClustSize
    . " points, is the biggest. UCRCategories= ".$UCRCategoriesId
    . " crimeId= ".$crimeId
    . " addressId=".$addressId
    . " zip=".$zip
    . " lat=".$lat
    . " lon=".$lon
    );
?>
