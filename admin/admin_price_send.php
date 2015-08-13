<?php
/**
  * Output Price send page.
  *
  *
  */
  require 'admin.inc';

  $template->assign_vars(array('page_name' => 'Автоматическая рассылка прайса', 'date' => date('d.m.Y H:m:s')));
  $template->assign_block_vars('way_item_l', array('link' => 'admin_options.php', 'name' => 'Административный интерфейс'));
  $template->assign_block_vars('way_item', array('name' => 'Автоматическая рассылка прайса'));

  // Adds subscriber
  if ( isset($sp_add) ) {
    $num = $app->fetch("SELECT COUNT(*) FROM `price_send` WHERE `mail` = '$sp_email'")['COUNT(*)'];
    if ($num) {
      $template->assign_vars(array('page_body_prefix_err' => 'Такой подписчик уже существует!'));
    } else {
      $app->execCount("INSERT INTO `price_send` (`mail`, `comment`) VALUES ('$sp_email', '$sp_comment')");
      $template->assign_vars(array('page_body_prefix_note' => 'Подписчик добавлен!'));
    }
  }

  // Deletes subscriber
  if (isset($del)) {
  $app->execCount("DELETE FROM `price_send` WHERE `id` = $del");
    $template->assign_vars(array('page_body_prefix_note' => 'Подписчик удален!'));
  }

  if (isset($sendMail)) {
    $rc = \MinskElectro\Admin_Util::mail_pricelist();
    $template->assign_vars(array('page_body_prefix_note' => 'Рассылка отправлена!<br/>' . $rc));
  }

  $num = $app->fetch("SELECT COUNT(*) FROM price_send")['COUNT(*)'];
  $template->assign_block_vars('admin_price_send', array('num' => $num));

  // Subscriber list
  $subscribers = $app->fetchAll("SELECT `id`, `mail`, `comment` FROM `price_send` ORDER BY mail");
  foreach ($subscribers as $k => $subscriber) {
    $template->assign_block_vars('admin_price_send.item', array(
      'id'     =>$subscriber['id'],
      'mail'   =>$subscriber['mail'],
      'comment'=>$subscriber['comment']
    ));
  }

  $template->pparse('page');