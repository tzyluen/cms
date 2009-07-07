<?php
require_once 'Debugger.inc';
require_once 'HTTP/Upload.php';
require_once 'model/Tfiles.inc';

/**
 *  File: uploadFileHandler.php:
 *  handler for client upload the xml file, and place the file in place
 */
define(DEBUG, true);
define(MAX_FSIZE, pow(1024, 3));  //10MB
$now = date("F j, Y, g:i:s:u a");
$handle = fopen('log/uploadFileHandler.log', 'w');

$debugger = new Debugger($handle);
$debugger->append($now . "\n=====================================\n");
$buf .= sprintf("%s\n", var_export($_FILES, true));
$debugger->append($buf);

$upload = new HTTP_Upload("en");
$files = $upload->getFiles();
foreach ($files as $file) {
  if ($file->isError()) {
    $debugger->append($file->getMessage());
    return;
  } elseif ($file->isValid()) {
    $file->setName($file->getProp('real'));
    $dest_name = Tfiles::UPLOAD_REPOSITORY;
    $debugger->append($dest_name);
    $dest_name .= $file->moveTo(Tfiles::UPLOAD_REPOSITORY);
    //store record on db
    $modelFiles = new Tfiles();
    $modelFiles->name = $file->getProp('real');
    $modelFiles->fpath = $dest_name;
    $modelFiles->save();

    if (PEAR::isError($dest_name)) {
      $debugger->append($dest_name->getMessage());
    }

    $real = $file->getProp();
  } elseif ($file->isMissing()) {
    $debugger->append("No file was provided.\n");
  }
  $debugger->append(var_export($file->getProp(), true));
}

$debugger->close();
?>
