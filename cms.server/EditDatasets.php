<?php
require_once 'DB.inc';

class EditDatasets {
  private $conn;

  public function __construct() {
    $this->conn = DB::getInstance();
  }

  public function updateAll() {
    //age
    //raceId
    //weaponId
    $stmt = $this->conn->prepare("ALTER TABLE suspects MODIFY age FLOAT(4,2)");
    $result = $stmt->execute();
    var_dump($result);

    $stmt = $this->conn->prepare("ALTER TABLE suspects MODIFY raceId FLOAT(4,2)");
    $result = $stmt->execute();
    var_dump($result);

    $stmt = $this->conn->prepare("ALTER TABLE suspects MODIFY weaponId FLOAT(4,2)");
    $result = $stmt->execute();
    var_dump($result);

    $this->update('age', 20.00, 0.2);
    $this->update('age', 25.00, 0.25);
    $this->update('age', 30.00, 0.30);
    $this->update('age', 35.00, 0.35);
    $this->update('age', 40.00, 0.40);
    $this->update('age', 45.00, 0.45);
    $this->update('age', 50.00, 0.50);
    $this->update('age', 55.00, 0.55);

    /*$this->update('raceId', 20.00, 0.2);
    $this->update('raceId', 25.00, 0.25);
    $this->update('raceId', 30.00, 0.30);
    $this->update('raceId', 35.00, 0.35);
    $this->update('raceId', 40.00, 0.40);
    $this->update('raceId', 45.00, 0.45);*/

    /*
    $this->update('raceId', 0.2, 0.21);
    $this->update('raceId', 0.25, 0.22);
    $this->update('raceId', 0.30, 0.23);
    $this->update('raceId', 0.35, 0.24);
    $this->update('raceId', 0.40, 0.25);
    $this->update('raceId', 0.45, 0.26);

    $this->update('weaponId', 0.2, 0.21);
    $this->update('weaponId', 0.25, 0.22);
    $this->update('weaponId', 0.30, 0.23);
    $this->update('weaponId', 0.35, 0.24);
    $this->update('weaponId', 0.40, 0.25);
    $this->update('weaponId', 0.45, 0.26);
    $this->update('weaponId', 0.50, 0.27);
    */
  }

  private function update($target, $from, $to) {
    $stmt = $this->conn->prepare("UPDATE suspects SET $target=$to WHERE $target=$from");
    $result = $stmt->execute();
    var_dump($result);
  }
}

$ed = new EditDatasets();
$ed->updateAll();
?>
