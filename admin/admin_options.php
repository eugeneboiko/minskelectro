<?php
/**
  * Output Options page.
  *
  *
  */
  require 'admin.inc';

  $template->assign_vars(array('page_name' => 'Общие настройки'));
  $template->assign_block_vars('way_item', array('name' => 'Общие настройки'));
  $template->assign_block_vars('way_item_l', array('link' => 'admin_options.php', 'name' => 'Административный интерфейс'));

  if ( isset($op) ) {
    $num = 0;
    foreach ($op as $key => $value) {
      $num += $app->execCount("UPDATE `options` SET `value` = " .  $app->pdo->quote($value) . " WHERE `id` = $key");
    }
    $template->assign_vars(array('page_body_prefix_note' => "Изменено опций: $num."));
  }

  $template->assign_block_vars('admin_options', array());

  $options = $app->fetchAll("SELECT * FROM `options` WHERE `comment` NOT LIKE ''");
  foreach ($options as $option) {
    $template->assign_block_vars('admin_options.item', array(
      'name'  => $option['comment'],
      'id'    => $option['id'],
      'value' => $option['value']
    ));
  }

  $template->pparse('page');