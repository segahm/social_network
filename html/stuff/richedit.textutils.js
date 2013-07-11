function removeNewlinesFromTags(s) {
  s = s.replace(/<[^>]+>/g, function(ref) {
    ref = ref.replace(/\r/g, '');
    ref = ref.replace(/\n/g, ' ');
    return ref;
  });
  return s;
}

function replaceEmptyDIVsWithBRs(s) {
  
  // Remove line feeds to avoid weird spacing in Edit HTML mode
  s = s.replace(/[\r]/g, '');  
  
  /*
   * sometimes IE's pasteHTML method of the range object will insert
   * troublesome newlines that aren't useful
   */
  if (Detect.IE()) {
    var blockElementsPattern = new RegExp(
      '\n<(' + 
       'BLOCKQUOTE' + '|' +
       'LI' + '|' +
       'OBJECT' + '|' +
       'H[\d]' + '|' +
       'FORM' + '|' +
       'TR' + '|' +
       'TD' + '|' +
       'TH' + '|' +
       'TBODY' + '|' +
       'THEAD' + '|' +
       'TFOOT' + '|' +
       'TABLE' + '|' +
       'DL' + '|' +
       'DD' + '|' +
       'DT' + '|' +
       'DIV' + '|' +
       ')([^>]*)>', 'gi'
    );
    s = s.replace(blockElementsPattern, '<$1$2>');
  }
  
  // remove newlines from inside of tags
  s = removeNewlinesFromTags(s);
  
  // convert newlines to HTML line breaks
  s = s.replace(/\n/g, '<BR>');
  
  /*
   * Append html string to element in order to remove DIVs as nodes
   * rather than using string manipulation
   */
  var tmp = document.createElement('div');
  tmp.style.display = 'none';
  
  // Attempt to persist relative paths
  if (Detect.IE()) {
    s = addRelativePathPlaceholders(s);
  }
  
  tmp.innerHTML = s;
  
  // determine which DIVs have NO attributes
  var plainDIVs = [];
  var divs = tmp.getElementsByTagName('div');
  for (var i = 0; i < divs.length; i++) {
    var hasAttributes = false;
    var attrs = divs[i].attributes;
    for (var x = 0; x < attrs.length; x++) {
      var val = attrs[x].specified;
      if (val) {
        hasAttributes = true; 
        break;
      }
    }
    if (!hasAttributes) plainDIVs.push(divs[i]);
  }
  
  // strip DIVs with no attributes, but keep their content!
  for (var i = 0; i < plainDIVs.length; i++) {
    var grandparent = plainDIVs[i].parentNode;
    if (grandparent) {
      plainDIVs[i].innerHTML += '<BR>';
      adoptGrandchildrenFromParent(document, plainDIVs[i]);
    }
  }
  
  // remove newlines
  var html = tmp.innerHTML;
  html = html.replace(/[\r|\n]/g, '');
  
  
  // Remove dummyURL in order to make links and images with relative links 
  // appear as requested and defeat IE's auto-complete
  if (Detect.IE()) {
    var NON_EXISTENT_URLPattern = new RegExp(NON_EXISTENT_URL, 'gi');
    html = html.replace(NON_EXISTENT_URLPattern, '');
  }
  
  return html;
}

function cleanMSHTML(s) {
  
  s = replaceEmptyDIVsWithBRs(s); 
  
  // Remove IE's inserted comments when text is pasted into the IFRAME
  s = s.replace(/<!--StartFragment -->&nbsp;/g,"");
  
  // Try to transform into well-formed markup
  s = cleanHTML(s);
  
  s = s.replace(/<br \/>/gi, '\n');
  
  // Remove IE's blockquote styling
  s = s.replace(/<blockquote dir=\"ltr" style=\"MARGIN-RIGHT: 0px\">/g,
    '<blockquote style="margin-top:0;margin-bottom:0;">');

  return s;
}

/*
 * cleanHTML()
 *
 * Attempts to transform ill-formed HTML into well-formed markup.
 */
function cleanHTML(s) {
  // make node names lower-case and add quotes to attributes
  s = cleanNodesAndAttributes(s);
  // Midas adds colors to <br> tags!  TODO: take newline conversion elsewhere
  if (Detect.MOZILLA()) s = s.replace(/<br [^>]+>/gi, '\n');  
  // make single nodes XHTML compatible
  s = s.replace(/<(hr|br)>/gi, '<$1 \/>');
  // make img nodes XHTML compatible
  s = s.replace(/<(img [^>]+)>/gi, '<$1 \/>');
  return s; 
}

/*
 * cleanNodesAndAttributes()
 *
 * Attempts to transform node names to lower-case and add double-quotes to
 * HTML element node attributes.
 */
function cleanNodesAndAttributes(s) { 
  
  // Get all of the start tags
  var htmlPattern = new RegExp('<[ ]*([\\w]+).*?>', 'gi');
  s = s.replace(htmlPattern, function(ref) {
    
    var cleanStartTag = ""; // for storing the result
    
    // Separate the tag name from its attributes
    var ref = ref.replace('^<[ ]*', '<'); // remove beginning whitespace
    var ndx = ref.search(/\s/);  // returns index of first match of whitespace    
    var tagname = ref.substring(0 ,ndx);
    var attributes = ref.substring(ndx, ref.length);
    
    // Make tag name lower case
    if (ndx == -1) return ref.toLowerCase(); // no attr/value pairs (i.e. <p>)
    cleanStartTag += tagname.toLowerCase();
    
    // Clean up attribute/value pairs    
    var pairs = attributes.match(/[\w]+\s*=\s*("[^"]*"|[^">\s]*)/gi);
    if (pairs) {
      for (var t = 0; t < pairs.length; t++) {
        var pair = pairs[t];
        var ndx = pair.search(/=/);  // returns index of first match of equals (=)   
        
        // Make attribute names lower case 
        var attrname = pair.substring(0, ndx).toLowerCase();
        
        // Put double-quotes around values that don't have them
        var attrval = pair.substring(ndx, pair.length);
        var wellFormed = new RegExp('=[ ]*"[^"]*"', "g");
        if (!wellFormed.test(attrval)) {
          var attrvalPattern = new RegExp('=(.*?)', 'g');
          attrval = attrval.replace(attrvalPattern, '=\"$1');
          // there's an IE bug that prevent this endquote from being appended
          // after the backreference.  no, seriously.
          attrval += '"';
        }
        // join the attribute parts
        var attr = attrname + attrval;
        cleanStartTag += " " + attr;
      }
    } 
    cleanStartTag += ">";

    return cleanStartTag;
  });

  // Makes all of the end tags lower case
  s = s.replace(/<\/\s*[\w]*\s*>/g, function (ref) {return ref.toLowerCase();});

  return s;
}


/*
 * convertAllFontsToSpans()
 *
 * Attempts to transform deprecated FONT nodes into well-formed XHTML-compliant 
 * markup.
 */
function convertAllFontsToSpans(s) { 
  
  startTagPattern = RegExp('<[^/]*font [^<>]*>', 'gi');
  var StartTags = s.match(startTagPattern);  
  if (StartTags) {
    for (var i = 0; i < StartTags.length; i++) {
      // adjacent tags get lost in some regexp searches in some browsers, so
      // we'll catch 'em here
      if (StartTags[i].indexOf('>') > 1) innerStartTags = StartTags[i].split('>');
      for (var x = 0; x < innerStartTags.length; x++) {
        if (innerStartTags[x]=='') continue;
        var thisTag = innerStartTags[x] + '>';
        modifiedStartTag = convertTagAttributeToStyleValue(thisTag, 
                                                            'face', 
                                                            'font-family');
        modifiedStartTag = convertTagAttributeToStyleValue(modifiedStartTag, 
                                                          'size', 
                                                          'font-size');
        modifiedStartTag = convertTagAttributeToStyleValue(modifiedStartTag, 
                                                          'color', 
                                                          'color');
        s = s.replace(thisTag, modifiedStartTag);
        
      }
    }
  }
  s = s.replace(/<font ([^>]*)>/gi, '<span $1>');
  s = s.replace(/<\/font>/gi, '</span>');
  
  // clean up extra spaces
  s = s.replace(/<span[ ]+style/gi, '<span style');
  return s;
}


/*
 * convertTagAttributeToStyleValue()
 *
 * Attempts to transfer specified HTML attributes into the 'style' attribute for 
 * the supplied start tag.
 */
function convertTagAttributeToStyleValue(s, attrName, styleAttrName) {
  // Get the style attribute value to convert
  attributePattern = new RegExp(attrName+'="([^"]*)"', 'gi');
  var matched = s.match(attributePattern);
  if (!matched) return s;
  var attrValue = RegExp.$1;

  // remove the old attribute
  s = s.replace(attributePattern, '');

  // add value as new style attribute value
  if (attrValue) { 
    if (attrName == 'size') attrValue = convertFontSizeToSpan(attrValue);
    stylePattern = new RegExp('(<[^>]*style="[^"]*)("[^>]*>)', 'gi');
    if (stylePattern.test(s)) {
      var style = RegExp.$1;
      if (style.indexOf(';') == -1) style += ';';
      s = s.replace(stylePattern, style + styleAttrName + ':' + attrValue + ';$2');
    } else {
      tagPattern = new RegExp('(<[^\/][^>]*)(>)', 'gi');
      s = s.replace(tagPattern, '$1 style="' + styleAttrName + ':' + attrValue + ';"$2');
    }

    //prevent colors with RGB values from aggregating with keyword / hex colors
    var colorPattern = new RegExp('(color\\:[\\s]*rgb[^;]*;)color\\:[^;]*;', 'gi');
    if (colorPattern.test(s)) {
      s = s.replace(colorPattern, '$1');
    }
  }

  return s;
}


/*
 * convertAllSpansToFonts()
 *
 * Attempts to transform well-formed SPAN nodes into WYSIWYG-acceptable formats.
 */
function convertAllSpansToFonts(s) {
  
  startTagPattern = RegExp('<[^\/]*span [^>]*>', 'gi');
  var StartTags = s.match(startTagPattern);  
  if (StartTags) {
    for (var i=0; i < StartTags.length; i++) {
      // adjacent tags get lost in some regexp searches in some browsers, so
      // we'll catch 'em here
      if (StartTags[i].indexOf('>') > 1) innerStartTags = StartTags[i].split('>');
      for (x = 0; x < innerStartTags.length; x++) {
        if (innerStartTags[x]=='') continue;
        var thisTag = innerStartTags[x] + '>';
        modifiedStartTag = convertTagStyleValueToAttribute(thisTag, 
                                                           'font-family', 
                                                           'face');
        modifiedStartTag = convertTagStyleValueToAttribute(modifiedStartTag, 
                                                         'font-size', 
                                                         'size');
        modifiedStartTag = convertTagStyleValueToAttribute(modifiedStartTag, 
                                                         'color', 
                                                         'color');
        
        var lastTwoCharsPattern = new RegExp(' >$','gim');
        modifiedStartTag = modifiedStartTag.replace(lastTwoCharsPattern, '>');  
        modifiedStartTag = modifiedStartTag.replace(/<span  /gi, '<span ');    
        
        s = s.replace(thisTag, modifiedStartTag);
      }
    }
  }
  s = s.replace(/<span ([^>]*)>/gi, '<font $1>');
  s = s.replace(/<\/span>/gi, '</font>');
  s = s.replace(/<span>/gi, '<font>');
  
  return s;
}


/*
 * convertTagStyleValueToAttribute()
 *
 * Attempts to transfer specified values within the 'style' attribute to single 
 * HTML attributes for the supplied start tag.
 */
function convertTagStyleValueToAttribute(s, styleVal, attrName) {
  // Get the style attribute value to convert
  stylePattern = new RegExp('style="[^"]*' + styleVal + ':([^;]*)[^"]*"', 'gi');
  var matched = s.match(stylePattern);
  if (!matched) return s;
  attrValue = RegExp.$1;

  if (attrValue) {
    
    attrValue = Trim(attrValue);  // extra spaces will cause problems in IE
    
    if (styleVal == 'color') {
      var rgbPattern = new RegExp('rgb\\([ ]*[\\d]*[ ]*,[ ]*[\\d]*[ ]*,[ ]*[\\d]*[ ]*\\)', 'gi'); 
      if (rgbPattern.test(attrValue)) {
        return s;  //TODO: add RGB to Hex conversion later
      }
    }
    if (styleVal == 'font-size') {
      attrValue = convertSpanSizeToFont(attrValue);
    }
    // remove the old style attribute
    valuePattern = new RegExp('(style="[^"]*)(' + styleVal + ':[^;]*)[;]*([^"]*")', 'gi');
    s = s.replace(valuePattern, '$1$3');

    // add value as new attribute
    stylePattern = new RegExp('(<[^>]*)(style="[^>]*>)', 'gi');
    s = s.replace(stylePattern, '$1' + attrName + '="' + attrValue + '" $2');

  }

  //remove empty style pairs
  s = s.replace(/style=""/gi, '');

  return s;
}

var FONT_SIZE_CONVERSIONS = [
  ['5', '180%'],
  ['4', '130%'],
  ['3', '100%'],
  ['2', '85%'],
  ['1', '78%']
];

function convertFontSizeToSpan(size) {
  return convertFontandSpanSizes(0, 1, size);
}

function convertSpanSizeToFont(size) {
  return convertFontandSpanSizes(1, 0, size);
}

function convertFontandSpanSizes(beforeIndex, afterIndex, size) {
  var conv = FONT_SIZE_CONVERSIONS; 
  size = Trim(size);
  for (z = 0; z < conv.length; z++) {
    if (size == conv[z][beforeIndex]) {
      size = conv[z][afterIndex];
      break;
    } 
  }
  return size;
}

function Trim(s) {
  return s.replace(/^\s+/, "").replace(/\s+$/, "");
}

// In order to optimize string concatenation, we're using arrays.  But their 
// use can break certain browsers, so we're adding the push() method to the 
// Array object in cases where that support is missing, as in the case of 
// IE 5.01
if (!Detect.SAFARI() && typeof Array.push == 'undefined') {
  Array.prototype.push = function() {
    var s;
    var i = 0;
    while(s = arguments[i++]) {
      this[this.length] = s;
    }
    return this.length;
  }
}

