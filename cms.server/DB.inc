<?php
require_once 'appconf.inc';

/** DB Factory */
class DB {
  private static $conn;

  private function __construct() {}

  public static function getInstance() {
    if (!$conn) {
      self::$conn = new PDO(DSN, DBUNAME, DBPASS);
    }
    return self::$conn;
  }
}
?>
