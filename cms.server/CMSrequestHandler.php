<?php
require_once 'CMSrequestHandler.inc';
/** File: CMSrequestHandler.php
 *  handler for the client request crime dataset
 */

define('DEBUG', true);
$request = new CMSrequestHandler();
$request->debugger->append(Debugger::now() . "\n");

switch ($_POST['options']) {
  case 'clustering':
    $result = $request->clustering($_POST);
    if (DEBUG)
      $request->debugger->append("clustering xml output: " . $result);
    print $result;
    break;
  case 'requestTfiles':
    $result = $request->requestTfiles();
    if (DEBUG) {
      $request->debugger->append(Debugger::now() . "\n");
      $request->debugger->append($result);
    }
    print $result;
    break;
  case 'requestTcrimesSize':
    $result = $request->requestTcrimesSize();
    print $result;
    break;
  case 'getDatasets':
    $result = $request->getDatasets($_POST);
    if (DEBUG)
      $request->debugger->append("getDatasets: \n" . $result);
    print $result;
    break;
  case 'getStates':
    $result = $request->getStates();
    if (DEBUG) {
      $request->debugger->append('getStates called');
      $request->debugger->append($result);
    }
    print $result;
    break;
  case 'getCounties':
    $result = $request->getCounties();
    if (DEBUG)
      $request->debugger->append($result);
    print $result;
    break;
  case 'getPreSelectionTotalOfCrimes':
    $result = $request->getPreSelectionTotalOfCrimes($_POST);
    print $result;
    break;
  case 'getPercentageOfTotalCrime':
    $result = $request->getPercentageOfTotalCrime(
                array('state'=>$_POST['state'],
                      'county'=>$_POST['county']
                ));
    if (DEBUG)
      $request->debugger->append($result);
    print $result;
    break;
  case 'getNumberOfCrimesPerCategory':
    $result = $request->getNumberOfCrimesPerCategory(
                array('state'=>$_POST['state'],
                      'county'=>$_POST['county']
                ));
    if (DEBUG)
      $request->debugger->append($result);
    print $result;
    break;
  case 'processFileImportation':
    if (DEBUG) {
      $request->debugger->append(Debugger::now() . "\n");
      $request->debugger->append('$_POST[\'fname\']: ' . $_POST['fname'] . "\n");
    }
    $request->processFileImportation($_POST['fname']);
    break;
  case 'processFileDeletion':
    $request->processFileDeletion($_POST['fname']);
    break;
}

//if ($_POST['options'] == 'requestTfiles') {
//  $result = $request->requestTfiles();
//  if (DEBUG) {
//    $request->debugger->append(Debugger::now() . "\n");
//    $request->debugger->append($result);
//  }
//  print $result;
//} elseif ($_POST['options'] == 'processFileImportation') {
//  if (DEBUG) {
//    $request->debugger->append(Debugger::now() . "\n");
//    $request->debugger->append('$_POST[\'fname\']: ' . $_POST['fname'] . "\n");
//  }
//  $request->processFileImportation($_POST['fname']);
//} elseif ($_POST['options'] == 'getAnalyticsData') {
//  if (DEBUG) {
//    $request->debugger->append(Debugger::now() . "\n");
//    $request->debugger->append('$_POST[\'state\']:'.$_POST['state']."\n");
//    $request->debugger->append('$_POST[\'startDate\']:'.$_POST['startDate']."\n");
//  }
//  $result = $request->getAnalyticsData(array('state'=>$_POST['state'], 'startDate'=>$_POST['startDate']));
//  print $result;
//} elseif ($_POST['options'] == 'getStates') {
//  $result = $request->getStates();
//  print $result;
//} elseif ($_POST['options'] == 'getCounties') {
//  $result = $request->getCounties();
//  print $result;
//} elseif ($_POST['options'] == 'requestTcrimesSize') {
//  $result = $request->requestTcrimesSize();
//  print $result;
//}
?>
