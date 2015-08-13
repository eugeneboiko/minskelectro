<?php
/**
  * Output Review page.
  *
  *
  */
  require 'admin.inc';

  $products_per_page = (int)$products_per_page < 1 ? 15 : $products_per_page;

  // Pagination
  // $pg - current, $num - all items, $pr_per_page - items on page, $ord - link, $nei - neighbours
  function page_print($pg = 1, $num, $pr_per_page = '', $ord = '', $nei = 4){

    global $template, $products_per_page;
    $pg = $pg > 0 ? $pg : 1;
    $pr_per_page = ($pr_per_page ? $pr_per_page : $products_per_page);
    $pgs = ceil($num / $pr_per_page);

    if ($pgs > 1) {

      if ($pg > 1) {
        $prev = '<a href="' . $ord . ($pg - 1) . '" class="pages">предыдущие ' . $pr_per_page . ' элем.</a>';
      } else {
        $prev = '';
      }
      if ($pg < $pgs) {
        $next = '<a href="' . $ord . ($pg + 1) . '" class="pages">следующие ' . (($pg == $pgs - 1) ? $num%$pr_per_page : $pr_per_page) . ' элем.</a>';
      } else {
        $next = '';
      }
      if ($prev and $next) {
        $prev_next = $prev.' | '.$next;
      } else {
        $prev_next = $prev.$next;
      }

      $template->assign_block_vars('pgs', array(
        'link' => $ord,
        'prev_next' => " ( $prev_next ) "
      ));

      for ($i = 0; $i <= $pgs; $i++)
      switch ($i) {
      case ($i == $pg) :             // название без ссылки
        $template->assign_block_vars('pgs.item.item2', array(
          'num' => "&lt;$i&gt;"
        ));
        break;
        case (
            (
             (($pg - $i) < $nei) and (($pg - $i) >- $nei)
          ) or (
           $i == 1
          ) or (
           $i == $pgs
          )
         ) :                       // ссылка на страницу
        $template->assign_block_vars('pgs.item.item1', array(
          'num' => $i
        ));
        break;
        case ((($pg-$i)==$nei) or (($pg-$i)==-$nei)) :
          $template->assign_block_vars('pgs.item.item2', array(
            'num' => '...'
          ));
        break;
      }
    }
    return $pgs;
  }

  $template->assign_vars(array('page_name' => 'Заказы'));
  $template->assign_block_vars('way_item', array( 'name' => 'Управление заказами'));
  $template->assign_block_vars('way_item_l', array('link' => 'admin_options.php', 'name' => 'Административный интерфейс'));

  // Accepts order
  if ( isset($submit_order, $id_order) ) {
    $app->execCount("UPDATE `orders` SET `done` = 1 WHERE `id` = $id_order");
    $template->assign_vars(array('page_body_prefix_note' => 'Заказ принят.'));
  }

  // shows orders
  if ( !isset($show) ) {
    $show = '';
  }
  $template->assign_block_vars('admin_orders', array(
    'shown'         => ($show != 'all' ? 'непринятые' : 'все'),
    'to_show'       => ($show == 'all' ? 'непринятые' : 'все'),
    'to_show_link'  => '/admin/admin_orders.php?show=' . ($show == 'all' ? 'new' : 'all')
  ));

  // orders on the page
  $pg = (int)isset($pg) < 1 ? 1 : $pg;
  $num = $app->fetch("SELECT COUNT(*) FROM `orders`" . ($show != 'all' ? ' WHERE `done` = 0' : '') )['COUNT(*)'];
  page_print($pg, $num, $products_per_page, "/admin/admin_orders.php?orders=1&show=$show&pg=");

  $orders = $app->fetchAll("SELECT * FROM orders" . ($show != 'all' ? ' WHERE `done` = 0' : '') . " ORDER BY `time` DESC LIMIT " . (($pg - 1) * $products_per_page) . ", $products_per_page" );

  $i = 0;
  foreach($orders as $k => $order) {
    $template->assign_block_vars('admin_orders.item', array(
      'id'      => $order['id'],
      'name'    => stripslashes($order['u_name']),
      'phone'   => stripslashes($order['u_phone']),
      'email'   => stripslashes($order['u_email']),
      'address' => stripslashes($order['u_address']),
      'comment' => stripslashes($order['addition']),
      'summ'    => $order['summ'],
      'prod'    => stripslashes($order['products']),
      'time'    => date('d.m.Y H:m:i',$order['time']),
      'color'   => ($i/2==floor($i/2)?'#CBE7C2':'#E8F4E3')
    ));

    if ($order['done']) {
      $template->assign_block_vars('admin_orders.item.done', array());
    } else {
      $template->assign_block_vars('admin_orders.item.add', array());
    }
    $i++;
  }

  $template->pparse('page');