<?php
/** 
 *  File: ActiveRecord.inc
 */
require_once 'Debugger.inc';
require_once 'DB.inc';

abstract class ActiveRecord {
  protected $conn;
  protected $dhandle;
  public $debugger;

  public function __construct() {
    $this->conn = DB::getInstance();
  }

  public function __destruct() {
    $this->debugger->close();
  }

  public abstract function get(Array $attr);
  public abstract function getAll();
  public abstract function save();
}
?>
