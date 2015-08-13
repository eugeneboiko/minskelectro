<?php
namespace MinskElectro {

    require_once( 'typedefs.inc' );

    class Util
    {
    //----------------------------------------------------------------------------
    //  data for imports
        const       FILE_1_CAT          = 'shop_sections.txt';          // cat
        const       FILE_2_PROD         = 'shop_products.txt';          // prod
        const       FILE_3_PROD_LINKS   = 'shop_products_links.txt';    // prod
        const       FILE_4_PROD_TOP     = 'shop_products_ontop.txt';    // prod
        const       FIELD_DELIM         = "\t";
        static
        public      $file_names         = array(
                                            self::FILE_1_CAT,
                                            self::FILE_2_PROD,
                                            self::FILE_3_PROD_LINKS,
                                            self::FILE_4_PROD_TOP );
    //----------------------------------------------------------------------------

        protected   $logger             = NULL;

        public
        function
        __construct(
            &$logger )
        {
            $this->logger   = &$logger;
        }

        public
        function
        __destruct()
        {
            $this->logger   = NULL;
        }

    //----------------------------------------------------------------------------
        public
        function
        unzip(
            &$files_found,
            $file_names,
            $zip_file,
            $unzip_dir )
        {
            $rc                     = RC_SUCCESS;
            $irc                    = TRUE;
            do {
                $zip                = new \ZipArchive();
                $irc                = $zip->open( $zip_file );
                if( TRUE !== $irc ) {
                    $rc             = $irc;
                    $this->logger->log( LOG_ERR, $rc, 'Failed to open archive file: \''.$zip_file.'\'.' );
                    break;
                }
                do {
                    $files_found    = array();
                    for( $i = $zip->numFiles - 1; $i > -1; --$i ) {
                        $index      = $zip->locateName( $file_names[$i] );
                        if( FALSE !== $index ) {
                            array_push( $files_found, $file_names[$i] );
                        }
                    }
                    $irc            = $zip->extractTo( $unzip_dir, $files_found );
                    if( TRUE !== $irc ) {
                        $rc         = RC_FAIL;
                        $this->logger->log( LOG_ERR, $rc, 'Failed to extract data files from archive: \''.$zip_file.'\'.' );
                        break;
                    }
                } while( FALSE );
                $zip->close();
            } while( FALSE );

            return $rc;
        }
    //----------------------------------------------------------------------------
        public
        function
        get_timestamp(
            $fs_compat  = TRUE )
        {
            $date       = new \DateTime( 'now', new \DateTimeZone( 'Europe/Minsk' ) );
        //    list( $usec, $sec ) = explode( ' ', microtime() );
            return $date->format( $fs_compat ? 'Y-m-d_H.i.s' : 'Y-m-d H:i:s' );
        }
    //----------------------------------------------------------------------------
        public
        function
        format_time(
            $time,
            $fs_compat  = TRUE )
        {
            $date       = new \DateTime( 'now', new \DateTimeZone( 'Europe/Minsk' ) );
            $date->setTimestamp( $time );
            return $date->format( $fs_compat ? 'Y-m-d_H.i.s' : 'Y-m-d H:i:s' );
        }
    //----------------------------------------------------------------------------
        public
        function
        import_data(
            $access,
            $zip_file )
        {
            $rc                 = RC_SUCCESS;
            do {
                $this->logger->log( LOG_INFO, $rc, 'Using file \''.$zip_file.'\'.' );
                if( ! file_exists( $zip_file ) ) {
                    $rc         = RC_NOTFOUND;
                    $this->logger->log( LOG_ERR, $rc, 'Archive file not found: \''.$zip_file.'\'.' );
                    break;
                }

                // unzip archive
                $work_dir       = ME_TMP_DIR.'/'.$this->get_timestamp();
                if( FALSE === @mkdir( $work_dir ) ) {
                    $e          = error_get_last();
                    $rc         = $e['type'];
                    $this->logger->log( LOG_ERR, $rc, 'Failed to create dir \''.$work_dir.'\'. '.$e['message'] );
                    break;
                }

                $this->logger->log( LOG_DEBUG, $rc, 'Created temp dir \''.$work_dir.'\'.' );

                $files_found    = null;
                $rc             = $this->unzip( $files_found, self::$file_names, $zip_file, $work_dir );
                if( RC_SUCCESS != $rc ) {
                    $this->logger->log( LOG_ERR, $rc, 'Failed to unzip file: \''.$zip_file.'\'.' );
                    break;
                }

                do {
                    $rc         = $this->_import_data( $access, $files_found, $work_dir );
                    if( RC_SUCCESS != $rc ) {
                        $this->logger->log( LOG_ERR, $rc, 'Failed to import data. Database may be in invalid state.' );
                        break;
                    }
                } while( FALSE );

                // cleanup unzipped files
                $dir            = opendir( $work_dir );

                while( $contents = readdir( $dir ) ) {
                    if( $contents != '.' && $contents != '..' ) {
                        unlink( $work_dir.'/'.$contents );
                    }
                }

                closedir( $dir );

                rmdir( $work_dir );

            } while( FALSE );
            return $rc;
        }
    //----------------------------------------------------------------------------
        protected
        function
        _import_data(
                $access,
                $files,
                $work_dir )
        {
            $rc                 = RC_SUCCESS;
            do {
                if( in_array( self::FILE_1_CAT, $files ) ) {
                    $fname      = $work_dir.'/'.self::FILE_1_CAT;
                    $rc         = $access->init_categories_from_file( $fname );
                    if( RC_SUCCESS != $rc ) {
                        $this->logger->log( LOG_ERR, $rc, 'Failed to init categories from file \''.$fname.'\'.' );
                        break;
                    }
                }
                if( in_array( self::FILE_2_PROD, $files ) ) {
                    $fname      = $work_dir.'/'.self::FILE_2_PROD;
                    $rc         = $access->init_products_from_file( $fname );
                    if( RC_SUCCESS != $rc ) {
                        $this->logger->log( LOG_ERR, $rc, 'Failed to init products from file \''.$fname.'\'.' );
                        break;
                    }
                }
                if( in_array( self::FILE_3_PROD_LINKS, $files ) ) {
                    $fname      = $work_dir.'/'.self::FILE_3_PROD_LINKS;
                    $rc         = $access->update_products_from_file( $fname );
                    if( RC_SUCCESS != $rc ) {
                        $this->logger->log( LOG_ERR, $rc, 'Failed to update products from file \''.$fname.'\'.' );
                        break;
                    }
                }
                if( in_array( self::FILE_4_PROD_TOP, $files ) ) {
                    $fname      = $work_dir.'/'.self::FILE_4_PROD_TOP;
                    $rc         = $access->update_products_from_file( $fname );
                    if( RC_SUCCESS != $rc ) {
                        $this->logger->log( LOG_ERR, $rc, 'Failed to update products from file \''.$fname.'\'.' );
                        break;
                    }
                }
            } while( FALSE );

            // create file with last update info
            $fname              = ME_IMPORT_STATUS_FILE;
            $irc                = RC_SUCCESS;
            $f                  = fopen( @$fname, 'w' );
            do {
                if( ! $f ) {
                    $e          = error_get_last();
                    $irc        = $e['type'];
                    $this->logger->log( LOG_ERR, $irc, 'Failed to open file for writing: \''.$fname.'\'. '.$e['message'] );
                    break;
                }

                $stat           = NULL;
                $irc            = $access->get_stat( $stat );
                if( RC_SUCCESS != $irc ) {
                    $this->logger->log( LOG_ERR, $irc, 'Failed to get stats from DB.' );
                    break;
                }

                $content   = '<p><table cellspacing=0>';
                $imp_file       = ME_IMPORT_FILE_PATHNAME;

                if( file_exists( $imp_file ) ) {
                    clearstatcache( TRUE, $imp_file );
                    $ctime      = filemtime( $imp_file );
                    $content   .= '<tr><td>Исходный файл:</td><td>\''.$imp_file.'\'</td><tr>'.
                                  '<tr><td>Время изменения файла:</td><td>'.$this->format_time( $ctime, FALSE ).'</td></tr>';
                }

                $content  .= '<tr><td>Время импорта:</td><td>'.$this->get_timestamp( FALSE ).'</td></tr>'.
                                  '<tr><td>Товаров после импорта:</td><td>'.$stat['products'].'</td></tr>'.
                                  '<tr><td>Категорий после импорта:</td><td>'.$stat['categories'].'</td></tr>'.
                                  '</table></p>';
                if( FALSE === @fwrite( $f, $content ) ) {
                    $e          = error_get_last();
                    $irc        = $e['type'];
                    $this->logger->log( LOG_ERR, $irc, 'Failed to write to file \''.$fname.'\'. '.$e['message'] );
                    break;
                }
            } while( FALSE );
            if( RC_SUCCESS != $irc ) {
                $this->logger->log( LOG_ERR, $irc, 'Failed to save stats in file \''.$fname.'\'.  Error ignored.' );
            }
            if( $f ) {
                if( TRUE !== @fclose( $f ) ) {
                    $e          = error_get_last();
                    $irc        = $e['type'];
                    $this->logger->log( LOG_ERR, $irc, 'Failed to close file \''.$fname.'\'. '.$e['message'] );
                } else {
                    $this->logger->log( LOG_DEBUG, RC_SUCCESS, 'Updated file \''.$fname.'\'.' );
                }
                $f              = NULL;
            }

            return $rc;
        }
    //----------------------------------------------------------------------------
        public
        function
        prepare_pricelist(
                $access,
                $dest_file,
                $data_file )
        {
            $rc                 = RC_SUCCESS;
            $ts                 = $this->get_timestamp();
            $tmp_data_file      = $dest_file.'.'.$ts.'.data';
            $tmp_zip_file       = $dest_file.'.'.$ts.'.zip';
            do {
                // extract pricelist
                $rc             = $access->extract_pricelist( $tmp_data_file );
                if( RC_SUCCESS != $rc ) {
                    $this->logger->log( LOG_ERR, $rc, 'Failed to extract price list from DB.' );
                    break;
                }
                // archive pricelist
                $zip            = new \ZipArchive();
                $irc            = $zip->open( $tmp_zip_file, \ZipArchive::CREATE );
                if( TRUE !== $irc ) {
                    $rc         = $irc;
                    $this->logger->log( LOG_ERR, $rc, 'Failed to create archive file \''.$tmp_zip_file.'\'.' );
                    break;
                }
                if( ! $zip->addFile( $tmp_data_file, $data_file ) ) {
                    $rc         = RC_FAIL;
                    $this->logger->log( LOG_ERR, $rc, 'Failed to add file \''.$tmp_data_file.'\' to archive file \''.$tmp_zip_file.'\'.' );
                    break;
                }
                if( ! $zip->close() ) {
                    $rc         = RC_FAIL;
                    $this->logger->log( LOG_ERR, $rc, 'Failed to close archive file \''.$tmp_zip_file.'\'.' );
                    break;
                }
                if( ! @rename( $tmp_zip_file, $dest_file ) ) {
                    $e          = error_get_last();
                    $irc        = $e['type'];
                    $this->logger->log( LOG_ERR, $irc, 'Failed to rename file \''.$tmp_zip_file.'\' to \''.$dest_file.'\'. '.$e['message'] );
                    break;
                }
            } while( FALSE );
            if( file_exists( $tmp_data_file ) ) {
                if( ! @unlink( $tmp_data_file ) ) {
                    $e          = error_get_last();
                    $irc        = $e['type'];
                    $this->logger->log( LOG_ERR, $irc, 'Failed to delete file \''.$tmp_data_file.'\'. '.$e['message'] );
                }
            }
            if( file_exists( $tmp_zip_file ) ) {
                if( ! @unlink( $tmp_zip_file ) ) {
                    $e          = error_get_last();
                    $irc        = $e['type'];
                    $this->logger->log( LOG_ERR, $irc, 'Failed to delete file \''.$tmp_zip_file.'\'. '.$e['message'] );
                }
            }
            return $rc;
        }
    //----------------------------------------------------------------------------
        public
        function
        send_mail(
            $params,
            $attachments            = NULL )
        {
            $rc                     = RC_SUCCESS;
            do {
    /*
                $os                 = strtoupper( substr( PHP_OS, 0, 3 ) );
                if( 'WIN' == $os ) {
                    $eol            = "\r\n";
                } else if( 'MAC' == $os ) {
                    $eol            = "\r";
                } else {
                    $eol            = "\n";
                }
    */
                $eol                = "\r\n";
                $charset            = isset( $params['charset'] ) ? $params['charset'] : 'us-ascii';
                $if_encode          = strtolower( $charset ) != 'us-ascii';
                if( ! isset( $params['subject'] ) ) {
                    $subject        = '';
                } else {
                    $subject        = $if_encode ? '=?'.$charset.'?B?'.base64_encode( $params['subject'] ).'?=' : $params['subject'];
                }
                if( ! isset( $params['from'] ) ) {
                    $rc             = RC_NOTFOUND;
                    $this->logger->log( LOG_ERR, $rc, 'From Address not found in email params.' );
                    break;
                } else {
    //                $from           = $if_encode ? '=?'.$charset.'?B?'.base64_encode( $params['from'] ).'?=' : $params['from'];
                    $from           = $params['from'];
                }
                if( ! isset( $params['to'] ) ) {
                    $rc             = RC_NOTFOUND;
                    $this->logger->log( LOG_ERR, $rc, 'To Address not found in email params.' );
                    break;
                } else {
    //                $to             = $if_encode ? '=?'.$charset.'?B?'.base64_encode( $params['to'] ).'?=' : $params['to'];
                    $to             = $params['to'];
                }

                $headers           = 'From: '.$from.$eol;
                $headers           .= 'Reply-To: '.$from.$eol;
        //        $headers .= 'Return-Path: Jonny <jon@example.com>'.$eol;     // these two to set reply address
        //        $headers .= "Message-ID: <".$now." TheSystem@".$_SERVER['SERVER_NAME'].">".$eol;
                $headers           .= "X-Mailer: PHP v".phpversion().$eol;           // These two to help avoid spam-filters
                # Boundry for marking the split & Multitype Headers
                $headers           .= 'MIME-Version: 1.0'.$eol;
                $mime_boundary      = md5( time() );
                $headers           .= "Content-Type: multipart/mixed; boundary=\"".$mime_boundary."\"".$eol;
                $msg                = "";

                if( $attachments ) {
                    $finfo          = finfo_open( defined('FILEINFO_MIME_TYPE') ? FILEINFO_MIME_TYPE : FILEINFO_MIME );
                    if( FALSE === $finfo ) {
                        $e          = error_get_last();
                        $rc         = $e['type'];
                        $this->logger->log( LOG_ERR, $rc, 'Failed to create finfo resource. '.$e['message'] );
                        break;
                    }
                    foreach( $attachments as $name => $path ) {
                        $content    = file_get_contents( $path );
                        if( FALSE === $content ) {
                            $e      = error_get_last();
                            $rc     = $e['type'];
                            $this->logger->log( LOG_ERR, $rc, 'Failed to read content of attachment file \''.$path.'\'. '.$e['message'] );
                            break;
                        }
                        # Attachment
                        $mimetype   = finfo_file( $finfo, $path );
                        if( FALSE === $mimetype ) {
                        }
                        $msg       .= "--".$mime_boundary.$eol;
                        $msg       .= "Content-Type: ".$mimetype."; name=\"".$name."\"".$eol;   // sometimes i have to send MS Word, use 'msword' instead of 'pdf'
                        $msg       .= "Content-Transfer-Encoding: base64".$eol;
                        $msg       .= "Content-Disposition: attachment; filename=\"".$name."\"".$eol.$eol; // !! This line needs TWO end of lines !! IMPORTANT !!
                        $msg       .= chunk_split( base64_encode( $content ), 76, $eol ).$eol.$eol;
                    }
                    if( RC_SUCCESS != $rc ) {
                        break;
                    }
                }

                # Setup for text OR html
                $msg               .= "--".$mime_boundary.$eol;
                $mime_boundary2     = $mime_boundary.'x';
                $msg               .= "Content-Type: multipart/alternative; boundary=\"".$mime_boundary2."\"".$eol.$eol;

                if( isset( $params['body_text'] ) ) {
                    $msg           .= "--".$mime_boundary2.$eol;
                    $msg           .= "Content-Type: text/plain; charset=$charset".$eol;
                    if( $if_encode ) {
                        $msg       .= "Content-Transfer-Encoding: base64".$eol.$eol;
                        $msg       .= chunk_split( base64_encode( $params['body_text'] ), 76, $eol ).$eol.$eol;
                    } else {
                        $msg       .= $params['body_text'].$eol.$eol;
                    }
                }
                if( isset( $params['body_html'] ) ) {
                    $msg           .= "--".$mime_boundary2.$eol;
                    $msg           .= "Content-Type: text/html; charset=$charset".$eol;
                    if( $if_encode ) {
                        $msg       .= "Content-Transfer-Encoding: base64".$eol.$eol;
                        $msg       .= chunk_split( base64_encode( $params['body_html'] ), 76, $eol ).$eol.$eol;
                    } else {
                        $msg       .= $params['body_html'].$eol.$eol;
                    }
                }

                $msg               .= "--".$mime_boundary2.$eol;

                # Finished
                $msg               .= "--".$mime_boundary."--".$eol.$eol;   // finish with two eol's for better security. see Injection.

                # SEND THE EMAIL
    //            ini_set(sendmail_from,'from@example.com');  // the INI lines are to force the From Address to be used !
                if( ! mail( $to, $subject, $msg, $headers ) ) {
                    $e              = error_get_last();
                    $rc             = $e['type'];
                    $this->logger->log( LOG_ERR, $rc, 'Failed to send mail to \''.$to.'\'. '.$e['message'] );
                    break;
                }
    //            ini_restore(sendmail_from);
            } while( FALSE );
            return $rc;
        }
    //----------------------------------------------------------------------------
        public
        function
        download_update(
            $url )
        {
            $rc                             = RC_SUCCESS;
            $f                              = FALSE;
            $fe                             = FALSE;
            $ch                             = FALSE;
            $ts                             = $this->get_timestamp();
            $dst_fname                      = ME_IMPORT_FILE_PATHNAME;
            $tmp_fname                      = $dst_fname.'.'.$ts.'.tmp';
            $err_fname                      = $dst_fname.'.'.$ts.'.err';
            $change_tm                      = (-1);
            do {
                $ch                         = curl_init();
                if( ! $ch ) {
                    $e                      = error_get_last();
                    $rc                     = $e['type'];
                    $this->logger->log( LOG_ERR, $rc, 'Failed to init curl. '.$e['message'] );
                    break;
                }
                if( ! $f = @fopen( $tmp_fname, 'w' ) ) {
                    $e                      = error_get_last();
                    $rc                     = $e['type'];
                    $this->logger->log( LOG_ERR, $rc, 'Failed to create output file \''.$tmp_fname.'\'. '.$e['message'] );
                    break;
                }
                if( ! $fe = @fopen( $err_fname, 'w' ) ) {
                    $e                      = error_get_last();
                    $rc                     = $e['type'];
                    $this->logger->log( LOG_ERR, $rc, 'Failed to create error file \''.$err_fname.'\'. '.$e['message'] );
                    break;
                }
                $timeout                    = 5;
                if( ! @curl_setopt( $ch, CURLOPT_CONNECTTIMEOUT, $timeout ) ) {
                    $rc                     = curl_errno( $ch );
                    $this->logger->log( LOG_ERR, $rc, 'Failed to set conn_timeout to '.$timeout.'. '.curl_error( $ch ) );
                    break;
                }
                if( ! @curl_setopt( $ch, CURLOPT_FILETIME, 1 ) ) {
                    $rc                     = curl_errno( $ch );
                    $this->logger->log( LOG_ERR, $rc, 'Failed to set filetime option of curl. '.$e['message'] );
                    break;
                }
                if( ! @curl_setopt( $ch, CURLOPT_URL, $url ) ) {
                    $rc                     = curl_errno( $ch );
                    $this->logger->log( LOG_ERR, $rc, 'Failed to set URL option of curl to \''.$url.'\'. '.$e['message'] );
                    break;
                }
                if( ! @curl_setopt( $ch, CURLOPT_FILE, $f ) ) {
                    $rc                     = curl_errno( $ch );
                    $this->logger->log( LOG_ERR, $rc, 'Failed to set output file option of curl to \''.$tmp_fname.'\'. '.curl_error( $ch ) );
                    break;
                }
                if( ! @curl_setopt( $ch, CURLOPT_STDERR, $fe ) ) {
                    $rc                     = curl_errno( $ch );
                    $this->logger->log( LOG_ERR, $rc, 'Failed to set output file option of curl to \''.$err_fname.'\'. '.curl_error( $ch ) );
                    break;
                }
                if( ! @curl_exec( $ch ) ) {
                    $rc                     = curl_errno( $ch );
                    $this->logger->log( LOG_ERR, $rc, 'Failed to download from url \''.$url.'\'. '.curl_error( $ch ) );
                    break;
                }
                $opts                       = curl_getinfo( $ch );
                if( ! $opts ) {
                    $irc                    = curl_errno( $ch );
                    $this->logger->log( LOG_ERR, $irc, 'Failed to get info on transfer. '.curl_error( $ch ) );
                    //break;
                } else {
                    $change_tm              = $opts['filetime'];
                    $t                      = $change_tm;
                    if( (-1) != $t ) {
                        $t                  = $this->format_time( $t, FALSE );
                    }
                    $this->logger->log( LOG_DEBUG, $rc, 'CURL info retrieved: '.
                        'filetime = '.$t.', '.
                        'total_time = '.$opts['total_time'].' sec, '.
                        'connect_time = '.$opts['connect_time'].' sec, '.
                        'pretransfer_time = '.$opts['pretransfer_time'].' sec, '.
                        'size_download = '.$opts['size_download'].' B, '.
                        'speed_download = '.$opts['speed_download'].' B/sec.' );
                }
            } while( FALSE );
            if( $ch ) {
                curl_close( $ch );
            }
            if( $f ) {
                if( ! @fclose( $f ) ) {
                    $e                      = error_get_last();
                    $irc                    = $e['type'];
                    $this->logger->log( LOG_ERR, $irc, 'Failed to close downloaded file \''.$tmp_fname.'\'. '.$e['message'] );
                } else {
                    if( RC_SUCCESS == $rc ) {
                        if( @rename( $tmp_fname, $dst_fname ) ) {
                            $this->logger->log( LOG_INFO, $rc, 'Updated file \''.$dst_fname.'\' from URL \''.$url.'\'.' );
                            if( (-1) != $change_tm ) {
                                if( ! @touch( $dst_fname, $change_tm ) ) {
                                    $e      = error_get_last();
                                    $irc    = $e['type'];
                                    $this->logger->log( LOG_INFO, $irc, 'Failed to set change time of file \''.$dst_fname.'\' to \''.$this->format_time( $t, FALSE ).'\'. '.$e['message'] );
                                }
                            }
                        } else {
                            $e          = error_get_last();
                            $rc         = $e['type'];
                            $this->logger->log( LOG_ERR, $rc, 'Failed to rename file \''.$tmp_fname.'\' to \''.$dst_fname.'\'. '.$e['message'] );
                        }
                    }
                    if( file_exists( $tmp_fname ) ) {
                        if( ! @unlink( $tmp_name ) ) {
                            $e          = error_get_last();
                            $irc        = $e['type'];
                            $this->logger->log( LOG_ERR, $irc, 'Failed to delete file \''.$tmp_fname.'\. '.$e['message'] );
                        }
                    }
                }
            }
            if( $fe ) {
                if( ! @fclose( $fe ) ) {
                    $e                  = error_get_last();
                    $irc                = $e['type'];
                    $this->logger->log( LOG_ERR, $irc, 'Failed to close error file \''.$err_fname.'\'. '.$e['message'] );
                } else {
                    if( RC_SUCCESS != $rc ) {
                        $errors         = @file_get_contents( $err_fname );
                        if( FALSE === $errors ) {
                            $e          = error_get_last();
                            $irc        = $e['type'];
                            $this->logger->log( LOG_ERR, $irc, 'Failed to get content of error file \''.$err_fname.'\'. '.$e['message'] );
                        } else {
                            $this->logger->log( LOG_ERR, $rc, 'CURL errors: '.$errors );
                        }
                    }
                    if( file_exists( $err_fname ) ) {
                        if( ! @unlink( $err_fname ) ) {
                            $e          = error_get_last();
                            $irc        = $e['type'];
                            $this->logger->log( LOG_ERR, $irc, 'Failed to delete file \''.$tmp_fname.'\. '.$e['message'] );
                        }
                    }
                }
            }

            return $rc;
        }
    }

} // namespace MinskElectro
?>