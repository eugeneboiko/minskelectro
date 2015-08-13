<?php
/**
  * Output Subscribers page.
  *
  *
  */
  require 'admin.inc';

  $template->assign_vars(array('page_name' => 'Отправка писем рассылки', 'date' => date('d.m.Y H:m:s')));
  $template->assign_block_vars('way_item_l', array('link' => 'admin_options.php', 'name' => 'Административный интерфейс'));
  $template->assign_block_vars('way_item', array('name' => 'Отправка писем рассылки'));

  // Sends mail to subscribers from the list
  if ( isset($s_text) ) {
    $subscribers = $app->fetchAll("SELECT `mail`, `approved` FROM `subscribes`");
    $res = '';
    foreach ($subscribers as $k => $subscriber) {
      if ($subscriber['approved']) {
        try {
          $app->myMail( $subscriber['mail'], $s_subject, $s_text . "$text\n_____\nОтписать " . $subscriber['mail'] . " можно тут http://minskelectro.com/?un=" . md5( crypt($subscriber['mail'], 'mms_shop')) );
        } catch ( myException $e ) {
          $res = $e->getMessage();
        }
      }
    }
    try {
      $app->myMail( $shop_mail, $s_subject, $s_text );
    } catch ( myException $e ) {
      $res .= '2) ' . $e->getMessage();
    }
    if ( $res ) {
      $template->assign_vars(array( 'page_body_prefix_err' => $res ));
    } else {
      $template->assign_vars(array( 'page_body_prefix_note' => 'Все письма отправлены!' ));
    }
  }

  // Deletes subscriber from the list
  if ( isset($del) ) {
    $num = $app->execCount("DELETE FROM `subscribes` WHERE `code` = '$del'");
    if ( $num ) {
      $template->assign_vars(array( 'page_body_prefix_note' => 'Адрес удалён успешно!' ));
    } else {
      $template->assign_vars(array( 'page_body_prefix_err' => 'Ошибка при удалении адреса.' ));
    }
  }

  // Confirms subscriber from the list
  if ( isset($confirm) ) {
    $num = $app->execCount("UPDATE `subscribes` SET `approved` = 1, `approve_tm` = " . time() . " WHERE `code` = '$confirm'");
    if ( $num ) {
      $template->assign_vars(array('page_body_prefix_note' => 'Подписка подтверждена!'));
    } else {
      $template->assign_vars(array('page_body_prefix_err' => 'Ошибка при подтверждёнии подписки.'));
    }
  }

  $num = $app->fetch("SELECT COUNT(*) FROM `subscribes`")['COUNT(*)'];
  $template->assign_block_vars('admin_subscriber', array('num' => $num));

  $subscribers = $app->fetchAll("SELECT * FROM `subscribes` ORDER BY `create_tm`" );
  foreach ($subscribers as $k => $subscriber) {
    $template->assign_block_vars('admin_subscriber.item', array(
      'id'          => $subscriber['id'],
      'name'        => $subscriber['name'],
      'mail'        => $subscriber['mail'],
      'code'        => $subscriber['code'],
      'approved'    => $subscriber['approved'] ? 'Подтверждён' : 'Не подтверждён',
      'create_tm'   => date("Y-m-d H:i:s", intval($subscriber['create_tm'])),
      'approve_tm'  => $subscriber['approve_tm'] ? date("Y-m-d H:i:s", intval($subscriber['approve_tm'])) : '',
      'ip'          => $subscriber['ip'],
      'approve'     => $subscriber['approved'] ? '' : 'Подтвердить'
    ));
  }

  $template->pparse('page');