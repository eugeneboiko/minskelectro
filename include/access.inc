<?php
namespace MinskElectro {

  class Access
  {
      protected   $link               = null;
      protected   $logger             = null;
      protected   $is_open            = FALSE;
      const       TBL_PREFIX          = '';
      const       MAX_FILE_LINE_LEN   = 1000000; //eb 10240;
  //    const       MAX_IMPORT_ERRORS   = 100;
      static
      protected   $prod_fields        = array(
                                          'id'            => 'i',
                                          'name'          => 's',
                                          'price_out'     => 'd',
                                          'description'   => 's',
                                          'short_desc'    => 's',
                                          'absent'        => 'i',
                                          'parent'        => 'i',
                                          'linkto'        => 'i',
                                          'on_top'        => 'i',
                                          'warranty'      => 'i',
                                          'supplier'      => 's',
                                          'date_update'   => 'i',
                                          'date_add'      => 'i',
                                          'price_in'      => 'd',         //eb 's',
                                          'delivery_time' => 's',
                                          'category'      => 's' );
      static
      protected   $cat_fields         = array(
                                          'id'            => 'i',
                                          'name'          => 's',
                                          'parent'        => 'i',
                                          'linkto'        => 'i',
                                          'order_n'       => 'i',
                                          'hidden'        => 's',
                                          'desc'          => 's' );

      public function __construct( &$logger )
      {
          do {
              $this->link     = mysqli_init();
              if( ! $this->link ) {
                  $rc         = RC_FAIL;
                  $this->logger->log( LOG_ERR, $rc, 'Failed to instantiate a mysqli object.' );
                  break;
              }
              $this->link->options( MYSQLI_OPT_CONNECT_TIMEOUT, 5 );
              $this->link->options( MYSQLI_INIT_COMMAND, 'SET NAMES utf8' );
              $this->logger   = &$logger;
          } while( false );
      }

      public function __destruct()
      {
          $this->close();
          $this->link = NULL;
      }

      public function open()
      {
          $rc                 = RC_SUCCESS;
          do {
              if( ! $this->link->real_connect( ME_DB_HOST, ME_DB_USR, ME_DB_PWD, ME_DB_NAME ) ) {
                  $rc         = $this->link->connect_errno;
                  $this->logger->log( LOG_ERR, $rc, 'Failed to open mysqli connection to host=\''.ME_DB_HOST.'\', usr=\''.ME_DB_USR.'\', db=\''.ME_DB_NAME.'\'. '.$this->link->connect_error );
                  break;
              }
              $this->is_open  = TRUE;
          } while( false );
          return $rc;
      }

      public function close()
      {
          $rc                     = RC_SUCCESS;
          if( $this->link ) {
              if( $this->is_open ) {
                  if( ! $this->link->close() ) {
                      $rc         = $this->link->errno;
                      $this->logger->log( LOG_ERR, $rc, 'Failed to close DB connection. '.$this->link->error );
                  }
                  $this->is_open  = FALSE;
              }
          }
          return $rc;
      }

      protected function truncate_table( $table )
      {
          $rc         = RC_SUCCESS;
          do {
              if( ! $this->is_open ) {
                  $rc = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'Open connection expected.' );
                  break;
              }
              if( ! $this->link->real_query( "truncate `$table`" ) ) {
                  $rc = $this->link->errno;
                  $this->logger->log( LOG_ERR, $rc, 'Failed to truncate table \''.$table.'\'. '.$this->link->error );
                  break;
              }
              $this->logger->log( LOG_DEBUG, $rc, 'Truncated table \''.$table.'\'.' );
          } while( false );
          return $rc;
      }

      public function init_categories_from_file( $file )
      {
          $rc         = RC_SUCCESS;
          do {
              if( ! $this->is_open ) {
                  $rc = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'Open connection expected.' );
                  break;
              }
              $table  = self::TBL_PREFIX.'categories';
              $rc     = $this->truncate_table( $table );
              if( RC_SUCCESS != $rc ) {
                  $this->logger->log( LOG_ERR, $rc, 'Failed to delete all categories.' );
                  break;
              }
              $rc     = $this->update_table_from_file( $file, $table, self::$cat_fields, TRUE );
              if( RC_SUCCESS != $rc ) {
                  $this->logger->log( LOG_ERR, $rc, 'Failed to insert categories.' );
                  break;
              }
          } while( false );
          return $rc;
      }

      public function init_products_from_file( $file )
      {
          $rc         = RC_SUCCESS;
          do {
              if( ! $this->is_open ) {
                  $rc = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'Open connection expected.' );
                  break;
              }
              $table  = self::TBL_PREFIX.'products';
              $rc     = $this->truncate_table( $table );
              if( RC_SUCCESS != $rc ) {
                  $this->logger->log( LOG_ERR, $rc, 'Failed to delete all products.' );
                  break;
              }
              $rc     = $this->update_table_from_file( $file, $table, self::$prod_fields, TRUE );
              if( RC_SUCCESS != $rc ) {
                  $this->logger->log( LOG_ERR, $rc, 'Failed to insert products.' );
                  break;
              }
          } while( false );
          return $rc;
      }

      public function update_products_from_file( $file )
      {
          $rc         = RC_SUCCESS;
          do {
              if( ! $this->is_open ) {
                  $rc = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'Open connection expected.' );
                  break;
              }
              $table  = self::TBL_PREFIX.'products';
              $rc     = $this->update_table_from_file( $file, $table, self::$prod_fields, FALSE );
              if( RC_SUCCESS != $rc ) {
                  $this->logger->log( LOG_ERR, $rc, 'Failed to update products.' );
                  break;
              }
          } while( false );
          return $rc;
      }

      protected function update_table_from_file( $file, $table, $field_types, $if_try_insert_first )
      {
          $rc                             = RC_SUCCESS;
          do {
              if( ! $this->is_open ) {
                  $rc                     = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'Open connection expected.' );
                  break;
              }
              $handle                     = @fopen( $file, 'r' );
              if( FALSE === $handle ) {
                  $e                      = error_get_last();
                  $rc                     = $e['type'];
                  $this->logger->log( LOG_ERR, $rc, 'Failed to open file \''.$file.'\'. '.$e['message'] );
                  break;
              }
              $rows_failed                = 0;
              $rows_inserted              = 0;
              $rows_updated               = 0;
              $bad_lines                  = array();
              $ins_stmt                   = FALSE;
              $upd_stmt                   = FALSE;
              do {
                  // read header
                  $fnames                 = \fgetcsv( $handle, self::MAX_FILE_LINE_LEN, "\t" );
                  if( NULL === $fnames || FALSE === $fnames ) {
                      $e                  = error_get_last();
                      $rc                 = $e['type'];
                      $this->logger->log( LOG_ERR, $rc, 'Failed to read header of file \''.$file.'\'. '.$e['message'] );
                      break;
                  }
                  $total_fields           = count( $fnames );

                  // verify fields and collect info for prepared statements
                  $types                  = '';
                  $vals                   = array();
                  $ins_sql                = array();
                  $upd_sql                = array();
                  $real_fields_ix         = array();
                  $id_ix                  = -1;
                  for( $i = 0; $i < $total_fields; ++$i ) {
                      $fnames[$i]         = strtolower( trim( $fnames[$i] ) );
                      if( ! isset( $field_types[$fnames[$i]] ) ) {
                          $this->logger->log( LOG_WARNING, RC_NOTFOUND, 'Skipped unknown field \''.$fnames[$i].'\' in file \''.$file.'\'.' );
                          continue;
                      }
                      if( 'id' == $fnames[$i] ) {
                          $id_ix          = $i;
                      } else {
                          array_push( $real_fields_ix, $i );
                          $esc            = '`'.$this->link->real_escape_string( $fnames[$i] ).'`';
                          array_push( $ins_sql, $esc );
                          array_push( $vals, '?' );
                          $types         .= $field_types[$fnames[$i]];
                          array_push( $upd_sql, $esc.'=?' );
                      }
                  }
                  if( RC_SUCCESS != $rc ) {
                      break;
                  }
                  if( $id_ix > -1 ) {
                      // add index of field 'id'
                      array_push( $real_fields_ix, $id_ix );
                      $types             .= $field_types[$fnames[$id_ix]];
                  } else {
                      $rc                 = RC_NOTFOUND;
                      $this->logger->log( LOG_ERR, $rc, 'Field \'id\' not found in file \''.$file.'\'.' );
                      break;
                  }

                  $nfields                = count( $real_fields_ix );

                  // prepare SQL statements
                  $sql                    = 'insert into '.$table.' ('.implode( ',', $ins_sql ).',id) values ('.implode( ',', $vals ).',?)';

                  $ins_stmt               = $this->link->prepare( $sql );
                  if( FALSE === $ins_stmt ) {
                      $rc                 = $this->link->errno;
                      $this->logger->log( LOG_ERR, $rc, 'Failed to prepare statement: \''.$sql.'\'. '.$this->link->error );
                      break;
                  }

                  $sql                    = 'update '.$table.' set '.implode( ',', $upd_sql ).' where id = ?';
                  $upd_stmt               = $this->link->prepare( $sql );
                  if( FALSE === $upd_stmt ) {
                      $rc                 = $this->link->errno;
                      $this->logger->log( LOG_ERR, $rc, 'Failed to prepare statement: \''.$sql.'\'. '.$this->link->error );
                      break;
                  }

                  // read file and import rows
                  $line                   = 1;
                  $id_map                 = array();
                  while( ! feof( $handle ) ) {
                      // read next line
                      ++$line;

  //                    $data               = \fgetcsv( $handle, self::MAX_FILE_LINE_LEN, "\t" );
  // ab                    $text               = stream_get_line( $handle, self::MAX_FILE_LINE_LEN, "\r\n" );
                      $text               = rtrim( fgets( $handle, self::MAX_FILE_LINE_LEN ), "\r\n" );

                      if( $text == '' ) {
                          continue;
                      }

                      $data               = explode( "\t", $text );
                      if( FALSE === $data ) {
                          if( feof( $handle ) ) {
                              // end of file
                              break;
                          } else {
                              $e          = error_get_last();
                              $rc         = $e['type'];
                              $this->logger->log( LOG_ERR, $rc, 'Failed to read line '.$line.' of file \''.$file.'\'. '.$e['message'] );
                              break;
                          }
                      } else if( NULL === $data ) {
                          $rc             = RC_INVSTATE;
                          $this->logger->log( LOG_ERR, $rc, 'Invalid handle for file \''.$file.'\'.' );
                          break;
                      } else if( count( $data ) == 1 && NULL === $data[0] ) {
                          // empty line
                          continue;
                      }
                      $cur_cols           = count( $data );
                      if( $total_fields != $cur_cols ) {
                          $this->logger->log( LOG_ERR, RC_INVVALUE, 'Unexpected number of columns ('.$cur_cols.') on line '.$line.' in file \''.$file.'\'. Expected: '.$total_fields.'.' );
                          echo( '<li>'.str_replace( "\t", '<span style=color:red>TAB</span>', $text ) );
                          ++$rows_failed;
                          break;
                      }
                      //
  //                    $this->logger->log( LOG_INFO, 0, $data[1] );
  //                    break;

                      // check for duplicates
                      $id                 = $data[$id_ix];
                      if( isset( $id_map[$id] ) ) {
                          ++$id_map[$id];
                          $this->logger->log( LOG_INFO, RC_DUPVALUE, 'Duplicate id = '.$id.' (encountered '.$id_map[$id].' times) on line '.$line.' in file \''.$file.'\'.' );
                      } else {
                          $id_map[$id]    = 1;
                      }

                      $params             = array( $types );
                      for( $i = 0; $i < $nfields; ++$i ) {
                          $ix             = $real_fields_ix[$i];
                          //$ref            = &$data[$ix];
                          //array_push( $params, $ref );

                          // start hotfix: converts dates from string to datetime: 110720131210_00003 => 1337991957 s and 20110720131210 => 1337991957 s
                          if (count($data) == 13) {  //  for products data

                              $d1 = $params[$i + 1] = &$data[$ix];
                              switch ($ix) {

                                  case 9://'date_update':
                                    $d2 = date_create_from_format('YmdHis', $d1);
                                    if (FALSE === $d2) {
                                      $this->logger->log( LOG_ERR, 0, "Incorrect date format. Expect $d1 to be like 12345678901234_12345" );
                                    }
                                    $params[$i + 1] = ( FALSE !== $d2 ? date_timestamp_get( $d2 ) : 0);
                                    break;

                                  case 10://'date_add':
                                    $d2 = date_create_from_format('YmdHis', '20' . substr($d1, 0, 12) );
                                    if (FALSE === $d2) {
                                      $this->logger->log( LOG_ERR, 0, "Incorrect date format. Expect $d1 to be like 123456789012_12345" );
                                    }
                                    $params[$i + 1] = ( FALSE !== $d2 ? date_timestamp_get( $d2 ) : 0);
                                    break;

                                  default:
                              }

                          } else {
                              $params[$i + 1] = &$data[$ix];
                          }
                          // end hotfix;

                      }

                      $row_is_bad         = FALSE;
                      $insert_tried       = FALSE;
                      $update_tried       = FALSE;
                      $try_insert         = $if_try_insert_first;

                      do {
                          if( ! $insert_tried && $try_insert ) {
                              // try insert
                              do {
                                  $irc        = call_user_func_array( array( $ins_stmt, 'bind_param' ), $params );
                                  if( FALSE === $irc ) {
                                      $row_is_bad
                                              = TRUE;
                                      $irc    = $ins_stmt->errno;
                                      $this->logger->log( LOG_ERR, $irc, 'Failed to bind params for data on line '.$line.' in file \''.$file.'\'. '.$ins_stmt->error );
                                      break;
                                  }
                                  $irc        = $ins_stmt->execute();
                                  if( FALSE === $irc ) {
                                      $irc    = $ins_stmt->errno;
                                      $this->logger->log( LOG_ERR, $irc, 'Failed to execute insert statement for line '.$line.' in file \''.$file.'\'. '.$ins_stmt->error );
                                      break;
                                  }
                              } while( FALSE );

                              if( $row_is_bad ) {
                                  ++$rows_failed;
                                  break;
                              }
                              if( TRUE === $irc ) {
                                  // inserted
                                  ++$rows_inserted;
                                  break;
                              }

                              $insert_tried   = TRUE;
                          } else if( ! $update_tried ) {
                              // try update
                              do {
                                  $irc        = call_user_func_array( array( $upd_stmt, 'bind_param' ), $params );
                                  if( FALSE === $irc ) {
                                      $row_is_bad
                                              = TRUE;
                                      $rc     = $upd_stmt->errno;
                                      $this->logger->log( LOG_ERR, $rc, 'Failed to bind params for data on line '.$line.' in file \''.$file.'\'. '.$upd_stmt->error );
                                      break;
                                  }
                                  $irc        = $upd_stmt->execute();
                                  if( FALSE === $irc ) {
                                      $rc     = $upd_stmt->errno;
                                      $this->logger->log( LOG_ERR, $rc, 'Failed to execute update statement for line '.$line.' in file \''.$file.'\'. '.$upd_stmt->error );
                                      break;
                                  }
                                  if( 1 == $upd_stmt->affected_rows ) {
                                      ++$rows_updated;
                                  } else {
                                      $irc    = FALSE;
  //                                    $this->logger->log( LOG_WARNING, $rc, 'No records updated (id '.$id.' not found) for line '.$line.' in file \''.$file.'\'.' );
  //                                    ++$rows_failed;
                                  }
                              } while( false );

                              if( $row_is_bad ) {
                                  ++$rows_failed;
                                  break;
                              }
                              if( TRUE === $irc ) {
                                  // updated
                                  break;
                              }

                              $update_tried   = TRUE;
                              $try_insert     = TRUE;
                          } else {
                              ++$rows_failed;
                              $this->logger->log( LOG_WARNING, $rc, 'Failed to import recort on line '.$line.' in file \''.$file.'\'.' );
                              break;
                          }
                      } while( TRUE );
                  }
                  if( RC_SUCCESS != $rc ) {
                      break;
                  }
              } while( FALSE );

              if( FALSE !== $ins_stmt ) {
                  $ins_stmt->close();
              }
              if( FALSE !== $upd_stmt ) {
                  $upd_stmt->close();
              }
              if( ! @fclose( $handle ) ) {
                  $e                          = error_get_last();
                  $rc                         = $e['type'];
                  $this->logger->log( LOG_WARNING, $rc, 'Failed to close file \''.$file.'\'. '.$e['message'] );
              }
          } while( FALSE );

          $this->logger->log( LOG_INFO, RC_SUCCESS, 'Finished importing file \''.$file.'\' into table \''.$table.'\'. Rows inserted: '.$rows_inserted.', updated: '.$rows_updated.', failed: '.$rows_failed.'.' );

          return $rc;
      }

      public
      function
      get_stat(
              &$stat )
      {
          $rc             = RC_SUCCESS;
          do {
              if( ! $this->is_open ) {
                  $rc     = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'Open connection expected.' );
                  break;
              }
              $sql        = 'select count(*) from '.self::TBL_PREFIX.'products';
              $result     = $this->link->query( $sql );
              if( FALSE === $result ) {
                  $rc     = $this->link->errno;
                  $this->logger->log( LOG_ERR, $rc, 'Failed to execute query: \''.$sql.'\'. '.$this->link->error );
                  break;
              }
              $row        = $result->fetch_row();
              if( NULL === $row ) {
                  $rc     = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'No rows returned where count of all products was expected.' );
                  break;
              }
              $nprods     = $row[0];

              $sql        = 'select count(*) from '.self::TBL_PREFIX.'categories';
              $result     = $this->link->query( $sql );
              if( FALSE === $result ) {
                  $rc     = $this->link->errno;
                  $this->logger->log( LOG_ERR, $rc, 'Failed to execute query: \''.$sql.'\'. '.$this->link->error );
                  break;
              }
              $row        = $result->fetch_row();
              if( NULL === $row ) {
                  $rc     = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'No rows returned where count of all categories was expected.' );
                  break;
              }
              $ncats      = $row[0];

              $stat       = array( 'products' => $nprods, 'categories' => $ncats );
          } while( FALSE );
          return $rc;
      }

      public
      function
      extract_pricelist(
              $file )
      {
          $rc                     = RC_SUCCESS;
          $f                      = FALSE;
          do {
              if( ! $this->is_open ) {
                  $rc             = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'Open connection expected.' );
                  break;
              }
              $f                  = @fopen( $file, 'w' );
              if( FALSE === $f ) {
                  $e              = error_get_last();
                  $rc             = $e['type'];
                  $this->logger->log( LOG_ERR, $rc, 'Failed to create file \''.$file.'\'. '.$e['message'] );
                  break;
              }
              $content            = "Название\tЦена\tКатегория товара\tКраткое описание\tГарантия\tСсылка\tИконка\n";
              if( FALSE === @fwrite( $f, $content ) ) {
                  $e              = error_get_last();
                  $rc             = $e['type'];
                  $this->logger->log( LOG_ERR, $rc, 'Failed to write header to file \''.$file.'\'. '.$e['message'] );
                  break;
              }

              $sql                = ' select name, price_out, category, short_desc, warranty, id'.
                                    ' from '.self::TBL_PREFIX.'products'.
                                    ' where absent = 0'.
                                    ' order by name asc';
              $result             = $this->link->query( $sql );
              if( ! $result ) {
                  $rc             = $this->link->errno;
                  $this->logger->log( LOG_ERR, $rc, 'Failed to execute query: \''.$sql.'\'. '.$this->link->error );
                  break;
              }
              $nrows              = 0;
              $no_img             = 0;
              $no_price           = 0;
              while( list( $name, $price_out, $category, $short_desc, $warranty, $id ) = $result->fetch_row() ) {
                  if( $price_out == 0 ) {
                      ++$no_price;
                      continue;
                  }
                  $img_base       = 'pictures/'.str_pad( $id, 4, '0', STR_PAD_LEFT );
                  if( file_exists( ME_WWWROOT.$img_base.'.jpg' ) ) {
                      $img        = ME_WWWADDR.$img_base.'.jpg';
                  } else if( file_exists( ME_WWWROOT.$img_base.'.gif' ) ) {
                      $img        = ME_WWWADDR.$img_base.'.gif';
                  } else {
                      $img        = '';
                      ++$no_img;
                  }
                  if( FALSE === @fputcsv(
                                      $f,
                                      array(
                                          $name,
                                          $price_out,
                                          $category,
                                          $short_desc,
                                          $warranty,
                                          ME_WWWADDR.'p~'.$id,
                                          $img ),
                                      "\t",
                                      '"' ) ) {
                      $e          = error_get_last();
                      $rc         = $e['type'];
                      $this->logger->log( LOG_ERR, $rc, 'Failed to write row # '.$nrows.' to file \''.$file.'\'. '.$e['message'] );
                      break;
                  }
                  ++$nrows;
              }
              if( RC_SUCCESS != $rc ) {
                  break;
              }
              $this->logger->log( LOG_DEBUG, $rc, 'Wrote '.$nrows.' rows ('.( $nrows - $no_img ).' images) to file \''.$file.'\'. Skipped rows with zero price: '.$no_price.'.' );

          } while( FALSE );
          if( FALSE !== $f ) {
              if( FALSE === @fclose( $f ) ) {
                  $e              = error_get_last();
                  $irc            = $e['type'];
                  $this->logger->log( LOG_ERR, $irc, 'Failed to close file \''.$file.'\'. '.$e['message'] );
                  if( RC_SUCCESS != $rc ) {
                      $rc         = $irc;
                  }
              }
              if( RC_SUCCESS != $rc ) {
                  if( file_exists( $file ) ) {
                      if( FALSE === @unlink( $file ) ) {
                          $e      = error_get_last();
                          $irc    = $e['type'];
                          $this->logger->log( LOG_ERR, $irc, 'Failed to delete file \''.$file.'\'. '.$e['message'] );
                      }
                  }
              }
          }
          return $rc;
      }
  //----------------------------------------------------------------------------
      public
      function
      get_pricelist_mail_options(
              &$recipients,
              &$body,
              &$mail_from )
      {
          $rc                     = RC_SUCCESS;
          do {
              if( ! $this->is_open ) {
                  $rc             = RC_INVSTATE;
                  $this->logger->log( LOG_ERR, $rc, 'Open connection expected.' );
                  break;
              }
              $sql                = ' select distinct mail'.
                                    ' from '.self::TBL_PREFIX.'price_send'.
                                    ' order by mail asc';
              $result             = $this->link->query( $sql );
              if( ! $result ) {
                  $rc             = $this->link->errno;
                  $this->logger->log( LOG_ERR, $rc, 'Failed to execute query: \''.$sql.'\'. '.$this->link->error );
                  break;
              }
              $recipients         = array();
              while( list( $email ) = $result->fetch_row() ) {
                  array_push( $recipients, $email );
              }

              $sql                = ' select value'.
                                    ' from '.self::TBL_PREFIX.'options'.
                                    ' where name = "price_send_body"';
              $result             = $this->link->query( $sql );
              if( ! $result ) {
                  $rc             = $this->link->errno;
                  $this->logger->log( LOG_ERR, $rc, 'Failed to execute query: \''.$sql.'\'. '.$this->link->error );
                  break;
              }
              list( $body )       = $result->fetch_row();

              $sql                = ' select value'.
                                    ' from '.self::TBL_PREFIX.'options'.
                                    ' where name = "shop_mail"';
              $result             = $this->link->query( $sql );
              if( ! $result ) {
                  $rc             = $this->link->errno;
                  $this->logger->log( LOG_ERR, $rc, 'Failed to execute query: \''.$sql.'\'. '.$this->link->error );
                  break;
              }
              list( $mail_from )  = $result->fetch_row();
          } while( FALSE );
          return $rc;
      }
  }

} // namespace MinskElectro