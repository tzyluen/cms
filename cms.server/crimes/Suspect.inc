<?php
class Suspect {
  private $id;
  private $age;
  private $race;
  private $weapon;

  public function __construct($id, $age, $race, $weapon) {
    $this->id = $id;
    $this->age = $age;
    $this->race = $race;
    $this->weapon = $weapon;
  }

  public function get($attr) {
    return $this->$attr;
  }

  public function __toString() {
  }
}
?>
