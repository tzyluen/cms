<?php
require_once 'ActiveRecord.inc';

class Tconvicts extends ActiveRecord {
  const INSERT_SQL = 'INSERT INTO convicts(crimeId, fname, middle, lname, age, raceId, weaponId, address) VALUES(:crimeId, :fname, :middle, :lname, :age, :raceId, :weaponId, :address)';

  //fields mapped to table `convicts` field
  public $id;
  public $crimeId;  //reference key to table `crimes` id
  public $fname;
  public $middle;
  public $lname;
  public $age;
  public $raceId;
  public $weaponId;
  public $address;

  public function __construct() {
    $this->conn = DB::getInstance();
    $this->dhandle = fopen('log/Tconvicts.log', 'w');
    $this->debugger = new Debugger($this->dhandle);
  }

  public function get(Array $attr) {
    $argc = count($attr);
    $sql = "SELECT * FROM convicts WHERE "; 
        
    foreach ($attr as $k=>$v) {
      switch ($k) {
        case 'id': $sql .= $k . '=' . $v; break;
        case 'crimeId': $sql .= $k . '=' . "'$v'"; break;
        case 'age': $sql .= $k . '=' . $v; break;
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

  public function getAll() {
    $stmt = $this->conn->prepare('SELECT * FROM convicts;');
    $stmt->execute();
    $result = $stmt->fetchAll(PDO::FETCH_ASSOC);

    return $result;
  }

  public function save() {
    $stmt = $this->conn->prepare(self::INSERT_SQL);
    $saveTarget = array(
        ':crimeId'=>$this->crimeId,
        ':fname'=>$this->fname,
        ':middle'=>$this->middle,
        ':lname'=>$this->lname,
        ':age'=>$this->age,
        ':raceId'=>$this->raceId,
        ':weaponId'=>$this->weaponId,
        ':address'=>$this->address);

    $this->debugger->append("Tconvicts saveTarget dump: " . var_export($saveTarget, true));
    $result = $stmt->execute($saveTarget);
    $this->debugger->append(var_export($result, true));

    if ($result) {
      $this->id = (int)$this->conn->lastInsertId('id');
    } else {
      $this->debugger->append("oops, " . var_export($this->conn->errorInfo(), true));
    }
  }

  public function lastInsertId() {
    //$this->debugger->append($this->conn->lastInsertId('id');
    return (int)$this->conn->lastInsertId();
  }
}
?>
