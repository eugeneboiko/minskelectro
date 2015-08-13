<?php
  /**
  * Creates my exception output message
  *
  */
	class My_Exception extends Exception {

    /**
      * Overrides the method so that the message was required
      *
      */
    public function __construct($message, $code = 0, Exception $previous = null) {
        parent::__construct($message, $code, $previous);
    }

    /**
      * Overrides show message method
      *
      */
    public function __toString() {
      return APP_TEST_BRANCH ? parent::__toString() : __CLASS__ . ": " . $this->getMessage();
    }
	}