/** -------------------------------------------------------
 * Author: Chris Wetherell at Google, based on code by Chris 
 * Wetherell before Google.
 * 
 * Textbar() [object]
 *
 * A class that can take the currently selected (highlighted) 
 * text and wrap it in HTML tags commonly used for formatting.  
 *
 * TODO: This could be generalized for, say, template use(!)
 *
 * Based on code found at massless.org.  Which, given that *I* am 
 * that site's author makes this credit declaration intentionally 
 * confusing.
 *
 * Warning: Expects presence of dom.common.js and detect.js.
 *
 * Typical usage:
 * On an HTML element: onclick="Textbar.Bold();"
  ------------------------------------------------------- */

function Textbar() {}
Textbar.ELEMENT_ID;
Textbar.PREVIEW_BUTTON;


/* -----------
 * Format tags
 * ----------- */
// Undoing tag formatting in designMode is browser-specific.  Mozilla / Gecko 
// prefers SPAN tag with style while IE prefers explicit formatting tags.
 
Textbar.Bold = function()
{
  if (!Detect.IE()) {
    this.wrapSelection('<span style="font-weight:bold;">','</span>');
  } else {
    this.wrapSelection('<strong>','</strong>');
  }
}

Textbar.Italic = function()
{
  if (!Detect.IE()) {
    this.wrapSelection('<span style="font-style:italic;">','</span>');
  } else {
    this.wrapSelection('<em>','</em>');
  }
}

Textbar.Blockquote = function()
{
  this.wrapSelection('<blockquote>','</blockquote>');
}

Textbar.Link = function()
{
  this.wrapSelectionWithLink(this.getElement());
}



/**
 * getElement()
 *
 * For storing the element (usually a <textarea>) where the 
 * text will be modified.
 */
Textbar.Element = false;
Textbar.getElement = function()
{
  if (!this.Element) {
     this.Element = d(this.ELEMENT_ID);
  }
  return this.Element;
}



/**
 * wrapSelection()
 *
 * Branches out the tag wrapping code for differing implementations 
 * of selection management. *sigh*  Standards, where art thou?
 */
Textbar.wrapSelection = function(lft, rgt) {
  if (Detect.IE()) {
    this.IEWrap(lft, rgt);
  } else if (DOM) {
    this.mozWrap(lft, rgt);
  }
}



/**
 * wrapSelectionWithLink()
 *
 * Wrap a hyperlink around some text by prompting the user for 
 * a URL.
 */
Textbar.wrapSelectionWithLink = function() {
  var my_link = prompt("Enter URL:","http://");
  if (my_link != null) {
    lft="<a href=\"" + my_link + "\">";
    rgt="</a>";
    this.wrapSelection(lft, rgt);
  }
  return;
}



/**
 * mozWrap()
 *
 * Wraps tags around text in Mozilla/Gecko browsers.
 */
Textbar.mozWrap = function(lft, rgt) {

  var txtarea=this.getElement();
  var v = txtarea.value;
  var selLength = txtarea.textLength;
  var selStart = txtarea.selectionStart;
  var selEnd = txtarea.selectionEnd;
  if (selEnd==1 || selEnd==2) selEnd=selLength;
  var s1 = (v).substring(0,selStart);
  var s2 = (v).substring(selStart, selEnd)
  var s3 = (v).substring(selEnd, selLength);
  txtarea.value = s1 + lft + s2 + rgt + s3;
}


/**
 * IEWrap()
 *
 * Wraps tags around text in Internet Explorer.
 */
Textbar.IEWrap = function(lft, rgt) {

  txtarea=this.getElement();
  strSelection = document.selection.createRange().text;

  if (strSelection!="")
  {
    document.selection.createRange().text = lft + strSelection + rgt;
  } else {
    txtarea.focus()
    strSelection = document.selection.createRange().text;
    txtarea.value = txtarea.value + lft + rgt;
  }

}

// Emulate the 'click' function in IE for Mozilla
// TODO: Add to cross-browser event library
if (Detect.MOZILLA()) {
  HTMLElement.prototype.click = function () {
    if (typeof this.onclick == 'function')
      this.onclick({type: 'click'});
  };
}



/**
 * activateKeyCommands()
 *
 * Based on a combination of keystrokes and keyholds, activate 
 * a particular formatting method.
 * 
 * Warning: In Safari, only the Preview button onclick command works 
 * since there is no selection management yet for that browser.
 */
Textbar.activateKeyCommands = function(e) {
  
  if (Detect.IE()) e = window.event;
  
  setKeysetByEvent(e);
  
  if (CTRL_SHFT_P) {d(Textbar.PREVIEW_BUTTON).click();}
  if (CTRL_S || CTRL_SHFT_S) d(Posting.PUBLISH_BUTTON).click();
  if (CTRL_D || CTRL_SHFT_D) d(Posting.DRAFT_BUTTON).click(); 
  
  // WARNING: The following can delete data in a textarea in Safari as of 
  // 1.2.1 (v125.1)
  if (!Detect.SAFARI()) { 
    if (CTRL_B || CTRL_SHFT_B) Textbar.Bold();
    if (CTRL_I || CTRL_SHFT_T) Textbar.Italic();
    if (CTRL_L || CTRL_SHFT_L) Textbar.Blockquote();
    if (CTRL_SHFT_A) Textbar.Link();
  }
  
  // prevent sidebars or dialogs from opening with reserved keypresses
  if (CTRL_B || CTRL_I || CTRL_S || CTRL_D) {
    if (Detect.IE()) {
      e.returnValue = false;
    } else {
      e.preventDefault();
    }
  }
  
  return true;
}

