<?php
  // shows full description of the product
  $id = (int)$_GET['p'];

  $num = $app->fetch("SELECT COUNT(*) FROM `products` WHERE `id` = $id")['COUNT(*)'];
  if ($num) {
    // If the element refers to another, gets the last.
    $product = $app->fetch("SELECT `linkto` FROM `products` WHERE `id` = $id");
    if ($product['linkto']) {
      $id = $product['linkto'];
    }
    $num = $app->fetch("SELECT COUNT(*) FROM `products` WHERE `id` = $id")['COUNT(*)'];
    if ($num) {
      $product = $app->fetch("SELECT * FROM `products` WHERE `id` = $id");

      if (file_exists('pictures/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '.jpg')) {
        $img = '/pictures/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '.jpg';
      } else {
        if (file_exists('pictures/' . str_pad($product[0], 4, '0', STR_PAD_LEFT) . '.gif')) {
          $img = '/pictures/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '.gif';
        } else {
          $img = '/images/tn_no_icon.jpg';
        }
      }
      $template->assign_vars(array(
        'page_name' => explode('%s', $page_name_tpl, 3)[0] . strip_tags($product['name']) . explode('%s', $page_name_tpl, 3)[2]
      ));
      $template->assign_block_vars('product', array(
        'id'            => $product['id'],
        'image'         => $img,
        'name'          => $product['name'],
        'title'         => strip_tags($product['name']),
        'description'   => nl2br($product['description']),
        'price'         => $currency_symbol . $product['price_out'] . $currency_name . ' = ' . ($product['price_out'] * $currency_exchange_rate) . ' р.',
        'in_stock'      => $product['absent'] ? 'Продукт в данный момент отсутствует на складе!' : '',
        'warranty'      => $product['warranty'],
        'delivery_time' => $product['delivery_time'],
        'delivery_free' => ( $product['supplier'] == 'BEL' || $product['supplier'] == 'SIU' ) ? 'Бесплатная доставка' : '',
        'date_update'   => date('Y-m-d H:i:s', $product['date_update']),
        'date_add'      => date('Y-m-d H:i:s', $product['date_add'])
      ));

      // Shows the logical way
      $product = $app->fetch("SELECT `parent`, `name` FROM `products` WHERE id = $id");
      $parent = $product['parent'];
      if(!$product['name']) {
        $product['name'] = '';
      }
      $buf = array();
      if ($parent) {
        $category = $app->fetchAll("SELECT `id`, `name`, `parent` FROM `categories` WHERE id = " . $parent);
        while ( count($category) ) {
          array_push($buf, $category[0]['id'], $category[0]['name']);
          $category = $app->fetchAll("SELECT `id`, `name`, `parent` FROM `categories` WHERE id = " . $category[0]['parent']);
        }
      }
      $template->assign_block_vars('way_item', array( 'name' => $product['name'] ));
      array_push($buf, 1, 'Главная');
      // prints in reverse order
      for ($i = count($buf); $i > 0; $i = $i - 2){
        $template->assign_block_vars('way_item_l', array(
          'link' => '/c~' . $buf[$i - 2],
          'name' => strip_tags($buf[$i - 1])
        ));
      }

    }
  } else {
    $template->assign_vars(array( 'page_body_prefix_err' => 'Товар с ID=' . $id . ' не найден!'	));
  }