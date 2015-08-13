<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta name="author" content="Eugene V. Boiko">
    <meta name="robots" content="noimageindex">
    <title>{page_name}</title>
    <link rel="shortcut icon" href="favicon.ico">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap{test_branch_min}.css">
    <link rel="stylesheet" href="../css/me{test_branch_min}.css" type="text/css">

    <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
    <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv{test_branch_min}.js"></script>
    <script src="https://oss.maxcdn.com/libs/respond.js/1.3.0/respond{test_branch_min}.js"></script>
    <![endif]-->
  </head>

  <body>

    <div class="container-fluid">

      <header>
        <!-- BEGIN admin_menu -->
        <menu class="menu">
          <!-- BEGIN item -->
          <li><a href="{admin_menu.item.link}" title="{admin_menu.item.name}">{admin_menu.item.name}</a></li>
          <!-- END item -->
        </menu>
        <!-- END admin_menu -->

        <div class="head_back">

          <img src="/images/top_line_738x150.jpg" alt="" class="header1">

          <table class="header2">
            <tr>
              <td><img src="/images/line_top_left.jpg" alt="" height="150"></td>
              <td class="structure"></td>
              <td><img src="/images/line_top_right.jpg" alt="" height="150"></td>
            </tr>
          </table>

        </div>

        <div class="head_front">
          <h1 class="center">Административный интерфейс</h1>
        </div>
        
      </header>

      <section>

        <div class="center">
          <span class="label label-danger">{page_body_prefix_err}</span>
          <span class="label label-success">{page_body_prefix_note}</span>
        </div>

        <div class="nav">
          <ol class="breadcrumb">
            <li>
              <!-- BEGIN way_item_l -->
              <a href="{way_item_l.link}">{way_item_l.name}</a> /
              <!-- END way_item_l -->
              <!-- BEGIN way_item -->
              <span>{way_item.name}{way_item.link}</span>
              <!-- END way_item -->
            </li>
          </ol>
        </div>

        <!-- BEGIN sub_cat -->
        <ul class="menu">
          <!-- BEGIN item -->
          <li><a href="/c~{sub_cat.item.id}" title="Подраздел {sub_cat.item.title}">{sub_cat.item.name}</a></li>
          <!-- END item -->
        </ul>
        <!-- END sub_cat -->

        <!-- BEGIN pgs -->
        <div class="btn-group center" role="group" aria-label="Pages">
          <!-- BEGIN item -->
          <!-- BEGIN item1 -->
          <button type="button" class="btn btn-default" title="Страница #{pgs.item.item1.num}"><a href="{pgs.link}{pgs.item.item1.num}">{pgs.item.item1.num}</a></button>
          <!-- END item1 -->
          <!-- BEGIN item2 -->
          <button type="button" class="btn btn-default" title="Страница #{pgs.item.item2.num}">{pgs.item.item2.num}</button>
          <!-- END item2 -->
          <!-- END item -->
          <button type="button" class="btn btn-default">{pgs.prev_next}</button>
        </div>
        <!-- END pgs -->

        <!-- BEGIN admin_options -->
        <form method="post" name="optionsForm">
          <input name="options" type="hidden" value="1">
          <ul class="list-group">
            <!-- BEGIN item -->
            <li class="list-group-item">
              <span>{admin_options.item.name}</span>
              <textarea class="contact form-control" name="op[{admin_options.item.id}]">{admin_options.item.value}</textarea>
            </li>
            <!-- END item -->
          </ul>
          <input type="submit" name="submit" class="btn btn-primary center" value="Сохранить">
        </form>
        <!-- END admin_options -->

        <!-- BEGIN admin_import -->
        {admin_import.status}
        <h3>Инструкции</h3>
        <ul>
          <li>Архивный файл может содержать произвольные файлы.</li>
          <li>Если в архивном файле будут найдены один или несколько из приведенных ниже файлов, то они импортируются в приведенном порядке:
            <ol>{admin_import.files}</ol>
          </li>
          <li>В первой строке каждого файла должны быть перечислены имена импортируемых полей. Порядок полей не имеет значения.</li>
          <li>Имена и значения полей должны разделяться символом табуляции '\t'.</li>
        </ul>
        <form method="post" onsubmit="subBut.disabled=true" class="center">
          <input type="hidden" name="import" value="1">
          <input type="hidden" name="cmd" value="1">
          <input type="submit" value="Начать Импорт" class="btn btn-primary" name="subBut">
        </form>
        <!-- END admin_import -->

        <!-- BEGIN admin_own_pages -->
        <form action="admin_own_pages.php" method="post" name="ownPagesForm">
          <input name="own_pages" type="hidden" value="1">
          <h3>Менеджер дополнительных страниц{admin_own_pages.new}:</h3>

          <table>
            <tr>
              <td colspan="2"><em>{admin_own_pages.doing} страницы:<br/><input name="id" type="hidden" value="{admin_own_pages.edit_id}"></em></td>
            </tr>
            <tr>
              <td style="background:#FEF4E7">Название<span class="comment"><sup>1</sup></span>:</td>
              <td style="background:#FEF4E7"><input name="o_title" type="text" id="o_title" style="width:400px" value="{admin_own_pages.title}" maxlength="255"></td>
            </tr>
            <tr>
              <td>Имя в меню<span class="comment"><sup>1</sup></span>:</td>
              <td><input name="o_menu_name" type="text" id="o_menu_name" style="width:400px;" value="{admin_own_pages.menu_name}" maxlength="150"></td>
            </tr>
            <tr>
              <td style="width:32%;vertical-align:top;background:#FEF4E7">Текст страницы<span class="comment"><sup>1</sup></span>:<br/>
                <span class="comment">если нужно, иначе укажите URL файла<br/>или URL редиректа</span>
              </td>
              <td style="width:68%;background:#FEF4E7"><textarea name="o_text" wrap="PHYSICAL" id="o_text" style="width:400px; height:150px;">{admin_own_pages.text}</textarea></td>
            </tr>
            <tr>
              <td>URL файла:<br/><span class="comment">локальный поуть к файлу от www-root каталога (обычно public_html); напр., такой как для файла /admin/index.inc; закачивайте файлы по ftp</span></td>
              <td><input name="o_file" type="text" id="o_file" style="width:400px;" value="{admin_own_pages.file}" maxlength="255"></td>
            </tr>
            <tr>
              <td style="background:#FEF4E7">URL редиректа:<br/><span class="comment">если ссылается на другой сайт - полный (с http://), для локальных файлов может быть относительным</span></td>
              <td style="background:#FEF4E7"><input name="o_redirect" type="text" id="o_redirect" style="width:400px;" value="{admin_own_pages.redirect}" maxlength="255"></td>
            </tr>
            <tr>
              <td colspan="2"><p>Обратите внимание, что поля Текст<br/>страницы, URL файла и URL редиректа имеют приоритет в соответствии<br/>с их размещением.</p><p><span class="comment"><sup>1</sup>&nbsp;&#8212; можно использовать html-теги</span></p></td>
            </tr>
            <tr>
              <td colspan="2"><div style="text-align:center"><input name="submit" type="submit" value="Отправить »"><br/><span class="comment"></span></div></td>
            </tr>
          </table>

          <table style="width:98%;text-align:right">
            <tr style="text-align:center;background:#FF9875">
              <td style="width:5%"><b><img src="/images/menu-folder.gif" width="13" height="15"> </b></td>
              <td style="width:15%"><b>Тип</b></td>
              <td style="width:60%"><b>&nbsp;Имя в меню </b></td>
              <td style="width:10%"><strong>Действ.</strong></td>
              <td style="width:10%"><strong>Поз.</strong></td>
            </tr>
            <!-- BEGIN item -->
            <tr>
              <td style="background:{admin_own_pages.item.color};text-align:center"><a href="/admin/admin_own_pages.php?edit={admin_own_pages.item.id}"><img src="/images/menu-page.gif" width="13" height="15" title="Редактировать"></a></td>
              <td style="background:{admin_own_pages.item.color};text-align:center">{admin_own_pages.item.type}</td>
              <td style="background:{admin_own_pages.item.color}"><a href="/admin/admin_own_pages.php?edit={admin_own_pages.item.id}">{admin_own_pages.item.menu_name}</a></td>
              <td style="text-align:center;background:{admin_own_pages.item.color}">
                <a href="#" onClick="if(confirm('Вы уверены, что хотите удалить \'{admin_own_pages.item.menu_name}\'?')){window.location.href='/admin/admin_own_pages.php?delete={admin_own_pages.item.id}';}">
                  <img src="/images/icon_delete.gif" width="15" height="15" title="удалить">
                </a>
                <a href="/admin/admin_own_pages.php?{admin_own_pages.item.vis_do}={admin_own_pages.item.id}">
                  <img src="/images/icon_visible{admin_own_pages.item.vis_off}.gif" width="15" height="15" title="{admin_own_pages.item.vis_text}">
                </a>
              </td>
              <td style="text-align:center;background:{admin_own_pages.item.color}"><a href="/admin/admin_own_pages.php?move={admin_own_pages.item.id}&how=up"><img src="/images/arrow-top.gif" width="15" height="12" title="переместить выше"></a>&nbsp;<a href="/admin/admin_own_pages.php?move={admin_own_pages.item.id}&how=down"><img src="/images/arrow-bottom.gif" width="15" height="12" title="переместить ниже"></a></td>
            </tr>
            <!-- END item -->
          </table>
        </form>
        <!-- END admin_own_pages -->

        <!-- BEGIN admin_subscriber -->
        <form action="admin_subscribers.php" method="post" name="subscribeForm">
          <input name="subscribes" type="hidden" value="1">

          <ul class="list-group">
            <li class="list-group-item">
              <span>Тема рассылки</span>
              <div><input name="s_subject" type="text" class="contact form-control" value="Новости {short_name}"></div>
              <span>Текст рассылки (html формат) обязательно оставьте в конце письма подпись и обратный e-mail:</span>
              <textarea name="s_text" class="contact form-control">---<br>С уважением,<br>{short_name}.</textarea>
            </li>
          </ul>
          <span>Копия письма придёт на административный e-mail {email_address}</span>
          <div><input name="submit" type="submit" class="btn btn-primary center" value="Отправить"></div>

          <h3>Подписчики: {admin_subscriber.num}</h3>
          <table>
            <tr>
              <th style="width:12.5%">Имя</th>
              <th style="width:12.5%">E-mail</th>
              <th style="width:12.5%">Статус</th>
              <th style="width:12.5%">Дата добавления</th>
              <th style="width:12.5%">Дата подтверждения</th>
              <th style="width:12.5%">IP</th>
              <th style="width:12.5%">Подтвердить?</th>
              <th style="width:12.5%">Удалить?</th>
            </tr>
            <!-- BEGIN item -->
            <tr>
              <td>{admin_subscriber.item.name}</td>
              <td><a href="mailto:{admin_subscriber.item.mail}">{admin_subscriber.item.mail}</a></td>
              <td>{admin_subscriber.item.approved}</td>
              <td>{admin_subscriber.item.create_tm}</td>
              <td>{admin_subscriber.item.approve_tm}</td>
              <td>{admin_subscriber.item.ip}</td>
              <td><a href="#" onclick="if( confirm('Вы действительно желаете подтвердить подписчика {admin_subscriber.item.mail}?') ){window.location.href='/admin/admin_subscribers.php?confirm={admin_subscriber.item.code}'}">{admin_subscriber.item.approve}</a></td>
              <td><a href="#" onclick="if( confirm('Вы действительно желаете удалить подписчика {admin_subscriber.item.mail}?') ){window.location.href='/admin/admin_subscribers.php?del={admin_subscriber.item.code}'}">Удалить</a></td>
            </tr>
            <!-- END item -->
          </table>

        </form>
        <!-- END admin_subscriber -->

        <!-- BEGIN admin_review -->
        <h3>Отзывы: {admin_review.num}</h3>
        <table>
          <tr>
            <th style="width:85%">Модель товара, Рейтинг, Отзыв</th>
            <th style="width:5%">Имя, Телефон, E-mail, IP, Дата добавления, Дата подтверждения, Статус</th>
            <th style="width:5%">Подтвердить?</th>
            <th style="width:5%">Удалить?</th>
          </tr>
          <!-- BEGIN item -->
          <tr>
            <td title="#{admin_review.item.id}"><span style="font-weight:bold;">{admin_review.item.product} <span class="rating rating_{admin_review.item.rating}0"></span></span><br>{admin_review.item.review}</td>
            <td>{admin_review.item.name}<br>{admin_review.item.phone}<br>{admin_review.item.mail}<br>{admin_review.item.ip}<br>{admin_review.item.create_tm}<br>{admin_review.item.approve_tm}<br>{admin_review.item.approved}</td>
            <td><a href="#" onclick=" if( confirm('Вы действительно желаете подтвердить отзыв {admin_review.item.mail}?') ){window.location.href='/admin/admin_reviews.php?confirm={admin_review.item.code}'}">{admin_review.item.approve}</a></td>
            <td><a href="#" onclick=" if( confirm('Вы действительно желаете удалить отзыв {admin_review.item.mail}?') ){window.location.href='/admin/admin_reviews.php?del={admin_review.item.code}'}">Удалить</a></td>
          </tr>
          <!-- END item -->
        </table>
        <!-- END admin_review -->

        <!-- BEGIN admin_price_send -->
        <form action="admin_price_send.php" method="post" name="pricesendForm">
          <input name="price_send" type="hidden" value="1">
          <input name="sp_email" type="email" class="contact form-control" placeholder="E-mail">
          <input name="sp_comment" type="text" class="contact form-control" placeholder="Комментарий">
          <input name="sp_add" type="hidden" value="1">
          <input name="submit" type="submit" class="btn btn-primary" value="Добавить подписчика">
        </form>
        <h3>Подписчики: {admin_price_send.num}</h3>
        <table>
          <tr>
            <th style="width:33%">Имя</th>
            <th style="width:33%">E-mail</th>
            <th style="width:33%">Удалить?</th>
          </tr>
          <!-- BEGIN item -->
          <tr>
            <td>{admin_price_send.item.comment}</td>
            <td><a href="mailto:{admin_price_send.item.mail}" target="_blank">{admin_price_send.item.mail}</a></td>
            <td><a href="#" onclick="if( confirm('Вы действительно желаете удалить подписчика {admin_price_send.item.mail}?') ){window.location.href='/admin/admin_price_send.php?del={admin_price_send.item.id}';}">Удалить</a></td>
          </tr>
          <!-- END item -->
        </table>
        <form method="post">
          <input name="price_send" type="hidden" value="1">
          <input name="sendMail" type="hidden" value="1">
          <input name="sp_mail" type="submit" class="btn btn-primary" value="Сделать рассылку сейчас">
        </form>
        <!-- END admin_price_send -->

        <!-- BEGIN admin_news -->
        <form action="admin_news.php" method="post" name="newsForm">
          <input name="news" type="hidden" value="1">
          <ul class="list-group">
            <li class="list-group-item">
              <div>Дата</div>
              <input name="dates[0]" type="text" value="{admin_news.datenow}">
              <div>Текст</div>
              <textarea name="news[0]"></textarea>
              <input name="news_add" type="submit" value="Добавить новость">
            </li>
            <!-- BEGIN item -->
            <li class="list-group-item">
              <div>#{admin_news.item.id} Дата</div>
              <input name="dates[{admin_news.item.id}]" type="text" value="{admin_news.item.date}">
              <span>Удалить эту новость </span>
              <input type="checkbox" name="del[{admin_news.item.id}]" value="1">
              <div>Текст</div>
              <textarea name="news[{admin_news.item.id}]">{admin_news.item.text}</textarea>
              <input name="news_save" type="submit" value="Сохранить новость">
            </li>
            <!-- END item -->
          </ul>
        </form>
        <!-- END admin_news -->

        <!-- BEGIN admin_orders -->
        <form method="post" name="admOrdersForm" id="admOrdersForm">
          <input name="orders" type="hidden" value="1">
          <table>
            <tr>
              <td colspan="3">Показаны: {admin_orders.shown}, <a href="{admin_orders.to_show_link}">показать {admin_orders.to_show}</a></td>
            </tr>
            <tr>
              <td>ID <input name="id_order" type="hidden" id="id_order"></td>
              <td>Выполнен</td>
              <td>Наименование</td>
            </tr>
            <!-- BEGIN item -->
            <tr>
              <td width="33%" style="background:{admin_orders.item.color}">#{admin_orders.item.id}</td>
              <td width="33%" style="background:{admin_orders.item.color}">
                <!-- BEGIN add -->
                <input name="submit_order" type="submit" id="submit_order" onclick="document.admOrdersForm.id_order.value={admin_orders.item.id}" value="Принять">
                <!-- END add -->
                <!-- BEGIN done -->
                <span>Принят</span>
                <!-- END done -->
              </td>
              <td width="33%" style="background:{admin_orders.item.color}">
                (<a href="#{admin_orders.item.cat}" title="раскрыть или скрыть информацию о заказе" onclick="tmp=document.getElementById('tbl{admin_orders.item.id}'); if(tmp.style.display=='none'){tmp.style.display='';}else{tmp.style.display='none';}">раскрыть/скрыть данные</a>)
                <table id="tbl{admin_orders.item.id}" style="display:none">
                  <tr>
                    <td>
                      <p>Данные заказа:</p>
                      <ul>
                        <li>Имя: {admin_orders.item.name}</li>
                        <li>Телефон: {admin_orders.item.phone}</li>
                        <li>E-mail: {admin_orders.item.email}</li>
                        <li>Адрес доставки: {admin_orders.item.address}</li>
                        <li>Комментарий:{admin_orders.item.comment}</li>
                        <li>На сумму: {admin_orders.item.summ}</li>
                        <li>На товары: {admin_orders.item.prod}</li>
                        <li>Время отправки: {admin_orders.item.time}</li>
                      </ul>
                    </td>
                  </tr>
                </table>
              </td>
            </tr>
            <!-- END item -->
          </table>
        </form>
        <!-- END admin_orders -->

      </section>

      <footer>
        <span>&copy; 2004-2015 {copyright}</span>
      </footer>

    </div>
  </body>
</html>