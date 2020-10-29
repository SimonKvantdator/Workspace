var flag = true;

function toggle() {
  if (flag) {
    browser.tabs.executeScript({code:'document.body.style.background = "#404040";'});
    flag = false;
  } else {
    browser.tabs.executeScript({code: 'document.body.style.background = "white";'});
    flag = true;
  }
}

browser.browserAction.onClicked.addListener(toggle);






