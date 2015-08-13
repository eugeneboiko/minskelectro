<?php
/**
  * Returns JSON brief data of section.
  *
  * @param number id Section ID.
  * @param number current_page Current requested page.
  * @param number filter_sort_order Products page sort order.
  *
  * @return array JSON data of section.
  */
  include 'app.inc';
  $app = new App();

  $error_count = 1;
  $return = array(
    'product_count'     => 0,            // total count of products in current section
    'subsection_count'  => 0,            // total count of subsections in current section
    'name'              => '',           // name of section
    'description'       => '',           // notice: main page (id==1) has no description
    'product'           => array(),      // value is limited to $products_per_page
    'subsection'        => array(),      // value is limited to 100
    'error'             => ''
  );

  $options = $app->fetchAll('SELECT * FROM `options`');

  if ( count($options) ) {
    foreach ($options as $key => $option) {
      $$option['name'] = $option['value'];
    }

    $secId              = isset( $_GET['id'] ) ? (int)$_GET['id'] : 1;
    $current_page       = isset($_GET['current_page']) ? (int)$_GET['current_page'] : 1;
    $filter_sort_order  = isset($_GET['filter_sort_order']) ? (int)$_GET['filter_sort_order'] : $filter_sort_order;
    $filter_minprice    = isset($_GET['filter_minprice']) ? (int)$_GET['filter_minprice'] : 0;
    $filter_maxprice    = isset($_GET['filter_maxprice']) ? (int)$_GET['filter_maxprice'] : MAX_PRICE;
    $filter_stock       = isset($_GET['filter_stock']) ? (int)$_GET['filter_stock'] : 0;

    $products_per_page = (int)$products_per_page < 1 ? 50 : $products_per_page;
    $current_page = $current_page < 1 ? 1 : $current_page;
    $filter_maxprice    = $filter_maxprice > 0 ? $filter_maxprice : MAX_PRICE;
    if ($filter_minprice > $filter_maxprice) {
      $tmp = $filter_maxprice;
      $filter_maxprice = $filter_minprice;
      $filter_minprice = $tmp;
    }
    switch ( $filter_sort_order ) {
      default:
      case '0': $order_by = '`price_out`'; break;
      case '1': $order_by = '`name`'; break;
      case '2': $order_by = '`price_out` DESC'; break;
      case '3': $order_by = '`name` DESC'; break;
      case '4': $order_by = '`date_add`'; break;
      case '5': $order_by = '`date_add` DESC'; break;
    }

    // products of section
    $return['product_count'] = $app->fetch("SELECT COUNT(*) FROM `products` WHERE "
        . ($secId == 1 ? '`on_top` = 1': "`parent` = $secId") . " AND " . ($filter_stock == 0 ? "`absent` = 0 AND " : '')
        . "`price_out` >= $filter_minprice AND `price_out` <= $filter_maxprice AND `linkto` = 0")['COUNT(*)'];

    if ($return['product_count']) {

      // products of current page
      $products = $app->fetchAll("SELECT * FROM `products` WHERE " . ($secId == 1 ? '`on_top` = 1': "`parent` = $secId")
          . " AND " . ($filter_stock == 0 ? "`absent` = 0 AND " : '') . "`price_out` >= $filter_minprice AND "
          . "`price_out` <= $filter_maxprice AND `linkto` = 0 ORDER BY $order_by LIMIT "
          . (($current_page - 1) * $products_per_page) . ", $products_per_page");

      foreach ($products as $k => $product) {

        $icon = '../tn/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '_m.';
        if ( file_exists($icon . 'jpg') ) {
          $icon = '/' . $icon . 'jpg';
        } else {
          if( file_exists($icon . 'gif') ) {
            $icon = '/' . $icon . 'gif';
          } else {
            $icon = '';
          }
        }

        $rating_average = 0;
        $reviews = $app->fetchAll("SELECT rating FROM `reviews` WHERE `product_id` = {$product['id']} AND `approved` = 1");
        if ( count($reviews) ) {
          foreach ($reviews as $review) {
            $rating_average += 10 * $review['rating'];
          }
          $rating_average = round($rating_average / count($reviews));
          $rating_average -= ($rating_average % 5);
        }

        $return['product'][$k] = array (
          'id'            => $product['id'],
          'name'          => $product['name'],
          'price'         => $product['price_out'],
          'warranty'      => $product['warranty'],
          'short_desc'    => $product['short_desc'],
          'icon'          => $icon,
          'delivery_time' => $product['delivery_time'],
          'delivery_free' => ( ( $product['supplier'] == 'BEL' || $product['supplier'] == 'SIU' ) ? '1' : '0'),
          'delivery_n_a'  => $product['absent'],
          'date_update'   => intval($product['date_update']) * 1000,
          'date_add'      => intval($product['date_add']) * 1000,
          'rating'        => $rating_average,
          'rating_count'  => count($reviews)
        );
      }
    }

    // name and description of current section
    $num = $app->fetch("SELECT COUNT(*) FROM `categories` WHERE `id` = $secId")['COUNT(*)'];
    if ($num) {
      $sections = $app->fetch("SELECT * FROM `categories` WHERE `id` = $secId");
      $return['name']         = $sections['name'];
      $return['description']  = $sections['desc'];
    }

    if ($secId == 1) {
      $return['name']         = 'Главная';
    }

    // subsections of current section
    $return['subsection_count'] = $app->fetch("SELECT COUNT(*) FROM `categories` WHERE "
        . ($secId == 1 ? '`parent` = 0': "`parent` = $secId") . " AND `hidden` <> 1")['COUNT(*)'];
    if ($return['subsection_count']) {
      $subsections = $app->fetchAll("SELECT * FROM `categories` WHERE "
          . ($secId == 1 ? '`parent` = 0': "`parent` = $secId") . " AND `hidden` <> 1 ORDER BY `name` LIMIT 100");
      foreach ($subsections as $k => $subsection) {
        $return['subsection'][$k] = array(
          'id' => $subsection['id'],
          'name' => $subsection['name']
        );
      }
    }

  } else {
    $return['error'] .= ($error_count++) . '. Количество опций: 0.';
  }

  echo json_encode($return);