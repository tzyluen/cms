<?php
require_once 'ActiveRecord.inc';

class Tcrimes extends ActiveRecord {
  const INSERT_SQL = 'INSERT INTO crimes(file, UCRCategories, UCRCategoriesId, crimeCategory, business, county, startDate, addressId, location, city, state, zip, service, accuracy, lat, lon, description, convictId, suspectId) VALUES(:file, :UCRCategories, :UCRCategoriesId, :crimeCategory, :business, :county, :startDate, :addressId, :location, :city, :state, :zip, :service, :accuracy, :lat, :lon, :description, :convictId, :suspectId)';
  const UPDATE_SQL = 'UPDATE crimes set file=?, UCRCategories=?, UCRCategoriesId=?, crimeCategory=?, business=?, county=?, startDate=?, addressId=?, location=?, city=?, state=?, zip=?, service=?, accuracy=?, lat=?, lon=?, description=?, convictId=?, suspectId=? WHERE id=?';

  //fields mapped to table `files` field
  public $id;
  public $file;
  public $UCRCategories;
  public $UCRCategoriesId;
  public $crimeCategory;
  public $business;
  public $county;
  public $startDate;
  public $addressId;
  public $location;
  public $city;
  public $state;
  public $zip;
  public $service;
  public $accuracy;
  public $lat;
  public $lon;
  public $description;
  public $convictId;
  public $suspectId;

  private $cache;
  private $size;  //caching the table size

  public function __construct() {
    $this->conn = DB::getInstance();
    $this->dhandle = fopen('log/Tcrimes.log', 'w');
    $this->debugger = new Debugger($this->dhandle);
  }

  public function getStatistic($state, $startDate) {
    $stmt = $this->conn->prepare('SELECT COUNT(*) FROM crimes WHERE state='.
        $state . ' AND ' . 'startDate=' . $startDate
        );
    $stmt->execute();
    $result = $stmt->fetchALL(PDO::FETCH_ASSOC);

    if ($result) {
      return $result[0];
    } return 0;
//select count(*) from crimes where state='VA' and startDate='2007-12-06';
  }

  //implementation revised:
  //public function get(Array $attr) {
  //  $argc = count($attr);
  //  $sql = "SELECT * FROM crimes WHERE "; 

  //  foreach ($attr as $k=>$v) {
  //    switch ($k) {
  //      case 'id': $sql .= $k . '=' . $v; break;
  //      case 'file': $sql .= $k . '=' . "'$v'"; break;
  //      case 'UCRCategories': $sql .= $k . '=' . "'$v'"; break;
  //      case 'UCRCategoriesId': $sql .= $k . '=' . $v; break;
  //      case 'crimeCategory': $sql .= $k . '=' . "'$v'"; break;
  //      case 'crimeId': $sql .= $k . '=' . $v; break;
  //      case 'business': $sql .= $k . '=' . "'$v'"; break;
  //      case 'county': $sql .= $k . '=' . "'$v'"; break;
  //      case 'startDate': $sql .= 'DATE(' . $k . ")" . '=' . "'$v'"; break;
  //      case 'addressId': $sql .= $k . '=' . $v; break;
  //      case 'location': $sql .= $k . '=' . "'$v'"; break;
  //      case 'city': $sql .= $k . '=' . "'$v'"; break;
  //      case 'state': $sql .= $k . '=' . "'$v'"; break;
  //      case 'zip': $sql .= $k . '=' . $v; break;
  //      case 'service': $sql .= $k . '=' . "'$v'"; break;
  //      case 'accuracy': $sql .= $k . '=' . $v; break;
  //      case 'lat': $sql .= $k . '=' . $v; break;
  //      case 'lon': $sql .= $k . '=' . $v; break;
  //      //case 'description': $sql += $k . '=' . $v; break;
  //      default:
  //        continue 2;
  //    }

  //    if ($i < $argc - 1) //next element is last elements
  //      $sql .= ' and ';
  //    ++$i;
  //  }
  //  $this->debugger->append("DEBUG: sql: " . $sql . "\n");

  //  $stmt = $this->conn->prepare($sql);
  //  $stmt->execute();
  //  $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
  //  $this->debugger->append(var_export($result, true));
  //  
  //  return $result;
  //}

  /**
   * function parameters expect associative array with key mapped to db table
   * column name. 
   * A missing second var length parameter expect the array containing
   * `UCRCategories` for the query.  
   */
  public function get(Array $attr) {
    $argc = count($attr);

    $this->debugger->append(var_export($attr, true));
    //pass `unsolved` key into attr parameters caused the query to return only unsolved crimes
    if ($attr['solved'] == 'solved') {
      --$argc;
      $sql = "SELECT * FROM crimes JOIN convicts ON (crimes.convictId = convicts.id) WHERE ";
    } elseif ($attr['solved'] == 'unsolved') {
      --$argc;
      $sql = 'SELECT * FROM crimes JOIN suspects ON (crimes.suspectId = suspects.id) WHERE ';
    } elseif ($attr['solved'] == 'both') {
      --$argc;
      $sql = "SELECT * FROM crimes LEFT JOIN convicts ON (crimes.convictId = convicts.id) WHERE "; 
    } else {
      $sql = "SELECT * FROM crimes WHERE ";
    }

    
    if (@$UCRCategories = func_get_arg(1)) {
      $sql .= '(';
      foreach ($UCRCategories as $v) {
        $sql .= "UCRCategories = '" . $v . "'";

        if ($j < (count($UCRCategories) - 1))
          $sql .= ' or ';
        ++$j;
      }

      if ($argc > 0)
        $sql .= ') and ';
    }

    foreach ($attr as $k=>$v) {
      switch ($k) {
        case 'id': $sql .= $k . '=' . $v; break;
        case 'file': $sql .= $k . '=' . "'$v'"; break;
        case 'UCRCategoriesId': $sql .= $k . '=' . $v; break;
        case 'crimeCategory': $sql .= $k . '=' . "'$v'"; break;
        case 'business': $sql .= $k . '=' . "'$v'"; break;
        case 'county': $sql .= $k . '=' . "'$v'"; break;
        case 'startDate': $sql .= 'DATE(' . $k . ")" . '=' . "'$v'"; break;
        case 'addressId': $sql .= $k . '=' . $v; break;
        case 'location': $sql .= $k . '=' . "'$v'"; break;
        case 'city': $sql .= $k . '=' . "'$v'"; break;
        case 'state': $sql .= $k . '=' . "'$v'"; break;
        case 'zip': $sql .= $k . '=' . $v; break;
        case 'service': $sql .= $k . '=' . "'$v'"; break;
        case 'accuracy': $sql .= $k . '=' . $v; break;
        case 'lat': $sql .= $k . '=' . $v; break;
        case 'lon': $sql .= $k . '=' . $v; break;
        case 'convictId': $sql .= $k . '=' . $v; break;
        case 'suspectId': $sql .= $k . '=' . $v; break;
        //case 'description': $sql += $k . '=' . $v; break;
        default:
          continue 2;
      }

      if ($i < $argc - 1) //next element is last elements
        $sql .= ' AND ';
      ++$i;
    }
    $this->debugger->append("SQL query string: " . $sql . "\n");

    $stmt = $this->conn->prepare($sql);
    $stmt->execute();
    $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
    $this->debugger->append("SQL query result dump: " . var_export($result, true));
    
    return $result;
  }


  /** Return two dimensional array with associative array in side outer index
   *  array.
   *  arg: a single attribute for table
   */
  public function getDistinct($arg) {
    $stmt = $this->conn->prepare('SELECT DISTINCT ' . $arg . ' FROM crimes');
    $stmt->execute();
    $result = $stmt->fetchALL(PDO::FETCH_ASSOC);
    
    if ($result) {
      //$this->debugger->append(Debugger::now());
      //$this->debugger->append(var_export($result, true)."\n");
      //foreach ($result as $row) {
      //  foreach ($row as $k=>$v) {
      //    $this->debugger->append("[$k]=>$v\n");
      //  }
      //}

      return $result;
    }
  }


  public function getAll() {
    if ($this->cache)
      return $this->cache;

    $stmt = $this->conn->prepare('SELECT * FROM crimes;');
    $stmt->execute();
    $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
    //$buf;
    //foreach ($result as $row) {
    //  foreach ($row as $k=>$v) {
    //    $buf .= sprintf("[%s]:%s\n", $k, $v);
    //  }
    //}
    //$this->debugger->append(Debugger::now());
    //$this->debugger->append($buf);

    $this->cache = $result;
    return $result;
  }

  public function lastInsertId() {
    return (int)$this->conn->lastInsertId();
  }

  public function save() {
    $stmt = $this->conn->prepare(self::INSERT_SQL);
    $saveTarget = array(
          ':file'=>$this->file,
          ':UCRCategories'=>$this->UCRCategories,
          ':UCRCategoriesId'=>$this->UCRCategoriesId,
          ':crimeCategory'=>$this->crimeCategory,
          ':business'=>$this->business,
          ':county'=>$this->county,
          ':startDate'=>$this->dateConversion($this->startDate),
          ':addressId'=>$this->addressId,
          ':location'=>$this->location,
          ':city'=>$this->city,
          ':state'=>$this->state,
          ':zip'=>$this->zip,
          ':service'=>$this->service,
          ':accuracy'=>$this->accuracy,
          ':lat'=>$this->lat,
          ':lon'=>$this->lon,
          ':description'=>$this->description,
          ':convictId'=>$this->convictId,
          ':suspectId'=>$this->suspectId);
    $this->debugger->append("saveTarget:\n" . var_export($saveTarget, true));
    $result = $stmt->execute($saveTarget);
    $this->debugger->append(var_export($result, true));
    if ($result) {
      $this->id = (int)$this->conn->lastInsertId(); //assign last insert id
    } else {
      //trigger_error('DB Error: '.$this->conn->errorInfo());
      $this->debugger->append("ERROR on Tcrimes SAVE, " . var_export($this->conn->errorInfo(), true));
    }
  }

  public function update() {
    if (isset($this->id) && isset($this->update)) {
      $stmt = $this->conn->prepare(self::UPDATE_SQL);
      $updateTarget = array(
          $this->file,
          $this->UCRCategories,
          $this->UCRCategoriesId,
          $this->crimeCategory,
          $this->business,
          $this->county,
          $this->startDate,
          $this->addressId,
          $this->location,
          $this->city,
          $this->state,
          $this->zip,
          $this->service,
          $this->accuracy,
          $this->lat,
          $this->lon,
          $this->description,
          $this->convictId,
          $this->suspectId,
          $this->id);

      $result = $stmt->execute($updateTarget);
      $this->debugger->append("update(): " . var_export($result, true));
      if ($result) {
      } else {
        $this->debugger->append("ERROR ON Tcrimes UPDATE, " . var_export($this->conn->errorInfo(), true));
      }
    }
  }

  /**
   *  Return an instance of Tcrimes, parameter id serve as unique value to
   *  create an object of self instance.
   *  WARNING: DO NOT USE ANY instance variable within this static method, it
   *  will silently terminate the execution without any error message from the
   *  client.
   */
  public static function getInstance($id) {
    $helper = new Tcrimes();
    $result = $helper->get(array('id'=>$id));
    $row = $result[0];
    $debugger = new Debugger(fopen('log/Tcrimes.getInstance.log', 'w'));
    $crime = new Tcrimes();

    $crime->id = $row['id'];
    $crime->file = $row['file'];
    $crime->UCRCategories = $row['UCRCategories'];
    $crime->UCRCategoriesId = $row['UCRCategoriesId'];
    $crime->crimeCategory = $row['crimeCategory'];
    $crime->business = $row['business'];
    $crime->county = $row['county'];
    $crime->startDate = $row['startDate'];
    $crime->addressId = $row['addressId'];
    $crime->location = $row['location'];
    $crime->city = $row['city'];
    $crime->state = $row['state'];
    $crime->zip = $row['zip'];
    $crime->service = $row['service'];
    $crime->accuracy = $row['accuracy'];
    $crime->lat = $row['lat'];
    $crime->lon = $row['lon'];
    $crime->description = $row['description'];

    $crime->update = true;
    
    $debugger->append("getInstance result:\n" . var_export($crime, true));
    $debugger->close();

    return $crime;
  }

  public function size() {
    if (!$this->size) {
      $stmt = $this->conn->prepare("SHOW TABLE STATUS FROM cms like 'crimes'");
      $stmt->execute();
      $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
      $size = $result[0]['Rows'];
      //$this->debugger->append("table criems size: " . $size . "\n");
      $this->size = $size;
      return $size;
    }
  }


  private function dateConversion($str) {
    return date("Y-m-d H:i:s", strtotime($str));
  }
}

//$crimes = new Tcrimes();
//$crimes->getAll();
//$crimes->get(array('zip'=>'22046', 'crimeCategory'=>'Larceny'),
//    array('LARCENY', 'BURGLARY', 'ROBERRY'));
?>
