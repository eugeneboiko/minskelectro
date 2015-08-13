<!DOCTYPE html>
<html ng-app="shopModule" ng-controller="MainController as mainCtrl" lang="ru">
  <head>
    <meta charset="utf-8">

    <!-- for IE8+ -->
    <meta http-equiv="X-UA-Compatible" content="IE=edge">

    <!-- for mobile devices -->
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <meta name="description" content="Интернет-магазин бытовой техники и компьютерных комплектующих">
    <meta name="author" content="Eugene V. Boiko">
    <meta name="keywords" content="{page_name} интернет-магазин каталог">
    <meta name="robots" content="noimageindex">
    <title ng-bind="appData.config.title + ' :: MinskElectro'">{page_name}</title>
    <link rel="shortcut icon" href="favicon.ico">

    <!-- for adaptive design -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap{test_branch_min}.css">

    <link rel="stylesheet" href="/css/me{test_branch_min}.css" type="text/css">

    <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
    <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv{test_branch_min}.js"></script>
    <script src="https://oss.maxcdn.com/libs/respond.js/1.3.0/respond{test_branch_min}.js"></script>
    <![endif]-->

    <!-- Google analytics -->
    <script>
      (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
      (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
      m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
      })(window,document,'script','//www.google-analytics.com/analytics.js','ga');
      ga('create', 'UA-62558546-1', 'auto');
      ga('send', 'pageview');
    </script>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/angular.js/1.4.1/angular{test_branch_min}.js"></script>
    <!-- inserts HTML into DOM -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/angular.js/1.4.1/angular-sanitize{test_branch_min}.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/angular.js/1.4.1/angular-animate{test_branch_min}.js"></script>
    <script src="/js/me{test_branch_min}.js"></script>

    <!-- for Angular $location -->
    <base href="/">
  </head>

  <body>
    <div class="container">

      <header>

        <img src="/images/throbber-progress.gif"
             height="32"
             width="32"
             alt="Выполняется запрос к серверу"
             class="ng-hide ajax_progress"
             ng-show="appData.user.ajax_progress"
             title="Выполняется запрос к серверу">

        <menu>
          <li><a href="#" ng-click="mainCtrl.showNews()">+Новости</a></li>
          {menu}
        </menu>

        <div class="well text-center">
          <span><img src="/images/velcom.gif" alt="Velcom" title="Velcom" height="15" width="16">xxxxxxx (Vel),</span>
          <span><img src="/images/mts.gif" alt="МТС" title="МТС" height="16" width="16">xxxxxxx (МТС),</span>
          <span><img src="/images/life.gif" alt="Life :)" title="Life :)" height="16" width="17">xxxxxxx (Life),</span>
          <span><img src="/images/icq_alive.jpg" alt="ICQ" height="16" width="16">#xxxxxxxx,</span>
          <span><img src="/images/skype.jpg" alt="Skype" title="Skype" height="16" width="16">xxxxxxxxxxxx,</span>
          <a href="#" class="ng-hide" ng-show="true" ng-click="mainCtrl.showFeedback()" title="Отправить письмо">
            <span><img src="/images/mainmenu-feedback.gif" alt="Отправить письмо" width="17" height="16">e-mail</span>
          </a>
        </div>

        <a href class="head_back" ng-click="mainCtrl.getSection(1)" title="На главную">

          <img src="/images/top_line_738x150.jpg" alt="" class="header1 img-flex">

          <table class="header2">
            <tr>
              <td><img src="/images/line_top_left.jpg" alt="" height="150"></td>
              <td class="structure"></td>
              <td><img src="/images/line_top_right.jpg" alt="" height="150"></td>
            </tr>
          </table>

        </a>

        <div class="head_front">

          <div class="ng-hide input-group head_shop_search" ng-show="true">
            <input type="search"
                   ng-model="appData.user.search_text"
                   ng-model-options="{debounce: 1000}"
                   class="form-control"
                   aria-label="Live search"
                   ng-change="appData.section.page.current = 1;mainCtrl.getSearch(appData.user.search_text)"
                   autocomplete="off"
                   placeholder="Введите текст для поиска">
            <span class="input-group-addon" ng-bind="appData.section.product_count" ng-if="mainCtrl.isCurrentState('search')" title="Найдено товаров"></span>
          </div>

          <div ng-hide="true" class="input-group alert alert-danger center" role="alert">Для полной функциональности этого сайта необходимо включить JavaScript.</div>

        </div>
      </header>

      <section>

        <div class="text-center">
          <button class="btn btn-primary ng-hide" ng-show="mainCtrl.calcOrderProductsCount()" ng-click="mainCtrl.showCart()">
            <span>Товаров в корзине: </span>
            <span ng-bind="mainCtrl.calcOrderProductsCount()"></span>
          </button>
        </div>

        <div class="well well-sm ng-hide"
             ng-bind-html="appData.config.page_main_center_info"
             ng-if="appData.config.page_main_center_info && appData.section.id == 1"
             ng-show="true">
        </div>

        <!-- If use *.tpl Chrome shows Warning: Request Headers: Provisional headers are shown-->
        <div ng-switch on="appData.state[appData.state.length - 1]">
          <div ng-switch-when="me">
            <section ng-include="'/tpl/me_me_en.html'"></section>
          </div>
          <div ng-switch-when="review">
            <section ng-include="'/tpl/me_messages.html'"></section>
            <section ng-include="'/tpl/me_review.html'"></section>
          </div>
          <div ng-switch-when="feedback">
            <section ng-include="'/tpl/me_messages.html'"></section>
            <section ng-include="'/tpl/me_feedback.html'"></section>
          </div>
          <div ng-switch-when="error">
            <section ng-include="'/tpl/me_messages.html'"></section>
            <section ng-include="'/tpl/me_error.html'"></section>
          </div>
          <div ng-switch-when="easteregg">
            <section ng-include="'/tpl/me_easteregg.html'"></section>
          </div>
          <div ng-switch-when="news">
            <section ng-include="'/tpl/me_news.html'"></section>
          </div>
          <div ng-switch-when="cart">
            <section ng-include="'/tpl/me_messages.html'"></section>
            <section ng-include="'/tpl/me_cart.html'"></section>
          </div>
          <div ng-switch-when="product">
            <section ng-include="'/tpl/me_navigation.html'"></section>
            <section ng-include="'/tpl/me_messages.html'"></section>
            <section ng-include="'/tpl/me_product.html'"></section>
          </div>
          <div ng-switch-when="section">
            <section ng-include="'/tpl/me_navigation.html'"></section>
            <section ng-include="'/tpl/me_messages.html'"></section>
            <section ng-include="'/tpl/me_section.html'"></section>
          </div>
          <div ng-switch-when="search">
            <section ng-include="'/tpl/me_navigation.html'"></section>
            <section ng-include="'/tpl/me_messages.html'"></section>
            <section ng-include="'/tpl/me_section.html'"></section>
          </div>
          <div ng-switch-default></div>
        </div>

        <!-- for non-support JS browsers and search engines -->
        <div ng-if="false">
          <div class="center-text">
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
          <menu>
            <!-- BEGIN item -->
            <li><a href="/c~{sub_cat.item.id}" title="Подраздел {sub_cat.item.title}">{sub_cat.item.name}</a></li>
            <!-- END item -->
          </menu>
          <!-- END sub_cat -->

          <!-- BEGIN section_description -->
          <div class="breadcrumb" title="Описание секции">{section_description.description}</div>
          <!-- END section_description -->

          <!-- BEGIN pgs -->
          <div class="btn-group center" role="group" aria-label="Pages">
            <!-- BEGIN item -->
            <!-- BEGIN item1 -->
            <button type="button" class="btn btn-default" title="Страница #{pgs.item.item1.num}">
              <a href="{pgs.link}{pgs.item.item1.num}">{pgs.item.item1.num}</a>
            </button>
            <!-- END item1 -->
            <!-- BEGIN item2 -->
            <button type="button" class="btn btn-default" title="Страница #{pgs.item.item2.num}">{pgs.item.item2.num}</button>
            <!-- END item2 -->
            <!-- END item -->
            <button type="button" class="btn btn-default">{pgs.prev_next}</button>
          </div>
          <!-- END pgs -->

          <ul class="list-group">
            <!-- BEGIN product_item -->
            <li class="list-group-item">
              <div class="media">
                <div class="media-left">
                  <a href="/p~{product_item.id}">
                    <img class="media-object" src="{product_item.image}" ng-if="product.icon" width="100" height="100" alt="подробнее...">
                  </a>
                </div>
                <div class="media-body">
                  <h4 class="media-heading">
                    <a href="/p~{product_item.id}" title="#{product_item.id}">{product_item.name} (#{product_item.id})</a>
                  </h4>
                  <p>
                    <span>Цена: <span class="label label-primary">{product_item.price}</span></span>
                    <span>Гарантия: <span class="label label-default">{product_item.warranty} мес.</span></span>
                    <span>Доставка: <span class="label label-default">{product_item.delivery_time}</span></span>
                    <span>Добавлен: <span class="label label-default">{product_item.date_add}</span></span>
                    <span>Обновлен: <span class="label label-default">{product_item.date_update}</span></span>
                    <span class="label label-danger">{product_item.in_stock}</span>
                  <div class="label label-success">{product_item.delivery_free}</div>
                  </p>
                  <p>{product_item.short_desc}</p>
                </div>
              </div>
            </li>
            <!-- END product_item -->
          </ul>

          <!-- BEGIN product -->
          <div class="panel panel-default">
            <div class="panel-body">
              <div class="media">
                <div class="media-left">
                  <img class="media-object" src="{product.image}" title="{product.title}" alt="{product.title}">
                </div>
                <div class="media-body">
                  <h4 class="media-heading" title="#{product.id}">{product.name} (#{product.id})</h4>
                  <p>
                    <span>Цена: <span class="label label-primary">{product.price}</span></span>
                    <span>Гарантия: <span class="label label-primary">{product.warranty} мес.</span></span>
                    <span>Доставка: <span class="label label-primary">{product.delivery_time}</span></span>
                    <span>Добавлен: <span class="label label-primary">{product.date_add}</span></span>
                    <span>Обновлен: <span class="label label-primary">{product.date_update}</span></span>
                    <span class="label label-danger">{product.in_stock}</span>
                  <div class="label label-success">{product.delivery_free}</div>
                  </p>
                  <p>{product.description}</p>
                </div>
              </div>
            </div>
          </div>
          <!-- END product -->

          {page_bottom_info}
        </div>
      </section>

      <footer class="panel panel-default">
        <div class="panel-body">
          <a href="#" ng-click="mainCtrl.showMe()" alt="Об авторе" title="Об авторе">&copy; 2004-2015 {copyright}</a>
          <span class="soc">
            <a href="#" ng-click="mainCtrl.popupSoc(1)" title="Vkontakte" class="icon-vk-12"></a>
            <a href="#" ng-click="mainCtrl.popupSoc(2)" title="Одноклассники.ru" class="icon-ok-12"></a>
            <a href="#" ng-click="mainCtrl.popupSoc(3)" title="livejournal" class="icon-lj-12"></a>
            <a href="#" ng-click="mainCtrl.popupSoc(4)" title="Twitter" class="icon-twitter-12"></a>
            <a href="#" ng-click="mainCtrl.popupSoc(5)" title="Facebook" class="icon-facebook-12"></a>
            <a href="#" ng-click="mainCtrl.popupSoc(6)" title="Я.ру" class="icon-yaru-12"></a>
            <a href="#" ng-click="mainCtrl.popupSoc(7)" title="Мой Мир" class="icon-moimir-12"></a>
            <a href="#" ng-click="mainCtrl.popupSoc(8)" title="Мой Круг" class="icon-moikrug-12"></a>
          </span>
        </div>
      </footer>

    </div>
  </body>
</html>