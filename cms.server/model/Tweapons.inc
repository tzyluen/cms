<?php
require_once 'ActiveRecord.inc';

class Tweapons extends ActiveRecord {

  //fields mapped to table `convicts` field
  public $id;
  public $name;

  public function __construct() {
    $this->conn = DB::getInstance();
    $this->dhandle = fopen('log/Tweapons.log', 'w');
    $this->debugger = new Debugger($this->dhandle);
  }

  public function get(Array $attr) {
    $argc = count($attr);
    $sql = "SELECT * FROM weapons WHERE "; 
        
    foreach ($attr as $k=>$v) {
      switch ($k) {
        case 'id': $sql .= $k . '=' . $v; break;
        case 'name': $sql .= $k . '=' . $v; break;
        default:
          continue 2;
      }   

      if ($i < $argc - 1) //next element is last elements
        $sql .= ' and ';
      ++$i;
    }   
    $this->debugger->append("DEBUG: sql: " . $sql . "\n");

    $stmt = $this->conn->prepare($sql);
    $stmt->execute();
    $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
    $this->debugger->append(var_export($result, true));
        
    return $result;
  }

  public function lastInsertId() {
    return (int)$this->conn->lastInsertId();
  }
}
?>
