<?php
  error_reporting(E_ALL);
/**
  * Output CAPTCHA image for <img src .
  *
  *
  */
  include 'app.inc';
  $app = new App();

  $font        = 'captcha.ttf';
  $charset     = '0123456789';         // list possible characters to include on the CAPTCHA
  $code_length = 4;                    // how many characters include in the CAPTCHA
  $height      = 16;                   // antispam image height
  $width       = 50;                   // antispam image width

  $code        = '';
  for( $i = 0; $i < $code_length; $i++ ) {
    $code      = $code . substr( $charset, mt_rand( 0, strlen( $charset ) - 1 ), 1 );
  }

  $ins = $app->execCount("INSERT INTO captchas (id, value, create_tm, ip, useragent_md5) VALUES ({$_GET['id']}, $code, "
    . ($t = time()) . ", '" . $_SERVER['REMOTE_ADDR'] . "', '" . md5( $_SERVER['HTTP_USER_AGENT'] ) . "')" );
  $del = $app->execCount("DELETE FROM captchas WHERE abs( $t - create_tm ) > 10 * 60" );
  if ($ins) {
    $app->myLog( LOG_INFO, "CAPTCHA set: captcha_id = {$_GET['id']}, captcha_value = $code" );
  } else {
    $app->myLog( LOG_INFO, "CAPTCHA set error: inserted $ins, deleted $del" );
  }

  $font_size        = $height * 0.7;
  $image            = imagecreate( $width, $height );
  $background_color = imagecolorallocate( $image, 255, 255, 255 );
  $noise_color      = imagecolorallocate( $image, 200, 200, 200 );

  // add image noise
  for( $i = 0; $i < ( $width * $height ) / 4; $i++ ) {
    imageellipse( $image, mt_rand( 0, $width ), mt_rand( 0, $height ), 1, 1, $noise_color );
  }

  // render text
  $text_color       = imagecolorallocate( $image, 20, 40, 100 );
  imagettftext( $image, $font_size, -2, 7, 15, $text_color, $font, $code )
    or die( 'Cannot render TTF text.' );

  // output image to the browser
  header( 'Content-Type: image/png' );
  header( 'Cache-Control: no-cache, must-revalidate' ); # HTTP/1.1
  header( 'Expires: Sat, 26 Jul 1997 05:00:00 GMT' );   # Date in the past
  @imagepng( $image ) or die( 'imagepng error!' );
  @imagedestroy( $image );