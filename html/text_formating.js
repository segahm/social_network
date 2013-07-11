<!--
var textelem = null;
function init(elem){
	textelem = getObjectByID(elem);
	textelem.onselect = storeCaret;
	textelem.onclick = storeCaret;
	textelem.onkeyup = storeCaret;
	textelem.onchange = storeCaret;
}
function edit_highlight(obj,mode){
	obj.style.backgroundImage = "url(/images/edit/"+(mode?"hoverbg.gif)":"bg.gif)");
}
// Remember the current position.
function storeCaret()
{
	// Only bother if it will be useful.
	if (typeof(textelem.createTextRange) != 'undefined')
		textelem.caretPos = document.selection.createRange().duplicate();
}
// Replaces the currently selected text with the passed text.
function replaceText(text)
{
	// Attempt to create a text range (IE).
	if (typeof(textelem.caretPos) != "undefined" && textelem.createTextRange)
	{
		var caretPos = textelem.caretPos;

		caretPos.text = caretPos.text.charAt(caretPos.text.length - 1) == ' ' ? text + ' ' : text;
		caretPos.select();
	}
	// Mozilla text range replace.
	else if (typeof(textelem.selectionStart) != "undefined")
	{
		var begin = textelem.value.substr(0, textelem.selectionStart);
		var end = textelem.value.substr(textelem.selectionEnd);
		var scrollPos = textelem.scrollTop;

		textelem.value = begin + text + end;

		if (textelem.setSelectionRange)
		{
			textelem.focus();
			textelem.setSelectionRange(begin.length + text.length, begin.length + text.length);
		}
		textelem.scrollTop = scrollPos;
	}
	// Just put it on the end.
	else
	{
		textelem.value += text;
		textelem.focus(textelem.value.length - 1);
	}
}

// Surrounds the selected text with text1 and text2.
function surroundText(text1, text2)
{
	// Can a text range be created?
	if (typeof(textelem.caretPos) != "undefined" && textelem.createTextRange){
		var caretPos = textelem.caretPos;
		caretPos.text = text1 + caretPos.text + text2;
		caretPos.select();
	}else if (typeof(textelem.selectionStart) != "undefined"){
		// Mozilla text range wrap.
		var begin = textelem.value.substr(0, textelem.selectionStart);
		var selection = textelem.value.substr(textelem.selectionStart, textelem.selectionEnd - textelem.selectionStart);
		var end = textelem.value.substr(textelem.selectionEnd);
		var newCursorPos = textelem.selectionStart;
		var scrollPos = textelem.scrollTop;

		textelem.value = begin + text1 + selection + text2 + end;

		if (textelem.setSelectionRange){
			if (selection.length == 0)
				textelem.setSelectionRange(newCursorPos + text1.length, newCursorPos + text1.length);
			else
				textelem.setSelectionRange(newCursorPos, newCursorPos + text1.length + selection.length + text2.length);
			textelem.focus();
		}
		textelem.scrollTop = scrollPos;
	}else{
		// Just put them on the end, then.
		textelem.value += text1 + text2;
		textelem.focus(textelem.value.length - 1);
	}
}

// Checks if the passed input's value is nothing.
function isEmptyText(theField)
{
	// Copy the value so changes can be made..
	var theValue = theField.value;

	// Strip whitespace off the left side.
	while (theValue.length > 0 && (theValue.charAt(0) == ' ' || theValue.charAt(0) == '\t'))
		theValue = theValue.substring(1, theValue.length);
	// Strip whitespace off the right side.
	while (theValue.length > 0 && (theValue.charAt(theValue.length - 1) == ' ' || theValue.charAt(theValue.length - 1) == '\t'))
		theValue = theValue.substring(0, theValue.length - 1);

	if (theValue == '')
		return true;
	else
		return false;
}
//-->