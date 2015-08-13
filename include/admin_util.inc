<?php
namespace MinskElectro {

    require_once( 'typedefs.inc' );
    require( 'logger.inc' );
    require( 'access.inc' );
    require( 'util.inc' );

    class Admin_Util
    {
        static
        public
        function
        prepare_pricelist()
        {
            $rc                 = RC_SUCCESS;
            $file               = ME_EXPORT_ARCHIVE_FILE_PATH;
            $logger             = new \MinskElectro\Logger();

            do {
                $rc             = $logger->open();
                if( RC_SUCCESS != $rc ) {
                    echo( 'Fatal error '.$rc.': failed to open logger.' );
                    break;
                }

                $util           = new \MinskElectro\Util( $logger );
                $access         = new \MinskElectro\Access( $logger );

                do {
                    $rc         = $access->open();
                    if( RC_SUCCESS != $rc ) {
                        $logger->log( LOG_ERR, $rc, 'Failed to open database connection.' );
                        break;
                    }

                    do {
                        $rc     = $util->prepare_pricelist( $access, $file, ME_EXPORT_DATA_FILE_NAME );
                        if( RC_SUCCESS != $rc ) {
                            $logger->log( LOG_ERR, $rc, 'Failed to prepare pricelist in file \''.$file.'\'.' );
                            break;
                        }
                        $logger->log( LOG_INFO, $rc, 'Created new pricelist file \''.$file.'\'.' );
                    } while( FALSE );

                    $irc        = $access->close();
                    if( RC_SUCCESS != $irc ) {
                        $logger->log( LOG_ERR, $irc, 'Failed to close database connection.' );
                        break;
                    }
                } while( FALSE );

                $irc            = $logger->close();
                if( RC_SUCCESS != $irc ) {
                    echo( 'Error '.$irc.': failed to close logger.' );
                    break;
                }
            } while( FALSE );

            if( ! ME_IS_CLI ) {
                echo( 'File created: '.$file );
            }
        }
    //----------------------------------------------------------------------------
        static
        public
        function
        mail_pricelist()
        {
            $rc                         = RC_SUCCESS;
            $logger                     = new \MinskElectro\Logger();
            $send_ok                    = array();
            $send_failed                = array();

            do {
                $rc                     = $logger->open();
                if( RC_SUCCESS != $rc ) {
                    echo( 'Fatal error '.$rc.': failed to open logger.' );
                    break;
                }

                $util                   = new \MinskElectro\Util( $logger );
                $access                 = new \MinskElectro\Access( $logger );

                do {
                    $rc                 = $access->open();
                    if( RC_SUCCESS != $rc ) {
                        $logger->log( LOG_ERR, $rc, 'Failed to open database connection.' );
                        break;
                    }

                    $rcpts              = NULL;
                    $body               = NULL;
                    $mail_from          = NULL;

                    $rc                 = $access->get_pricelist_mail_options( $rcpts, $body, $mail_from );
                    if( RC_SUCCESS != $rc ) {
                        $logger->log( LOG_ERR, $rc, 'Failed to get list of pricelist recipients.' );
    //                    break;
                    }

                    $irc                = $access->close();
                    if( RC_SUCCESS != $irc ) {
                        $logger->log( LOG_ERR, $irc, 'Failed to close database connection.' );
                        break;
                    }
                    if( RC_SUCCESS != $rc ) {
                        break;
                    }

                    // TEMP
    //                $rcpts              = array( 'alex@boiko.net', 'alex@irbisnet.com' );

                    $nrecip             = count( $rcpts );
                    if( $nrecip < 1 ) {
                        $rc             = RC_NOTFOUND;
                        $logger->log( LOG_ERR, $rc, 'No pricelist recipients found.' );
                        break;
                    }

                    $date               = new \DateTime( 'now', new \DateTimeZone( 'Europe/Minsk' ) );
                    $dt                 = $date->format( 'd.m.Y' );
                    $tm                 = $date->format( 'H:i' );

                    $params             = array(
                                            'from'      => $mail_from,
                                            'subject'   => "MinskElectro ($dt $tm)",
                                            'body_text' => $body,
                                            'charset'   => 'utf-8' );
                    $attach             = array(
                                            basename( ME_EXPORT_ARCHIVE_FILE_PATH ) => ME_EXPORT_ARCHIVE_FILE_PATH );
                    for( $i = 0; $i < $nrecip; ++$i ) {
                        $params['to']   = $rcpts[$i];
                        $irc            = $util->send_mail( $params, $attach );
                        if( RC_SUCCESS != $irc ) {
                            $logger->log( LOG_ERR, $irc, 'Failed to send pricelist to \''.$params['to'].'\'.' );
                            array_push( $send_failed, $params['to'] );
                        } else {
                            $logger->log( LOG_INFO, $irc, 'Sent pricelist to \''.$params['to'].'\'.' );
                            array_push( $send_ok, $params['to'] );
                        }
                    }
                } while( FALSE );

                $logger->log( LOG_INFO, $rc, ( RC_SUCCESS == $rc ? 'Price list mailed successfully.' : 'Failed to mail price list.' ) );

                $irc                    = $logger->close();
                if( RC_SUCCESS != $irc ) {
                    echo( 'Error '.$irc.': failed to close logger.' );
                    break;
                }
            } while( FALSE );

            if( ! ME_IS_CLI ) return 'Success sent to addresses: '.implode( ', ', $send_ok )."\nFailed to send on addresses: ".implode( ', ', $send_failed );

        }
    }
} // namespace MinskElectro