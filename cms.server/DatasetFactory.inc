<?php
require_once 'crimes/Crime.inc';
require_once 'crimes/Burglary.inc';
require_once 'crimes/DestructionOfProperty.inc';
require_once 'crimes/DisorderlyConduct.inc';
require_once 'crimes/DrivingUnderTheInfluence.inc';
require_once 'crimes/Drug.inc';
require_once 'crimes/Drunkenness.inc';
require_once 'crimes/Embezzlement.inc';
require_once 'crimes/Kidnapping.inc';
require_once 'crimes/Larceny.inc';
require_once 'crimes/LiquorLawViolations.inc';
require_once 'crimes/MotorVehicleTheft.inc';
require_once 'crimes/Others.inc';
require_once 'crimes/PeepingTom.inc';
require_once 'crimes/Robbery.inc';

require_once 'model/Tcrimes.inc';

class DatasetFactory {
  private function __construct() {}

  public static function create($type, $attr, $clusteringAttrs) {
    switch ($type) {
      case 'ALL OTHER OFFENSES':
        return new Others($attr, $clusteringAttrs);
      case 'BURGLARY':
        return new Burglary($attr, $clusteringAttrs);
      case 'DESTRUCTION OF PROPERTY':
        return new DestructionOfProperty($attr, $clusteringAttrs);
      case 'DISORDERLY CONDUCT':
        return new DisorderlyConduct($attr, $clusteringAttrs);
      case 'DRIVING UNDER THE INFLUENCE':
        return new DrivingUnderTheInfluence($attr, $clusteringAttrs);
      case 'DRUG':
        return new Drug($attr, $clusteringAttrs);
      case 'DRUNKENNESS':
        return new Drunkenness($attr, $clusteringAttrs);
      case 'EMBEZZLEMENT':
        return new Embezzlement($attr, $clusteringAttrs);
      case 'KIDNAPPING':
        return new Kidnapping($attr, $clusteringAttrs);
      case 'LARCENY':
        return new Larceny($attr, $clusteringAttrs);
      case 'LIQUOR LAW VIOLATIONS':
        return new LiquorLawViolations($attr, $clusteringAttrs);
      case 'MOTOR VEHICLE THEFT':
        return new MotorVehicleTheft($attr, $clusteringAttrs);
      case 'PEEPING TOM':
        return new PeepingTom($attr, $clusteringAttrs);
      case 'ROBBERY':
        return new Robbery($attr, $clusteringAttrs);
    }
  }
}

/* Unit Test: */
function testDatasetFactory() {
  $clusteringAttrs = array('UCRCategoriesId'=>'', 'addressId'=>'', 'zip'=>'', 'lat'=>'', 'lon'=>'');
  $testData= array(array('type'=>'ALL OTHER OFFENSES'),
                   array('type'=>'BURGLARY'),
                   array('type'=>'DESTRUCTION OF PROPERTY'),
                   array('type'=>'DISORDERLY CONDUCT'),
                   array('type'=>'DRIVING UNDER THE INFLUENCE'),
                   array('type'=>'DRUG'),
                   array('type'=>'DRUNKENNESS'),
                   array('type'=>'EMBEZZLEMENT'),
                   array('type'=>'KIDNAPPING')
                  );

  $crimeObjs = array();
  foreach ($testData as $v) {
    $crimeObjs[] = DatasetFactory::create($v['type'], $v, $clusteringAttrs);
  }
  var_dump($crimeObjs);

  echo "====";
  $attrs = $crimeObjs[0]->get('clusteringAttrs');
  var_dump($attrs);
}

function fullDBTest() {
  error_reporting(E_ALL);
  //$clusteringAttrs = array('UCRCategoriesId'=>'', 'addressId'=>'', 'zip'=>'', 'lat'=>'', 'lon'=>'');
  $clusteringAttrs = array('lat'=>'', 'lon'=>'');

  $crimes = new Tcrimes();
  //$result = $crimes->get(array('county'=>'Falls Church'));
  $result = $crimes->get(array('county'=>'Falls Church'));

  $crimeRecords = array();
  foreach ($result as $row) {
    $crimeRecords[] = DatasetFactory::create($row['UCRCategories'], $row, $clusteringAttrs);
  }

  printf("count: %d\n", count($crimeRecords));
  for ($i = 0; $i < count($crimeRecords); ++$i) {
    echo $i . ": " . $crimeRecords[$i]->get('lat') . "\n";
  }
}


//testDatasetFactory();
//fullDBTest();
?>
