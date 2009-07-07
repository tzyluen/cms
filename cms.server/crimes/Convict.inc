<?php
class Convict {
  private $id;
  private $fname;
  private $middle;
  private $lname;
  private $age;
  private $address;

  public function __construct($id, $fname, $middle, $lname, $age, $address) {
    $this->id = $id;
    $this->fname = $fname;
    $this->middle = $middle;
    $this->lname = $lname;
    $this->age = $age;
    $this->address= $address;
  }

  public function get($attr) {
    return $this->$attr;
  }

  public function __toString() {
  }
}
?>
