<!--
// Set the "inside" HTML of an element.
function setInnerHTML(element, toValue)
{
	// IE has this built in...
	if (typeof(element.innerHTML) != 'undefined')
		element.innerHTML = toValue;
	else
	{
		var range = document.createRange();
		range.selectNodeContents(element);
		range.deleteContents();
		element.appendChild(range.createContextualFragment(toValue));
	}
}
/**
* getXY()
 *
 * Returns the position of any element as an object.
 *
 * Typical usage:
 * var pos = getXY(object);
 * alert(pos.x + " " +pos.y);
 */
function getXY(el) {
  var x = el.offsetLeft;
  var y = el.offsetTop;
  if (el.offsetParent != null) {
    var pos = getXY(el.offsetParent);
    x += pos.x;
    y += pos.y;
  }
  return {x: x, y: y}
}
// Set the "outer" HTML of an element.
function setOuterHTML(element, toValue)
{
	if (typeof(element.outerHTML) != 'undefined')
		element.outerHTML = toValue;
	else
	{
		var range = document.createRange();
		range.setStartBefore(element);
		element.parentNode.replaceChild(range.createContextualFragment(toValue), element);
	}
}

// Get the inner HTML of an element.
function getInnerHTML(element)
{
	if (typeof(element.innerHTML) != 'undefined')
		return element.innerHTML;
	else
	{
		var returnStr = '';
		for (var i = 0; i < element.childNodes.length; i++)
			returnStr += getOuterHTML(element.childNodes[i]);

		return returnStr;
	}
}

function getOuterHTML(node)
{
	if (typeof(node.outerHTML) != 'undefined')
		return node.outerHTML;

	var str = '';

	switch (node.nodeType)
	{
		// An element.
		case 1:
			str += '<' + node.nodeName;

			for (var i = 0; i < node.attributes.length; i++)
			{
				if (node.attributes[i].nodeValue != null)
					str += ' ' + node.attributes[i].nodeName + '="' + node.attributes[i].nodeValue + '"';
			}

			if (node.childNodes.length == 0 && in_array(node.nodeName.toLowerCase(), ['hr', 'input', 'img', 'link', 'meta', 'br']))
				str += ' />';
			else
				str += '>' + getInnerHTML(node) + '</' + node.nodeName + '>';
			break;

		// 2 is an attribute.

		// Just some text..
		case 3:
			str += node.nodeValue;
			break;

		// A CDATA section.
		case 4:
			str += '<![CDATA' + '[' + node.nodeValue + ']' + ']>';
			break;

		// Entity reference..
		case 5:
			str += '&' + node.nodeName + ';';
			break;

		// 6 is an actual entity, 7 is a PI.

		// Comment.
		case 8:
			str += '<!--' + node.nodeValue + '-->';
			break;
	}

	return str;
}
function cutLength(str,l){
	if (str != null && str.length > l) str = str.substr(0,l-3)+'...';
	return str;
}
function getObjectByID(id,o) {
 var c,el,els,f,m,n; if(!o)o=document; if(o.getElementById) el=o.getElementById(id);
 else if(o.layers) c=o.layers; else if(o.all) el=o.all[id]; if(el) return el;
 if(o.id==id || o.name==id) return o; if(o.childNodes) c=o.childNodes; if(c)
 for(n=0; n<c.length; n++) { el=getObjectByID(id,c[n]); if(el) return el; }
 f=o.forms; if(f) for(n=0; n<f.length; n++) { els=f[n].elements;
 for(m=0; m<els.length; m++){ el=getObjectByID(id,els[n]); if(el) return el; } }
 return null;
}
function Trim(s) {
  return s.replace(/^\s+/, "").replace(/\s+$/, "");
}
//-->