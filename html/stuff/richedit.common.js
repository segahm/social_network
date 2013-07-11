/*
 * Copyright, Google.com - 2004
 * Author: Chris Wetherell
 * 
 * Note: The data arrays are based on a WYSIWSYG example by Bay-Wei Chang.
 */


/*
 * RichEditor (object)
 *
 */
function RichEditor()
{
  
  // ===================================================================
  // Settings
  // ===================================================================
  
  this.IMAGES_LOCATION = "/img/"; 
  this.DEFAULT_ALIGNMENT = "left";
  this.EDIT_SOURCE = true; // if false, the mode tabs are never exposed
  this.ENABLE_IFRAME = true; // if false, the IFRAME is never appended
  this.ENABLE_KEYBOARD_CONTROLS = true; 
  this.ALLOW_HTML_ENTRY = false;
  this.ALLOW_FULL_PASTE = true;
  this.ALLOW_LINK_ONLY_PASTE = false;
  this.DEBUG = false;
  this.frameBodyStyle = "border:0;margin:0;padding:3px;width:auto;"
    + "font:normal 100%/120% Georgia, serif;";  
  //above line adds CSS notation to the body of the IFRAME
  
  
  
  // ===================================================================
  // Design Mode Toolbar Data
  // ===================================================================
  // First of each pair is the display name, second is the value.
  // If name ends in asterisk, then it is the default (otherwise first one is default.)
  // If value is null, then it is same as display name.
  // Keep values lowercase for compatibility between IE and Moz.
  this.FONTS = [
    ['Font', ''], 
    ['Arial', 'arial'], 
    ['Courier', 'courier new'],
    ['Georgia', 'georgia'], 
    ['Lucida Grande', 'lucida grande'], 
    ['Times', 'times new roman'],
    ['Trebuchet', 'trebuchet ms'], 
    ['Verdana', 'verdana'],
    ['Webdings', 'webdings']
  ];
  
  this.FONT_SIZES = [
    ['Huge', '5'], 
    ['Large', '4'], 
    ['Normal Size*', '3'], 
    ['Small', '2'], 
    ['Tiny', '1']
  ];  
  
  this.BACKCOLOR = Detect.IE() ? 'BackColor' : 'HiliteColor';  
  
  // [ExecCommand, tooltip, icon, depressable]
  // If tooltip is null, means same as ExecCommand name.
  // menus have an extra elements: [..., optionarray].
  // ['|'] is a vertical separator.
  // NOTE: The "RemoveFormat" control does NOT work on Firefox 0.8 though it 
  // does work on versions 0.9 and 0.91.
  this.CONTROLS = [    
    ['FontName', null, null, false, this.FONTS],
    ['FontSize', null, null, false, this.FONT_SIZES],
    ['|'],
    
    ['Bold', 'Bold', 'gl.bold', true],
    ['Italic', 'Italic', 'gl.italic', true],
    ['|'],
    
    ['ForeColor', 'Text Color', 'gl.color.fg', true],
    [this.BACKCOLOR, 'Background Color', 'gl.color.bg', true],
    ['|'],
    
    ['CreateLink', 'Link', 'gl.link', true],
    ['|'],
    
    ['JustifyLeft', 'Align Left', 'gl.align.left', true],
    ['JustifyCenter', 'Align Center', 'gl.align.center', true],
    ['JustifyRight', 'Align Right', 'gl.align.right', true],
    ['|'],
    
    ['InsertOrderedList', 'Numbered List', 'gl.list.num', true],
    ['InsertUnorderedList', 'Bulleted List', 'gl.list.bullet', true],
    ['Outdent', 'Indent Less', 'gl.indent.less', true],
    ['Indent', 'Indent More', 'gl.indent.more', true],
    ['|'],
    ['RemoveFormat', 'Remove formatting from selection', 'gl.clean', true],
    ['|']
    ];
  
  
  this.HTML_CONTROLS = [   
    ['Textbar.Bold();', 'insert bold tags', 'gl.bold'],
    ['Textbar.Italic();', 'insert italic tags', 'gl.italic'],
    ['|'],
    
    ['Textbar.Link();', 'insert link', 'gl.link'],
    ['|'],
    
    ['Textbar.Blockquote();', 'insert blockquote', 'gl.quote'],
    ['|']    
    ];
  
  this.MODE_TABS = [];
  if (this.EDIT_SOURCE) {
    this.MODE_TABS.push(['ShowRichEditor', 'RichEdit.ShowRichEditor()', 
      'Compose', 'this.START_MODE == this.DESIGN_MODE']);
    this.MODE_TABS.push(['ShowSourceEditor', 'RichEdit.ShowSourceEditor()', 
      'Edit HTML', 'this.START_MODE == this.HTML_MODE']); 
  }
  
  this.DEPRESSABLE = ["Bold", "Italic", "JustifyLeft", "ForeColor", this.BACKCOLOR, 
    "JustifyCenter", "JustifyRight", "InsertOrderedList", "InsertUnorderedList", 
    "CreateLink", "Indent", "Outdent", "Blockquote"];
  
  
  this.KEY_COMMANDS = [];
  this.KEY_COMMANDS.push(['CTRL_SHFT_A', 'CreateLink', 'CreateLink()']);
  this.KEY_COMMANDS.push(['CTRL_B', 'Bold', 
    'RichEdit.frameDoc.execCommand("Bold", false, "")']);
  this.KEY_COMMANDS.push(['CTRL_I', 'Italic', 
    'RichEdit.frameDoc.execCommand("Italic", false, "")']);
  this.KEY_COMMANDS.push(['CTRL_Z', null, 
    'RichEdit.frameDoc.execCommand("Undo", false, "")']);
  // add redo for non-IE browsers.  IE has it by default.
  if (!Detect.IE()) {
    this.KEY_COMMANDS.push(['CTRL_Y', null, 
      'RichEdit.frameDoc.execCommand("Redo", false, "")']);
  }
  // support this common key mapping of redo as well
  // TODO: doesn't work for IE
  this.KEY_COMMANDS.push(['CTRL_SHFT_Z', null, 
    'RichEdit.frameDoc.execCommand("Redo", false, "")']);
  
  
  // ===================================================================
  // Properties
  // ===================================================================
  this.id = "RichEdit"; //the name of this object within the window
  this.divId = "RichEdit"; //the name of the container surrounding the textarea
  this.frameId = "richeditorframe"; 
  this.debugField;
  this.UNSUPPORTED_MODE = 0;
  this.DESIGN_MODE = 1;
  this.HTML_MODE = 2;
  this.START_MODE = this.DESIGN_MODE;  // start in WYSIWYG mode by default
  this.mode;
  
  // ===================================================================
  // The make() method
  // ===================================================================
  this.make = function() { 
    
    this.div = document.getElementById(this.divId);
    if (!this.div) return; 
		
		// store this user-defined object in a globally accessible
		// variable that is unique to the rich editor object
    eval("window."+this.id+" = this");
    
    // get the textarea within the rich editor
    
    this.textarea_orig = this.div.getElementsByTagName('textarea')[0];
    
    // create palette container
    
    this.palette = createElementandAppend('div', 'palette');
    
    // create style container for the bars and editable areas, copy the
    // textarea into this container, and delete the original textarea
    
    this.editarea = createElementandAppend('div', 'editarea', this.div);
    appendClearObj(this.editarea);
    this.textarea = this.textarea_orig.cloneNode(true);
    
    if (Detect.SAFARI() || Detect.OPERA()) {
      // cloneNode isn't capturing node attributes or values for some browsers
      this.textarea.value = this.textarea_orig.value;
      this.textarea.name = this.textarea_orig.name;
      this.textarea.id = this.textarea_orig.id;
      
      // If we don't set the unique attributes, Safari sees dups between the 
      // clone and the original, even if the original is removed!
      this.textarea_orig.id = 'tmp-fix-for-Safari-ID-dups'; 
      this.textarea_orig.name = 'tmp-fix-for-Safari-ID-dups'; 
    }
    
    this.editarea.appendChild(this.textarea);
    this.div.removeChild(this.textarea_orig);
    
    // create toolbars container
    
    this.richbars = createElementandInsertBefore
      ('div', 'richbars', this.editarea, this.textarea);   
    this.richbars.setAttribute('unselectable','on');
    appendClearObj(this.richbars);
    
    // create Design Mode toolbar
    
    this.formatbar = this.createToolbar('formatbar', 
      this.getRichBarButtonHTML());
    
    // create HTML Mode toolbar
    if (this.EDIT_SOURCE) { 
      this.htmlbar = this.createToolbar('htmlbar', this.getHtmlBarButtonHTML());
      
      // hide the html bar if started in design mode
      if (this.START_MODE == this.DESIGN_MODE) {
        this.htmlbar.style.display = "none";
      }
      
      // set listener for the HTML Source mode
      if (this.ENABLE_KEYBOARD_CONTROLS) {
        if (Detect.IE()) {
          // the key codes aren't captured correctly for IE if called during keypress
          this.textarea.onkeydown = Textbar.activateKeyCommands;
        } else { 
          this.textarea.onkeypress = Textbar.activateKeyCommands;
        }
      }
      
      // Set the textarea to be used for keyboard event capture
      Textbar.Element = this.textarea;
    }
    
    // hide the rich bar if not started in design mode
    if (this.START_MODE != this.DESIGN_MODE) {
      this.formatbar.style.display = 'none';
    } 
          
    appendClearObj(this.richbars);      
    
    // hide textarea, if started in design mode
    if (this.START_MODE != this.HTML_MODE) {
      this.textarea.style.display = "none";   
    }
    
    // add editor frame to page
    if (this.ENABLE_IFRAME) {
      var iframe = createElementandAppend('iframe', 'richeditorframe', 
        this.editarea);
      if (Detect.OPERA()) iframe.src = 'serverid';

      // get all objects needed to transform the frame
      
      this.frame = getIFrame(this.frameId);
      this.frameWin = this.frame.contentWindow;
      this.frameDoc = getIFrameDocument(this.frame);
      
      // turn on rich-text editing for the frame ... for IE
      // (which has to be BEFORE its body is set)
      
      if (Detect.IE()) this.frameDoc.designMode = "On";
      
      // write the body, or else the frame's Document object can't be acted upon.
      
      setIFrameBody(this.frame, this.frameBodyStyle + 'text-align:' 
        + this.DEFAULT_ALIGNMENT);
      
      // Now, IE needs a different traversal method to affect the IFRAME and BODY
      // and one HTML container to prevent <p> tags from being inserted after 
      // each carriage return.
      if (Detect.IE()) {
        this.frame = getElement(this.frameId);
        this.frameDoc = this.frame.contentWindow.document;
        this.frameDoc.body.innerHTML = "<div></div>";
      }
      
      // add event handling
      
      //Now, IE needs a different traversal method to set IFRAME events
      var iframe = (Detect.IE()) ? getIFrame(this.frameId) : this.frame;
      setIFrameEvent(iframe, "click", CheckFormatting);      
      
      if (this.ENABLE_KEYBOARD_CONTROLS) {
        if (Detect.IE()) {
          // the key codes aren't captured correctly for IE if called during keypress
          setIFrameEvent(iframe, "keydown", this.activateKeyCommands);
        } else { 
          setIFrameEvent(iframe, "keypress", this.activateKeyCommands);
        }
      }
      
      if (!this.ALLOW_FULL_PASTE) { 
        this.frameDoc.body.onbeforepaste = cleanPaste;
      }
      if (this.ALLOW_LINK_ONLY_PASTE) { 
        this.frameDoc.body.onbeforepaste = smartPaste;
      }
      
      if (this.APPEND_TO_ACTIONS) {
        for (var x = 0; x < this.APPEND_TO_ACTIONS.length; x++) {
          var prepend = false;
          if (this.APPEND_TO_ACTIONS[x][2]) prepend = true;
          this.appendToAction(this.APPEND_TO_ACTIONS[x][0], 
                              this.APPEND_TO_ACTIONS[x][1],
                              prepend);
        }
                              
      }
      
      
      // turn on rich-text editing for the frame ... for Mozilla
      // (which has to be AFTER its body is set, and any time after the display 
      //  style property is changed.)
      
      if (!Detect.IE()) this.frameDoc.designMode = "On";
      
      
      // hide iframe, if started in html mode
      if (this.START_MODE != this.DESIGN_MODE) {
        this.frame.style.display = "none";   
      }
      
    } // end of ENABLE_IFRAME block
    
    this.makeModeBar();
      
    // For Debug Mode
    if (this.DEBUG) {
      var debugtitle = createElementandAppend
        ('div', 'debugtitle', this.editarea); 
      debugtitle.innerHTML = 'Debug output:';
      this.debugField = createElementandAppend
        ('textarea', 'debug', this.editarea); 
    }
    
    //move content from textarea to IFRAME
    if (this.START_MODE == this.DESIGN_MODE) {
      this.mode = this.DESIGN_MODE;
      this.ShowRichEditor();
      // remember to fill IE with a div to prevent double-spacing
      if (Detect.IE() && this.frameDoc.body.innerHTML == '') {
        this.frameDoc.body.innerHTML = '<div></div>';
      }
    } else {
      this.mode = this.HTML_MODE;
    }
    
    document.close();
    
  }
  
}

function CorrectLinkAutoCompletionInImages() {
  
  if (!Detect.IE()) return; // only IE currently has hyperlink auto-completion
  
  var parent = getSelectedParentNode();
  
  // if typed into the WYSIWYG edit area, auto-complete the end of the certain
  // tags and bypass IE's auto-complete feature, so that a link doesn't wind up
  // in the href or src attributes 
  // (i.e. <img src="<a href="http://p/">http://p/</a>">)
  try {
    var httpImgPattern = new RegExp('&lt;img[^>]+src="[^"]+"$', 'gi');
    var httpAPattern = new RegExp('&lt;a[^>]+href="[^"]+"$', 'gi');
    var isImg = httpImgPattern.test(parent.innerHTML);  
    var isAnchor = httpAPattern.test(parent.innerHTML);
    if (isImg || isAnchor) {
      var event = getRichEditorEventForIE();
      var shiftPressed = isShiftKeyPressed(event);
      var key = getKey(event);
      
      // auto-complete the end of the tag if the user enters a space after
      // the src attribute
      if (key == 32) {
        var endTag = (isImg) ? ' \/>' : '>';
        parent.innerHTML += endTag;
      }
    }
  } catch(e) {
    RichEdit.addDebugMsg('CorrectLinkAutoCompletionInImages() failed. \n');
    return true;
  }

}

function RemoveLinksWithinTags(html) {
  var httpPattern 
    = new RegExp('&lt;img([^>]+)src=["]*(<a href="[^"]+">)([^<]+)</a>["]*', 'gi');
  if (httpPattern.test(html)) {
    html = html.replace(httpPattern, '&lt;img$1src="$3"');
  }
  var httpPattern 
    = new RegExp('&lt;a([^>]+)href=["]*(<a href="[^"]+">)([^<]+)</a>["]*', 'gi');
  if (httpPattern.test(html)) {
    html = html.replace(httpPattern, '&lt;a$1href="$3"');
  }
  return html;
}

RichEditor.prototype.appendToAction = function(methodName, func, prepend) {
  var strFunc = eval('this.'+methodName).toString();
  if (prepend) {
    var funcPattern = new RegExp('function[ ]?\\(\\) \\{', 'gi');
    var newFunc = strFunc.replace(funcPattern, 'function () {\n' + func + '\n');
  } else {
    var newFunc = strFunc.replace(/}[\n\t\s]*$/g, func + '\n}');
  }
  eval('RichEditor.prototype.' + methodName + ' = ' + newFunc);
}

RichEditor.prototype.makeModeBar = function() {
  
  if (this.MODE_TABS.length == 0) return;
  
  this.moderow = createElementandInsertBefore('div', 'modebar', this.div, 
    this.editarea);  
  
  if (Detect.IE()) {
    var min = createElementandAppend('div', 'minwidth-mode', this.moderow);
    min.className = 'minwidth';
  }

  if (!Detect.IE()) appendClearObj(this.moderow);
  
  for (var i = 0; i < this.MODE_TABS.length; i++) {
    var el = createElementandAppend('span', this.MODE_TABS[i][0], this.moderow);
    
    eval('el.onclick = function() {if (this.className != "on") '+this.MODE_TABS[i][1]+'}');
    
    // runs a test to see which mode is active
    if (eval(this.MODE_TABS[i][3])) el.className = 'on';

    el.onmouseover = function() {
      if (this.className!='on') this.style.backgroundColor='#fff';
    }

    el.onmouseout = function() {this.style.backgroundColor='';}
    
    //prevent the text from being selected
    el.onselectstart = function() {return false;}
    el.onmousedown = function() {return false;}
    
    el.innerHTML = this.MODE_TABS[i][2];
  }
  
  if (!Detect.IE()) appendClearObj(this.moderow);

}

RichEditor.prototype.clearDebugMsg = function() {
  if (this.DEBUG) {
    this.debugField.value = '';
  }
}

RichEditor.prototype.addDebugMsg = function(s) {
  if (this.DEBUG) {
    this.debugField.value += (s + '\n\n');
  }
}

RichEditor.prototype.createToolbar = function(strId, html) {
  var bar = createElementandAppend('div', strId, this.richbars);
  bar.setAttribute('unselectable','on');
  appendClearObj(bar);
  bar.innerHTML = html; 
  appendClearObj(bar);
  return bar;
}

function appendClearObj(obj) {
  var div = document.createElement('div');
  var divStyle = div.style;
  divStyle.clear = "both";
  obj.appendChild(div);
}


// ===================================================================
// Copy-n-paste 
// ===================================================================

var hiddenPaste, currentEditorSelection;

function smartPaste() { 
  currentEditorSelection = GetRange();
  hiddenPaste = createElementandAppend('div', 'smartPaste');
  hiddenPaste.style.height = 1;
  hiddenPaste.style.width = 1;
  hiddenPaste.style.overflow = "hidden";
  hiddenPaste.onpaste = waitToPaste;
  hiddenPaste.contentEditable = true;
  hiddenPaste.focus();
}

function waitToPaste() { 
  setTimeout('convertPaste()', 100);
}

function convertPaste() {
  var html = hiddenPaste.innerHTML;
  
  // strip very bad HTML that IE may add after paste.  most of this occurs when
  // someone is pasting hand-coded HTML and IE performs its auto-completion for
  // URLs so that the result is something very ugly like:
  // &lt;img src="<A href='http://p/">http://p/</a'>&gt;foo&lt;/a</A>
  html = RemoveLinksWithinTags(html);
  var horridAnchorStartPattern = new RegExp("<A href=[^<]+<[\\/]?a'>", 'gi');
  var horridAnchorEndPattern = new RegExp("\\&lt;\\/a<\\/A>", 'gi');
  var html = html.replace(horridAnchorStartPattern, '');
  var html = html.replace(horridAnchorEndPattern, '&lt;/a');
  html = StripHTMLExceptLinks(html);
  
  // get rid of line feeds
  html = html.replace(/\r/g, '');
  // compress repeated line breaks
  html = html.replace(/\n{2,}/g, '\n');
  // convert line breaks
  html = html.replace(/\n/g, "<br />");
  
  // paste into the editor at the last cursor position
  currentEditorSelection.pasteHTML(html);
  currentEditorSelection.select();
}

function cleanPaste() {
  var content = window.clipboardData.getData('Text');
  if (content) content = content.replace(/\&nbsp;/gi, ' ');
  window.clipboardData.clearData();
  window.clipboardData.setData('Text', content);
}

function StripHTMLExceptLinks(html) {
  // make a private encoding pattern for tag delimiters
  var strTag = '~~~!#';
  var endTag = '#!~~~';
  var strTagPattern = new RegExp(strTag, 'gi');
  var endTagPattern = new RegExp(endTag, 'gi');
  
  // save the anchor tags
  html = html.replace(/<a ([^>]*)>/gi, strTag + 'a $1' + endTag);
  html = html.replace(/<\/a>/gi, strTag + '\/a' + endTag);
  
  // erase all other HTML
  html = html.replace(/(<([^>]+)>)/gi, ""); 
  
  // restore the anchor tags
  html = html.replace(strTagPattern, '<');
  html = html.replace(endTagPattern, '>');
  
  return html;
}
