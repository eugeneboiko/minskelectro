<?php
/**
  * Gets subscription status.
  *
  * @return array JSON data of result.
  */

  include 'app.inc';
  $app = new App();

  $error_count = 1;
  $success_count = 1;
  $return = array(
    'success' => '',
    'error' => ''
  );

  $options = $app->fetchAll('SELECT * FROM `options`');

  if ( count($options) ) {
    foreach ($options as $key => $option) {
      $$option['name'] = $option['value'];
    }

    $code     = isset($_GET['code']) ? htmlentities(strip_tags($_GET['code'])) : '';
    $command  = isset($_GET['command']) ? htmlentities(strip_tags($_GET['command'])) : '';

    switch ($command) {

      case 'review':
        $num = $app->fetch("SELECT COUNT(*) FROM `reviews` WHERE `code` = " . $app->pdo->quote($code))['COUNT(*)'];
        if ($num) {
          $review = $app->fetch("SELECT * FROM `reviews` WHERE `code` = " . $app->pdo->quote($code));

          if ($review['approved'] == 1) {
            $return['success'] .= ($success_count++) . '. Ваш e-mail уже подтверждён. ';
          } else {
            $num = $app->execCount("UPDATE `reviews` SET `approved` = 1, `approve_tm` = " . time() . " WHERE `code` = "
                . $app->pdo->quote($code));
            if ($num) {
              $return['success'] .= ($success_count++) . '. Ваш отзыв подтверждён.';

              // Sends email to customer about confirming subscription
              if ( $app->myMail(
                $review['mail'],
                'ME Notification',
                "<html><body>Отзыв подтверждён.<br><br>{$review['review']}</body></html>"
              ) ) {
                $return['success'] .= ($success_count++) . '. Письмо об успешном подтверждении отзыва отправлено.';
              }

              // Sends email to admin about confirming subscription
              if ( $app->myMail(
                  $order_mail,
                  'ME Notification',
                  "<html><body>E-mail {$review['mail']} подтверждён.<br><br>{$review['review']}</body></html>"
                ) ) {
                $return['success'] .= ($success_count++) . '. Письмо об успешном подтверждении отзыва отправлено.';
              }
            } else {
              $return['error'] .= ($error_count++) . '. Не удалось обновить статус отзыва на сервере. ';
            }
          }
        } else {
          $return['error'] .= ($error_count++) . '. Такого e-mail нет в нашей рассылке. ';
        }
        break;

      case 'subscribe':
        $num = $app->fetch("SELECT COUNT(*) FROM `subscribes` WHERE `code` = " . $app->pdo->quote($code))['COUNT(*)'];
        if ($num) {
          $subscriber = $app->fetch("SELECT * FROM `subscribes` WHERE `code` = " . $app->pdo->quote($code));

          if ($subscriber['approved'] == 1) {
            $return['success'] .= ($success_count++) . '. Ваш e-mail уже подтверждён. ';
          } else {
            $num = $app->execCount("UPDATE `subscribes` SET `approved` = 1, `approve_tm` = "
                . time() . " WHERE `code` = " . $app->pdo->quote($code));
            if ($num) {
              $return['success'] .= ($success_count++) . '. Ваш e-mail подтверждён.';

              // Sends email to customer about confirming subscription
              if ( $app->myMail(
                $subscriber['mail'],
                'ME Notification',
                "<html><body>E-mail {$subscriber['mail']} подтверждён.</body></html>"
              ) ) {
                $return['success'] .= ($success_count++) . '. Subscription e-mail sent to the customer.';
              }

              // Sends email to admin about confirming subscription
              if ( $app->myMail(
                  $order_mail,
                  'ME Notification',
                  "<html><body>E-mail {$subscriber['mail']} подтверждён.</body></html>"
                ) ) {
                $return['success'] .= ($success_count++) . '. Subscription e-mail sent to the admin.';
              }
            } else {
              $return['error'] .= ($error_count++) . '. Не удалось обновить статус e-mail на сервере. ';
            }
          }
        } else {
          $return['error'] .= ($error_count++) . '. Такого e-mail нет в нашей рассылке. ';
        }
        break;

      case 'unsubscribe':
        $num = $app->fetch("SELECT COUNT(*) FROM `subscribes` WHERE `code` = " . $app->pdo->quote($code))['COUNT(*)'];
        if ($num) {
          $subscriber = $app->fetch("SELECT * FROM `subscribes` WHERE `code` = " . $app->pdo->quote($code));
          $num = $app->execCount("DELETE FROM `subscribes` WHERE `code` = " . $app->pdo->quote($code));
          if ($num) {
            $app->myMail(
                $email,
                'ME Notification',
                "<html><body>E-mail {$subscriber['mail']} удалён из рассылки.</body></html>"
            );
            $return['success'] .= ($success_count++) . '. Ваш e-mail удалён.';
          } else {
            $return['error'] .= ($error_count++) . '. Не удалось удалить ваш email на сервере. ';
          }
        } else {
          $return['error'] .= ($error_count++) . '. Такого e-mail нет в нашей рассылке. ';
        }
        break;

      default: $return['error'] .= ($error_count++) . '. Неизвестная команда. ';

    }
  } else {
    $return['error'] .= ($error_count++) . '. Инициализация магазина.';
  }

  echo json_encode($return);