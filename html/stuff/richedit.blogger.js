/*
 * Copyright, Google.com - 2004
 * Author: Chris Wetherell
 */

function BloggerRichEditor() {  
  this.ALLOW_HTML_ENTRY = true;
  this.ALLOW_LINK_ONLY_PASTE = true;
  this.PREVIEW_IS_HIDDEN = true;

  if (editorModeDefault) {
    this.START_MODE = editorModeDefault; // set in EditPost.gxp
  }
  
  this.frameBodyStyle = "border:0;margin:0;padding:3px;width:auto;"
    + "font:normal 100% Georgia, serif;";
  
  this.KEY_COMMANDS.push(
    ['CTRL_SHFT_P', null, 'toggle(e);'], 
    ['CTRL_SHFT_S', null, 'd(Posting.PUBLISH_BUTTON).click();'],
    ['CTRL_SHFT_D', null, 'd(Posting.DRAFT_BUTTON).click();'],
    ['CTRL_S', null, 'd(Posting.PUBLISH_BUTTON).click();'],
    ['CTRL_D', null, 'd(Posting.DRAFT_BUTTON).click();']
  );
  
  this.CONTROLS = [
    ['FontName', null, null, false, this.FONTS],
    ['FontSize', null, null, false, this.FONT_SIZES],
    ['|'],
    
    ['Bold', 'Bold', 'gl.bold', true],
    ['Italic', 'Italic', 'gl.italic', true],
    ['|'],
    
    ['ForeColor', 'Text Color', 'gl.color.fg', true],
    ['|'],
    
    ['CreateLink', 'Link', 'gl.link', true],
    ['|'],
    
    ['JustifyLeft', 'Align Left', 'gl.align.left', true],
    ['JustifyCenter', 'Align Center', 'gl.align.center', true],
    ['JustifyRight', 'Align Right', 'gl.align.right', true],
    ['JustifyFull', 'Justify Full', 'gl.align.full', true],
    ['|'],
    
    ['InsertOrderedList', 'Numbered List', 'gl.list.num', true],
    ['InsertUnorderedList', 'Bulleted List', 'gl.list.bullet', true],
    ['Blockquote', 'Blockquote', 'gl.quote', true],
    ['|']
  ];
    
  if (userCanSpellcheck) {
    this.CONTROLS.push(
      ['SpellCheck','Check Spelling','gl.spell', true, null, 'spellcheck();'],
      ['|']);
  }

  if (userCanUploadPhotos) {
    this.CONTROLS.push(['Add_Image','Add Image','gl.photo', true, null, '', 'addImage();'],
      ['|']);
  }

  if (userCanUploadFiles) {
      this.CONTROLS.push(['Upload_File','Upload File','gl.file', true, null, '', 'uploadFile();'],
      ['|']);
  }

  this.CONTROLS.push(['RemoveFormat', 'Remove formatting from selection', 'gl.clean', true],
    ['|'],
    ['PreviewAction','Preview', null, true, null, 'toggle();']);

  if (userCanSpellcheck) {
    this.HTML_CONTROLS.push(
      ['spellcheck();','Check Spelling','gl.spell'],
      ['|']);
  }

  if (userCanUploadPhotos) {
    this.HTML_CONTROLS.push(['','Add Image','gl.photo', '', 'addImage();'], 
    ['|']);
  }

  if (userCanUploadFiles) {
    this.HTML_CONTROLS.push(['','Upload File','gl.file', '', 'uploadFile();'], 
    ['|']);
  }

  this.HTML_CONTROLS.push(['toggle();','Preview', null, 'PreviewAction']);
  
  var showPostOptions = 'showElement(d(Posting.OPTIONS));';
  
  this.APPEND_TO_ACTIONS = [
    ['ShowSourceEditor', 'hidePreview();\n' + showPostOptions, true],
    ['ShowRichEditor', 'hidePreview();\n' + showPostOptions, true]
  ];
  
  this.DEBUG = false;
  this.DEBUG_FRAME_KEY_EVENTS = false;
  this.DEBUG_CHECK_FORMATTING = false;
}

BloggerRichEditor.prototype = new RichEditor();


function BloggerPreviewOnlyEditor()
{    
  this.MODE_TABS = [];
  this.ENABLE_IFRAME = false;
  this.PREVIEW_IS_HIDDEN = true;
  this.START_MODE = this.HTML_MODE;
  
  this.HTML_CONTROLS = [];

  if (userCanSpellcheck) {
    this.HTML_CONTROLS.push(['spellcheck();','Check Spelling','gl.spell'],
    ['|']);
  }

  if (userCanUploadPhotos) {
    this.HTML_CONTROLS.push(['addImage();','Add Image','gl.photo'], ['|']);
  }
  if (userCanUploadFiles) {
    this.HTML_CONTROLS.push(['uploadFile();','Upload File','gl.file'], 
    ['|']);
  }
  this.HTML_CONTROLS.push(['toggle();','Preview', null, 'PreviewAction']);  

  this.ENABLE_KEYBOARD_CONTROLS = false; 
  this.DEBUG = false;  
}
BloggerPreviewOnlyEditor.prototype = new RichEditor();

function getKeyCommandsHtml() {
  var html = [];
  html.push('<div class="clear"></div>\n');  
  html.push('For keyboard shortcuts, \n');
  html.push('press <em><strong>Ctrl</strong></em> with: ');
  html.push('<em>B</em> = <b>Bold</b>,&nbsp;&nbsp;');
  html.push('<em>I</em> = <b>Italic</b>,&nbsp;&nbsp;');
  html.push('<em>S</em> = <b>Publish</b>,&nbsp;&nbsp;');
  html.push('<em>D</em> = <b>Draft</b>&nbsp;&nbsp;');
  html.push('&nbsp;&nbsp;<em><a target="_blank" '
    + 'href="http://help.blogger.com/bin/answer.py?answer=880">'
    + 'more &raquo;</a>');
  html.push('<div class="clear"></div>  \n');
  return html.join('');
}

/*
 * setBloggerEditor()
 *
 * Sets the editor based on what browser supports which feature set.
 * Requires: Detect() object from detect.js
 */
function setBloggerEditor() { 
  if (Detect.IE_5_5_newer() || Detect.MOZILLA()) { 
    setRichTextEditor();
    /*
     * Mozilla needs the frame body to have some value before certain 
     * operations can take place.
     */
    if (RichEdit.frameDoc.body.innerHTML == "" && Detect.MOZILLA()) {
      RichEdit.frameDoc.body.innerHTML = IFRAME_INIT_MESSAGE;
    }
  } else if (Detect.SAFARI() || Detect.OPERA()) { 
    setPreviewOnlyEditor();
  } 
}

/*
 * setRichTextEditor()
 *
 * Creates the Blogger WYSIWYG editor interface.
 */
function setRichTextEditor() {
  new BloggerRichEditor().make();
  NestlingFormFields();
  var div = document.createElement('div');
  div.id = 'key_commands';
  div.innerHTML = getKeyCommandsHtml();
  RichEdit.editarea.appendChild(div);
}

/*
 * setPreviewOnlyEditor()
 *
 * Creates an editor interface that has a Preview function on the 
 * mode bar that works with Safari 1.2- and Opera.  Ideally, it should be the 
 * "Edit HTML" textarea interface including all of its formatting functions 
 * including key commands.
 */
function setPreviewOnlyEditor() {
  new BloggerPreviewOnlyEditor().make();
  d('RichEdit').style.marginTop = '1em';
}

/*
 * NestlingFormFields()
 *
 * Adjusts the form fields' positions to align more snugly with the tabbed
 * WYSIWYG editor.
 */
function NestlingFormFields() {
  if (Detect.IE()) {
    var titles = d('titles');
    var editor = d('RichEdit');
        
    // re-position the title fields, since IE requires this area to be 
    // absolutely positioned in order for them to be placed as a layer 
    // above the editor
    var pos = getXY(editor);
    titles.style.position = "absolute";
    titles.style.zIndex = "1";
    titles.style.left = pos.x + "px";
    titles.style.top = (pos.y - titles.offsetHeight -10) + "px";
    
    // the titles table can have a varying number of rows, and thus vary in 
    // height.  The editor needs to be set accordingly.
    titleHeight = titles.offsetHeight;
    if (titleHeight > 40) {
      titleHeight -= 35;
    } else {
      titleHeight = 0;
    }
    editor.style.paddingTop = titleHeight;
    
  } else {
    d('RichEdit').style.marginTop = '-20px';
  }
}


// ===================================================================
// Keyboard commands for the document area OUTSIDE of the WYSIWYG editor
// ===================================================================

document.onkeypress = function(e) { 
  var e = getEvent(e);
  
  setKeysetByEvent(e); 
  
  if (CTRL_SHFT_P) toggle();
  if (CTRL_S || CTRL_SHFT_S) d(Posting.PUBLISH_BUTTON).click();
  if (CTRL_D || CTRL_SHFT_D) d(Posting.DRAFT_BUTTON).click(); 
  
  return true;
}



// ===================================================================
// Form utilites (Posting page only)
// ===================================================================

/**
 * setFocus()
 *
 * Sets the focus to the first available form field of the editor.
 */
function setFocus()
{ 
  // ensure that keypresses don't submit form or prevent tabbing
  setPostingFormEvents();
  
  var title = d(Posting.TITLE);
  var url = d(Posting.URL)
  var editor = d(RichEdit.frameId);

  if (title)  {
    if (Detect.IE_5_5_newer()) d(RichEdit.frameId).tabIndex = 2;
    title.focus();
  }
  else if (url) {
    if (Detect.IE_5_5_newer()) d(RichEdit.frameId).tabIndex = 3;
    url.focus();
  }
  else if (editor) {
    if (Detect.IE_5_5_newer()) editor.tabIndex = 1;
    RichEdit.frame.contentWindow.focus();
  }
}

function setPostingFormEvents() {
  if (d(Posting.URL)) {    
    d(Posting.URL).onkeypress = OnPostingFormKeypress;    
  }
  if (d(Posting.TITLE)) {
    d(Posting.TITLE).onkeypress = OnPostingFormKeypress;
  }
}

function OnPostingFormKeypress(e) { 
  var evt = getEvent(e);
  // prevent pressing Enter on a single-text-field form from submitting
  if (getKey(evt) == RETURN) {
    if (Detect.IE()) evt.returnValue = false;
    return false;
  }
  // Mozilla has a different way of focusing within designMode window objects.
  if (Detect.MOZILLA() && RichEdit != undefined) {
    var src = getEventSource(evt);
    if (!isShiftKeyPressed(evt)) { // allow for back tabbing
      if (src.id == Posting.URL) {   
        MozillaTabToFrameFix(evt);
      } else if ((src.id == Posting.TITLE) && !d(Posting.URL)) { 
        MozillaTabToFrameFix(evt);
      }
    }
  }
}

// So that Firefox 0.7+ can tab to the IFRAME...
function MozillaTabToFrameFix(evt) {
  setKeysetByEvent(evt);
  if (TAB && RichEdit.mode == RichEdit.DESIGN_MODE) {
    var html = RichEdit.frameDoc.body.innerHTML;
    /*
     * Mozilla needs the frame body to have some value before certain 
     * operations can take place.
     */
    if (html = "" || html == "<br>") {
      RichEdit.frameDoc.body.innerHTML = IFRAME_INIT_MESSAGE;
    }
    setRichEditFocus();
    evt.preventDefault();
  }
}

// add an image to a post
function addImage() {
  prefs = 'toolbar=0,scrollbars=1,location=0,statusbar=1,menubar=0,' + 
        'resizable=1,width=770,height=400,top=150';
        
  function savePostToWYSIMWYG(html) {
    if (html) setPost(html + getPost());
  }

  // put this logic in Environs...
  var isDev = location.href.match('.corp.google.com');
  var isTest = location.href.match('bla');
  var imagesHost = (isDev) ? '' : 
                   (isTest) ? 'http://bla186.prod.google.com/' : 
                   'http://photos.blogger.com/';
                   
  crossDomainPopup(imagesHost + 'upload-image.g?blogID=' + document.stuffform.blogID.value, 
                   'photocookie', savePostToWYSIMWYG, 250, prefs).timer();
}

/*
 * Popup function that will close the window if it's already
 * open so that the new window is guaranteed to come out on top.
 */
function popUp(URL, preferences) {
  if (window.popup) { 
    try {
      window.popup.close();
    } catch(e) {}
  }

  if (!preferences) {
    preferences = 'toolbar=0,scrollbars=1,location=0,statusbar=1,' + 
    'menubar=0,resizable=1,width=490,height=400,top=150';
  }
  
  window.popup = window.open(URL, 'bloggerPopup', preferences);
}

/**
 * A generic way to access data across subdomains. We popup a form
 * in another subdomain and monitor a shared cookie with an interval
 * timer. Once the window is closed we place the cookie 
 * value into the node provided. Usually this node is a form variable 
 * or somewhere else explicit although that's not required. After the 
 * window is closed, we remove the interval timer. 
 *
 * url        The url to load.
 * cookie     The name of the cookie to store the data in.
 * setContent A function to store the value in once the cookie is populated.
 *            This can be a simple wrapper around setting a node.value in a 
 *            posting form, etc.
 * [freq]     The amount of time to run the interval on in millis. 
 *            Defaults to 250.
 * [prefs]    The prefs to the load the url with.
 */
function crossDomainPopup(url, cookie, setContent, freq, prefs) { 
  var id;
  BloggerImages = new Object();
  BloggerImages.win = window.open(url, 'bloggerPopup', prefs);
  BloggerImages.setContent = setContent;
  BloggerImages.cookie = cookie;
  BloggerImages.timer = function() {
    id = setInterval(this.checkClosed, (freq) ? freq : 250);
  }

  BloggerImages.checkClosed = function() {
    if (BloggerImages.win.closed) {
      clearInterval(id);
      BloggerImages.setContent(getCookie(BloggerImages.cookie));
    }
  }
  return BloggerImages;
}


/*
 * cleanupDeletion()
 *
 * Since we can't get the selection of certain elements with certainty, we 
 * have to cleanup extraneous markup downstream on occasion.
 */
BloggerRichEditor.prototype.cleanupDeletion = function() {
  var tags = this.frameDoc.body.getElementsByTagName('font');
  for (var i = 0; i < tags.length; i++) {
    var el = tags[i];
    // cleanup Blogger Images markup
    this.removeEmptyImageElements(el);
  }
}

/* 
 * removeEmptyImageElements()
 *
 * If the given element is a Blogger Image container, then the node is 
 * removed while its content is retained.
 */
BloggerRichEditor.prototype.removeEmptyImageElements = function(obj) {
  if (!obj) return;
  var imgTagPattern = new RegExp('<img', 'i'); 
  if (obj.className == 'blogger-image' && !obj.innerHTML.match(imgTagPattern)) {
    
    // remove the last node if it's the carriage return that was inserted for
    // Firefox
    if (Detect.MOZILLA()) {
      var last_node = obj.childNodes[obj.childNodes.length-1];
      obj.removeChild(last_node);
    }
    
    // if there's content within the old container, insert it to a new node 
    // in order to cleanup the styles before removing the container
    var DELETE_ID = 'TEMP-RICHEDIT-DELETE';
    if (obj.innerHTML.length > 0) {  
      
      // make a temporary new node in the IFRAME
      var new_node = this.frameDoc.createElement('font');
      new_node.id = DELETE_ID;
    
      // transfer the content of the image container to a new container
      new_node.innerHTML = obj.innerHTML;
      obj.parentNode.insertBefore(new_node, obj);
      
    }  
    
    // remove the old, empty image container
    obj.parentNode.removeChild(obj);
    
    // cleanup unneeded elements
    if (this.frameDoc.getElementById(DELETE_ID)) {
      
      // remove the temp element but keep its content
      var deletePattern = new RegExp('<font id="' + DELETE_ID + '">' + 
        new_node.innerHTML + '</font>', 'i'); 
        
      this.frameDoc.body.innerHTML = 
        this.frameDoc.body.innerHTML.replace(deletePattern, new_node.innerHTML);
      
      // occasionally a stray empty temp element can remain (especially in IE)
      // and should be removed  
      var deletePattern2 = new RegExp('<font id="' + DELETE_ID + 
        '"></font>', 'i');
      
      this.frameDoc.body.innerHTML = 
        this.frameDoc.body.innerHTML.replace(deletePattern2, '');
      
    }
  }
}


/*
 * Fix persistent image selection bug in Mozilla 
 */
function deselectBloggerImageGracefully() {
  try {
    var re = RichEdit;
    hideElement(re.frame);
    showElement(re.frame);
    re.frameDoc.designMode='On';
    var html = re.frameDoc.body.innerHTML;
    setRichEditFocus();
    re.frameDoc.body.innerHTML = html;
  } catch(e) {}
}
