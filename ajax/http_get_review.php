<?php
/**
  * Gets review status.
  *
  * @return array JSON data of result.
  */

  include 'app.inc';
  $app = new App();

  $error_count = 1;
  $return = array(
    'review' => array(),
    'error'  => ''
  );

  $prodId = isset($_GET['id']) ? (int)$_GET['id'] : 1;

  $num = $app->fetch("SELECT COUNT(*) FROM `reviews` WHERE product_id = $prodId AND `approved` = 1")['COUNT(*)'];
  if ($num) {
    $reviews = $app->fetchAll("SELECT * FROM `reviews` WHERE product_id = $prodId AND `approved` = 1 ORDER BY create_tm");
    foreach ($reviews as $k => $review) {
      $return['review'][$k] = array (
        'rating'      => $review['rating'],
        'name'        => $review['name'],
        'review'      => $review['review'],
        'create_tm'   => date("Y-m-d H:i:s", intval($review['create_tm']))
      );
    }
  }

  echo json_encode($return);