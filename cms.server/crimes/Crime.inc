<?php
require_once 'Debugger.inc';
//require_once 'Location.inc';
require_once 'Convict.inc';
require_once 'Suspect.inc';

abstract class Crime {
  public $handle;
  public $debugger;
  protected $file;
  protected $UCRCategories;
  protected $crimeCategory;
  protected $business;
  protected $county;
  protected $startDate;
  //protected $address;
  protected $service;
  protected $accuracy;
  protected $description;
  protected $convict;
  protected $suspect;

  protected $UCRCategoriesId; //clustering attr
  protected $addressId;       //clustering attr
  protected $zip;             //clustering attr
  protected $lat;             //clustering attr
  protected $lon;             //clustering attr

  protected $cluster;
  protected $euDt;
  protected $clusteringAttrs;

  public function __construct($attr, $clusteringAttrs) {
    $this->handle = fopen('log/crime.log', 'w');
    $this->debugger = new Debugger($this->handle);

    $this->file = $attr['file'];
    $this->crimeCategory = $attr['crimeCategory'];
    $this->business = $attr['business'];
    $this->county = $attr['county'];
    $this->startDate = $attr['startDate'];
    //$this->location = new Location(); 
    $this->service = $attr['service'];
    $this->accuracy = $attr['accuracy'];
    $this->UCRCategories = $attr['UCRCategories'];
    $this->UCRCategoriesId = $attr['UCRCategoriesId'];
    $this->addressId = $attr['addressId'];
    $this->zip = $attr['zip'];
    $this->lat = $attr['lat'];
    $this->lon = $attr['lon'];
    $this->description = $attr['description'];

    //$this->convict = new Convict($attr['firstName'], $attr['middleName'],
    //    $attr['lastName'], $attr['age'], $attr['address']);

    $this->clusteringAttrs = $clusteringAttrs;
  }

  public function getCluster() {
    return $this->cluster;
  }

  public function setCluster($cluster) {
    $this->cluster = $cluster;
    $this->calcEuclideanDistance();
  }

  public function calcEuclideanDistance() {
    $centroid = $this->cluster->getCentroid();

    //called when DP is added to a cluster or when a Centroid is recalculated.
    $tempEuDt;
    foreach ($this->clusteringAttrs as $v) {
      switch ($v) {
        case 'suspectAge':
          $tempEuDt += pow(($this->suspect->get('age') - $centroid->get($v)), 2);
          break;
        case 'suspectRace':
          $tempEuDt += pow(($this->suspect->get('race') - $centroid->get($v)), 2);
          break;
        case 'suspectWeapon':
          $tempEuDt += pow(($this->suspect->get('weapon') - $centroid->get($v)), 2);
          break;
        case 'convictAge':
          $tempEuDt += pow(($this->convict->get('age') - $centroid->get($v)), 2);
          break;
        case 'convictRace':
          $tempEuDt += pow(($this->convict->get('race') - $centroid->get($v)), 2);
          break;
        case 'convictWeapon':
          $tempEuDt += pow(($this->convict->get('weapon') - $centroid->get($v)), 2);
          break;
        default:
          $tempEuDt += pow(($this->$v - $centroid->get($v)), 2);
      }
    }

    $this->euDt = sqrt($tempEuDt);
  }

  public function testEuclideanDistance($centroid) {
    $tempEuDt;
    foreach ($this->clusteringAttrs as $v) {
      switch ($v) {
        case 'suspectAge':
          $tempEuDt += pow(($this->suspect->get('age') - $centroid->get($v)), 2);
          break;
        case 'suspectRace':
          $tempEuDt += pow(($this->suspect->get('race') - $centroid->get($v)), 2);
          break;
        case 'suspectWeapon':
          $tempEuDt += pow(($this->suspect->get('weapon') - $centroid->get($v)), 2);
          break;
        case 'convictAge':
          $tempEuDt += pow(($this->convict->get('age') - $centroid->get($v)), 2);
          break;
        case 'convictRace':
          $tempEuDt += pow(($this->convict->get('race') - $centroid->get($v)), 2);
          break;
        case 'convictWeapon':
          $tempEuDt += pow(($this->convict->get('weapon') - $centroid->get($v)), 2);
          break;
        default:
          $tempEuDt += pow(($this->$v - $centroid->get($v)), 2);
      }
    }

    return sqrt($tempEuDt);
  }

  public function getCurrentEuDt() {
    return $this->euDt;
  }

  public function get($attr) {
    //$this->debugger->append("DEBUG: get:".$attr."  ".$this->$attr."\n");
    //perform missing data (null) imputation by substitute with the mean value
    switch ($attr) {
      case 'suspectId':
        return (isset($this->suspect)) ? $this->suspect->get('id') : null;
      case 'suspectAge':
        $this->debugger->append("DEBUG: get:".$attr." ".$this->suspect->get('age')."\n");
        return (isset($this->suspect)) ? $this->suspect->get('age') : null;
      case 'suspectRace':
        $this->debugger->append("DEBUG: get:".$attr." ".$this->suspect->get('race')."\n");
        return (isset($this->suspect)) ? $this->suspect->get('race') : null;
      case 'suspectWeapon':
        $this->debugger->append("DEBUG: get:".$attr." ".$this->suspect->get('weapon')."\n");
        return (isset($this->suspect)) ? $this->suspect->get('weapon') : null;
      case 'convictId':
        return (isset($this->convict)) ? $this->convict->get('id') : null;
      case 'convictFname':
        return (isset($this->convict)) ? $this->convict->get('fname') : null;
      case 'convictMiddle':
        return (isset($this->convict)) ? $this->convict->get('middle') : null;
      case 'convictLname':
        return (isset($this->convict)) ? $this->convict->get('lname') : null;
      case 'convictAge':
        return (isset($this->convict)) ? $this->convict->get('age') : null;
      case 'convictRace':
        return (isset($this->convict)) ? $this->convict->get('race') : null;
      case 'convictWeapon':
        return (isset($this->convict)) ? $this->convict->get('weapon') : null;
      case 'convictAddress':
        return (isset($this->convict)) ? $this->convict->get('address') : null;
      default:
        return $this->$attr;
    }
  }

  public function addConvict($convict) {
    $this->convict = $convict;
  }

  public function addSuspect($suspect) {
    $this->suspect = $suspect;
  }
}
?>
