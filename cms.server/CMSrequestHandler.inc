<?php
/** File: CMSrequestHandler.inc
 *  domain logic for the CMSrequestHandler.php 
 */
require_once 'Debugger.inc';
require_once 'model/Tfiles.inc';
require_once 'model/Tcrimes.inc';
require_once 'model/Tconvicts.inc';
require_once 'model/CrimeStat.inc';
require_once 'DatasetFactory.inc';
require_once 'KMeans/KMeans.inc';

class CMSrequestHandler {
  public $handle;
  public $debugger;
  private $tcrimes;
  private $tfiles;

  public function __construct() {
    $this->handle = fopen('log/CMSrequestHandler.log', 'w');
    $this->debugger = new Debugger($this->handle);
    $this->debugger->append("debugger initialized\n");
    $this->tcrimes = new Tcrimes();
    $this->tfiles = new Tfiles();
  }

  public function __destruct() {
    $this->debugger->append("destructor called\n");
    $this->debugger->close();
  }

  public function clustering($attr) {
    $result = $this->filterPOSTData($attr);
    $k = $attr['k'];
    $iterations = $attr['iterations'];
    $attr = $result['attr'];
    $UCRCategories = $result['UCRCategories'];
    $clusteringAttrs = $result['clusteringAttrs'];
    $datasets = $this->tcrimes->get($attr, $UCRCategories);

    //$this->debugger->append("DEBUG dataaset dump: ". var_export($datasets,true));

    //$this->debugger->append("DEBUG datasets count: " . count($datasets) . "\n");
    //$this->debugger->append("\n\nDEBUG clusteringAttrs: " . var_export($clusteringAttrs, true) . "\n\n");

    //initialize datasets
    $crimes = array();
    foreach ($datasets as $d) {
      $crime = DatasetFactory::create($d['UCRCategories'], $d, $clusteringAttrs);

      //add an instance of Convict to crime if the return value from dataset of
      //this crime is solved.
      if ($d['convictId'] != null) {
        $crime->addConvict(new Convict($d['convictId'], $d['fname'], $d['middle'], $d['lname'], $d['age'], $d['address']));
        //$this->debugger->append("DEBUG adding convict: " . var_export($crime, true));
      } else {
        $crime->addSuspect(new Suspect($d['suspectId'], $d['age'], $d['raceId'], $d['weaponId']));
        if ($d['age'] != null || (strlen($d['age']) > 0))
          ;//$this->debugger->append("DEBUG adding suspect: " . var_export($crime, true));
      }
      $crimes[] = $crime;
    }

    //$this->debugger->append("DEBUG dumping crimes after add suspect/convict: \n" . var_export($crimes, true));

    //$this->debugger->append("DEBUG crimes count: \n" . count($crimes) . "\n");

    $kmeans = new KMeans($k, $iterations, $crimes, $clusteringAttrs);
    $kmeans->start(true);
    $clusters = $kmeans->getClusterOutput();

    $buf = "<CrimeReports>\n";
    $i = 0;
    foreach ($clusters as $cluster) {
      foreach ($cluster as $crime) {
        $buf .= "    <CrimeReport>\n";
        $buf .= "      <cluster>".$i."</cluster>";
        $buf .= "      <UCRCategories>".$crime->get('UCRCategories')."</UCRCategories>\n";
        $buf .= "      <business>".$crime->get('business')."</business>\n";
        $buf .= "      <county>".$crime->get('county')."</county>\n";
        $buf .= "      <startDate>".$crime->get('startDate')."</startDate>\n";
        $buf .= "      <lat>".$crime->get('lat')."</lat>\n";
        $buf .= "      <lon>".$crime->get('lon')."</lon>\n";
        $buf .= "      <zip>".$crime->get('zip')."</zip>\n";
        $buf .= "      <crimeCategory>".$crime->get('crimeCategory')."</crimeCategory>\n";
        $buf .= "      <description>".$crime->get('description')."</description>\n";

        //convict
        if (strlen($crime->get('convictId')) > 0) {
          $buf .= "      <convictId>".$crime->get('convictId')."</convictId>\n";
          $buf .= "      <age>".$crime->get('convictAge')."</age>\n";
          $buf .= "      <raceId>".$crime->get('convictRace')."</raceId>\n";
          $buf .= "      <fname>".$crime->get('convictFname')."</fname>\n";
          $buf .= "      <middle>".$crime->get('convictMiddle')."</middle>\n";
          $buf .= "      <lname>".$crime->get('convictLname')."</lname>\n";
          $buf .= "      <address>".$crime->get('convictAddress')."</address>\n";
          $buf .= "      <weaponId>".$crime->get('convictWeapon')."</weaponId>\n";
        } elseif (strlen($crime->get('suspectId') > 0)){
          $buf .= "      <suspectId>".$crime->get('suspectId')."</suspectId>\n";
          $buf .= "      <age>".$crime->get('suspectAge')."</age>\n";
          $buf .= "      <raceId>".$crime->get('suspectRace')."</raceId>\n";
          $buf .= "      <weaponId>".$crime->get('suspectWeapon')."</weaponId>\n";
        }

        $buf .= "    </CrimeReport>\n";
      }
      ++$i;
    }
    $buf .= "</CrimeReports>";
    return $buf;
  }

  public function getNumberOfCrimesPerCategory($attr) {
    //$this->debugger->append("attr: " . var_export($attr, true));
    $result = $this->tcrimes->get($attr);
    $crimeStat = new CrimeStat($result);
    $numberOfCrimesPerCategory = $crimeStat->numberOfCrimesPerCategory();
    //$this->debugger->append("number: \n" .  var_export($numberOfCrimesPerCategory, true));

    $buf = "<crimes>\n";
    foreach ($numberOfCrimesPerCategory as $k=>$v) {
      $buf .= "  <crime>\n";
      $buf .= sprintf("    <crimeCategory>%s</crimeCategory><number>%s</number>\n", $k, $v);
      $buf .= "  </crime>\n";
    }
    $buf .= "</crimes>\n";

    return $buf;
  }

  public function getPercentageOfTotalCrime($attr) {
    //$this->debugger->append("attr: " . var_export($attr, true));
    $result = $this->tcrimes->get($attr);
    $crimeStat = new CrimeStat($result);
    $percentage = $crimeStat->percentageOfTotalCrime();
    //$this->debugger->append("\npercentage: \n" . var_export($percentage, true));

    $buf = "<percentages>\n";
    foreach ($percentage as $k=>$v) {
      $buf .= "  <crime>\n";
      $buf .= sprintf("    <crimeCategory>%s</crimeCategory><percent>%s</percent>\n", $k, $v);
      $buf .= "  </crime>";
    }
    $buf .= "</percentages>\n";

    return $buf;
  }

  public function getStates() {
    $result = $this->tcrimes->getDistinct('state');
    $buf = "<states>\n";
    foreach ($result as $row) {
      foreach ($row as $k=>$v) {
        $buf .= sprintf("    <%s>%s</%s>\n", $k, $v, $k);
      }
    }
    $buf .= "</states>";
    return $buf;
  }

  public function getCounties() {
    $result = $this->tcrimes->getDistinct('county');
    $buf = "<counties>\n";
    foreach ($result as $row) {
      foreach ($row as $k=>$v) {
        $buf .= sprintf("    <%s>%s</%s>\n", $k, $v, $k);
      }
    }
    $buf .= "</counties>";
    return $buf;
  }

  //FIXME: construct new associative array for valid parameters for getDatasets
  private function filterPOSTData($attr) {
    $this->debugger->append('attr: '.var_export($attr, true));
    $filtered = array();
    $UCRCategories = array();
    $clusteringAttrs = array();

    foreach ($attr as $k=>$v) {
      switch ($k) {
        case 'UCRCategories':
          foreach ($v as $cat) {
            $UCRCategories[] = $cat;
          }
          break;
        case 'county': $filtered[$k] = $v; break;
        case 'state': $filtered[$k] = $v; break;
        case 'solved': $filtered[$k] = $v; break;
        case 'clusteringAttrs':
          foreach ($v as $clusteringAttr) {
            $clusteringAttrs[] = $clusteringAttr;
          }
          break;
        //case 'k': $filtered[$k] = $v; break;
        //case 'iterations': $filtered[$k] = $v; break;
      }
    }

    //$this->debugger->append("filtered:" . var_export($filtered, true));
    //$this->debugger->append("UCRCategories: " . var_export($UCRCategories, true));
    //$this->debugger->append("clusteringAttrs: ". var_export($clusteringAttrs, true));
    
    return array('attr'=>$filtered,
                 'UCRCategories'=>$UCRCategories,
                 'clusteringAttrs'=>$clusteringAttrs);
  }

  public function getDatasets($attr) {
    $result = $this->filterPOSTData($attr);
    $attr = $result['attr'];
    $UCRCategories = $result['UCRCategories'];
    $result = $this->tcrimes->get($attr, $UCRCategories);

    $buf = "<CrimeReports>\n";
    foreach ($result as $crime) {
      $buf .= "  <CrimeReport>\n";
      foreach ($crime as $k=>$v) {
        $buf .= sprintf("    <%s>%s</%s>\n", $k, $v, $k);
      }
      $buf .= "  </CrimeReport>\n";
    }

    $buf .= "</CrimeReports>";
    return $buf;
  }

  public function getPreSelectionTotalOfCrimes($attr) {
    $result = $this->filterPOSTData($attr);
    $attr = $result['attr'];
    $UCRCategories = $result['UCRCategories'];
    if (!count($UCRCategories)) return 0;
    $result = $this->tcrimes->get($attr, $UCRCategories);
    
    return count($result);
  }


  /**
   *  Return an xml string with list of all files in table `files`
   */
  public function requestTfiles() {
    $result = $this->tfiles->getAll();
    $buf = "<files>\n";
    foreach ($result as $row) {
      $buf .= "  <file>\n";
      foreach ($row as $k=>$v) {
        $buf .= sprintf("    <%s>%s</%s>\n", $k, $v, $k);
      }
      $buf .= "  </file>\n";
    }
    $buf .= "</files>";
    return $buf;
  }

  /** Return the size of table crimes */
  public function requestTcrimesSize() {
    //$buf = "<size>".$this->tcrimes->size()."</size>";
    //return $buf;
    return $this->tcrimes->size();
  }

  public function processFileDeletion($name) {
    $file = $this->tfiles->get(array('name'=>$name));
    unlink($file['fpath']); //delete correspond file
    $this->tfiles->name = $name;
    $this->tfiles->delete();
    $this->debugger->append("deleting: ".$this->tfiles->name);
  }

  public function processFileImportation($name) {
    $this->debugger->append("processFileImportation:\n");
    $this->debugger->append($file);

    //get the file object/array on local system
    $file = $this->tfiles->get(array('name'=>$name));

    $buf = var_export($file, true);
    $this->debugger->append($buf);
    $this->debugger->append("fpath: " . $file['fpath'] . "\n");


    $assocArrayCrimeReports = array();
    $xml = new XMLReader();
    $result = $xml->open($file['fpath']); //lookup for `file` path
    $this->debugger->append("opened: " . $result . "\n");

    $assocArrayCrimeReports = array();
    //turn the $xml to assciative array; or return `tree` multi-dimensional array
    $tree = self::advxml2assoc($xml, &$assocArrayCrimeReports);

    $this->debugger->append("count: " . count($assocArrayCrimeReports) . "\n");
    //$this->debugger->append(var_export($assocArrayCrimeReports, true));

    $objCrimeReports = array();
    //create crime objects from associative array crime reports
    foreach ($assocArrayCrimeReports as $aacr) {
      //$this->debugger->append(var_export($aacr, true));

      $crime = new Tcrimes();
      $crime->file = $aacr['file'];
      $crime->UCRCategories = $aacr['UCRCategories'];
      $crime->UCRCategoriesId = $aacr['UCRCategoriesId'];//
      $crime->crimeCategory = $aacr['crime'];
      $crime->business = $aacr['bussiness'];
      $crime->county = $aacr['county'];
      $crime->startDate = $aacr['startDate'];
      $crime->addressId = $aacr['AddressId'];//
      $crime->location = $aacr['location'];
      $crime->city = $aacr['city'];
      $crime->state = $aacr['state'];
      $crime->zip = $aacr['zip'];
      $crime->service = $aacr['service'];
      $crime->accuracy = $aacr['accuracy'];
      $crime->lat = $aacr['lat'];
      $crime->lon = $aacr['lon'];
      $crime->description = $aacr['description'];

      $crime->save();

      if (isset($aacr['ArrestedId'])) {
        $convict = new Tconvicts();
        $convict->crimeId = $crime->lastInsertId();
        $convict->fname = $aacr['firstName'];
        $convict->middle = $aacr['middleName'];
        $convict->lname = $aacr['lastName'];
        $convict->age = $aacr['age'];
        $convict->address = $aacr['address'];
        $convict->save();

        $targetCrime = Tcrimes::getInstance($crime->lastInsertId());
        $targetCrime->convictId = $convict->lastInsertId();
        $targetCrime->update();

        $debugger = new Debugger(fopen('log/crime.get.log', 'a'));
        $debugger->append("crime::update(): " . var_export($targetCrime, true));
        //$debugger->append("DEBUG get by crime id: " .
        //    var_export($crime->get(array('id'=>$crime->lastInsertId())), true));
        $debugger->close();
      }
    }
  }

  /** Enhanced version of xml2assoc: 
   *  $xml, the input
   *  &$assocArrayCrimeReports, the target output of crime reports in 2D associative array
   *  &$i, the helper variable for array index
   */
  public static function advxml2assoc($xml, $assocArrayCrimeReports, $i = -1) { 
    $tree = null; 
    while ($xml->read()) 
      switch ($xml->nodeType) { 
        case XMLReader::END_ELEMENT: return $tree; 
        case XMLReader::ELEMENT: 
          if ($xml->name == 'file') ++$i; //increment if element == file
          $node = array('tag'=>$xml->name, 'value'=>$xml->isEmptyElement ? '' : self::advxml2assoc($xml, &$assocArrayCrimeReports, &$i)); 
          switch ($xml->name) {
            case 'file': $assocArrayCrimeReports[$i]['file'] = $node['value']; break;
            case 'UCRCategories': $assocArrayCrimeReports[$i]['UCRCategories'] = $node['value']; break;
            case 'crime': $assocArrayCrimeReports[$i]['crime'] = $node['value']; break;
            case 'bussiness': $assocArrayCrimeReports[$i]['bussiness'] = $node['value']; break;
            case 'county': $assocArrayCrimeReports[$i]['county'] = $node['value']; break;
            case 'startDate': $assocArrayCrimeReports[$i]['startDate'] = $node['value']; break;
            case 'Address':
              //FIXME: Address extra id attrbutes
              break;
            case 'location': $assocArrayCrimeReports[$i]['location'] = $node['value']; break;
            case 'city': $assocArrayCrimeReports[$i]['city'] = $node['value']; break;
            case 'state': $assocArrayCrimeReports[$i]['state'] = $node['value']; break;
            case 'zip': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'service': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'accuracy': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'lat': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'lon': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'description': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'Arrested': //FIXME: Arrested extra id attributes
              break;
            case 'firstName': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'middleName': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'lastName': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'age': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            case 'address': $assocArrayCrimeReports[$i][$xml->name] = $node['value']; break;
            default:
          }
          if ($xml->hasAttributes) {
            $parentName = $xml->name;
            while ($xml->moveToNextAttribute()) { 
              $node['attributes'][$parentName.ucfirst($xml->name)] = $xml->value; 
              //echo $node['attributes'][$parentName.ucfirst($xml->name)];
              $assocArrayCrimeReports[$i][$parentName.ucfirst($xml->name)] = $xml->value;
            }
          }
          $tree[] = $node; 
          break; 
        case XMLReader::TEXT: 
        case XMLReader::CDATA: 
          $tree .= $xml->value; 
      } 
    return $tree;
  }
}

//$r = new CMSrequestHandler();
//$r->processFileImportation('FallsChurchParserReports.xml');
//$r->getAnalyticsData(array('state'=>'VA', 'startDate'=>'2007-01-16'));
//$r->getAnalyticsData(array('state'=>'VA'));
//$r->getStates();
//$r->getCounties();
?>
