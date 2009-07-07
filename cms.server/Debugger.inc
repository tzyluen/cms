<?php
/**
 * Trivial debugger log
 */
class Debugger {
  private $handle;
  private $message;

  public function __construct($handle) {
    $this->setOutputStream($handle);
  }
  public function setOutputStream($handle) {
    $this->handle = $handle;
  }

  public function append($message) {
    fwrite($this->handle, $message);
  }

  public function close() {
    fclose($this->handle);
  }

  public static function now() {
    return date("F j, Y, g:i:s:u a");
  }
}
?>
