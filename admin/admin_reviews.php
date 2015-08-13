<?php
/**
  * Output Review page.
  *
  *
  */
  require 'admin.inc';

  $template->assign_vars(array('page_name' => 'Отзывы', 'date' => date('d.m.Y H:m:s')));
  $template->assign_block_vars('way_item', array('name' => 'Отзывы'));
  $template->assign_block_vars('way_item_l', array('link' => 'admin_options.php', 'name' => 'Административный интерфейс'));

  if ( isset($confirm) ) {
    $num = $app->execCount("UPDATE `reviews` SET `approved` = 1, `approve_tm` = " . time() . " WHERE `code` = '$confirm'");
    if ( $num ) {
      $template->assign_vars(array('page_body_prefix_note' => 'Отзыв подтверждён!'));
    } else {
      $template->assign_vars(array('page_body_prefix_err' => 'Ошибка при подтверждёнии отзыва.'));
    }
  }

  if ( isset($del) ) {
    $num = $app->execCount("DELETE FROM `reviews` WHERE `code` = '$del'");
    if ( $num ) {
      $template->assign_vars(array('page_body_prefix_note' => 'Отзыв удалён успешно!'));
    } else {
      $template->assign_vars(array('page_body_prefix_err' => 'Ошибка при удалении отзыва.'));
    }
  }

  $num = $app->fetch("SELECT COUNT(*) FROM `reviews`")['COUNT(*)'];
  $template->assign_block_vars('admin_review', array('num' => $num));

  $reviews = $app->fetchAll("SELECT * FROM `reviews` ORDER BY `create_tm`" );
  foreach ($reviews as $k => $review) {
    $template->assign_block_vars('admin_review.item', array(
      'id'          => $review['id'],
      'product'     => $review['product_name'] . " (#{$review['product_id']})",
      'rating'      => $review['rating'],
      'name'        => $review['name'],
      'phone'       => $review['phone'],
      'review'      => $review['review'],
      'mail'        => $review['mail'],
      'code'        => $review['code'],
      'approved'    => $review['approved'] ? 'Подтверждён' : 'Не подтверждён',
      'create_tm'   => date("Y-m-d H:i:s", intval($review['create_tm'])),
      'approve_tm'  => $review['approve_tm'] ? date("Y-m-d H:i:s", intval($review['approve_tm'])) : '',
      'ip'          => $review['ip'],
      'approve'     => $review['approved'] ? '' : 'Подтвердить'
    ));
  }

  $template->pparse('page');