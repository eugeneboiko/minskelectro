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
    'error' => ''
  );

  $options = $app->fetchAll('SELECT * FROM `options`');

  if ( count($options) ) {
    foreach ($options as $key => $option) {
      $$option['name'] = $option['value'];
    }

    // AngularJS transmits data using Content-Type: application/json and JSON serialization,
    // which unfortunately some Web server languages—notably PHP—do not unserialize natively.
    // So server has empty $_POST array.
    $http_post = json_decode( file_get_contents("php://input") );

    $order          = $http_post->order;
    $s1             = $currency_name ? '' : $currency_symbol;         // currency symbol
    $s2             = $currency_name ? ' ' . $currency_name : '';     // currency name
    $r              = (int)$currency_exchange_rate;                   // exchange rate
    $d              = (int)$delivery_free;                            // delivery free
    $p              = (int)$delivery_cost;                            // delivery price
    $captcha_id     = intval($order->user->captcha->id);
    $captcha_value  = intval($order->user->captcha->value);
    $name           = htmlentities(strip_tags($order->user->name));
    $phone          = htmlentities(strip_tags($order->user->phone));
    $address        = htmlentities(strip_tags($order->user->address));
    $email          = isset($order->user->email) ? htmlentities(strip_tags($order->user->email)) : '';
    $comment        = isset($order->user->comment) ? htmlentities(strip_tags($order->user->comment)) : '';
    $referral       = isset($order->user->referral) ? htmlentities(strip_tags($order->user->referral)) : '';
    $subscribe      = isset($order->user->subscribe) ? htmlentities(strip_tags($order->user->subscribe)) : '';
    $subscribe_now      = '';
    $subscribe_already  = '';
    $subscribe_decline  = '';

    if ( $app->execCount("DELETE FROM captchas WHERE id = $captcha_id AND value = $captcha_value") ) {

      if ( count($order->product) ) {

        if( $name and $phone and $address ) {
          if ( $subscribe and $email ) {
            $num = $app->fetch("SELECT COUNT(*) FROM `subscribes` WHERE `mail` = '" . $email . "'")['COUNT(*)'];
            if ($num) {
              $code = $app->fetch("SELECT `code` FROM `subscribes` WHERE `mail`= '" . $email . "'")[0];
              $subscribe_already = "подписаны ранее";
            }	else {
              if ((preg_match('#(.+)@(.+)\.(.){2,6}#', $email)) and (strlen($email) < 100)) {
                $code = md5( crypt($email, 'mms_shop') );
                $num = $app->execCount("INSERT INTO subscribes (`name`, `mail`, `code`, `approved`, `create_tm`, `approve_tm`, `ip`) VALUES (" . $app->pdo->quote($name) . ", "
                  . $app->pdo->quote($email) . ", '" . $code . "', 0, " . time() .", 0, '" . $_SERVER['REMOTE_ADDR'] . "' )");
                $subscribe_now = "подписаны сейчас";

                // Sends email to customer to approve subscription
                $app->myMail(
                  $email,
                  'Подписка в MinskElectro',
                  "<html><body><h1>Здравствуйте, $name.</h1>"
                      . "E-mail $email был указан при подписке на новости сайта MinskElectro.com. Для подтверждения подписки вам следует перейти по <a href='http://www.minskelectro.com/?subscribe=$code'>этой ссылке</a>."
                      . "<p>С уважением,<br>MinskElectro.com</p>"
                      . "</body></html>",
                  'MinskElectro',
                  $order_mail
                );
                $return['success'] .= ($success_count++) . '. Subscription e-mail sent to the customer.';
              } else {
                $return['error'] .= ($error_count++) . 'Email is invalid.';
              }
            }
          }
        } else {
          $subscribe_decline = 'отказались';
        }

        $i = 1;
        $products_price = 0;
        $order_price = 0;
        $product_table_for_client_email = "<table style='background:#ddd;text-align:right;width:1100px'><tr>"
            . "<th style='width:30px'>№</th><th style='width:70px'>ID</th><th style='text-align:left'>Название</th>"
            . "<th style='width:100px'>Гарантия,мес.</th><th style='width:100px'>Цена,$s2</th>"
            . "<th style='width:100px'>Кол-во</th><th style='width:100px'>Сумма, $s2</th></tr>";
        $product_table_for_admin_email  = "<table style='background:#ddd;text-align:right;width:1200px'><tr>"
            . "<th style='width:30px'>№</th><th style='width:70px'>ID</th><th style='text-align:left'>Название</th>"
            . "<th style='width:100px'>Гарантия,мес.</th><th style='width:100px'>Пост.</th><th style='width:100px'>Опт цена, $s2</th>"
            . "<th style='width:100px'>Цена, $s2</th><th style='width:100px'>Кол-во</th><th style='width:100px'>Сумма, $s2</th></tr>";
        $product_table_for_admin_sms    = '';
        $footer_for_client_email        = '';
        $footer_for_admin_email         = '';
        $footer_for_admin_sms           = '';
        foreach( $order->product as $k => $product ) {

          $num = $app->fetch("SELECT COUNT(*) FROM `products` WHERE `id` = " . $product->id)['COUNT(*)'];
          if ($num) {
            $prod = $app->fetch("SELECT * FROM `products` WHERE `id` = " . $product->id);
            if ($prod['linkto']) {
              $num = $app->fetch("SELECT COUNT(*) FROM `products` WHERE `id` = " . $prod['linkto'])['COUNT(*)'];
              if ($num) {
                $prod = $app->fetch("SELECT * FROM `products` WHERE `id` = " . $prod['linkto']);
              }
            }
          }

          $total_price = $product->count * $prod['price_out'];
          if ( $num ) {
            $product_table_for_client_email .= "<tr><td>$i</td><td>{$prod['id']}</td><td style='text-align:left'>"
                . "{$prod['name']}</td><td>{$prod['warranty']}</td><td>$s1{$prod['price_out']}</td><td>{$product->count}</td><td>"
                . "$s1$total_price</td></tr>";
            $product_table_for_admin_email .= "<tr><td>$i</td><td>{$prod['id']}</td><td>{$prod['name']}</td>"
                . "<td>{$prod['warranty']}</td><td>{$prod['supplier']}</td><td>{$prod['price_in']}</td><td>{$prod['price_out']}</td>"
                . "<td>{$product->count}</td><td>$s1$total_price</td></tr>";
            $product_table_for_admin_sms .= "$i. {$prod['name']} ({$prod['supplier']}|{$prod['warranty']}m|{$prod['price_in']}/"
                . "$s1{$prod['price_out']}$s2*{$product->count}шт.)\n";
            $products_price += $product->count * $prod['price_out'];
          } else {
            $product_table_for_client_email .= "<tr><td>$i</td><td>{$prod['id']}</td><td colspan='5'>Не найден! Не существует или продан.</td></tr>";
            $product_table_for_admin_email  .= "<tr><td>$i</td><td>{$prod['id']}</td><td colspan='5'>Не найден! Не существует или продан.</td></tr>";
            $product_table_for_admin_sms    .= "$i. #{$prod['id']} не найден!\n";
          }

          $i++;
        }

        $order_price = $products_price;
        if (($products_price >= $d) and ($d != -1)) {
          $footer_for_client_email  .= "<tr><td colspan='6'>Доставка входит в стоимость</td><td>0</td></tr>";
          $footer_for_admin_email   .= "<tr><td colspan='8'>Доставка входит в стоимость</td><td>0</td></tr>";
          $footer_for_admin_sms     .= "";
        } else {
          $order_price += $p;
          $footer_for_client_email  .= "<tr><td colspan='6'>За доставку:</td><td>$s1$p</td></tr>";
          $footer_for_admin_email   .= "<tr><td colspan='8'>За доставку:</td><td>$s1$p</td></tr>";
          $footer_for_admin_sms     .= "";
        }

        $footer_for_client_email  .= "<tr><td colspan='6'>Курс:</td><td>$r</td></tr><tr><td colspan='6'>Итого:</td><td><strong>$s1{$order_price}</strong></td></tr>";
        $footer_for_admin_email   .= "<tr><td colspan='8'>Курс:</td><td>$r</td></tr><tr><td colspan='8'>Итого:</td><td><strong>$s1{$order_price}</strong></td></tr>";
        $footer_for_admin_sms     .= "Итого:$s1({$products_price} + {$p})*$r ";

        // Sends order email to admin
        if ( $app->myMail(
            $order_mail,
            'ME Order',
            "<html><body><h1>Заказ от $name.</h1>"
                . "$product_table_for_admin_email$footer_for_admin_email</table>\r\n<table>"
                . "<tr><td>1. <strong>Телефон:</strong></td><td>$phone</td></tr>"
                . "<tr><td>2. <strong>Адрес доставки:</strong></td><td>$address</td></tr>"
                . "<tr><td>3. <strong>E-mail:</strong></td><td>$email</td></tr>"
                . "<tr><td>4. <strong>Откуда узнали:</strong></td><td>$referral</td></tr>"
                . "<tr><td>5. <strong>Комментарий:</strong></td><td>$comment</td></tr>"
                . "<tr><td>6. <strong>Подписка на новости:</strong></td><td>"
                . ($subscribe_already ? $subscribe_already : '') . ($subscribe_now ? $subscribe_now : '') . ($subscribe_decline ? $subscribe_decline : '') . "</td></tr>"
                . "</table></body></html>",
            $name,
            $email
          ) ) {
          $return['success'] .= ($success_count++) . '. Order e-mail sent to the admin.';
        } else {
          $return['error'] .= ($error_count++) . '. Order e-mail sent to the admin.';
        }

        // Sends order to admin via SMS
        $app->myMail(
          $order_mail_sms,
          'ME Order',
          iconv( "utf-8", "cp1251", "$footer_for_admin_sms, $name, Тел:$phone, Адр:$address, $product_table_for_admin_sms, Ком:$comment"),
          'ME'
        );
        $return['success'] .= ($success_count++) . '. SMS sent to the admin.';

        // Sends order to the customer
        if( $email ) {
          $app->myMail(
            $email,
            'Заказ в MinskElectro',
            "<html><body><h1>Здравствуйте, $name.</h1>"
                . "$product_table_for_client_email$footer_for_client_email</table>\r\n<table>"
                . "<tr><td colspan='2'>Вы указали следующие данные</td></tr>"
                . "<tr><td>1. <strong>Телефон:</strong></td><td>$phone</td></tr>"
                . "<tr><td>2. <strong>Адрес доставки:</strong></td><td>$address</td></tr>"
                . "<tr><td>3. <strong>Комментарий:</strong></td><td>$comment</td></tr>"
                . "<tr><td>4. <strong>Подписка на новости:</strong></td><td>"
                . ($subscribe_already ? $subscribe_already : '') . ($subscribe_now ? $subscribe_now : '') . ($subscribe_decline ? $subscribe_decline : '') . "</td></tr>"
                . "</table></p><p>В течение $order_time с вами могут связаться для подтверждения, уточнения или дополнения заказа.</p>"
                . "<p>Спасибо, что выбрали именно наш каталог.</p>\r\n"
                . "<p>С уважением,<br>MinskElectro.com.</p>"
                . ($subscribe_already ? "<p style='font-size:14px;font-weight:bold;'>Внимание! Вам следует перейти по <a href='http://www.minskelectro.com/?subscribe=$code'>этой ссылке</a>, чтобы подтвердить добавление в рассылку.</p>" : '')
                . ($subscribe_now ? "<p style='font-size:11px;'>Вам следует перейти по <a href='http://www.minskelectro.com/?unsubscribe=$code'>этой ссылке</a>, чтобы отказаться от рассылки.</p>" : '')
                . "<p style='font-size:11px;'>Вы получили это письмо, потому что $email был указан при заказе.</p>"
                . "</body></html>",
            'MinskElectro',
            $order_mail
          );
          $return['success'] .= ($success_count++) . '. Email sent to the customer.';
        } else {
          $return['error'] .= ($error_count++) . '. Email sent to the customer.';
        }

      } else {
        $return['error'] .= ($error_count++) . '. Ваша корзина пуста! Невозможно принять заказ.';
      }
    } else {
      $return['error'] .= ($error_count++) . '. Неправильно введены цифры с картинки!';
    }
  } else {
    $return['error'] .= ($error_count++) . '. Количество опций: 0.';
  }

  echo json_encode($return);