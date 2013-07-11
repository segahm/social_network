
// ===================================================================
// Mode Toggling
// ===================================================================

RichEditor.prototype.ShowSourceEditor = function() {
  showElement(this.textarea);
  hideElement(this.frame);
  hideElement(this.formatbar);
  showElement(this.htmlbar);
  var ShowHtmlSource = d('ShowSourceEditor');
  if (ShowHtmlSource) {
    ShowHtmlSource.className = "on";
    ShowHtmlSource.style.backgroundColor='';
    getElement('ShowRichEditor').className = "";
  }
  
  this.movePostBodyToTextarea();
  HidePalette();
  
  // preview needs to know which link to hide or show
  Textbar.PREVIEW_BUTTON = Preview.HTML_PREVIEW_BUTTON;
  
  this.mode = this.HTML_MODE;
}



var NON_EXISTENT_URL  = "http://not-a-real-namespace/"; //dummy URL
var REMOVAL_PLACEHOLDER = "~~SPECIAL_REMOVE!#~~"; //for entity escaping

function removeRelativePathPlaceholders(doc) {
  try {
    var images = doc.images;
    var links = doc.links;
    var NON_EXISTENT_URLPattern = new RegExp(NON_EXISTENT_URL, 'gi');
    for (var i = 0; i < images.length; i++) {
      images[i].src = images[i].src.replace(NON_EXISTENT_URLPattern, '');
    }
    for (var i = 0; i < links.length; i++) {
      links[i].href = links[i].href.replace(NON_EXISTENT_URLPattern, '');
    }
  } catch(e) {}
}

function addRelativePathPlaceholders(strBody) {
  
  /*
   * Replace links and images that have relative links to have a dummy URL
   * in order to have them appear as requested and defeat IE's auto-complete
   */
  if (Detect.IE()) { 
    var quot = "'" + '"' ; // mixed-syntax easier on the eyes in code editors
    
    /* 
     * this reg exp is pre-pending a dummy URL because IE will try and 
     * auto-complete certain attributes that start with "http://"
     */
    var tagPattern 
      = new RegExp('<(a|img)(([^>\\s]*[\\s]+)+)(href|src)[\\s]*=[\\s]*' 
                   + '[' + quot + ']?([^' + quot + '\\s>]+)[' + quot + ']?', 
                  'gi');
    strBody = strBody.replace(tagPattern, '<$1$2$4="' + NON_EXISTENT_URL 
      + '$5"');
    }

    return strBody;
}


RichEditor.prototype.ShowRichEditor = function() {
  
  showElement(this.frame);
  hideElement(this.textarea);
  showElement(this.formatbar);
  hideElement(this.htmlbar);
  var ShowHtmlSource = d('ShowSourceEditor');
  if (ShowHtmlSource) {ShowHtmlSource.className = "";}
  var ShowDesignMode = d('ShowRichEditor');
  if (ShowDesignMode) {
    ShowDesignMode.className = "on";
    ShowDesignMode.style.backgroundColor='';
  }
  
  // Replace text line breaks with HTML link breaks
  var strBody = this.textarea.value;   
  
  // Newlines within tags could present problems in WYSIWYG
  strBody = removeNewlinesFromTags(strBody);
  
  strBody = strBody.replace(/\&lt;/g,"&" + REMOVAL_PLACEHOLDER + "lt;");
  strBody = strBody.replace(/\&gt;/g,"&" + REMOVAL_PLACEHOLDER + "gt;");
  
  //allow textual representation of HTML entities
  strBody = strBody.replace(/\&amp;([^\\s]+;)/gi, '&amp;amp;$1');
  
  // Attempt to persist relative paths
  if (Detect.IE()) {
    strBody = addRelativePathPlaceholders(strBody);
  }
    
  if (Detect.IE()) {
    if (strBody == '<p>&nbsp;</p>') strBody = '';
    strBody = "<div>" + strBody + "</div>";
  }

  // getDesignModeHtml() converts this back to newlines, and there may be 
  // be issues when copying into designMode without a newline after the 
  // <br> tag for Firefox 0.8
  this.frameDoc.body.innerHTML = strBody.replace(/\n/g,"<br>");

  if (Detect.IE()) {
    //don't let DIVs or carriage returns accumulate on each toggle    
    var repeatedDivPattern = new RegExp('^(<div>\r\n){1,}', 'gi');
    var repeatedDivLinePattern = new RegExp('(<div></div>)$', 'gi');
    var repeatedDivEndPattern = new RegExp('<br>(<\/div>){1,}$', 'gi');
    this.frameDoc.body.innerHTML 
      = this.frameDoc.body.innerHTML.replace(repeatedDivPattern, '');
    this.frameDoc.body.innerHTML 
      = this.frameDoc.body.innerHTML.replace(repeatedDivLinePattern, '');
    this.frameDoc.body.innerHTML 
      = this.frameDoc.body.innerHTML.replace(repeatedDivEndPattern, '</div>') ;
  }
  
  this.frameDoc.body.innerHTML 
    = cleanHTML(this.frameDoc.body.innerHTML);
  this.frameDoc.body.innerHTML 
    = convertAllSpansToFonts(this.frameDoc.body.innerHTML);

  // Remove all encoding placeholders
  var removalPattern = new RegExp(REMOVAL_PLACEHOLDER, 'g');
  this.frameDoc.body.innerHTML
    = this.frameDoc.body.innerHTML.replace(removalPattern, '');
  
  // Remove dummyURL in order to make links and images with relative links 
  // appear as requested and defeat IE's auto-complete
  removeRelativePathPlaceholders(this.frameDoc);

  // must remind moz that designmode is on after its display style is changed
  if (!Detect.IE()) this.frameDoc.designMode = "On";

  // preview needs to know which link to hide or show
  Textbar.PREVIEW_BUTTON = Preview.PREVIEW_BUTTON;

  this.mode = this.DESIGN_MODE;
}

// ===================================================================
// Mode-to-Mode HTML Transfer
// ===================================================================

RichEditor.prototype.movePostBodyToTextarea = function() {
  this.textarea.value = getDesignModeHtml();  
  this.textarea.value = cleanHTML(this.textarea.value);
  this.textarea.value = convertAllFontsToSpans(this.textarea.value);
  var val = this.textarea.value;
  /*
   * fix for showing a single newline when toggling from Compose
   * to Edit HTML
   */
  if (val == '\r\n' || val == '\n') this.textarea.value = '';
  
  // An IE fix for removing sometimes-inserted empty lines
  if (Detect.IE()) {
    if (val == '<p>&nbsp;</p>' || val == '<div></div>') {
      this.textarea.value = '';
    }
  }
}

function ShowHtml() {
  RichEdit.textarea.value = getDesignModeHtml();
}

function getDesignModeHtml()
{
  // Replace HTML line breaks with text line breaks
  var strBody = RichEdit.frameDoc.body.innerHTML;
  
  if (Detect.IE()) {
    
    // Do our best to make the MSHTML markup valid.
    strBody = cleanMSHTML(strBody);
    
  } else {
    
    // Midas can crash the browser if enough Windows line feeds accumulate
    strBody = strBody.replace(/\r/g,"");
    
    /*
     * Mozilla needs the space replacement or it will shove words at end-of-line 
     * boundaries together
     */
    strBody = strBody.replace(/\n/g," ");
    
    // get rid of trailing spaces after <br>
    strBody = strBody.replace(/<br> /gi,"<br>");
    
    // convert HTML line breaks to newlines
    strBody = strBody.replace(/<br>/gi,"\n");
  }
  
  if (RichEdit.ALLOW_HTML_ENTRY) {
    strBody = strBody.replace(/\&lt;/g,"<");
    strBody = strBody.replace(/\&gt;/g,">");
    strBody = strBody.replace(/\&amp;lt;/g,"&lt;");
    strBody = strBody.replace(/\&amp;gt;/g,"&gt;");
    
    //allow HTML entities
    strBody = strBody.replace(/\&amp;([^;]+;)/gi, '&$1');
  }
  
  // convert space entities
  strBody = strBody.replace(/\&nbsp;/g," ");
  strBody = strBody.replace(/ \n/g, '\n');
  
  return strBody;
}

