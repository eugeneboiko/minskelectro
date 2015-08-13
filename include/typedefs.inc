<?php

// ------------------------- typedefs -----------------------------

define( 'APP_NAME',                     'GXShop by BLR.BIZ Web Development');
define( 'APP_VERSION',                  '1.3.0');

define( 'ME_IS_CLI',                    ( 'cli' == php_sapi_name() ) );

define( 'ME_DB_HOST',                   'localhost' );
define( 'ME_DB_USR',                    'minskelectro' );
define( 'ME_DB_PWD',                    'nVp5KkCyD5gd' );
define( 'ME_DB_NAME',                   'minskelectro' );
# eb 20130123 define( 'ME_DB_NAME',                   'minskelectro_utf8' );

define( 'ME_LOG_IDENT',                 'www-me' );
define( 'ME_LOG_FACILITY',              LOG_LOCAL6 );
define( 'ME_SITE_NAME_SHORT',           'me' );
define( 'ME_SITE_NAME',                 'www.minskelectro.com' );
define( 'ME_WWWADDR',                   'http://minskelectro.com/' );
define( 'ME_WWWROOT',                   '/usr/local/www/www.minskelectro.com/' );
define( 'ME_ADMIN_EMAIL_ADDR',          'admin@minskelectro.com' );
define( 'ME_ADMIN_DISPLAY_NAME',        'MinskElectro Admin' );
define( 'ME_ADMIN_ADDR',                ME_ADMIN_DISPLAY_NAME.' <'.ME_ADMIN_EMAIL_ADDR.'>' );

define( 'ME_IMPORT_FILE_PATH',          ME_WWWROOT.'data_import/' );
define( 'ME_EXPORT_ARCHIVE_FILE_PATH',  ME_IMPORT_FILE_PATH.'me2pt.zip' );
define( 'ME_IMPORT_FILE_NAME',          'data.zip' );
define( 'ME_IMPORT_FILE_PATHNAME',      ME_IMPORT_FILE_PATH.ME_IMPORT_FILE_NAME );
define( 'ME_IMPORT_FILE_URL',           'http://unadm.at.tut.by/data.zip' );
define( 'ME_EXPORT_DATA_FILE_NAME',     'me2pt.xls' );
define( 'ME_TMP_DIR',                   ME_WWWROOT.'.tmp' );
define( 'ME_IMPORT_STATUS_FILE',        ME_TMP_DIR.'/import_stat.html' );
define( 'ME_DB_CAPTCHA_FILE',           ME_WWWROOT.'include/captcha/captcha.sqlite3' );
define( 'MAX_PRICE',                    9999 );

//-------------------------- errcodes -----------------------------

define( 'ME_APP',           100000 );

define( 'RC_SUCCESS',       0 );
define( 'RC_FAIL',          (-1) );
define( 'RC_NOTFOUND',      (ME_APP + 1) );
define( 'RC_FREADFAILED',   (ME_APP + 2) );
define( 'RC_FWRITEFAILED',  (ME_APP + 3) );
define( 'RC_INVPARAM',      (ME_APP + 4) );
define( 'RC_INVSTATE',      (ME_APP + 5) );
define( 'RC_MISMATCH',      (ME_APP + 6) );
define( 'RC_DBCONNFAIL',    (ME_APP + 7) );
define( 'RC_INVVALUE',      (ME_APP + 8) );