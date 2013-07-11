function removeNewlinesFromTags(s) {
  s = s.replace(/<[^>]+>/g, function(ref) {
    ref = ref.replace(/\r/g, '');
    ref = ref.replace(/\n/g, ' ');
    return ref;
  });
  return s;
}
function isCompleteTags(str){
	var alwdEl = ['br','img','p','hr'] //single tags
	var list = new Array();	//holds pushed elements
	var inx = -1;
	while ((inx = str.search('[<>"]')) != -1){
		var elem = str.substr(inx,1);
		//update string start
		str = str.substr(inx+1);
		if (elem == '"'){
			if (list[list.length-1] == elem){
				list.pop();
			}else{
				list.push(elem);
			}
			continue;
		}
		//if in quotes continue
		if (list[list.length-1] == '"'){
			continue;
		}
		if (elem == '>'){
			if (list[list.length -1 ] == '<'){
				list.pop();
			}else{
				return 'can\'t continue, tag: ">" requires a "<" tag';
			}
			continue;
		}
		var sps_inx = -1;
		var el_name = '';
		if (str.substr(0,1) == '/'){
			//if this is a closing tag
			sps_inx = str.search('>');
			el_name = str.substring(1,sps_inx).toLowerCase();
			if (list[list.length -1] != el_name){
				return 'can\'t continue, invalid ending tag:'+el_name;
			}
			list.pop();
			//add '<'
			list.push('<');
			continue;
		}
		sps_inx = str.search('[\/ >]');
		el_name = str.substring(0,sps_inx).toLowerCase();
		//find if this element does not need a closing tag
		var needClosing = true;
		for (var i in alwdEl){
			if (alwdEl[i].toLowerCase() == el_name){
				needClosing = false;
				break;
			}
		}
		if (needClosing) list.push(el_name);
		list.push('<');
	}
	if (list.length != 0){
		return 'can\'t continue, tag: '+list[list.length -1]+' requires a closing tag';
	}
	return null;
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
