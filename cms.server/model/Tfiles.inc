<?php
/**
 *  File: Tfiles.inc
 */
require_once 'ActiveRecord.inc';

class Tfiles extends ActiveRecord {
  const UPLOAD_REPOSITORY= 'upload/';
  const INSERT_SQL = 'INSERT INTO files(name, fpath, crimestable, adddate) VALUES(:name, :fpath, :crimestable, now())';
  const SELECT_SQL_ID_NAME = 'SELECT * FROM files WHERE name=:name';

  //fields mapped to table `files` field
  private $id;
  public $name;
  public $fpath;
  public $crimestable = 0;
  private $adddate;

  public function __construct() {
    $this->conn = DB::getInstance();
    $this->dhandle = fopen('log/Tfiles.log', 'w');
    $this->debugger = new Debugger($this->dhandle);
    $this->debugger->append(Debugger::now());
  }

  public function __destruct() {
    $this->debugger->append(__CLASS__ . "destructed\n");
    $this->debugger->close();
  }

  /** return a record matching row from the table whose `id` and `name` equal to
   *  input arguments.
   */
  public function get(Array $attr) {
    $name = $attr['name'];
    $stmt = $this->conn->prepare(self::SELECT_SQL_ID_NAME);
    $stmt->execute(array(':name'=>$name));
    $result = $stmt->fetchALL(PDO::FETCH_ASSOC);

    if ($result) {
      //return $result;
      $buf = var_export($result, true);
      $this->debugger->append($buf."\n");

      return $result[0];
    } return 0;
  }

  public function getAll() {
    $stmt = $this->conn->prepare('SELECT * FROM files;');
    $stmt->execute();
    $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
    $buf;

    foreach ($result as $row) {
      foreach ($row as $k=>$v) {
        $buf .= sprintf("[%s]:%s\n", $k, $v);
      }
    }
    $this->debugger->append($buf);

    return $result;
  }

  public function save() {
    $stmt = $this->conn->prepare(self::INSERT_SQL);
    $result = $stmt->execute(array(
          ':name'=>$this->name,
          ':fpath'=>$this->fpath,
          ':crimestable'=>$this->crimestable));
    if ($result) {
      $this->id = (int)$this->conn->lastInsertId(); //assign last insert id
    } else {
      trigger_error('DB Error: '.$this->conn->errorInfo());
    }
  }

  public function delete() {
    $this->debugger->append("Tfiles::name's scope".$this->name."\n");
    $stmt = $this->conn->prepare("DELETE FROM files WHERE name='".$this->name."'");
    $stmt->execute();
  }
}

//$files = new Tfiles();
//$files->name = 'junkie';
//$files->fpath = 'kurt';
//$files->crimestable = '0';
//$files->save();
//
//$buf = var_export($files->getAll(), true);
//echo $buf;
?>
