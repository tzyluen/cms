<?php
class Location {

  //[location] => 6700 Wilson Blvd
  //[city] => Falls Church
  //[state] => VA
  //[zip] => 
  private $address;
  private $street;
  private $postcode;
  private $state;
  private $country;

  public function __construct($address, $street, $postcode, $state, $country) {
    $this->address = $address;
    $this->street = $street;
    $this->postcode = $postcode;
    $this->state = $state;
    $this->country = $country;
  }

  public function getAddress() {
    return $this->address;
  }

  public function setAddress($address) {
    $this->address = $address;
  }

  public function getStreet() {
    return $this->street;
  }

  public function setStreet($street) {
    $this->street = $street;
  }

  public function getPostcode() {
    return $this->postcode;
  }

  public function setPostcode($postcode) {
    $this->postcode = $postcode;
  }

  public function getState() {
    return $this->state;
  }

  public function setState($state) {
    $this->state = $state;
  }

  public function getCountry() {
    return $this->country;
  }

  public function setCountry($country) {
    $this->country = $country;
  }
}
?>
