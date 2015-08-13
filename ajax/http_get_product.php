<?php
/**
  * Returns JSON full data of product.
  *
  * @param number id Product ID.
  *
  * @return array JSON data of product.
  */

  include 'app.inc';
  $app = new App();

  $error_count = 1;
  $return = array(
    'product' => '',
    'error' => ''
  );

  $prodId = (int)($_GET['id']);
  $num = $app->fetch("SELECT COUNT(*) FROM `products` WHERE `id` = $prodId")['COUNT(*)'];

  if ($num == 1) {
    $product = $app->fetch("SELECT * FROM `products` WHERE `id` = $prodId");

    // for product details
    $image = '../pictures/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '.';
    if ( file_exists($image . 'jpg') ) {
   	  $image = '/' . $image . 'jpg';
    } else {
      if( file_exists($image . 'gif') ) {
   	    $image = '/' . $image . 'gif';
      } else {
        $image = '';
      }
    }

    // for cart
    $icon = '../tn/' . str_pad($product['id'], 4, '0', STR_PAD_LEFT) . '_m.';
    if ( file_exists($icon . 'jpg') ) {
   	  $icon = '/' . $icon . 'jpg';
    } else {
      if( file_exists($icon . 'gif')) {
   	    $icon = '/' . $icon . 'gif';
      } else {
        $icon = '';
      }
    }

    $rating_average = 0;
    $reviews = $app->fetchAll("SELECT rating FROM `reviews` WHERE `product_id` = $prodId AND `approved` = 1");
    if ( count($reviews) ) {
      foreach ($reviews as $review) {
        $rating_average += 10 * $review['rating'];
      }
      $rating_average = round($rating_average / count($reviews));
      $rating_average -= ($rating_average % 5);
    }

    $return['product'] = array(
      'id'            => $prodId,
      'name'          => strip_tags($product['name']),
      'price'         => $product['price_out'],
      'warranty'      => $product['warranty'],
      'short_desc'    => $product['short_desc'],       // for cart
      'description'   => $product['description'],     // for product details
      'icon'          => $icon,                              // for cart
      'image'         => $image,                            // for product details
      'delivery_time' => $product['delivery_time'],
      'delivery_free' => ( $product['supplier'] == 'BEL' || $product['supplier'] == 'SIU' ) ? '1' : '0',
      'delivery_n_a'  => $product['absent'],
      'date_update'   => intval($product['date_update']) * 1000,
      'date_add'      => intval($product['date_add']) * 1000,
      'rating'        => $rating_average,
      'rating_count'  => count($reviews)
    );
  } else {
    $return['error'] .= ($error_count++) . '. Количество товаров с таким ID равно ' . $num;
  }
  echo json_encode($return);