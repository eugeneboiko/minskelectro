<?php
/**
  * Output Import page.
  *
  *
  */
  require 'admin.inc';

  $template->assign_vars(array('page_name' => 'Импорт данных и создание прайса'));
  $template->assign_block_vars('way_item', array('name' => 'Импорт данных и создание прайса'));
  $template->assign_block_vars('way_item_l', array('link' => 'admin_options.php', 'name' => 'Административный интерфейс'));

  $logger = new \MinskElectro\Logger();
  $rc = $logger->open();

  if( isset($cmd) ) {

    $rc         = RC_SUCCESS;
    $zip_file   = ME_IMPORT_FILE_PATHNAME;
    $url        = ME_IMPORT_FILE_URL;
    $util       = new \MinskElectro\Util($logger);
    $access     = new \MinskElectro\Access($logger);
    $rc         = $access->open();

    if( RC_SUCCESS != $rc ) {
      $logger->log( LOG_ERR, $rc, 'Failed to open database connection.' );
      $template->assign_vars(array( 'page_body_prefix_err' => 'Failed to open database connection.' ));
    } else {
      $rc       = $util->import_data( $access, $zip_file );
      if( RC_SUCCESS != $rc ) {
        $logger->log( LOG_ERR, $rc, 'Failed to import data from file \''.$zip_file.'\'.' );
        $template->assign_vars(array( 'page_body_prefix_err' => 'Failed to import data from \''.$zip_file.'\'.' ));
      } else {
        $file   = ME_EXPORT_ARCHIVE_FILE_PATH;
        $rc     = $util->prepare_pricelist( $access, $file, ME_EXPORT_DATA_FILE_NAME );
        if( RC_SUCCESS != $rc ) {
          $logger->log( LOG_ERR, $rc, 'Failed to prepare pricelist in file \''.$file.'\'.' );
         $template->assign_vars(array( 'page_body_prefix_err' => 'Failed to prepare pricelist in file \''.$file.'\'.' ));
        } else {
          $logger->log( LOG_INFO, $rc, 'Created new pricelist file \''.$file.'\'.' );
          $irc  = $access->close();
          if( RC_SUCCESS != $irc ) {
            $logger->log( LOG_ERR, $irc, 'Failed to close database connection.' );
            $template->assign_vars(array( 'page_body_prefix_err' => 'Failed to close database connection.' ));
          } else $template->assign_vars(array( 'page_body_prefix_note' => 'Импорт успешно завершен.' ));
        }
      }
    }
  }
  $files = '';
  for( $i = 0; $i < count( \MinskElectro\Util::$file_names ); ++$i ) {
    $files .= '<li>'.( \MinskElectro\Util::$file_names[$i] ).'</li>';
  }

  if( isset($cmd) ) {
//    include 'xlsx.php';
  }

  $template->assign_block_vars('admin_import', array(
    'files'        => $files,
    'status'       => file_get_contents(ME_IMPORT_STATUS_FILE)
  ));

  $irc = $logger->close();
  if( RC_SUCCESS != $irc ) {
    echo( 'Error '.$irc.': failed to close logger.' );
  }

  $template->pparse('page');