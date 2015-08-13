<?php
/**
  * Returns JSON data with path array.
  *
  * @param number id Product ID.
  *
  * @return array JSON data of path.
  */

  include 'app.inc';
  $app = new App();

  $buf = array();
  $sectionCount = 0;
  $error_count = 1;
  $return = array(
    'navigation' => '',
    'error' => ''
  );

  $id = (int)($_GET['id']);

  $productCount = $app->fetch("SELECT COUNT(*) FROM `products` WHERE `id` = $id")['COUNT(*)'];

  if ($productCount != 1) {
    $sectionCount = $app->fetch("SELECT COUNT(*) FROM `categories` WHERE `id` = $id")['COUNT(*)'];
  }

  if ( ($productCount == 1) or ($sectionCount == 1) ) {

    if ($productCount) {
      $product = $app->fetch("SELECT `parent`, `name` FROM `products` WHERE `id` = $id");
      $parent = $product['parent'];
    } else {
      $parent = $id;
    }

    if ($parent) {
      $category = $app->fetchAll("SELECT `id`, `name`, `parent` FROM `categories` WHERE `id` = " . $parent);
      while ( count($category) ) {
        array_push($buf, $category[0]['id'], $category[0]['name']);
        $category = $app->fetchAll("SELECT `id`, `name`, `parent` FROM `categories` WHERE `id` = " . $category[0]['parent']);
      }
    }
  } else {
      // main section id == 1. If so, suppress messege
      if ($id > 1) {
        $return['error'] .= ($error_count++) . '. С этим id категорий и товаров не найдено. ';
      }
  }
  // Always show link on main page
  $buf[] = 1;
  $buf[] = 'Главная';

  for ($i = count($buf); $i > 0; $i = $i - 2) {
    $return['navigation'][] = array('id' => $buf[$i - 2], 'name' => $buf[$i - 1], 'type' => 'section');
  }

  if ($productCount == 1) {
    $return['navigation'][] = array('id' => $id, 'name' => strip_tags($product['name']), 'type' => 'product');
  }

  echo json_encode($return);