/*
 * Copyright, Google.com - 2004
 * Author: Chris Wetherell
 *
 * Note: The CONTROLS management functions on this page 
 * are based on a WYSIWYG example by Bay-Wei Chang.
 *
 * Array iteration uses the long form syntax in the hopes that this 
 * script will eventually support IE 5 for the Mac.  This may be considered
 * out-of-scope later and may be modified.
 */


/*
 * getRichBarButtonHTML()
 *
 * Returns the HTML needed for the toolbar when the editor is in WYSIWYG mode.
 * This function uses an array from the RichEditor object that represents a map 
 * of each button to its label and event handling.
 */
RichEditor.prototype.getRichBarButtonHTML = function()
{  
  var CONTROLS = RichEdit.CONTROLS;
  var html = [];
  for (var i = 0; i < CONTROLS.length; i++) {
    var ctrl = CONTROLS[i];
    var cmd = ctrl[0];
    
    if (cmd == '|') {
      
      html.push(getBarSeparator());

    } else if (ctrl.length == 5) {

      // menu      
      html.push('<select id="' + cmd +
      '" onclick="HidePalette()" onchange="FormatbarMenu(this, ' + i + ')">');

      var menuitems = ctrl[4];

      for (var j = 0; j < menuitems.length; j++) {
        var optionname = menuitems[j][0];
        var optionvalue = menuitems[j][1];
        var selected = '';
        if (optionname.charAt(optionname.length - 1) == '*') {
          optionname = optionname.substring(0, optionname.length - 1);
          selected = ' selected';
        }
        html.push('<option value="' + optionvalue + '"' + selected + '>' 
          + optionname + '</option>');
      }

      html.push('</select>\n\n');

    } else {

      if (hasColorPalette(cmd)) {
        var onclick = "SelectColor(this,'" + cmd + "')";
      } else { 
        var onclick = "FormatbarButton('" + this.frameId + "', this, " + i + ")";
      }
      
      if (ctrl.length >= 6) onclick = ctrl[5];
      var onmouseup = ''; if (ctrl.length >= 7) onmouseup = ctrl[6];
      
      var img = '';
      var imgName = ctrl[2];
      if (imgName) {
        var img = this.IMAGES_LOCATION + imgName + '.gif';
      }
      html.push(makeToolbarButton(ctrl[1], onclick + ';',
        img, 'formatbar_' + cmd, onmouseup));
    }
  }

    return html.join('');
  }




/*
 * getHtmlBarButtonHTML()
 *
 * Returns the HTML needed for the toolbar when the editor is in "edit HTML 
 * source" mode. This function uses an array from the RichEditor object that 
 * represents a map of each button to its label and event handling.
 */
RichEditor.prototype.getHtmlBarButtonHTML = function()
{     
  var CONTROLS = RichEdit.HTML_CONTROLS;
  var html = [];
  for (var i = 0; i < CONTROLS.length; i++) {
    var ctrl = CONTROLS[i];
    if (ctrl[0] == '|') {
      html.push(getBarSeparator());
    } else {
      var img = '';
      var imgName = ctrl[2];
      if (imgName) {
        var img = this.IMAGES_LOCATION + imgName + '.gif';
      }
      var onmouseup = ''; if (ctrl.length >= 5) onmouseup = ctrl[4];
      var button =  makeToolbarButton(ctrl[1], ctrl[0],
        img, 'htmlbar_'+ctrl[3], onmouseup);
      html.push(button);          
    }
  }
  return html.join('');
}


/*
 * makeToolbarButton()
 *
 * Returns the HTML needed for buttons for the RichEditor() toolbar.
 */
function makeToolbarButton(title, onclick, imgLocation, strId, onmouseup) {
  var img = title;
  if (imgLocation) {
    img = '<img src="' + imgLocation + '"'
    + ' alt="' + title + '" border="0" />';
  } 
  return '<span'
  + ' id="' + strId + '"'
  + ' title="' + title + '"'
  + ' onmouseover="ButtonHoverOn(this);"'
  + ' onmouseout="ButtonHoverOff(this);"'
  + ' onmouseup="' + onmouseup + '"'
  + ' onmousedown="CheckFormatting(event);' + onclick + 'ButtonMouseDown(this);"'
  + '>'
  + img
  + '</span>\n';
}


/*
 * getBarSeparator()
 *
 * Returns the HTML needed for the separator for any RichEditor toolbar.
 */
function getBarSeparator() {
  return '<div class="vertbar">'
    + '<span class="g">&nbsp;</span><span class="w">&nbsp;</span>'
    + '</div>\n';
}



// ===================================================================
// Toolbar Event Handling (General)
// ===================================================================

/*
 * hasColorPalette()
 *
 * Determines if an object's ID indicates that its involved with palettes.
 */
function hasColorPalette(strId) {
  if (strId == null) return false;
  return (strId.indexOf("Color") > 0);
}
function ButtonHoverOn(obj) {
  if (obj.className != "down") obj.className = "on";
}
function ButtonHoverOff(obj) {
  if (obj.className != "down") obj.className = "";
}
function ButtonMouseDown(obj) {
  var ctrl; 
  // retrieve the control name
  if (obj.id) ctrl = obj.id.replace(/formatbar\_/g, '');
  var DEPRESSABLE = RichEdit.DEPRESSABLE;
  
  // is the button part of the DEPRESSABLE array?
  for (var i = 0; i < DEPRESSABLE.length; i++) {
    if (DEPRESSABLE[i] == ctrl) {
      // make the button look depressed
      obj.className = (obj.className == "down") ? "" : "down";
      RichEdit.addDebugMsg('toggle button -- ['+obj.className+', '+ctrl+']\n');
      
      // only one justify button can be depressed at any time
      ClearOtherJustify(ctrl);
      
      // return the focus to the page, but pause for ol' Mozilla, who'll
      // generate an error if we move too fast
      setTimeout('setRichEditFocus()', '100');
      return true;
    }
  }
}


// ===================================================================
// Toolbar Event Handling (Design Mode)
// ===================================================================

var FORMATTING_FAILED_MSG = 'An attempt to modify formatting failed '
+ 'unexpectedly. A possible '
+ 'solution may be to save your post as a draft and reopen this '
+ 'post and apply formatting again.';

function FormatbarMenu(el, ctrlid) {
  var ctrl = RichEdit.CONTROLS[ctrlid][0];
  var val = el.options[el.selectedIndex].value;
  try {
    RichEdit.frameDoc.execCommand(ctrl, false, val);
  } catch (e) {
    alert(FORMATTING_FAILED_MSG + '\n' + e);
  }
  
  setRichEditFocus();
  
}

function FormatbarButton(frameId, button, ctrlid) { 
  var cmd = RichEdit.CONTROLS[ctrlid][0]; 
  var idoc = RichEdit.frameDoc;
  try {
    if (cmd == "CreateLink") {
      CreateLink();
    } else if (cmd == "Blockquote") {
      RichEdit.Blockquote();
    } else if (cmd == "Strikethrough") {
      RichEdit.Strikethrough();
    } else if (cmd == "RemoveFormat") {
      RichEdit.RemoveFormat();
    } else {  
      idoc.execCommand(cmd, false, "");
    }
    RichEdit.addDebugMsg('-- RichEdit Formatting Button pressed: '+cmd);
    
  } catch (e) { 
    alert(FORMATTING_FAILED_MSG + '\n' + e);
  }
}


/*
 * Blockquote()
 *
 * Surrounds the current selection with a BLOCKQUOTE tag.
 */
RichEditor.prototype.Blockquote = function() {
  this.formatSelection('blockquote');
}


/*
 * Strikethrough()
 *
 * Surrounds the current selection with a STRIKE tag.
 */
RichEditor.prototype.Strikethrough = function() {
  this.formatSelection('strike');
}


/*
 * RemoveFormat()
 *
 * Attempts to remove formatting from a selection while preserving line breaks.
 */
RichEditor.prototype.RemoveFormat = function() {
  var rangeNode = getRangeAsDocumentFragment(GetRange());  
  if (!rangeNode) return;
  
  //remove the non-line-break nodes and collect them into an array
  var arrStrippedNodes = [];
  this.stripFormatNodesFromSelection(rangeNode, arrStrippedNodes);
  
  // put the new node collection into an inline-level element
  var strippedFragment = document.createElement('span');
  for (var x = 0; x < arrStrippedNodes.length; x++) {
    // IE starts with an extra <br> tag we can ignore
    if (Detect.IE() && x==0 && arrStrippedNodes[x].nodeName=="BR") continue;
    strippedFragment.appendChild(arrStrippedNodes[x]);
  }
  
  // replace the current IFRAME selection with the new node collection
  var frameWin = document.getElementById(this.frameId).contentWindow;  
  insertNodeAtSelection(frameWin, 'span', strippedFragment);
  
  // let the WYSIWYG library remove whatever this function missed
  this.frameDoc.execCommand("RemoveFormat", false, "");
}



/*
 * formatSelection()
 *
 * Surround the selection in the WYSIWYG IFrame with the specified tag.
 */
RichEditor.prototype.formatSelection = function(tagName) { 
  var parent = getSelectedParentNode();
  
  // if it's already there, remove it.
  if (parent) {
    var hasTagAsParent = parent.nodeName.toUpperCase() == tagName.toUpperCase();
    if (hasTagAsParent) {
      adoptGrandchildrenFromParent(this.frameDoc, parent);
      return;
    }
  } 
  
  // or add the tag if it's not there already.
  var frame = document.getElementById(this.frameId);
  surroundFrameSelection(frame, tagName);
}


/*
 * stripFormatNodesFromSelection()
 *
 * Removes formatting nodes from a selection but preserves their values and
 * line breaks in the supplied array.
 */
RichEditor.prototype.stripFormatNodesFromSelection = function(parent, 
                                                              arrReturnNodes) {
  var doc = document;
  var children = parent.childNodes;
  // iterate through all of the child nodes
  for (var x = 0; x < children.length; x++) {
    var child = children[x];
    // null? leave.
    if (!child) continue;
    
    var type = child.nodeType; 
    
    // if it's a text node, add its value to the array
    if (type == TEXT_NODE)  {
      arrReturnNodes.push(child);
      continue;
    }
    
    // however, if it's an element node...
    if (type == ELEMENT_NODE) {
      var childName = child.nodeName.toUpperCase();
      
      // ...and a regular carriage return, add a <br> node to the array
      if (childName == "BR") {
        var br = doc.createElement('br');
        arrReturnNodes.push(br);
        continue;
      } 
      
      // if it's a carriage return managed as a DIV (as IE does for our editor) 
      // then add a <br> node to the array and change the DIV to a SPAN so that
      // the user doesn't see two carriage returns where they'd expect only one.
      if (Detect.IE() && childName == "DIV") {
        var br = doc.createElement('br');
        arrReturnNodes.push(br);
        var span = doc.createElement('span');
        span.innerHTML = child.innerHTML;
        this.stripFormatNodesFromSelection(span, arrReturnNodes);
        continue;
      } 
      
      // otherwise continue recursively through the tree
      this.stripFormatNodesFromSelection(child, arrReturnNodes);
    }
  }
}

/*
 * adoptGrandchildrenFromParent()
 *
 * Removes the children of the supplied node and appends them to their
 * grandparent then removes the supplied node.
 */
function adoptGrandchildrenFromParent(doc, parent) {
  if (!parent) return;
  var grandparent = parent.parentNode;
  if (!grandparent) return;
  var grandchildren = parent.childNodes;
  if (!grandchildren) return;
  
  var nodes = [];
  for (var x = 0; x < grandchildren.length; x++) {
    var type = grandchildren[x].nodeType;   
    // if it's a text node, add the grandchild to the array
    if (type == 3)  {
      nodes.push(grandchildren[x]);
      continue;
    }
    
    // if it's an element node...
    if (type == 1) {
      // preserve the line breaks by adding a <br> node, else add the 
      // grandchild
      if (grandchildren[x].nodeName.toUpperCase() == "BR") {
        // create a new <br> node, because, mysteriously, it's difficult to
        // append the originals as nodes later.  needs looking into.
        var br = doc.createElement('br');
        nodes.push(br);
      } else {
        nodes.push(grandchildren[x]);
      }
    }
  }
  for (var x = 0; x < nodes.length; x++) {
    grandparent.insertBefore(nodes[x], parent);
  }
  grandparent.removeChild(parent);
}


function CreateLink() {
  var idoc = RichEdit.frameDoc;
  if (Detect.IE()) {
    idoc.execCommand("CreateLink", true);  // true == show ui
    // stop IE from overwriting the content when a key combination is pressed
    // i.e. CTRL+SHFT+A
    var evt = getAvailableEventForIE();
    evt.returnValue = false;
    
  } else {
    var NO_SEL_TEXT = "First select the text that you want to make "
    + "into a link.";
    var ENTER_URL = "Enter a URL:";
    if (GetSelection().isCollapsed) {
      alert(NO_SEL_TEXT);
    } else {
      var url = prompt(ENTER_URL, "http://");
      if (url != null) {  // url == null means dialog was cancelled
        url = Trim(url);
        if ((url != "") && (url != "http://")) {
          idoc.execCommand("CreateLink", false, url);
        } else {
          idoc.execCommand("Unlink", false, "");
        }
      }
    }
  }
}


// ===================================================================
// Color palettes
// ===================================================================

RichEditor.prototype.COLORS = [
  ["ffffff", "cccccc", "c0c0c0", "999999", "666666", "333333", "000000"], // blacks
  ["ffcccc", "ff6666", "ff0000", "cc0000", "990000", "660000", "330000"], // reds
  ["ffcc99", "ff9966", "ff9900", "ff6600", "cc6600", "993300", "663300"], // oranges
  ["ffff99", "ffff66", "ffcc66", "ffcc33", "cc9933", "996633", "663333"], // yellows
  ["ffffcc", "ffff33", "ffff00", "ffcc00", "999900", "666600", "333300"], // olives
  ["99ff99", "66ff99", "33ff33", "33cc00", "009900", "006600", "003300"], // greens
  ["99ffff", "33ffff", "66cccc", "00cccc", "339999", "336666", "003333"], // turquoises
  ["ccffff", "66ffff", "33ccff", "3366ff", "3333ff", "000099", "000066"], // blues
  ["ccccff", "9999ff", "6666cc", "6633ff", "6600cc", "333399", "330099"], // purples
  ["ffccff", "ff99ff", "cc66cc", "cc33cc", "993399", "663366", "330033"] // violets
  ];

function Palette() {}
Palette.cell = null;
Palette.obj = null;
Palette.colorSelectedValue = null;
Palette.colorSelectedButton = null;

function PaletteOver(e) {
  e.style.border = "1px solid #fff";
  Palette.cell = e;
}

function PaletteOut(e) {
  e.style.border = "1px solid #bbb";
  Palette.cell = null;
}

function ShowPalette(button) { 
  if (!Palette.obj) {
    var COLORS = RichEdit.COLORS;
    var html = [];
    html.push('<table id="xpalettetable" style="width:130px;" ' +
              'cellspacing="0" cellpadding="0">');
    for (var i = 0; i < COLORS.length; i++) {
      html.push('<tr>');
      for (var j = 0; j < COLORS[i].length; j++) {
        html.push('<td bgcolor="#' + COLORS[i][j] + '"' 
        + ' unselectable="on" onmouseover="PaletteOver(this)"' 
        + ' onmouseout=PaletteOut(this) ' 
        + "onclick=\"PaletteClick('#" + COLORS[i][j] + "')\">" 
        + '<img width="1" height="1"></td>');
      }
    }
    setInnerHTML("palette", html.join(""));
    Palette.obj = getElement("palette");
  }
  var pos = getXY(button); 
  var palette = Palette.obj;
  palette.style.left = pos.x + "px";
  palette.style.top = (pos.y + button.offsetHeight) + "px";
  showElement(palette); 
}

function SelectColor(button, cmd) {
  Palette.colorSelectedValue = cmd;
  Palette.colorSelectedButton = button;
  RichEdit.addDebugMsg('-- RichEdit Formatting Button pressed: '+cmd);
  ShowPalette(button);
}

function HidePalette() {
  if (Palette.obj) {
    if (Palette.cell) {
      // Moz doesn't call onmouseout on the palette
      // cell when the palette is hidden, so we
      // track it manually and do it ourselves.
      PaletteOut(Palette.cell);
    }
    hideElement(Palette.obj);
    if (Palette.colorSelectedButton) {
      Palette.colorSelectedButton.className = "";
    }
  }
}

function PaletteClick(color) {
  HidePalette();
  if (Palette.colorSelectedValue) {
    RichEdit.frameDoc.execCommand(Palette.colorSelectedValue, false, color);
    Palette.colorSelectedValue = null;
    Palette.colorSelectedButton = null;
  }
  setRichEditFocus(); // for moz
}



// ===================================================================
// Setting Cursor-Position-Based Button State 
// ===================================================================


var IFRAME_INIT_MESSAGE = "&nbsp;";

function getAvailableEventForIE() {
  if (!event) {
    return getRichEditorEventForIE();
  } else {
    return event;
  }
}

function getRichEditorEventForIE() {
  return getElement(RichEdit.frameId).contentWindow.event;
}

function CheckFormatting(e) { 
  if (RichEdit.mode == RichEdit.HTML_MODE) return;  //how's it getting called?
  
  e = (Detect.IE()) ? getAvailableEventForIE() : e;
  
  // START: Firefox bugfix 
  // In 0.8, the cursor doesn't appear after click or focus to the IFRAME.    
  var idoc = RichEdit.frameDoc;
  var iwin = RichEdit.frameWin; 
  if (Detect.MOZILLA() && idoc.body.innerHTML == IFRAME_INIT_MESSAGE) {
    if (e.type == "click") {
      var range = idoc.createRange();
      var sel = iwin.getSelection();
      var enodes = idoc.body.childNodes[0];
      range.setStart(enodes, 0);
      range.setEnd(enodes, 0);
      
      sel.removeAllRanges();
      sel.addRange(range);
      iwin.focus();
      sel.collapseToStart();
    } else {
      idoc.body.innerHTML = "";
    }
  }
  if (Detect.MOZILLA() && e.type == "keydown") return; //we'll check on keyup
  // END: Firefox bugfix
  
  ShowHtml();  
  CheckIfEmpty();  //Everyone, please thank Bay.
  
  // hide the palette except when the palette button is being clicked
  if (getEventSource(e).parentNode 
      && !hasColorPalette(getEventSource(e).parentNode.id)) {
    HidePalette(); 
  }
  
  // get all of the HTML nodes surrounding the current selection
  var nodes = getAncestors();
  var on = [];
  for (var i = 0; i < nodes.length; i++) {
    if (nodes) setButtonFromNode(nodes[i], on);
  }
  var baseNodeTotal = (Detect.IE()) ? 1 : 0;
  if (nodes.length == baseNodeTotal) {
    SetFontNameMenu('');
    SetFontSizeMenu(3);
  }  
  
  // depress those buttons whose nodes are ancestors of the selection
  var BUTTONS = [];
  var DEPRESSABLE = RichEdit.DEPRESSABLE;
  for (var i = 0; i < DEPRESSABLE.length; i++) {
    BUTTONS[DEPRESSABLE[i]] = true;
  }
  
  var SELECTED = [];
  for (var i = 0; i < on.length; i++) {
    SELECTED[on[i]] = true;
  }
  
  var DEBUG_CHECK_FORMATTING = false;
  if (RichEdit.DEBUG_CHECK_FORMATTING) {
    DEBUG_CHECK_FORMATTING = true;
    RichEdit.clearDebugMsg();
    RichEdit.addDebugMsg('CheckFormatting() -- [event: '+e.type+']\n');
  }
  for (var ctrl = 0; ctrl < BUTTONS.length; ctrl++) {
    if (BUTTONS[ctrl] == SELECTED[ctrl]) {
      // don't react to the toolbar button click, we need that to be in 
      // some other abstraction to manage palette-like buttons
      if (e.type != "mousedown") {
        Depress(ctrl);
        if (DEBUG_CHECK_FORMATTING) {
          RichEdit.addDebugMsg('Depress() -- [press: '+ctrl+']\n');
        }
      }
    } else {
      // don't react to the toolbar button click
      if (e.type != "mousedown") {
        if (DEBUG_CHECK_FORMATTING) {
          RichEdit.addDebugMsg('UnDepress() -- ['+e.type+', '+ctrl+']\n');
        }
        UnDepress(ctrl);      
      }
    }
  }
}

function ClearOtherJustify(ctrl) {
  if (ctrl.indexOf('Justify') != -1) {
    if (ctrl != 'JustifyLeft') {
      UnDepress('JustifyLeft');
      RichEdit.addDebugMsg('UnDepress() -- [JustifyLeft, '+ctrl+']\n');
    }
    if (ctrl != 'JustifyCenter') {
      UnDepress('JustifyCenter');
      RichEdit.addDebugMsg('UnDepress() -- [JustifyCenter, '+ctrl+']\n');
    }
    if (ctrl != 'JustifyRight') {
      UnDepress('JustifyRight');
      RichEdit.addDebugMsg('UnDepress() -- [JustifyRight, '+ctrl+']\n');
    }
  }
}

function setButtonFromNode(node, on) {
  if (isBoldMarkup(node)) { on.push("Bold");}
  if (isItalicMarkup(node)) {on.push("Italic");}
  if (isColorMarkup(node)) {on.push("ForeColor");}
  if (isNode(node, 'blockquote')) {on.push("Blockquote");}
  if (isNode(node, 'ul')) {on.push("InsertUnorderedList");}
  if (isNode(node, 'ol')) {on.push("InsertOrderedList");}
  if (isBkgColorMarkup(node)) {on.push(RichEdit.BACKCOLOR);}
  if (isAlignMarkup(node, 'left')) {
    on.push("JustifyLeft");
  } else if (isAlignMarkup(node, 'center')) {
    on.push("JustifyCenter");
  } else if (isAlignMarkup(node, 'right')) {
    on.push("JustifyRight");
  }
  if (hasSomeMargin(node, 'left')) {on.push("Indent");}
  var font = new Object();
  if (isFontMarkup(node, font)) {
    var fontFamily = (!font.FAMILY) ? '' : font.FAMILY;
    var fontSize = (!font.SIZE) ? 3 : font.SIZE;
    SetFontNameMenu(font.FAMILY);
    SetFontSizeMenu(font.SIZE);
  } 
}

function Depress(ctrl) {
  var el = d('formatbar_' + ctrl);
  if (el) {
    el.className = 'down';
  } else {
    RichEdit.addDebugMsg('Depress() failed for "' + ctrl + '"');
  }
}

function UnDepress(ctrl) {
  var el = d('formatbar_' + ctrl);
  if (el) {
    el.className = '';
  } else {
    RichEdit.addDebugMsg('UnDepress() failed for "' + ctrl + '"');
  }
}

function isFontMarkup(node, fontObj) {
  var nm = node.nodeName.toUpperCase(); 
  var fontFamily = getStyle(node, 'font-family');
  var fontSize = getStyle(node, 'font-size');
  var face = node.getAttribute('face');
  var size = node.getAttribute('size');
  var font = getStyle(node, 'font');
  if (font || fontFamily || fontSize || size || face) {
    if (font) fontObj.FONT = font;
    if (fontFamily) fontObj.FAMILY = fontFamily;
    if (fontSize) fontObj.SIZE = fontSize;
    if (face) {fontObj.FACE_ATTR = true; fontObj.FAMILY = face; }
    if (size) {fontObj.SIZE_ATTR = true; fontObj.SIZE = size;  }
    return true;
  } else {
    return false;
  }
}

function isBoldMarkup(node) {
  return (matchesStyledMarkup(node, 'font-weight', 'bold') || isNode(node, 'B') 
          || isNode(node, 'STRONG'));
}

function isItalicMarkup(node) {
  return (matchesStyledMarkup(node, 'font-style', 'italic') || isNode(node, 'I') 
          || isNode(node, 'EM'));
}

function isColorMarkup(node) { 
  if (Detect.IE()) {
    return node.getAttribute('color');
  } else {
    var style = node.getAttribute('style');
    if (!style) style = '';
    var bkgPattern = new RegExp('background-color:[^;"]*');
    style = style.replace(bkgPattern, '');
    return (style.indexOf('color') != -1);
  }
}

function isBkgColorMarkup(node) { 
  if (Detect.IE()) {
    return node.style.backgroundColor;
  } else {
    var style = node.getAttribute('style');
    if (!style) style = '';
    return (style.indexOf('background-color') != -1);
  }
}


function isAlignMarkup(node, direction) {
  return (matchesStyledMarkup(node, 'text-align', direction)
          || hasAttributeValue(node, 'align', direction));
}

function hasSomeMargin(node, direction) {
  var style = getStyle(node, 'margin-' + direction);
  return (style && style != 0 && style != '0px');
}

function hasAttributeValue(node, attr, value) {
  var attr = node.getAttribute(attr);
  if (attr) {
    return (attr == value);
  } else {
    return false;
  }
}

function isNode(node, tagName) {
  return (node.nodeName == tagName.toUpperCase());
}

function matchesStyledMarkup(node, attribute, styleValue) { 
  var nm = node.nodeName.toUpperCase(); 
  var style = getStyle(node, attribute);
  var font = getStyle(node, 'font');
  if (style || font) {
    return (style.toLowerCase() == styleValue 
            || font.toLowerCase().indexOf(styleValue.toLowerCase()) != -1) ? true : false;
  } else {
    return false;
  }
}

function getAncestors() {
  var ancestors = [];
  var parent = getSelectedParentNode();
  while (parent && (parent.nodeType == ELEMENT_NODE) 
         && (parent.tagName != "BODY")) { 
    ancestors.push(parent);
    parent = parent.parentNode;
  }
  return ancestors;
}

function getSelectedParentNode() {
  if (Detect.IE()) {
    var sel = GetRange();
    try {
      sel.collapse();
      return sel.parentElement();
    } catch(e) {
      // Images within MSHTML editing don't have parent elements and
      // should avoid performing this function.
      return; 
    }
  } else {
    var range = GetRange();
    if (!range) return;
    parent = range.commonAncestorContainer;
    if (parent.nodeType == TEXT_NODE) {
      parent = parent.parentNode;
    }
    return parent;
  }
}

function SetFontNameMenu(font) {
  try {
    SetMenu("FontName", font.toLowerCase());
  } catch(e) {
    RichEdit.addDebugMsg('-- SetFontNameMenu() failed when parsing: "'+font+'"');
  }
}

function SetFontSizeMenu(size) {
  SetMenu("FontSize", size);
}

function SetMenu(menuid, value) {
  var menu = document.getElementById(menuid);
  for (var i = 0; i < menu.options.length; i++) {
    if (menu.options[i].value == value) {
      menu.options.selectedIndex = i;
      break;
    }
  }
}

// In IE, editing must occur within <div></div> in order
// to have line breaks look like <br>'s rather than <p>'s.
// The div can be deleted if ctrl-A is used to select all
// and then everything is deleted.
function CheckIfEmpty() {
  var idoc = RichEdit.frameDoc;
  if (idoc.body.innerHTML == "<P>&nbsp;</P>") {
    resetIFrameBody();
  }
}

function resetIFrameBody() {
  var idoc = RichEdit.frameDoc;
  idoc.body.innerHTML = "<div></div>";
  // now put the input caret into the div
  var range = idoc.body.createTextRange();
  range.collapse();
  range.select();
}


function GetSelection() {
  if (Detect.IE()) {
    return RichEdit.frameDoc.selection;
  } else {
    return RichEdit.frameWin.getSelection();
  }
}

function GetRange() {
  var idoc = RichEdit.frameDoc;
  if (Detect.IE()) {
    return idoc.selection.createRange();
  } else {
    try {
      var range = idoc.createRange();
      var sel = GetSelection();
      range = sel.getRangeAt( sel.rangeCount - 1 ).cloneRange();
      return range;
    } catch(e) {
      RichEdit.addDebugMsg('--create/set range failed');
      return null;
    }
  }
}


function setRichEditFocus() {
  document.getElementById(RichEdit.frameId).contentWindow.focus();
}


// ===================================================================
// Keyboard commands
// ===================================================================


/*
 * activateKeyCommands()
 *
 * Based on a combination of keystrokes and keyholds, activate 
 * a particular formatting method.
 * 
 * Warning: In Safari the text formatting keyboard commands do not work
 * since there is no selection management in textareas yet for that browser.
 */
RichEditor.prototype.activateKeyCommands = function(e) { 
  
  if (Detect.IE()) e = RichEdit.frame.contentWindow.event;
  
  setKeysetByEvent(e); 
    
  if (RichEdit.DEBUG_FRAME_EVENTS) {
    RichEdit.clearDebugMsg();
    RichEdit.addDebugMsg('-- RichEdit event: '+e);
    RichEdit.addDebugMsg('-- RichEdit event type: '+e.type);
    RichEdit.addDebugMsg('-- RichEdit key: '+getKey(e));
  }
  
  KEY_COMMANDS = RichEdit.KEY_COMMANDS;
  
  for (x = 0; x < KEY_COMMANDS.length; x++) { 
    var ifKeyPressed = eval(KEY_COMMANDS[x][0]);
    if (ifKeyPressed) {
      if (KEY_COMMANDS[x][1] != null) {
        toggleButtonDisplay(KEY_COMMANDS[x][1]);
      }
      if (KEY_COMMANDS[x][2]) {
        // stop IE from duplicating a supported MSHTML action, like making 
        // text bold or italic.
        if (!Detect.IE() 
            || (Detect.IE() && !(CTRL_B || CTRL_I))) {
          
          eval(KEY_COMMANDS[x][2]);
          
          // stop IE from duplicating the post
          if (Detect.IE() && CTRL_SHFT_S) break;
          
        }
      }
    }
  }
  // prevent bookmarks sidebar from opening in Mozilla on Windows
  if (Detect.MOZILLA() && (CTRL_B || CTRL_I || CTRL_S || CTRL_D)) {
    e.preventDefault();
  }
  
  // attempt to defeat IE's link auto-completion...it ruins image sources.
  if (Detect.IE()) CorrectLinkAutoCompletionInImages();
  
  /*
   * stop IE from inserting a paragraph tag when a user is trying to delete 
   * the entire selection
   */
  if (Detect.IE() && !isCtrlKeyPressed(e) && !isAltKeyPressed(e)) {
    /*
     * get the selection... and see if it's "select all" by testing if its 
     * length is larger than the innerHTML length of the IFRAME
     */
    var rangeNode = getRangeAsDocumentFragment(GetRange()); 
    var strippedRange = rangeNode.innerHTML.replace(/(<([^>]+)>)/gi, ""); 
    var strippedBody = RichEdit.frameDoc.body.innerHTML.replace(/(<([^>]+)>)/gi, ""); 
    if (strippedRange.length >= strippedBody.length) {
      resetIFrameBody();
    }
  }
  
  // cleanup extraneous markup after delete operations are performed
  if (BACKSPACE || DELETE || RETURN) {
    setTimeout('RichEdit.cleanupDeletion()', 80);
  }

  return true;
}


/*
 * toggleButtonDisplay()
 *
 * Changes a specific toolbar button to look selected or un-selected.
 */
function toggleButtonDisplay(ctrl) {
  try {
    var obj = getElement('formatbar_' + ctrl);
    obj.className = (obj.className == 'down') ? '' : 'down';
  } catch(e) {
    RichEdit.addDebugMsg('toggleButtonDisplay() failed for "' + ctrl + '"');
  }
}
