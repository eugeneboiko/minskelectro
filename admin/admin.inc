<?php

  set_time_limit ( 60*20 );

  if (__FILE__ == '/usr/local/www/test.minskelectro.com/admin/admin.inc') {

    define( 'APP_TEST_BRANCH', TRUE);
    error_reporting(E_ALL);
    if ( set_include_path(".:/usr/local/www/test.minskelectro.com/include:/usr/local/share/pear") === 'FALSE' ) {
      throw new Exception('Error set_include_path for test branch of ME!');
    }
    echo '<h1 style="color:red;text-align:center">Test Branch</h1>';

  } else {
    define( 'APP_TEST_BRANCH', FALSE);
  }

  require 'class.template.inc';
  require 'app.inc';
  require 'admin_util.inc';

  $template = new Template( ME_WWWROOT );
  $app = new App();

  if (count($_POST)) {
    foreach ( $_POST as $k => $v ) @eval('$'.preg_replace('#(\W)#isU', '', $k)." = \$v;");
  } else {
    foreach ( $_GET as $k => $v ) @eval('$'.preg_replace('#(\W)#isU', '', $k)." = \$v;");
  }

  $ops = $app->fetchAll('SELECT * FROM `options`');

  foreach ($ops as $key => $option) {
    $$option['name'] = $option['value'];
  }

  $page_name_      = explode('%s', $page_name_tpl, 3);  // получаем массив для определения формата заголовка страницы
  $page_name_start = $page_name_[0];                    // начало заголовка всех страниц
  $page_name_spl   = $page_name_[1];                    // разделитель частей заголовка
  $page_name_end   = $page_name_[2];                    // конец заголовка всех страниц

  $template->set_filenames(array('page' => './tpl/admin_me.tpl'));

  $template->assign_vars(array(
    'copyright'              => $copyright,
    'short_name'             => $name,
    'full_name'              => $long_name,
    'address'                => $address,
    'email_address'          => $shop_mail,
    'webadmin_email_address' => $web_admin_mail,
    'test_branch_min'        => APP_TEST_BRANCH ? '' : '.min'
  ));

  function get_url_compatible( $text ) {
    return urlencode( preg_replace( '/\s{1,}/', ' ', trim( preg_replace( '/\([^\)]*?\)/', '', $text ) ) ) );
  }

  $template->assign_block_vars('admin_menu', array());
  $template->assign_block_vars('admin_menu.item', array(
    'link' => 'admin_options.php',
    'name' => 'Настройки'
  ));
  $template->assign_block_vars('admin_menu.item', array(
    'link' => 'admin_reviews.php',
    'name' => 'Отзывы'
  ));
  $template->assign_block_vars('admin_menu.item', array(
    'link' => 'admin_orders.php',
    'name' => 'Заказы'
  ));
  $template->assign_block_vars('admin_menu.item', array(
    'link' => 'admin_news.php',
    'name' => 'Новости'
  ));
  $template->assign_block_vars('admin_menu.item', array(
    'link' => 'admin_subscribers.php',
    'name' => 'Письмо подписчикам'
  ));
  $template->assign_block_vars('admin_menu.item', array(
    'link' => 'admin_own_pages.php',
    'name' => 'Мои страницы'
  ));
  $template->assign_block_vars('admin_menu.item', array(
    'link' => 'admin_import.php',
    'name' => 'Импорт + прайс'
  ));
  $template->assign_block_vars('admin_menu.item', array(
    'link' => 'admin_price_send.php',
    'name' => 'Рассылка прайса'
  ));