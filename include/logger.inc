<?php
namespace MinskElectro {

  class Logger {

    public function __construct() {}

    public function __destruct() {}

    public function open() {
      $rc     = RC_SUCCESS;
      if( ! @openlog( ME_LOG_IDENT, (LOG_ODELAY|LOG_PERROR), ME_LOG_FACILITY ) ) {
        $e  = error_get_last();
        print_r( $e );
        $rc = $e['type'];
      }
      return $rc;
    }

    public function close() {
      $rc     = RC_SUCCESS;
      if( ! @closelog() ) {
        $e  = error_get_last();
        print_r( $e );
        $rc = $e['type'];
      }
      return $rc;
    }

    public function log( $sev, $rc, $msg ) {
      $rc     = RC_SUCCESS;
      if( ! @syslog( LOG_ERR, 'rc:'.$rc.' '.$msg ) ) {
        $e  = error_get_last();
        print_r( $e );
        $rc = $e['type'];
      }
      return $rc;
    }
  }

} // namespace MinskElectro