// Copyright 2004 Google, Inc.

// A script that provides provides storage for a single string, backed by
// cookies.  
//
// The maximum string size is specified by:
// backupCookieMaxLen * backupCookieMaxNum. 
//
// backupCookieMaxLen should not exceed 4KB due to browser limitations.
// backupCookieMaxNum should be set conservatively to avoid running into
// domain specific cookie limits.

var backupCookieMaxLen = 640;
var backupCookieMaxNum = 5;
var backupCookieTime = 1800000; // 30 minutes
var backupCookieBaseName = "autoSaveCookie";
var backupBlogIDCookieBaseName = "backupBlogId";
var backupInterval = 2000; // in milliseconds
var backupLargerThan = 9;  // num of chars, important for IE
var backupLimit = 12000;    // character length, for strict web servers
var backupTimer;

function setBackupCookie(num, data, exp) {
  document.cookie = backupCookieBaseName + num + "=" + data +
                    "; expires=" + exp +
                    "; path=/";
  document.cookie = backupBlogIDCookieBaseName + "=" + 
                    document.stuffform.blogID.value +
                    "; expires=" + exp +
                    "; path=/";
}

// Start auto-save and generate its UI
function startAutoSave() {
  
  // add the HTML element for recovery
  setAutoSaveHtml();
  
  // Start auto-save and backup the post every x milliseconds
  backupTimer = setInterval('backupPost();', backupInterval);
}

function setAutoSaveHtml() {
  var div = document.createElement('div');
  div.id = 'recover';
  var span = document.createElement('span');
  span.innerHTML = 'Recover post';
  span.onclick = function() { 
    if (confirm("This will replace the current post with an older saved " + 
                "version. Continue?")) {
      setPost(loadBackupData()); 
    }
  }
  div.appendChild(span);
  d('richbars').appendChild(div);
}

function backupPost() {
  var post = getPost();
  // Don't auto-save very small posts. This could be delayed, unintended input.
  if (post.length <= backupLargerThan) return;
  // Prevent large content from surpassing content length limits on web servers
  if (post.length >= backupLimit) {
    post = post.substring(0, backupLimit);
  }
  clearBackupData();
  setBackupData(post);
}

// Get the post body.
function getPost() {
  if (RichEdit.mode == RichEdit.DESIGN_MODE) { 
    if (Detect.IE()) {
      return RemoveLinksWithinTags(getDesignModeHtml());
    } else {  
      return getDesignModeHtml();
    }
  }      
  if (RichEdit.mode == RichEdit.HTML_MODE) { 
    return getElement(Preview.TEXTAREA).value;
  }
}

// Set the editor text.
function setPost(data) {  
  if (RichEdit) { 
    getElement(Preview.TEXTAREA).value = data;
  }
  if (RichEdit.mode == RichEdit.DESIGN_MODE) { 
    RichEdit.ShowRichEditor();
  }    
}

// Store a string.
function setBackupData(unescapedData) {
  var cookieIdx = 0;
  var cookieNum = 0;

  var data = escape(unescapedData);

  while ((cookieIdx < data.length) && (cookieNum < backupCookieMaxNum)) {
    var cookieData = data.substring(cookieIdx, cookieIdx + backupCookieMaxLen);

    var date = new Date();
    date.setTime(date.getTime() + backupCookieTime);
    var cookieExp = date.toGMTString();
    setBackupCookie(cookieNum, cookieData, cookieExp);

    cookieIdx += backupCookieMaxLen;
    cookieNum++;
  }
}

// Clear the stored string.
function clearBackupData() {
  var date = new Date();
  date.setTime(0);
  var pastExp = date.toGMTString();
  
  for (var i = 0; i < backupCookieMaxNum; i++) {
    setBackupCookie(i, "", pastExp);
  }
}

// Load the stored string.
function loadBackupData() {
  var curCookie = document.cookie;
  var numParts = 0;
  var parts = new Array(backupCookieMaxLen); 
  var cookieParts = curCookie.split("; ");  
  var blogHasCookie = false;

  for (var i = 0; i < cookieParts.length; i++) {
    keyvalue = cookieParts[i].split("=");
    key = keyvalue[0];
    value = keyvalue[1];
    
    // Is this cookie for this blog?
    if (key == backupBlogIDCookieBaseName) { 
      if (value == document.stuffform.blogID.value) {
        blogHasCookie = true;
      }
    }

    if (key.substr(0, backupCookieBaseName.length) == backupCookieBaseName) {
      var whichPart = key.substr(backupCookieBaseName.length, 
                                 key.length - backupCookieBaseName.length);
      if (whichPart < backupCookieMaxNum) {
        parts[whichPart] = value;
        numParts++;
      }
    }
  }

  // Stop if cookie is empty or is not recoverable for this blog
  if (numParts == 0 || !blogHasCookie) {
    return "";
  }

  var data = "";

  for (var i = 0; i < numParts; i++) {
    if (parts[i] != null) {
      data = data + parts[i];
    } else {
      return "";
    }
  }

  return unescape(data);
}
