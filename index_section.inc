<?php
  // shows section (selects the main if not specified)
  $id           = isset($_GET['c']) ? (int)$_GET['c'] : 1;
  $current_page = isset($_GET['pg']) ? (int)$_GET['pg'] : 1;
  $order_by     = isset($_GET['so']) ? (int)$_GET['so'] : $filter_sort_order;
  switch ($order_by) {
    default:
    case '0': $order_by = '`price_out`'; break;
    case '1': $order_by = '`name`'; break;
    case '2': $order_by = '`price_out` DESC'; break;
    case '3': $order_by = '`name` DESC'; break;
    case '4': $order_by = '`date_add`'; break;
    case '5': $order_by = '`date_add` DESC'; break;
  }

  $num = $app->fetch("SELECT COUNT(*) FROM `categories` WHERE `id` = $id")['COUNT(*)'];
  if ( $num || ($id == 1) ) {

    $num = $app->fetch("SELECT COUNT(*) FROM `products` WHERE " . ($id == 1 ? '`on_top` = 1': "`parent` = $id")
        . " AND `absent` = 0 AND `linkto` = 0")['COUNT(*)'];

    if ($num > 0) {

      $products = $app->fetchAll("SELECT * FROM `products` WHERE " . ($id == 1 ? '`on_top` = 1': "`parent` = $id")
          . " AND `absent` = 0 AND `linkto` = 0 ORDER BY $order_by LIMIT "
          . (($current_page - 1) * $products_per_page) . ", $products_per_page");

      foreach ($products as $k => $product) {
        if (file_exists('tn/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '_m.jpg')) {
          $img = '/tn/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '_m.jpg';
        } else if (file_exists('tn/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '_m.gif')) {
          $img = 'tn/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '_m.gif';
        } else {
          $img = '/images/tn_no_icon.jpg';
        }
        $template->assign_block_vars('product_item', array(
          'id'            => $product['id'],
          'image'         => $img,
          'name'          => $product['name'],
          'url_name'      => get_url_compatible($product['name']),
          'short_desc'    => $product['short_desc'],
          'price'         => "$currency_symbol{$product['price_out']}$currency_name = " . ($product['price_out'] * $currency_exchange_rate) . " р.",
          'in_stock'      => ($product['absent'] ? 'Продукт в данный момент отсутствует на складе' : ''),
          'warranty'      => $product['warranty'],
          'delivery_time' => $product['delivery_time'],
          'delivery_free' => ($product['supplier'] == 'BEL' || $product['supplier'] == 'SIU') ? 'Бесплатная доставка' : '',
          'date_update'   => date('Y-m-d H:i:s', $product['date_update']),
          'date_add'      => date('Y-m-d H:i:s', $product['date_add'])
        ));
      }
    }

    // shows the page numbers
    if ($num) {

      $ord = "/c~$id/pg~";
      $pgs = ceil($num / $products_per_page);

      if ($pgs > 1) {

        if ($current_page > 1) {
          $prev = '<a href="' . $ord . ($current_page - 1) . '" class="pages">предыдущие ' . $products_per_page . ' элем.</a>';
        } else {
          $prev = '';
        }
        if ($current_page < $pgs) {
          $next = '<a href="' . $ord . ($current_page + 1) . '" class="pages">следующие ' . (($current_page == $pgs - 1) ? $num%$products_per_page : $products_per_page) . ' элем.</a>';
        } else {
          $next = '';
        }
        if ($prev and $next) {
          $prev_next = "$prev | $next";
        } else {
          $prev_next = "$prev$next";
        }

        $template->assign_block_vars('pgs', array(
          'link'      => $ord,
          'prev_next' => " ( $prev_next ) "
        ));


        for ($i = 0; $i <= $pgs; $i++)
        switch ($i) {
          // shows the current page without the link
          case ($i == $current_page):
            $template->assign_block_vars('pgs.item.item2', array('num' => "&lt;$i&gt;"));
            break;

          // shows page numbers with links
          case (
              (
               (($current_page - $i) <= $page_neighbors_count) and (($current_page - $i) >= -$page_neighbors_count)
            ) or (
             $i == 1
            ) or (
             $i == $pgs
            )
          ) :
            $template->assign_block_vars('pgs.item.item1', array('num' => $i));
            break;

          // shows dots instead page numbers
          case ((($current_page - $i - 1) == $page_neighbors_count) or (($current_page - $i + 1) == -$page_neighbors_count)) :
            $template->assign_block_vars('pgs.item.item2', array('num' => '...'));
            break;

          default: break;
        }
      }

    }

    // Shows the logical way
    $num2 = $app->fetch("SELECT COUNT(*) FROM `categories` WHERE `id` = $id")['COUNT(*)'];
    $buf = array();
    if ($num2) {
      $section = $app->fetch("SELECT `parent`, `name`, `desc` FROM `categories` WHERE `id` = $id");
      if ($section['name']) {

        $template->assign_vars(array(
          'page_name' => explode('%s', $page_name_tpl, 3)[0] . strip_tags($section['name']) . explode('%s', $page_name_tpl, 3)[2],
        ));
        if ($section['desc']) {
          $template->assign_block_vars('section_description', array( 'description' => $section['desc'] ));
        }

        $buf = array($id, $section['name']);
      };
      if ($section['parent']){
        $cat_arr = $app->fetchAll("SELECT `id`, `name`, `parent` FROM `categories` WHERE `id` = " . $section['parent']);
        while ( count($cat_arr) ) {
          array_push($buf, $cat_arr[0]['id'], $cat_arr[0]['name']);
          $cat_arr = $app->fetchAll("SELECT `id`, `name`, `parent` FROM `categories` WHERE `id` = " . $cat_arr[0]['parent']);
        }
      }
    }
    $template->assign_block_vars('way_item', array( 'name' => "$num элем." ));
    array_push($buf, 1, 'Главная');
    // prints in reverse order
    for ( $i = count($buf); $i > 0 ; $i = $i - 2) {
      $template->assign_block_vars('way_item_l', array(
        'link' => '/c~' . $buf[$i-2],
        'name' => $buf[$i-1]
      ));
    }

    // shows subsections
    $num = $app->fetch("SELECT COUNT(*) FROM `categories` WHERE `parent` = $id AND `hidden` <> 1")['COUNT(*)'];
    if ($num) {
      $subsection = $app->fetchAll("SELECT * FROM `categories` WHERE `parent` = $id AND `hidden` <> 1 ORDER BY `name` LIMIT 100");
      $template->assign_block_vars('sub_cat', array());
      for ( $i = 0; $i < $num; $i++) {
        $template->assign_block_vars('sub_cat.item', array(
          'id'        => $subsection[$i]['id'],
          'name'      => $subsection[$i]['name'],
          'url_name'  => get_url_compatible( $subsection[$i]['name'] ),
          'title'     => strip_tags($subsection[$i]['name']),
          'spl'       => (($i + 1 != $num) ? ', ' : '')
        ));
      }
    }

  } else {
    $template->assign_vars(array('page_body_prefix_err' => 'Раздел с ID=' . $id . ' не найден!'));
  }