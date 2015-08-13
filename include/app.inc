<?php
  require_once 'typedefs.inc';
  require_once 'my_exception.inc';

  /**
  * Adds logging to mail and database (mysql, sqlite) operations
  *
  */
  class App {

    // link on database
    public $pdo = null;
    protected $pdoConnStr = null;

    /**
      * Connects to the database and returns ptr
      *
      */
    public function __construct() {

      $pdoConnStr = "mysql:host=" . ME_DB_HOST . ";dbname=" . ME_DB_NAME;
      try {
        $pdo = new PDO($pdoConnStr, ME_DB_USR, ME_DB_PWD);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $pdo->query('SET NAMES utf8');
      } catch (PDOException $e) {
        $this->myLog( LOG_ERR, 'Fatal error: connect to database ' . $pdoConnStr . ';port=' . ini_get('mysqli.default_port')
            . ';dbuser=' . ME_DB_USR . ' in ' . $e->getFile() . ' on line ' . $e->getLine() );
        throw new My_Exception('Connect to database');
      }
      //$this->myLog( LOG_DEBUG, 'Success: ' . $sql . ';user=' . $user . ';pass=' . $pwd );
      $this->pdoConnStr = $pdoConnStr;
      $this->pdo = $pdo;
    }

    /**
      * Disconnects from the database and returns ptr
      *
      */
    public function __destruct() {
      $this->pdo = null;
    }

    /**
     * Log messages
     *
     * @param constant $priority
     * @param string $message
     */
    public function myLog( $priority, $message ) {

      if ( ! @openlog( ME_LOG_IDENT, (LOG_ODELAY|LOG_PERROR), ME_LOG_FACILITY ) ) {
        $e = error_get_last();
        throw new My_Exception('Open log' . htmlspecialchars($e['message']));
      }

      if( ! @syslog( $priority, $message ) ) {
        $e = error_get_last();
        throw new My_Exception('Write log' . htmlspecialchars($e['message']));
      }

      if( ! @closelog() ) {
        $e = error_get_last();
        throw new My_Exception('Close log' . htmlspecialchars($e['message']));
      }
    }

    /**
     * Sends mail with logger
     *
     * @param string $to
     * @param string $subj
     * @param string $body
     * @param string $fromName
     * @param string $fromMail
     * @param string $charset
     */
    public function myMail( $to, $subj, $body, $fromName = 'MinskElectro', $fromMail = 'robot@minskelectro.com', $charset = 'utf-8') {

      $from = "From: =?utf-8?B?" . base64_encode($fromName) . "?= <" . strip_tags($fromMail) . ">\r\n"
            . "X-Sender: $fromMail\r\n"
            . "MIME-Version: 1.0\r\n"
            . "Content-Type: text/html; charset=\"$charset\"\r\n";
      if ( @mail( $to, $subj, $body,  $from) ) {
        $this->myLog( LOG_INFO, "Sent email to: $to, subject: $subj." );
      } else {
        $e = error_get_last();
        $this->myLog( LOG_ERR, "Failed to send email to: $to, subject: $subj, error: " . $e['message'] );
        throw new My_Exception('Email send' . $e['message']);
      }
    }

    /**
     * Gets first record or nothing. Use pdoCount first to determine count of records
     *
     * @param string $sql
     * @return array successed result of query
     */
    public function fetch($sql) {
      try {
        $stmt = $this->pdo->query($sql);
      } catch (PDOException $e) {
        $err = $this->pdo->errorInfo();
        $this->myLog( LOG_ERR, $log = 'Failed to execute query: [' . $e->getMessage() . ']. sql=' . $sql . ', driver_error: ' . $err[1] . ', driver_message: [' . $err[2] . ']' );
        throw new My_Exception('Database query');
      }
      $return = $stmt->fetch();
      if ($return === FALSE) {
        $this->myLog( LOG_ERR, $log = 'Failed to fetch result from statement. sql: ' . $sql );
        throw new My_Exception('Database fetch');
      }
      if (FALSE === $stmt->closeCursor()) {
        $err = $stmt->errorInfo();
        $this->myLog( LOG_ERR, $log = 'Failed to close cursor. driver_error: ' . $err[1] . ', driver_message: [' . $err[2] . ']' );
        throw new My_Exception('Database closeCursor');
      }
      //$this->myLog( LOG_DEBUG, $sql );
      return $return;
    }

    /**
     * Gets all records
     *
     * @param string $sql
     * @return array successed result of query
     */
    public function fetchAll($sql) {
      try {
        $stmt = $this->pdo->query($sql);
      } catch (PDOException $e) {
        $err = $this->pdo->errorInfo();
        $this->myLog( LOG_ERR, $log = 'Failed to execute query: [' . $e->getMessage() . ']. sql:' . $sql . ', driver_error: ' . $err[1] . ', driver_message: [' . $err[2] . ']' );
        throw new My_Exception('Database query');
      }
      $return = $stmt->fetchAll(PDO::FETCH_ASSOC);
      if ($return === FALSE) {
        $this->myLog( LOG_ERR, $log = 'Failed to fetch result from statement. sql:' . $sql );
        throw new My_Exception('Database fetchAll');
      }
      if (FALSE === $stmt->closeCursor()) {
        $err = $stmt->errorInfo();
        $this->myLog( LOG_ERR, $log = 'Failed to close cursor. driver_error: ' . $err[1] . ', driver_message: [' . $err[2] . ']' );
        throw new My_Exception('Database closeCursor');
      }
      //$this->myLog( LOG_DEBUG, $sql );
      return $return;
    }

    /**
     * Gets count of INSERT, DELETE, etc. records
     *
     * @param string $sql
     * @return int
     */
    public function execCount($sql) {
      try {
        $return = $this->pdo->exec( $sql );
      } catch (PDOException $e) {
        $err = $this->pdo->errorInfo();
        $this->myLog( LOG_ERR, $log = 'SQLSTATE: ' . $err[0] . ', driver error: ' . $err[1] . ', driver message: [' . $err[2] . '], pdoConn: ' . $this->pdoConnStr . ', sql: ' . $sql );
        throw new My_Exception('Database execCount');
      }
      if ($return === FALSE) {
        $this->myLog( LOG_ERR, $log = 'Failed to exec statement. sql: ' . $sql );
        throw new My_Exception('Database execCount');
      }
      //$this->myLog( LOG_DEBUG, $sql );
      return $return;
    }

    /**
     * Backups tables
     *
     * @param string $file Filename.
     */
    public function backupDB($file){
      file_put_contents($file, '');

      $tables = array();
      $result = $this->fetchAll("SHOW TABLES;");
      foreach ($result as $table) {
        $tables[] = $table['Tables_in_minskelectro'];
      }

      if ( count($tables) > 0 ) {
        foreach($tables as $table){
          $this->backup_table_structure($file, $table);
          $this->backup_table_data($file, $table);
        }
      }
    }

    /**
     * Backups table structure
     *
     * @param string $file Filename.
     * @param array $table Name of tables.
     */
    protected function backup_table_structure($file, $table){
      $content = 'DROP TABLE IF EXISTS `' . $table . "`;\n\n";
      $table = $this->fetch( 'SHOW CREATE TABLE `' . $table . '`;' );
      $content .= $table[1] . ";\n\n";
      file_put_contents($file, $content, FILE_APPEND);
    }

    /**
     * Backups table data
     *
     * @param string $file Filename.
     * @param array $table Name of tables.
     */
    protected function backup_table_data($file, $table){

      $delta = 1000;          // Block of records to save
      $count = $this->fetch( 'SELECT COUNT(*) FROM `' . $table . '`;' )['COUNT(*)'];

      if ($count > 0) {
        // determines non-int fields
        $not_num = array();
        $result = $this->fetchAll( 'SHOW COLUMNS FROM `' . $table . '`;' );
        foreach ($result as $row) {
          if (!preg_match("/^(tinyint|smallint|mediumint|bigint|int|float|double|real|decimal|numeric|year)/", $row['Type'])) {
            $not_num[$row['Field']] = 1;
          }
        }

        // starts get data
        $start = 0;
        while ($count > 0) {
          $result = $this->fetchAll( 'SELECT * FROM `'.$table.'` LIMIT ' . $start . ', ' . $delta . ';' );
          $content = 'INSERT INTO `' . $table . '` VALUES ';
          $first = true;
          foreach ($result as $k => $row) {
            $content .= $first ? "\n(" : ",\n(";
            $first2 = true;

            foreach ($row as $index => $field) {
              if ( isset($not_num[$index]) ) {
                $field = addslashes($field);
//eb                $field = str_replace("\n", "\\n", $field);
                $content .= !$first2 ? (',"' . $field . '"') : ('"' . $field . '"');
              }else{
                $content .= !$first2 ? (',' . $field) : $field;
              }
              $first2 = false;
            }
            $content .= ')';
            $first = false;
          }

          // saves results to file
          file_put_contents($file, $content.";\n\n", FILE_APPEND);
          $count -= $delta;
          $start += $delta;
        }
      }
    }
  }