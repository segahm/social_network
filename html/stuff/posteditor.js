// Copyright 2005, Google Inc.

function setFormAndSubmit() {
  isSubmit=true;
  
  // IE 5.01 requires both checks or else errors occur.
  if (typeof RichEdit != 'undefined'
      && typeof RichEdit.mode != 'undefined') {
    
    // preserve the mode state to store in user preferences
    document.stuffform.editorModeDefault.value = RichEdit.mode;
    
    // move the rich-text into the textarea, if WYSIWYG was active
    if (RichEdit.mode == RichEdit.DESIGN_MODE) {
      RichEdit.movePostBodyToTextarea();
    }
  }
}

// upload file
function uploadFile() {
  l = (screen.width / 2) - 190
  t = (screen.height / 2) - 225
  winOptions = window.open("/upload-file.g?blogID=" + blogId,
    "uploadFileWin",
    "scrollbars=no,status=no,width=500,height=360,left="+l+",top="+t);
  winOptions.focus();
}

// _____________SPELL CHECK______________
// holds reference to popup
var ww; 
// change to point to the JSpell Spell Check Server
var spellCheckURL="/jspellhtml/JSpell.jsp"; 
// relative URL to JSpell button images directory
var imagePath="/jspellhtml/images"; 
// set to true, to remove the Learn words capability
var disableLearn=false; 
// force suggestions and spell checker to use upper case
var forceUpperCase=false; 
// ignore lower case sentence beginnings, etc.
var ignoreIrregularCaps=false;
// ignore if first character in a field is lowercase 
var ignoreFirstCaps=false;
// ignore words with embedded numbers 
var ignoreNumbers=false;
// ignore words in upper case 
var ignoreUpper=false; 
// ignore repated words
var ignoreDouble=false; 
// show warning before user 'learns' a word
var confirmAfterLearn=false; 
// show warning when replacing using a word not in the suggestions list.
var confirmAfterReplace=true; 
// optional supplemental word list kept at server.
var supplementalDictionary="";
// You can use this to hide the preview panel when running in directEdit 
// mode in IE 
var hidePreviewPanel=false;
// is highlighting done in original text control or is there a preview 
// panel (IE Windows only) 
var directmode=false; 
var styleSheetURL= baseAppUrl + "/jspellhtml/jspell.css"; 

function getSpellCheckItem(jspell_n) {
   var fieldsToCheck=getSpellCheckArray();
   return fieldsToCheck[jspell_n];
}

function getSpellCheckArray() {
   var fieldsToCheck=new Array();
   // make sure to enclose form/field object reference in quotes!
   if (showTitle) {
     fieldsToCheck[fieldsToCheck.length]='document.stuffform.title';
   }
   fieldsToCheck[fieldsToCheck.length]='document.stuffform.postBody';
   return fieldsToCheck;
}

function spellcheck() {
  var width=450; var height=200;
  if (navigator.appName == 'Microsoft Internet Explorer' && 
      navigator.userAgent.toLowerCase().indexOf("opera")==-1 && 
      hidePreviewPanel==false) {
    directmode=true; width=220;
  }

  if(hidePreviewPanel==true)
    width=220;

  var w = 1024, h = 768;
  if (document.all || document.layers) {
    w=eval("scre"+"en.availWidth"); h=eval("scre"+"en.availHeight");
  }

  var leftPos = (w/2-width/2), topPos = (h/2-height/2);

  // need to check if window is already open.
  ww=window.open("/jspellhtml/jspellpopup.html", "checker", 
    "width="+width+",height="+height+",top="+topPos+",left="+leftPos+
    ",toolbar=no,status=no,menubar=no,directories=no,resizable=yes");

  ww.focus();
}

function updateForm(jspell_m,newvalue) { 
  eval(getSpellCheckItemValue(jspell_m)+"=newvalue"); 
}

function getSpellCheckItemValue(jspell_j) { 
  return getSpellCheckItem(jspell_j)+".value"; 
}

function getSpellCheckItemValueValue(jspell_k) { 
  return eval(getSpellCheckItemValue(jspell_k)); 
}

function RefreshModes() {
  if (RichEdit && RichEdit.mode == RichEdit.DESIGN_MODE) {
    RichEdit.ShowRichEditor();
  }
}

/*
 * Tasks to perform if a user attempts to leave the page.
 */
window.onbeforeunload = function(e) {
  // Move the post from the IFRAME to the form, if WYSIWYG is enabled.

  if (typeof(RichEdit) != "undefined" 
      && RichEdit.mode == RichEdit.DESIGN_MODE) {
    RichEdit.movePostBodyToTextarea();
  }
  
  /*
   * If the form has changed, prompt the user to see if they want to 
   * discard their changes.
   */
  if (!e) e = event;
  return compareForm(e);
}
