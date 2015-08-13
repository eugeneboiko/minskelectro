<?php
/**
  * Gets site options and returns JSON data of config.
  *
  * @return array JSON data of result.
  */

  include 'app.inc';
  $app = new App();

  $error_count = 1;
  $return = array(
    'categories'  => '',
    'config'      => '',
    'error'       => ''
  );

  $options = $app->fetchAll('SELECT * FROM `options`');

  if ( count($options) ) {
    foreach ($options as $key => $option) {
      $$option['name'] = $option['value'];
    }

    // products of shop
    $product_count_instock = $app->fetch("SELECT COUNT(*) FROM `products` WHERE `linkto` = 0 AND `absent` = 0")['COUNT(*)'];
    $product_count_total = $app->fetch("SELECT COUNT(*) FROM `products` WHERE `linkto` = 0")['COUNT(*)'];

    // Filtered to the front-end
    $return['config'] = array(
      'filter_stock'              => (int)$filter_stock,                          // 0 - products in stock, 1 - all products
      'filter_sort_order'         => (int)$filter_sort_order,                     // 0 - price_out, 1 - name, 2 - price_out DESC, 3 - name DESC, 4 - date_add, 5 - date_add DESC.
      'filter_currency'           => (int)$filter_currency,                       // 0 - USD, 1 - BYR
      'delivery_cost'             => (int)$delivery_cost,
      'products_per_page'         => (int)$products_per_page < 1 ? 15 : $products_per_page,
      'currency_exchange_rate'    => (int)$currency_exchange_rate,
      'product_count_instock'     => (int)$product_count_instock,
      'product_count_total'       => (int)$product_count_total,
      'delivery_free'             => (int)$delivery_free,
      'currency_id'               => (int)$currency_name != 'у.е' ? 1 : 0,
      'neighbors_count'           => (int)$page_neighbors_count,
      'currency_name'             => $currency_name ? ' ' . $currency_name : '',
      'currency_symbol'           => $currency_name ? '' : $currency_symbol,
      'order_reaction_time'       => $order_time,
      'app_name'                  => $name,
      'page_main_center_info'     => $page_main_center_info,
      'page_center_info'          => $page_center_info,
      'page_bottom_info'          => $page_bottom_info
    );
  } else {
    $return['error'] .= ($error_count++) . '. Инициализация магазина.';
  }
  echo json_encode($return);