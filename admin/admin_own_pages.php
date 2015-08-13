<?php
/**
  * Output My pages.
  *
  *
  */
  require 'admin.inc';

  $template->assign_vars(array('page_name' => 'Мои страницы'));
  $template->assign_block_vars('way_item', array('name' => 'Мои страницы'));
  $template->assign_block_vars('way_item_l', array('link' => 'admin_options.php', 'name' => 'Административный интерфейс'));

  if (isset($id)) {
    if ($o_title > 255) {
      $template->assign_vars(array( 'page_body_prefix_err' => 'Поле Название длинее 255 символов.' ));
    } else {
      if ($o_menu_name > 150) {
        $template->assign_vars(array( 'page_body_prefix_err' => 'Поле Имя в меню длинее 150 символов.' ));
      } else {
        if (!($o_text or $o_file or $o_redirect)) {
          $template->assign_vars(array( 'page_body_prefix_err' => 'Необходимо установить одно из полей: Текст страницы, URL файла или URL редиректа.' ));
        } else {
          if (!$id) {
            // добавление
            $maxpos = $app->fetch("SELECT MAX(position) FROM `own_pages`")['MAX(position)'];
      /*
              $db->select('MAX(`position`)', 'own_pages');
              $maxpos = $db->fetch_row();
      */
              $app->execCount("INSERT INTO `own_pages` (`type`, `menu_name`, `title`, `text`, `url_f`, `url_r`, `visible`, `position`) VALUES (" . ($o_text ? 'текст': ($o_file ? 'файл' : 'редирект')) .", " . $o_menu_name . ", " . $o_title . ", " . $o_text . ", " . $o_file . ", " . $o_redirect .", " . "1, " . ($maxpos[0] + 1) . ")");
      /*
              $qq = array(
                ($o_text ? 'текст': ($o_file ? 'файл' : 'редирект')),
                $o_menu_name,
                $o_title,
                $o_text,
                $o_file,
                $o_redirect,
                '1',
                $maxpos[0] + 1
              );
              $db->insert('own_pages', '`type`, `menu_name`, `title`, `text`, `url_f`, `url_r`, `visible`, `position`', $qq);
      */

              $template->assign_vars(array( 'page_body_prefix_note' => 'Страница добавлена.' ));
          } else {
              // редактирование
              $app->execCount("UPDATE `own_pages` SET `type` = " . ($o_text ? 'текст': ($o_file ? 'файл' : 'редирект')) . ", `menu_name` = $o_menu_name, `title` = $o_title, `text` = $o_text, `url_f` = $o_file, `url_r` = $o_redirect WHERE id = $id");
      /*
              $qq = array(
                '`type`' => ($o_text ? 'текст': ($o_file ? 'файл' : 'редирект')),
                '`menu_name`' => $o_menu_name,
                '`title`' => $o_title,
                '`text`' => $o_text,
                '`url_f`' => $o_file,
                '`url_r`' => $o_redirect
              );
              $db->update('own_pages', $qq, "id=$id");
      */
              $template->assign_vars(array( 'page_body_prefix_note' => 'Страница обновлена.' ));
              $edit = $id;
          }
        }
      }
    }
  } else if (isset($delete) and is_numeric($delete)) {
    $app->execCount("DELETE FROM `own_pages` WHERE `id` = $delete");
/*
    $db->delete('own_pages', "id=$delete");
*/
    $template->assign_vars(array( 'page_body_prefix_note' => 'Страница удалена.' ));
  } else if (isset($hide) and is_numeric($hide)) {
    $app->execCount("UPDATE `own_pages` SET `visible` = 0 WHERE `id` = $hide");
/*
    $db->update('own_pages', array('`visible`' => '0'), "id=$hide");
*/
    $template->assign_vars(array( 'page_body_prefix_note' => 'Страница скрыта.' ));
  } else if(isset($show) and is_numeric($show)) {
    $app->execCount("UPDATE `own_pages` SET `visible` = 1 WHERE `id` = $show");
/*
    $db->update('own_pages', array('`visible`' => '1'), "id=$show");
*/
    $template->assign_vars(array( 'page_body_prefix_note' => 'Страница открыта.' ));
  } else if(isset($move) and is_numeric($move) and ($how == 'up' or $how == 'down')) {
    $pos = $app->fetch("SELECT `position` FROM `own_pages` WHERE `id` = $move")[0];
/*
    $db->select('`position`', 'own_pages', "id=$move");
    $pos = $db->fetch_row();
    $pos = $pos[0];
*/
    $max = $app->fetch("SELECT MAX(position) FROM `own_pages`")['MAX(position)'];
/*
    $db->select('MAX(`position`)', 'own_pages');
    $max = $db->fetch_row();
    $max = $max[0];
*/
    if($how == 'up' and $pos > 1) {
      $app->execCount("UPDATE `own_pages` SET `position` = `position` + 1 WHERE `position` = $pos - 1");
      $app->execCount("UPDATE `own_pages` SET `position` = $pos - 1 WHERE `id` = $move");
/*
      $db->raw_query("update own_pages set `position` = `position` + 1 where `position` = $pos - 1");
      $db->raw_query("update own_pages set `position` =  $pos - 1 where id = $move");
*/
    } else if($pos < $max) {
      $app->execCount("UPDATE `own_pages` SET `position` = `position` - 1 WHERE `position` = $pos + 1");
      $app->execCount("UPDATE `own_pages` SET `position` = $pos + 1 where `id` = $move");
/*
      $db->raw_query("update own_pages set `position` = `position` - 1 where `position` = $pos + 1");
      $db->raw_query("update own_pages set `position` =  $pos + 1 where id = $move");
*/
    }
    $template->assign_vars(array( 'page_body_prefix_note' => 'Страница перемещена.' ));
  }
  // открываем на редактирование
  if (isset($edit) and is_numeric($edit)) {
    $own_pages = $app->fetchAll("SELECT `title`, `menu_name`, `text`, `url_f`, `url_r` FROM `own_pages` WHERE `id` = $edit");
/*
  $db->select('`title`, `menu_name`, `text`, `url_f`, `url_r`', 'own_pages', "id=$edit");
  if($arr = $db->fetch_row())
*/
    if ( count($own_pages) ) {
      foreach ($own_pages as $k => $own_page)
        $template->assign_block_vars('admin_own_pages', array(
          'doing'     => '<b>Правка</b>',
          'edit_id'   => $edit,
          'title'     => $own_page['title'],
          'menu_name' => $own_page['menu_name'],
          'text'      => $own_page['text'],
          'file'      => $own_page['url_f'],
          'redirect'  => $own_page['url_r'],
          'new'       => ' (<a href="_admin_own_pages.php">Создать новую</a>)'
      ));
    } else {
      $template->assign_vars(array( 'page_body_prefix_err' => 'Страницы с таким ID не существует.' ));
    }
  } else {
    // for new record
    $template->assign_block_vars('admin_own_pages', array(
      'doing'   => '<b>Создание новой</b>',
      'edit_id' => '0'
    ));
  }

  $pages = $app->fetchAll("SELECT `id`, `type`, `menu_name`, `visible` FROM `own_pages` ORDER BY `position`" );
  foreach ($pages as $k => $page){
    $col = ($col == '#FFFAE6' ? '#FFF3E6' : '#FFFAE6');
    $template->assign_block_vars('admin_own_pages.item', array(
      'id'        => $page['id'],
      'type'      => $page['type'],
      'menu_name' => strip_tags($page['menu_name']),
      'vis_do'    => ($page['visible'] ? 'hide' : 'show'),
      'vis_off'   => ($page['visible'] ? '' : '_off'),
      'vis_text'  => ($page['visible'] ? 'скрыть' : 'показать'),
      'color'     => $col
    ));
  }

  $template->pparse('page');