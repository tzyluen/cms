<?php
require_once 'model/Tcrimes.inc';
require_once 'DatasetFactory.inc';

function testDatasetFactoryDB() {
  $crimes = new Tcrimes();
  $result = $crimes->getAll();

  $crimeRecords = array();
  foreach ($result as $row) {
    $crimeRecords[] = DatasetFactory::create($row['UCRCategories'], $row);
  }

  print_r($crimeRecords);
}

testDatasetFactoryDB();
?>
