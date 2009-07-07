<?php
/* A test to ensure the system setup is meet the requirement */

function testPEARInstalled() {
  require_once 'System.php';
  var_dump(class_exists('System'));
}

function testHTTPUploadInstalled() {
}

testPEARInstalled();
?>
