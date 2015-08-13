<?php
  error_reporting(E_ALL);
/**
  * Sends email to the customer and admin.
  *
  * @param array JSON data of order.
  *
  * @return array JSON data of sending a email status.
  */

  include 'app.inc';
  $app = new App();

  $error_count = 1;
  $success_count = 1;
  $return = array(
    'success' => '',
    'error'   => ''
  );

  // AngularJS transmits data using Content-Type: application/json and JSON serialization,
  // which unfortunately some Web server languages—notably PHP—do not unserialize natively.
  // So server has empty $_POST array.
  $http_post = json_decode( file_get_contents("php://input") );

  $review         = $http_post->review;
  $product_id     = intval($review->product_id);
  $rating         = intval($review->product_rating);
  $captcha_id     = intval($review->captcha_id);
  $captcha_value  = intval($review->captcha_value);
  $product_name   = htmlentities(strip_tags($review->product_name));
  $product_review = htmlentities(strip_tags($review->product_review));
  $name           = htmlentities(strip_tags($review->user_name));
  $phone          = htmlentities(strip_tags($review->user_phone));
  $email          = htmlentities(strip_tags($review->user_email));

  $options = $app->fetchAll('SELECT * FROM `options`');

  if ( count($options) ) {
    foreach ($options as $key => $option) {
      $$option['name'] = $option['value'];
    }
    if ( $app->execCount("DELETE FROM captchas WHERE id = $captcha_id AND value = $captcha_value") ) {

      if($product_id and $name and $phone and $email) {
        $num = $app->fetch("SELECT COUNT(*) FROM `reviews` WHERE `mail` = '$email' AND `product_id` = $product_id" )['COUNT(*)'];
        if ($num) {
          $return['error'] .= ($error_count++) . '. На этот товар вы уже оставили отзыв!';
        }	else {
          if ((preg_match('#(.+)@(.+)\.(.){2,6}#', $email)) and (strlen($email) < 100)) {
            $code = md5( crypt($email, 'mms_shop') );
            $num = $app->execCount("INSERT INTO `reviews` (`name`, `phone`, `mail`, `review`, `product_id`, `product_name`,"
                . " `rating`, `code`, `approved`, `create_tm`, `approve_tm`, `ip`) VALUES (" . $app->pdo->quote($name)
                . ", " . $app->pdo->quote($phone) . ", " . $app->pdo->quote($email) . ", " . $app->pdo->quote($product_review)
                . ", $product_id, " . $app->pdo->quote($product_name) . ", $rating, '$code', 0, " . time() . ", 0, '"
                . $_SERVER['REMOTE_ADDR'] . "' )");

            if ($num) {
              // Sends email to customer to approve subscription
              $app->myMail(
                $email,
                'Отзыв в MinskElectro',
                "<html><body><h1>Здравствуйте, $name.</h1>"
                    . "E-mail $email был указан при добавлении отзыва на сайте MinskElectro.com. Для подтверждения вам следует перейти по <a href='http://www.minskelectro.com/?confirm=$code'>этой ссылке</a>."
                    . "<p>С уважением,<br>MinskElectro.com</p>"
                    . "</body></html>",
                'MinskElectro',
                $order_mail
              );

              $return['success'] .= ($success_count++) . '. Сообщение для подтверждения отправлено.';
            } else {
              $return['error'] .= ($error_count++) . '. Отзыв не добавлен из-за ошибки сервера.';
            }
          } else {
            $return['error'] .= ($error_count++) . '. Неверно указан e-mail.';
          }
        }
      } else {
        $return['error'] .= ($error_count++) . '. Не указаны имя и/или телефон и/или e-mail и/или product_id.';
      }
    } else {
      $app->myLog( LOG_INFO, "CAPTCHA check error: captcha_id = $captcha_id, captcha_value = $captcha_value" );
      $return['error'] .= ($error_count++) . '. Неправильно введены цифры с картинки!';
    }
  } else {
    $return['error'] .= ($error_count++) . '. Количество опций: 0.';
  }

  echo json_encode($return);