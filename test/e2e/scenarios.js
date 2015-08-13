'use strict';

/* http://docs.angularjs.org/guide/dev_guide.e2e-testing */

var branch_test = true;

describe('ME App.', function() {

  var
  // controls
    inputTextSearch = element(by.model('appData.user.search_text')),
    inputTextUserName = element(by.model('appData.user.name')),
    inputTextUserPhone = element(by.model('appData.user.phone')),
    inputTextUserAddress = element(by.model('appData.user.address')),
    inputEmailUserEmail = element(by.model('appData.user.email')),
    inputTextUserReferral = element(by.model('appData.user.referral')),
    inputTextUserCaptchaValue = element(by.model('appData.user.captcha.value')),
    inputNumberMinPrice = element(by.model('appData.config.filter_minprice')),
    inputNumberMaxPrice = element(by.model('appData.config.filter_maxprice')),
    inputButtonSubmit = element(by.css('input[type="submit"]')),
    textAreaUserComment = element(by.model('appData.user.comment')),
    aRootSection = element(by.css('.head_back')),
    aMenuItemNews = element(by.cssContainingText('a', '+Новости')),
    aButtonNews = element(by.cssContainingText('a', 'Купить')),
    selectSortOrder = element(by.model('appData.config.filter_sort_order')),
    selectStock = element(by.model('appData.config.filter_stock')),
    selectCurrency = element(by.model('appData.config.filter_currency')),
    buttonCart = element(by.css('button[ng-click="mainCtrl.showCart()"]')),
    buttonCloseMsg = element.all(by.css('button[title="Закрыть"]')),
  // output
    repeaterProductsInSection = element.all(by.repeater('product in appData.section.product')),
    repeaterPagesInNavigation = element.all(by.repeater('page in appData.section.page.id')),
    repeaterLinksInNavigation = element.all(by.repeater('(level, link) in appData.navigation')),
    aNavigationLastPage = element(by.binding('appData.section.page.count')),
    spanProductCount = element.all(by.binding('appData.section.product_count')).get(0),
    urlIndex = 'http://' + (branch_test ? 'test' : 'www') + '.minskelectro.com',
    urlCategory = urlIndex + '/c~13577/fc~0/so~0/fs~0/pg~1/p1~0/p2~9999',
    urlSearch = urlIndex + '/s~iconbit/fc~0/so~0/fs~0/pg~1/p1~0/p2~9999',
    urlProduct = urlIndex + '/p~59377/fc~0/so~0/fs~0'
  ;

  describe('Index page.', function() {

    it('should get index page  ', function () {
       browser.get(urlIndex);
       expect(browser.getTitle()).toEqual(':: MinskElectro');
     });

   });


  describe('External following a link to the page. I check application title', function() {

    it('should equal to section title', function () {
      browser.get(urlCategory);
      expect(browser.getTitle()).toEqual('USB FlashDrive :: MinskElectro');
    });

    it('should equal to search title', function () {
      browser.get(urlSearch);
      expect(browser.getTitle()).toEqual('iconbit :: MinskElectro');
    });

    it('should equal to product title', function () {
      browser.get(urlProduct);
      expect(browser.getTitle()).toEqual('Car modulator Digma ESM200C :: MinskElectro');
    });

  });


  describe('Input Search', function() {

    it('should close Intro message', function() {
      buttonCloseMsg.get(0).click();
    });

    it('should show main page products list', function() {
      aRootSection.click();
      expect(repeaterProductsInSection.count()).toBe(9);
    });

    it('should count the results of the search', function() {
      inputTextSearch.clear().sendKeys('usb flashdriv').then(function(){
        // hack thats help to get last response after last request
        inputTextSearch.sendKeys('e');
      });
      expect(repeaterProductsInSection.count()).toBe(50);
      expect(spanProductCount.getText()).toBe('651');
      expect(aNavigationLastPage.getAttribute('title')).toEqual('Страница #14');
    });

  });


  describe('Section page with search results', function() {

    it('should change product list order: default (price ASC, option[value="0"])', function() {
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          firstProductOnPagePrice = product[0].element(by.binding('product.price')),
          firstProductOnPageWarranty = product[0].element(by.binding('product.warranty')),
          firstProductOnPageDeliveryTime = product[0].element(by.binding('product.delivery_time')),
          firstProductOnPageDateAdd = product[0].element(by.binding('product.date_add')),
          firstProductOnPageDateUpdate = product[0].element(by.binding('product.date_update')),
          firstProductOnPageShortDesc = product[0].element(by.binding('product.short_desc')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive SiliconPower SP004GBUF2101V1B (4Gb Helios 101 Blue) (#11774)');
        expect(firstProductOnPagePrice.getText()).toEqual('7 у.е.');
        expect(firstProductOnPageWarranty.getText()).toEqual('12 мес.');
        expect(firstProductOnPageDeliveryTime.getText()).toEqual('Пн-Сб');
        expect(firstProductOnPageDateAdd.getText()).toEqual('2010-04-30');
        expect(firstProductOnPageDateUpdate.getText()).toEqual('2013-07-06');
        expect(firstProductOnPageShortDesc.getText()).toEqual('Объём: 4GB');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive Qumo QM8GUD-CLK-Mint (8GB Click Mint) (#18460)');
      });
    });

    it('should change product list order: price DESC', function() {
      selectSortOrder.element(by.css('option[value="2"]')).click();
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPagePrice = product[0].element(by.binding('product.price')),
          lastProductOnPagePrice = product[49].element(by.binding('product.price'));
        expect(firstProductOnPagePrice.getText()).toEqual('91 у.е.');
        expect(lastProductOnPagePrice.getText()).toEqual('36 у.е.');
      });
    });

    it('should change product list order: name ASC', function() {
      selectSortOrder.element(by.css('option[value="1"]')).click();
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive A-Data AC71008GZZZRS (8GB C701) (#49384)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive Apacer AP4GAH110B-1 (4Gb) (#15117)');
      });
    });

    it('should change product list order: name DESC', function() {
      selectSortOrder.element(by.css('option[value="3"]')).click();
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive Verbatim Store-N-Go micro drive yellow (8Gb) (#24667)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive Transcend TS4GJFV15 (4GB JetFlash V15) (#3654)');
      });
    });

    it('should change product list order: date_add ASC', function() {
      selectSortOrder.element(by.css('option[value="4"]')).click();
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageDateAdd = product[0].element(by.binding('product.date_add')),
          lastProductOnPageDateAdd = product[49].element(by.binding('product.date_add'));
        expect(firstProductOnPageDateAdd.getText()).toEqual('2006-06-15');
        expect(lastProductOnPageDateAdd.getText()).toEqual('2008-11-28');
      });
    });

    it('should change product list order: date_add DESC', function() {
      selectSortOrder.element(by.css('option[value="5"]')).click();
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageDateAdd = product[0].element(by.binding('product.date_add')),
          lastProductOnPageDateAdd = product[49].element(by.binding('product.date_add'));
        expect(firstProductOnPageDateAdd.getText()).toEqual('2013-11-09');
        expect(lastProductOnPageDateAdd.getText()).toEqual('2013-09-21');
      });
    });

    it('should count the results of the search: stock = all products', function() {
      selectStock.element(by.css('option[value="1"]')).click();
      expect(spanProductCount.getText()).toBe('1209');
      expect(aNavigationLastPage.getAttribute('title')).toEqual('Страница #25');
    });

    it('should count the results of the search: out of stock', function() {
      var product_delivery_n_a = element.all(by.css('span[ng-if="product.delivery_n_a == 1"]'));
      expect(product_delivery_n_a.count()).toBe(0);
    });

    it('should count the results of the search: minprice', function() {
      inputNumberMinPrice.clear().sendKeys('1').then(function() {
        inputNumberMinPrice.sendKeys('0');
      });
      expect(spanProductCount.getText()).toBe('1042');
      expect(aNavigationLastPage.getAttribute('title')).toEqual('Страница #21');
    });

    it('should count the results of the search: maxprice', function() {
      inputNumberMaxPrice.clear().sendKeys('4').then(function() {
        inputNumberMaxPrice.sendKeys('0');
      });
      expect(spanProductCount.getText()).toBe('961');
      expect(aNavigationLastPage.getAttribute('title')).toEqual('Страница #20');
    });

    it('should change currency: USD -> BYR', function() {
      selectCurrency.element(by.css('option[value="1"]')).click();
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPagePrice = product[0].element(by.binding('product.price')),
          lastProductOnPagePrice = product[49].element(by.binding('product.price'));
        expect(firstProductOnPagePrice.getText()).toEqual('232500 р.');
        expect(lastProductOnPagePrice.getText()).toEqual('232500 р.');
      });
    });

    it('should change currency: min & max value USD -> BYR', function() {
      expect(inputNumberMinPrice.getAttribute('value')).toEqual('155000');
      expect(inputNumberMaxPrice.getAttribute('value')).toEqual('620000');
    });

    it('should check first page navigation, change 1 -> 2 and check second page', function() {
      repeaterPagesInNavigation.then(function(page) {
        expect(element(by.css('a[aria-label="Previous page"]')).getAttribute('title')).toEqual('Предыдущие 50 тов.');
        expect(element(by.css('a[aria-label="First page"]')).getAttribute('title')).toEqual('Страница #1');
        expect(page[0].getText()).toEqual('');
        expect(page[1].element(by.css('a')).getAttribute('title')).toEqual('Страница #2');
        expect(page[4].getText()).toEqual('...');
        expect(element(by.css('a[aria-label="Last page"]')).getAttribute('title')).toEqual('Страница #20');
        expect(element(by.css('a[aria-label="Next page"]')).getAttribute('title')).toEqual('Следующие 50 тов.');
        page[1].element(by.css('a')).click();
      });
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive Chieftec CEB-35S-U3 (3.5 SATA USB 3.0 Black) (#14974)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive Apacer AP16GAH130T-1 (16GB AH130 Orange) (#7403)');
      });
    });

    it('should check second page navigation, change 2 -> 3 and check third page', function() {
      repeaterPagesInNavigation.then(function(page) {
        expect(element(by.css('a[aria-label="Previous page"]')).getAttribute('title')).toEqual('Предыдущие 50 тов.');
        expect(element(by.css('a[aria-label="First page"]')).getAttribute('title')).toEqual('Страница #1');
        expect(page[5].getText()).toEqual('...');
        expect(element(by.css('a[aria-label="Last page"]')).getAttribute('title')).toEqual('Страница #20');
        expect(element(by.css('a[aria-label="Next page"]')).getAttribute('title')).toEqual('Следующие 50 тов.');
        page[2].element(by.css('a')).click();
      });
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive Apacer AP8GAH129P-1 (8GB AH129 Pink) (#7151)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive Sandisk SDCZ55-016G-B35R (16GB Cruzer Facet) (#13259)');
      });
    });

    it('should change 3 -> 5 and check fifth page navigation', function() {
      repeaterPagesInNavigation.then(function(page) {
        page[4].element(by.css('a')).click();
      });
      repeaterPagesInNavigation.then(function(page) {
        expect(element(by.css('a[aria-label="Previous page"]')).getAttribute('title')).toEqual('Предыдущие 50 тов.');
        expect(element(by.css('a[aria-label="First page"]')).getAttribute('title')).toEqual('Страница #1');
        expect(page[0].getText()).toEqual('...');
        expect(page[8].getText()).toEqual('...');
        expect(element(by.css('a[aria-label="Last page"]')).getAttribute('title')).toEqual('Страница #20');
        expect(element(by.css('a[aria-label="Next page"]')).getAttribute('title')).toEqual('Следующие 50 тов.');
      });
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive Verico VP08-08GVV1E Purple (8GB 480 Мб/сек, cкорость чтения/записи: 15МБ/с / 3МБ/с) (#23602)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive GoodRam PD16GH2GRCUBR9 (16GB Cube Blue) (#31676)');
      });
    });

    it('should change 5 -> previous page', function() {
      element(by.css('a[aria-label="Previous page"]')).click();
      repeaterPagesInNavigation.then(function(page) {
        expect(element(by.css('a[aria-label="Previous page"]')).getAttribute('title')).toEqual('Предыдущие 50 тов.');
        expect(element(by.css('a[aria-label="First page"]')).getAttribute('title')).toEqual('Страница #1');
        expect(page[7].getText()).toEqual('...');
        expect(element(by.css('a[aria-label="Last page"]')).getAttribute('title')).toEqual('Страница #20');
        expect(element(by.css('a[aria-label="Next page"]')).getAttribute('title')).toEqual('Следующие 50 тов.');
      });
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive Sandisk SDCZ55-016G-B35B Blue (16GB Cruzer Facet) (#13269)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive Emtec EKMMD8GM322 (8Gb Animals Monkey) (#17145)');
      });
    });

    it('should change 4 -> next page', function() {
      element(by.css('a[aria-label="Next page"]')).click();
      repeaterPagesInNavigation.then(function(page) {
        expect(element(by.css('a[aria-label="Previous page"]')).getAttribute('title')).toEqual('Предыдущие 50 тов.');
        expect(element(by.css('a[aria-label="First page"]')).getAttribute('title')).toEqual('Страница #1');
        expect(page[0].getText()).toEqual('...');
        expect(page[8].getText()).toEqual('...');
        expect(element(by.css('a[aria-label="Last page"]')).getAttribute('title')).toEqual('Страница #20');
        expect(element(by.css('a[aria-label="Next page"]')).getAttribute('title')).toEqual('Следующие 50 тов.');
      });
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive Verico VP08-08GVV1E Purple (8GB 480 Мб/сек, cкорость чтения/записи: 15МБ/с / 3МБ/с) (#23602)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive GoodRam PD16GH2GRCUBR9 (16GB Cube Blue) (#31676)');
      });
    });

    it('should change 5 -> first page', function() {
      element(by.css('a[aria-label="First page"]')).click();
      repeaterPagesInNavigation.then(function(page) {
        expect(element.all(by.css('a[aria-label="Previous page"]')).count()).toBe(0);
        expect(element(by.css('a[aria-label="First page"]')).getAttribute('title')).toEqual('Страница #1');
        expect(page[0].getText()).toEqual('');
        expect(page[4].getText()).toEqual('...');
        expect(element(by.css('a[aria-label="Last page"]')).getAttribute('title')).toEqual('Страница #20');
        expect(element(by.css('a[aria-label="Next page"]')).getAttribute('title')).toEqual('Следующие 50 тов.');
      });
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive Goodram PD16GH2GRCOMXR9 (16GB Colour Mix) (#3591)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive Mirex 13600-FMUCRR16 (CHROMATIC RED 16GB) (#19967)');
      });
    });

    it('should change 1 -> last page', function() {
      element(by.css('a[aria-label="Last page"]')).click();
      repeaterPagesInNavigation.then(function(page) {
        expect(element(by.css('a[aria-label="Previous page"]')).getAttribute('title')).toEqual('Предыдущие 50 тов.');
        expect(element(by.css('a[aria-label="First page"]')).getAttribute('title')).toEqual('Страница #1');
        expect(page[15].getText()).toEqual('...');
        expect(element(by.css('a[aria-label="Last page"]')).getAttribute('title')).toEqual('Страница #20');
        expect(element.all(by.css('a[aria-label="Next page"]')).count()).toBe(0);
      });
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[10].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive Transcend TS2GJFV10 (2GB JetFlash V10) (#40838)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive Transcend TS2GJFV30 (2GB JetFlash V30) (#33486)');
      });
      expect(repeaterProductsInSection.count()).toBe(11);
    });

    it('should change last page -> previous', function() {
      element(by.css('a[aria-label="Previous page"]')).click();
      repeaterPagesInNavigation.then(function(page) {
        expect(element(by.css('a[aria-label="Previous page"]')).getAttribute('title')).toEqual('Предыдущие 50 тов.');
        expect(element(by.css('a[aria-label="First page"]')).getAttribute('title')).toEqual('Страница #1');
        expect(page[14].getText()).toEqual('...');
        expect(element(by.css('a[aria-label="Last page"]')).getAttribute('title')).toEqual('Страница #20');
        expect(element(by.css('a[aria-label="Next page"]')).getAttribute('title')).toEqual('Следующие 11 тов.');
      });
      repeaterProductsInSection.then(function(product) {
        var
          firstProductOnPageName = product[0].element(by.binding('product.name')),
          lastProductOnPageName = product[49].element(by.binding('product.name'));
        expect(firstProductOnPageName.getText()).toEqual('USB FlashDrive Kingston DT101C/2GB (2GB DataTraveler) (#53636)');
        expect(lastProductOnPageName.getText()).toEqual('USB FlashDrive Corsair CMFUSB2.0-16Gb (16Gb Flash Voyager) (#41570)');
      });
    });

  });


  describe('Menu item News and button Buy', function() {

    it('should open menu News page', function() {
      aMenuItemNews.click();
    });

    it('should open Feedback page', function() {
      aButtonNews.click();
    });

  });


  describe('Page Feedback', function() {

    it('should fill Feedback and submit', function() {
      expect(inputButtonSubmit.getAttribute('disabled')).toEqual('true');
      inputTextUserName.sendKeys('Robot Protractor');
      inputTextUserPhone.sendKeys('1234567');
      inputEmailUserEmail.sendKeys('noreply@email.com');
      textAreaUserComment.sendKeys('Robot tries to submit form');
      inputTextUserCaptchaValue.sendKeys('1234');
      expect(inputButtonSubmit.getAttribute('disabled')).toBeNull();
      inputButtonSubmit.click();
    });

    it('should check Feedback Captcha Failed message', function() {
      element(by.css('button[ng-click="mainCtrl.previousState()"]')).click();
      expect(element.all(by.css('span[ng-bind-html="msg.content"]')).get(0).getText()).toEqual('1. Неправильно введены цифры с картинки!');
      buttonCloseMsg.get(0).click();
    });

  });


  describe('Page Cart', function() {

    it('should add product, open cart, fill form and submit', function() {
      browser.get(urlIndex);
      buttonCloseMsg.get(0).click();
      buttonCloseMsg.get(0).click();

      repeaterProductsInSection.then(function(products) {
        products[6].element(by.css('input')).sendKeys('1');
      });
      expect(element.all(by.css('span[ng-bind-html="msg.content"]')).get(0).getText()).toEqual('Товар добавлен в корзину. Чтобы перейти к оформлению, необходимо кликнуть на изображение корзины в самом верху.');
      buttonCloseMsg.get(0).click();
      buttonCart.click();
      element(by.css('input[name="count0"]')).sendKeys('5');
      expect(inputButtonSubmit.getAttribute('disabled')).toEqual('true');
      inputTextUserName.clear().sendKeys('Robot Protractor');
      inputTextUserPhone.clear().sendKeys('1234567');
      inputTextUserAddress.clear().sendKeys('http://www.protractortest.org/');
      inputEmailUserEmail.clear().sendKeys('noreply@email.com');
      inputTextUserReferral.clear().sendKeys('www.google.com');
      textAreaUserComment.clear().sendKeys('Robot tries to submit form');
      inputTextUserCaptchaValue.clear();
      expect(inputButtonSubmit.getAttribute('disabled')).toEqual('true');
      inputTextUserCaptchaValue.clear().sendKeys('9876');
      expect(inputButtonSubmit.getAttribute('disabled')).toBeNull();
      inputButtonSubmit.click();
    });

    it('should check response of submit, open cart, kill item and check response', function() {
      expect(element.all(by.css('span[ng-bind-html="msg.content"]')).get(0).getText()).toEqual('1. Неправильно введены цифры с картинки!');
      buttonCloseMsg.get(0).click();
      buttonCart.click();
      element.all(by.css('button[title="Удалить из корзины"]')).click();
      expect(element.all(by.css('span[ng-bind-html="msg.content"]')).get(0).getText()).toEqual('Корзина пуста. Для оформления заказа необходимо добавить в корзину товар!');
      buttonCloseMsg.get(0).click();
    });

  });

  describe('Check menu and navigation path', function() {

    it('should open service centers menu, than section menu', function() {
      element(by.cssContainingText('a', '+Серв. центры')).click();
      element(by.cssContainingText('a', 'Память (HDD,Flash...)')).click();
    });

    it('should open link from subSections Select, than Memory CF level of navigation', function() {
      element(by.cssContainingText('option', 'CompactFlash (CF)')).click();
      repeaterLinksInNavigation.then(function(links) {
        links[1].element(by.css('a')).click();
      });
    });

    it('should open link of main section of navigation', function() {
      element(by.cssContainingText('a', 'Главная')).click();
    });

  });

  describe('Page Review', function() {

    it('should add product, open cart and fill form', function() {
      repeaterProductsInSection.then(function(products) {
        products[3].element(by.css('a.rating')).click();
      });
      expect(inputButtonSubmit.getAttribute('disabled')).toEqual('true');
      inputTextUserName.clear().sendKeys('Robot Protractor');
      inputTextUserPhone.clear().sendKeys('1234567');
      inputEmailUserEmail.clear().sendKeys('noreply@email.com');
      textAreaUserComment.clear().sendKeys('Robot tries to submit form');
      element(by.css('a[title="Нормальный"]')).click();
      inputTextUserCaptchaValue.clear();
      expect(inputButtonSubmit.getAttribute('disabled')).toEqual('true');
      inputTextUserCaptchaValue.clear().sendKeys('5678');
      expect(inputButtonSubmit.getAttribute('disabled')).toBeNull();
      inputButtonSubmit.click();
      expect(element.all(by.css('span[ng-bind-html="msg.content"]')).get(0).getText()).toEqual('1. Неправильно введены цифры с картинки!');
      buttonCloseMsg.get(0).click();
    });

  });

  describe('Page Easteregg, than Me Page', function() {

    it('should open easteregg', function() {
      browser.actions().sendKeys('easteregg').perform();
      element(by.css('a[title="Об авторе"]')).click();
    });

  });


  xdescribe('Page Error', function() {

    it('should select the text', function() {
      browser.actions().sendKeys(protractor.Key.ESCAPE).perform();
      webdriver.executeScript(function () {
        var range = document.createRange();
        range.selectNode(document.body.querySelector('p[ng-bind-html="product.short_desc"]').firstChild);
        var sel = window.getSelection();
        sel.removeAllRanges();
        sel.addRange(range);
        return sel;
      });
      browser.actions().keyDown(protractor.Key.CONTROL).keyDown(protractor.Key.ENTER).perform();
    });

  });

});