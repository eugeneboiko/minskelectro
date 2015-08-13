<?php
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
    'error' => ''
  );

  // AngularJS transmits data using Content-Type: application/json and JSON serialization,
  // which unfortunately some Web server languages—notably PHP—do not unserialize natively.
  // So server has empty $_POST array.
  $feedback = json_decode( file_get_contents("php://input") );
  $captcha_id     = intval($feedback->captcha_id);
  $captcha_value  = intval($feedback->captcha_value);

  $options = $app->fetchAll('SELECT * FROM `options`');
  if ( count($options) ) {
    foreach ($options as $key => $option) {
      $$option['name'] = $option['value'];
    }
    if ( $app->execCount("DELETE FROM captchas WHERE id = $captcha_id AND value = $captcha_value") ) {
      try {
        $app->myMail( $order_mail, $feedback->subject, $feedback->message );
      } catch (myException $e) {
        $return['error'] .= ($error_count++) . '. ' . $e->getMessage();
      }
      if (!$return['error']) {
        $return['success'] .= ($success_count++) . '. Ваше сообщение отправлено!';
      }
    } else {
      $return['error'] .= ($error_count++) . '. Неправильно введены цифры с картинки!';
    }
  } else {
    $return['error'] .= ($error_count++) . '. Количество опций: 0.';
  }

  echo json_encode($return);