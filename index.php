<?php

  if (__FILE__ == '/usr/local/www/test.minskelectro.com/index.php' ) {

    define( 'APP_TEST_BRANCH', TRUE);
    error_reporting(E_ALL);
    if ( set_include_path(".:/usr/local/www/test.minskelectro.com/include:/usr/local/share/pear") === 'FALSE' ) {
      throw new Exception('Error set_include_path for test branch of ME!');
    }
    echo '<h1 style="color:red;text-align:center">Test Branch</h1>';
  } else {
    define( 'APP_TEST_BRANCH', FALSE);
  }


  // optimization for search engines: hostname/key1~val1[/key2~val2[/key3~val3 ...]] -> $key1=val1...
  if ( isset($_GET['a']) )
    foreach( explode('/', $_GET['a']) as $a ) {
      $a = explode('~', $a);
      if ($a[0]) $_GET[$a[0]] = urldecode($a[1]);
    }

  include_once 'class.template.inc';
  include_once 'app.inc';

  $template = new Template( ME_WWWROOT );
  $app = new App();

  $template->set_filenames(array('page' => 'tpl/me.tpl'));

  $options = $app->fetchAll('SELECT * FROM `options`');

  foreach ($options as $key => $option) {
    $$option['name'] = $option['value'];
  }

  $products_per_page = (int)$products_per_page < 1 ? 15 : $products_per_page;

  $template->assign_vars(array(
    'copyright'              => $copyright,
    'currency_exchange_rate' => $currency_exchange_rate,
    'short_name'             => $name,
    'full_name'              => $long_name,
    'address'                => $address,
    'email_address'          => $shop_mail,
    'webadmin_email_address' => $web_admin_mail,
    'page_bottom_info'       => $page_bottom_info,
    'test_branch_min'        => APP_TEST_BRANCH ? '' : '.min'
  ));

  function get_url_compatible( $text ) {
    return urlencode( preg_replace( '/\s{1,}/', ' ', trim( preg_replace( '/\([^\)]*?\)/', '', $text ) ) ) );
  }

  $html = '';
  $menu = array();
  // for menu tree
  $categories = $app->fetchAll("SELECT `id`, `name`, `parent` FROM `categories` WHERE `hidden` = 0 ORDER BY `name`");
  // sections of top level
  for ($i = 0; $i < count($categories); $i++) {
    $menuNames[$categories[$i]['id']] = $categories[$i]['name'];
    $menu[$categories[$i]['parent']][$categories[$i]['id']] = $categories[$i]['name'];
  }
  // sections of sub level
  foreach ($menu as $k => $t1) {
    foreach ($menu as $i => $sub) {
      foreach ($sub as $j => $t2) {
        if ($j == $k) {
          $menu[$i][$j] = $menu[$k];
        }
      }
      unset($t2);
    }
    unset($sub);
  }
  if ( count($menu) ) {
    foreach ($menu[0] as $id => $subLev1) {
      $html .= "<li><a href='/c~$id' onclick='return false;' ng-click='mainCtrl.getSection($id)'>" . $menuNames[$id] . "</a>";
      if ( is_array($subLev1) ) {
        $html .= "<ul>";
        foreach ($menu[0][$id] as $id2 => $sub) {
          $html .= "<li><a href='/c~$id2' onclick='return false;' ng-click='mainCtrl.getSection($id2)'>" . $menuNames[$id2] ."</a></li>";
        }
        unset($sub);
        $html .= "</ul>";
      }
      $html .= "</li>";
    }
  }
  $template->assign_vars(array('menu' => $html));

  if ( isset($_GET['p']) ) {
    include 'index_product.inc';
  } else {
    include 'index_section.inc';
  }

  $template->pparse('page');