<?php
/**
  * Output News page.
  *
  *
  */
  require 'admin.inc';

  $template->assign_vars(array('page_name' => 'Управление новостями'));
  $template->assign_block_vars('way_item', array('name' => 'Управление новостями'));
  $template->assign_block_vars('way_item_l', array('link' => 'admin_options.php', 'name' => 'Административный интерфейс'));

  if ( isset($news) and count($news) and isset($news_save) ) {
    foreach ($news as $k => $v) {
      $app->execCount("UPDATE `news` SET `text` = '$v', `date` = '{$dates[$k]}' WHERE `id` = $k");
      if ( isset($del[$k]) ) {
        $app->execCount("DELETE FROM `news` WHERE `id` = $k");
      }
    }
    $template->assign_vars(array('page_body_prefix_note' => 'Новости сохранены.'));
  } else if ( isset($news_add) ) {
    $app->execCount("INSERT INTO `news` (`date`, `text`) VALUES ('{$dates[0]}', '{$news[0]}')");
    $template->assign_vars(array('page_body_prefix_note' => 'Новость добавлена.'));
  }

  $template->assign_block_vars('admin_news', array('datenow' => date('d.m.Y')));

  $news = $app->fetchAll("SELECT * FROM `news` ORDER BY `id` DESC");
  foreach ($news as $k => $new) {
    $template->assign_block_vars('admin_news.item', array(
      'id'    => $new['id'],
      'date'  => $new['date'],
      'text'  => $new['text']
    ));
  }

  $template->pparse('page');