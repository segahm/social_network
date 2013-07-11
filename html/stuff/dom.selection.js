
function surroundFrameSelection(frame, tagName) {
  var win = frame.contentWindow;
  surroundSelection(win, tagName);
}

function surroundSelection(win, tagName) {
  if (Detect.IE()) {
    surroundSelection_IE(win.document, tagName);
  } else {
    var doc = (win.contentDocument) ? win.contentDocument : document;
    var el = doc.createElement(tagName);
    surroundSelection_DOM(win, el);
  }
}

function insertNodeAtSelection(win, tag, fragment) {
  if (Detect.IE()) {
    var doc = win.document;
    var range = doc.selection.createRange();
    insertNodeAtSelection_IE(doc, tag, fragment.innerHTML);
  } else {
    var doc = (win.contentDocument) ? win.contentDocument : document;
    var el = doc.createElement(tag);
    insertNodeAtSelection_DOM(win, el, fragment);
  }
}

function insertNodeAtSelection_IE(doc, tag, html) {
  try {
    var range = doc.selection.createRange();
    var startTag = '<' + tag + '>';
    var endTag = '</' + tag + '>';
    var replaceString = startTag + html + endTag; 
    
    var isCollapsed = range.text == '';
    range.pasteHTML(replaceString);
    
    if (!isCollapsed) {
      // move selection to html contained within the surrounding node
      range.moveToElementText(range.parentElement().childNodes[0]);
      range.select();
    }
  } catch(e) {
    RichEdit.addDebugMsg('insertNodeAtSelection_IE() failed for "' + tag + '"');
  }
}

function surroundSelection_IE(doc, tag) {
  try { 
    var range = doc.selection.createRange();
    var html = range.htmlText;
    
    // get rid of beginning newline
    if (html.substring(0,2) == '\r\n') html = html.substring(2, html.length);
    
    // resolve IE's special DIV cases
    html = replaceEmptyDIVsWithBRs(html);
    
    insertNodeAtSelection_IE(doc, tag, html);
  } catch(e) {
    RichEdit.addDebugMsg('surroundSelection_IE() failed for "' + tag + '"');
  }
}

function surroundSelection_DOM(win, tag) {
  try {
    var sel = win.getSelection();   
    var range = sel.getRangeAt(0); 
    insertNodeAtSelection_DOM(win, tag, range.cloneContents());
  } catch(e) {
    RichEdit.addDebugMsg('surroundSelection_DOM() failed for "' + tag + '"');
  }
}


/*
 * This function was taken from The Mozilla Organization's Midas demo. It has 
 * been modified.  In the future we may instead be able to use the
 * surroundContents() method of the range object, but a bug exists as of 
 * 7/6/2004 that prohibits our use of it in Mozilla.
 * (http://bugzilla.mozilla.org/show_bug.cgi?id=135928)
 */
function insertNodeAtSelection_DOM(win, insertNode, html)
{ 
  // get current selection
  var sel = win.getSelection();   
  
  // get the first range of the selection
  // (there's almost always only one range)
  var range = sel.getRangeAt(0);
  
  // insert specified HTML into the node passed by argument
  insertNode.appendChild(html);  
  
  // deselect everything
  sel.removeAllRanges();
  
  // remove content of current selection from document
  range.deleteContents();
  
  // get location of current selection
  var container = range.startContainer;
  var pos = range.startOffset;
  
  // make a new range for the new selection
  range=document.createRange();
  
  var afterNode;
  
  if (container.nodeType==3 && insertNode.nodeType==3) {
    
    // if we insert text in a textnode, do optimized insertion
    container.insertData(pos, insertNode.nodeValue);
    
  } else {
    
    
    if (container.nodeType==3) {
      
      // when inserting into a textnode
      // we create 2 new textnodes
      // and put the insertNode in between
      
      var textNode = container;
      container = textNode.parentNode;
      var text = textNode.nodeValue;
      
      // text before the split
      var textBefore = text.substr(0,pos);
      // text after the split
      var textAfter = text.substr(pos);
      
      var beforeNode = document.createTextNode(textBefore);
      var afterNode = document.createTextNode(textAfter);
      
      // insert the 3 new nodes before the old one
      container.insertBefore(afterNode, textNode);
      container.insertBefore(insertNode, afterNode);
      container.insertBefore(beforeNode, insertNode);
      
      // remove the old node
      container.removeChild(textNode);
      
    } else {
      
      // else simply insert the node
      afterNode = container.childNodes[pos];
      container.insertBefore(insertNode, afterNode);
    }
  }
  
  // select the modified html
  range.setEnd(insertNode, insertNode.childNodes.length);
  range.setStart(insertNode, insertNode);
  sel.addRange(range);
};




/*
 * getRangeAsDocumentFragment()
 *
 * Returns an HTML Document fragment representing the contents of the 
 * supplied selection range.
 */
function getRangeAsDocumentFragment(range) {
  try {
    if (Detect.IE()) {
      var el = document.createElement('span');
      el.innerHTML = range.htmlText;
      return el;
    } else {
      return range.cloneContents();
    }
  } catch(e) {
    RichEdit.addDebugMsg('--getRangeAsDocumentFragment() failed');
    return null;
  }
}

