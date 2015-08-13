exports.config = {
  allScriptsTimeout: 11000,

//  seleniumAddress: 'http://localhost:4444/wd/hub',
  specs: [
    'e2e/*.js'
  ],
/*
  capabilities: {
    'browserName': 'chrome'
  },
*/
  multiCapabilities: [{
    browserName: 'firefox'
  }, {
    browserName: 'chrome'
  }],

  chromeOnly: true,

  baseUrl: 'http://localhost:8000/',

  framework: 'jasmine',

  onPrepare: function() {
    //Disable animations so e2e tests run more quickly
    var disableNgAnimate = function() {
      angular.module('disableNgAnimate', []).run(function($animate) {
        $animate.enabled(false);
      });
    };
    browser.addMockModule('disableNgAnimate', disableNgAnimate);
  },

  jasmineNodeOpts: {
    defaultTimeoutInterval: 30000
  }
};
