<?php
$dsn = 'mysql:host=127.0.0.1;';
$pdo = new PDO($dsn, 'root', '');

$setup = DBSetup::getInstance($pdo);
$setup->start();

class DBSetup {
  private $pdo;
  const DEBUG = false;

  /** Singleton, prevent instantiation */
  private function __construct($pdo) {
    $this->pdo = $pdo;
  }

  public function start() {
    $this->createDB();
    $this->createTfiles();
    $this->createTcrimes();
    $this->createTConvicts();
    $this->createTSuspects();
    $this->createTWeapons();
    $this->createTRaces();
  }

  public function getInstance($pdo) {
    return new self($pdo); 
  }

  /** Create database: cms */
  private function createDB() {
$sql = <<<___SQL
  CREATE DATABASE IF NOT EXISTS `cms`;
  USE cms;
___SQL;

    $stmt = $this->pdo->prepare($sql);
    $result = $stmt->execute();
    if ($result) echo "$sql\n"; else print_r($stmt->errorInfo());
  }

  /** Create table: files */
  private function createTfiles() {
$sql = <<<___SQL
  CREATE TABLE IF NOT EXISTS `files` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `name` VARCHAR(255) NOT NULL,
    `fpath` VARCHAR(255) NOT NULL,
    `crimestable` BOOLEAN NOT NULL,
    `adddate` DATE NOT NULL,
    PRIMARY KEY(id)
  );
___SQL;
  
    $stmt = $this->pdo->prepare($sql);
    $result = $stmt->execute();
    if ($result) echo "$sql\n"; else print_r($stmt->errorInfo());
  }

  private function createTcrimes() {
$sql = <<<___SQL
  CREATE TABLE IF NOT EXISTS `crimes` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `file` VARCHAR(255) NOT NULL,
    `UCRCategories` VARCHAR(255) NOT NULL,
    `UCRCategoriesId` INT,
    `crimeCategory` VARCHAR(255),
    `business` VARCHAR(255),
    `county` VARCHAR(255),
    `startDate` DATETIME,
    `addressId` INT,
    `location` VARCHAR(255),
    `city` VARCHAR(255),
    `state` VARCHAR(255),
    `zip` INT,
    `service` VARCHAR(255),
    `accuracy` INT,
    `lat` FLOAT(11, 8),
    `lon` FLOAT(11, 8),
    `description` TEXT,
    `convictId` INT,
    `suspectId` INT,
    PRIMARY KEY(id),
    FOREIGN KEY(convictId) REFERENCES convicts(id),
    FOREIGN KEY(suspectId) REFERENCES suspects(id)
  );
___SQL;
    $stmt = $this->pdo->prepare($sql);
    $result = $stmt->execute();
    if ($result) echo "$sql\n"; else print_r($stmt->errorInfo());
  }

  private function createTConvicts() {
$sql = <<<___SQL
  CREATE TABLE IF NOT EXISTS `convicts` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `crimeId` INT,
    `fname` VARCHAR(25),
    `middle` VARCHAR(10),
    `lname` VARCHAR(40),
    `age` INT,
    `raceId` INT,
    `weaponId` INT,
    `address` VARCHAR(80),
    PRIMARY KEY(id),
    FOREIGN KEY(crimeId) REFERENCES crimes(id),
    FOREIGN KEY(raceId) REFERENCES races(id),
    FOREIGN KEY(weaponId) REFERENCES weapons(id)
  );
___SQL;
    $stmt = $this->pdo->prepare($sql);
    $result = $stmt->execute();
    if ($result) echo "$sql\n"; else print_r($stmt->errorInfo());
  }

  private function createTSuspects() {
$sql = <<<___SQL
  CREATE TABLE IF NOT EXISTS `suspects` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `crimeId` INT,
    `age` INT,
    `raceId` INT,
    `weaponId` INT,
    PRIMARY KEY(id),
    FOREIGN KEY(crimeId) REFERENCES crimes(id),
    FOREIGN KEY(raceId) REFERENCES races(id),
    FOREIGN KEY(weaponId) REFERENCES weapons(id)
  );
___SQL;
    $stmt = $this->pdo->prepare($sql);
    $result = $stmt->execute();
    if ($result) echo "$sql\n"; else print_r($stmt->errorInfo());
  }

  private function createTWeapons() {
$sql = <<<___SQL
  CREATE TABLE IF NOT EXISTS `weapons` (
    `id` INT,
    `name` VARCHAR(80),
    PRIMARY KEY(id)
  );

  INSERT INTO `weapons` (`id`,`name`) VALUES(100, 'pistol');
  INSERT INTO `weapons` (`id`,`name`) VALUES(110, 'short gun');
  INSERT INTO `weapons` (`id`,`name`) VALUES(200, 'knife');
  INSERT INTO `weapons` (`id`,`name`) VALUES(210, 'spears');
  INSERT INTO `weapons` (`id`,`name`) VALUES(220, 'sword');
  INSERT INTO `weapons` (`id`,`name`) VALUES(300, 'bat');
  INSERT INTO `weapons` (`id`,`name`) VALUES(310, 'nunchaku');

___SQL;
    $stmt = $this->pdo->prepare($sql);
    $result = $stmt->execute();
    if ($result) echo "$sql\n"; else print_r($stmt->errorInfo());
  }

  private function createTRaces() {
$sql = <<<___SQL
  CREATE TABLE IF NOT EXISTS `races` (
    `id` INT,
    `name` VARCHAR(80),
    PRIMARY KEY(id)
  );

  INSERT INTO `races` (`id`,`name`) VALUES(100, 'caucasian');
  INSERT INTO `races` (`id`,`name`) VALUES(110, 'latino');
  INSERT INTO `races` (`id`,`name`) VALUES(120, 'native american');
  INSERT INTO `races` (`id`,`name`) VALUES(130, 'black');
  INSERT INTO `races` (`id`,`name`) VALUES(200, 'asian');
  INSERT INTO `races` (`id`,`name`) VALUES(300, 'eskimo');
  
___SQL;
    $stmt = $this->pdo->prepare($sql);
    $result = $stmt->execute();
    if ($result) echo "$sql\n"; else print_r($stmt->errorInfo());
  }
}
?>
