//init variables
var mouse_pos = null;	//keeps track of the mouse pos. for img palete
var isRichText = false;
var rng;
var currentRTE;
var allRTEs = "";

var glb_rte = null;	//rte field

var isIE;
var isGecko;
var isSafari;
var isKonqueror;

var imagesPath;
var includesPath;
var cssFile;
var parseHTML = true;
var buttons_display;
var textarea_display;

var lang = "en";
var encoding = "iso-8859-1";
var rte_fonts = new Array();
rte_fonts.push(['Arial','Arial, Helvetica, sans-serif']);
rte_fonts.push(['Courier','Courier New, Courier, mono']);
rte_fonts.push(['Times','Times New Roman, Times, serif']);
rte_fonts.push(['Verdana','Verdana, Arial, Helvetica, sans-serif']);
rte_fonts.push(['Georgia','georgia']);
rte_fonts.push(['Lucida Grande','lucida grande']);
rte_fonts.push(['Trebuchet','trebuchet ms']);
rte_fonts.push(['Webdings','webdings']);

var InsertLink = null;
var InsertImage = null;
var rte_md_checked = false; //which view - i.e. designer or text edit
var field_updated = false;	//wherever key was pressed since last parsing
function changeView(rte){
	rte_md_checked = !rte_md_checked;
	toggleHTMLSrc(rte);
}
//Usage: initRTE(imagesPath, includesPath, cssFile, genXHTML,buttons panel,textarea panel)
function initRTE(imgPath, incPath, css,btn_id,txt_id) {
	//set browser vars
	var ua = navigator.userAgent.toLowerCase();
	isIE = ((ua.indexOf("msie") != -1) && (ua.indexOf("opera") == -1) && (ua.indexOf("webtv") == -1)); 
	isGecko = (ua.indexOf("gecko") != -1);
	isSafari = (ua.indexOf("safari") != -1);
	isKonqueror = (ua.indexOf("konqueror") != -1);
	
	buttons_display = getObjectByID(btn_id);
	textarea_display = getObjectByID(txt_id);
	//check to see if designMode mode is available
	//Safari/Konqueror think they are designMode capable even though they are not
	if (getObjectByID && document.designMode && !isSafari && !isKonqueror) {
		isRichText = true;
	}

	document.onmouseover = raiseButton;
	document.onmouseout  = normalButton;
	document.onmousedown = lowerButton;
	document.onmouseup   = raiseButton;
	
	//set paths vars
	imagesPath = imgPath;
	includesPath = incPath;
	cssFile = css;
	
	if (isRichText)	document.writeln('<style type="text/css">@import url('+cssFile+');</style>');
	
	//for testing standard textarea, uncomment the following line
	//isRichText = false;
}
//Usage: writeRichText(fieldname, html, width, height, buttons, readOnly)
function writeRichText(rte, html, width, height, buttons, readOnly) {
	glb_rte = rte;
	var html_text = '';
	if (isRichText) {
		if (allRTEs.length > 0) allRTEs += ";";
		allRTEs += rte;
		
		if (readOnly) buttons = false;
		
		//adjust minimum table widths
		var tablewidth = width + 4;
		html_text += '<div class="rteDiv">';
		
		if (buttons == true) {
			html_text += '<table class="rteBack" cellpadding="0" cellspacing="0" width="' + tablewidth + '"><tr><td>';
			html_text += '<table cellpading="0" cellspacing="0" id="Buttons1_' + rte + '">';
			html_text += '	<tr>';
			html_text += '		<td>';
			html_text += '			<select style="color:#000000;font-size:10px;" id="fontname_' + rte + '" onchange="selectFont(\'' + rte + '\', this.id)">';
			html_text += '				<option value="Font" selected>Font</option>';
			for (var i in rte_fonts){
				html_text += '<option value="'+rte_fonts[i][1]+'">'+rte_fonts[i][0]+'</option>';
			}
			html_text += '			</select>';
			html_text += '		</td>';
			html_text += '		<td>';
			html_text += '			<select style="color:#000000;font-size:10px;" unselectable="on" id="fontsize_' + rte + '" onchange="selectFont(\'' + rte + '\', this.id);">';
			html_text += '				<option value="Size">Size</option>';
			html_text += '				<option value="2">Tiny</option>';
			html_text += '				<option value="3">Small</option>';
			html_text += '				<option value="4">Normal</option>';
			html_text += '				<option value="5">Large</option>';
			html_text += '				<option value="6">Huge</option>';
			html_text += '			</select>';
			html_text += '		</td>';
			html_text += '	</tr>';
			html_text += '</table>';
			html_text += '</td></tr></table>';
			html_text += '<table class="rteBack" cellpadding="0" cellspacing="0" width="' + tablewidth + '"><tr><td>';
			html_text += '<table cellpadding="1" cellspacing="0" id="Buttons2_' + rte + '">';
			html_text += '	<tr>';
			html_text += '		<td><img id="bold" class="rteImage" src="'+imagesPath+'bold.gif" width="25" height="24" alt="Bold" title="Bold" onclick="rteCommand(\'' + rte + '\', \'bold\', \'\')"></td>';
			html_text += '		<td><img class="rteImage" src="'+imagesPath+'italic.gif" width="25" height="24" alt="Italic" title="Italic" onclick="rteCommand(\'' + rte + '\', \'italic\', \'\')"></td>';
			html_text += '		<td><img class="rteImage" src="'+imagesPath+'underline.gif" width="25" height="24" alt="Underline" title="Underline" onclick="rteCommand(\'' + rte + '\', \'underline\', \'\')"></td>';
			html_text += '		<td><img class="rteVertSep" src="'+imagesPath+'blackdot.gif" width="1" height="20" border="0" alt=""></td>';
			html_text += '		<td><img class="rteImage" src="'+imagesPath+'left_just.gif" width="25" height="24" alt="Align Left" title="Align Left" onclick="rteCommand(\'' + rte + '\', \'justifyleft\', \'\')"></td>';
			html_text += '		<td><img class="rteImage" src="'+imagesPath+'centre.gif" width="25" height="24" alt="Center" title="Center" onclick="rteCommand(\'' + rte + '\', \'justifycenter\', \'\')"></td>';
			html_text += '		<td><img class="rteImage" src="'+imagesPath+'right_just.gif" width="25" height="24" alt="Align Right" title="Align Right" onclick="rteCommand(\'' + rte + '\', \'justifyright\', \'\')"></td>';
			html_text += '		<td><img class="rteImage" src="'+imagesPath+'justifyfull.gif" width="25" height="24" alt="Justify Full" title="Justify Full" onclick="rteCommand(\'' + rte + '\', \'justifyfull\', \'\')"></td>';
			html_text += '		<td><img class="rteVertSep" src="'+imagesPath+'blackdot.gif" width="1" height="20" border="0" alt=""></td>';
			html_text += '		<td><img class="rteImage" src="'+imagesPath+'numbered_list.gif" width="25" height="24" alt="Ordered List" title="Ordered List" onclick="rteCommand(\'' + rte + '\', \'insertorderedlist\', \'\')"></td>';
			html_text += '		<td><img class="rteImage" src="'+imagesPath+'list.gif" width="25" height="24" alt="Unordered List" title="Unordered List" onclick="rteCommand(\'' + rte + '\', \'insertunorderedlist\', \'\')"></td>';
			html_text += '		<td><img class="rteVertSep" src="'+imagesPath+'blackdot.gif" width="1" height="20" border="0" alt=""></td>';
			//html_text += '		<td><img class="rteImage" src="'+imagesPath+'outdent.gif" width="25" height="24" alt="Outdent" title="Outdent" onclick="rteCommand(\'' + rte + '\', \'outdent\', \'\')"></td>';
			//html_text += '		<td><img class="rteImage" src="'+imagesPath+'indent.gif" width="25" height="24" alt="Indent" title="Indent" onclick="rteCommand(\'' + rte + '\', \'indent\', \'\')"></td>';
			html_text += '		<td><div id="forecolor_' + rte + '"><img class="rteImage" src="'+imagesPath+'textcolor.gif" width="25" height="24" alt="Text Color" title="Text Color" onclick="dlgColorPalette(\'' + rte + '\', \'forecolor\', \'\')"></div></td>';
			html_text += '		<td><div id="hilitecolor_' + rte + '"><img class="rteImage" src="'+imagesPath+'bgcolor.gif" width="25" height="24" alt="Background Color" title="Background Color" onclick="dlgColorPalette(\'' + rte + '\', \'hilitecolor\', \'\')"></div></td>';
			html_text += '		<td><img class="rteVertSep" src="'+imagesPath+'blackdot.gif" width="1" height="20" border="0" alt=""></td>';
			html_text += '		<td><img id="insertlink_'+rte+'" class="rteImage" src="'+imagesPath+'hyperlink.gif" width="25" height="24" alt="Insert Link" title="Insert Link" onclick="dlgInsertLink(\'' + rte + '\', \'link\')"></td>';
			html_text += '		<td><img id="addimage_'+rte+'" class="rteImage" src="'+imagesPath+'image.gif" width="25" height="24" alt="Add Image" title="Add Image" onclick="addImage(\'' + rte + '\')"></td>';
			html_text += '		<td><img class="rteVertSep" src="'+imagesPath+'blackdot.gif" width="1" height="20" border="0" alt=""></td>';
			/*html_text += '		<td><img class="rteImage" src="'+imagesPath+'undo.gif" width="25" height="24" alt="Undo" title="Undo" onclick="rteCommand(\'' + rte + '\', \'undo\')"></td>';
			html_text += '		<td><img class="rteImage" src="'+imagesPath+'redo.gif" width="25" height="24" alt="Redo" title="Redo" onclick="rteCommand(\'' + rte + '\', \'redo\')"></td>';
		*/
			html_text += '	</tr>';
			html_text += '</table>';
			html_text += '</td></tr></table>';
		}
		buttons_display.innerHTML = html_text;
		html_text = '';
		html_text += '<iframe class="rteTextArea" id="' + rte + '" name="' + rte + '" width="' + width + 'px" height="' + height + 'px" src="' + includesPath + 'blank.htm"></iframe>';
		//if (!readOnly) html_text += '<br /><input type="checkbox" id="chkSrc' + rte + '" onclick="toggleHTMLSrc(\'' + rte + '\',true);" /> <label for="chkSrc' + rte + '">View Source</label>';
		//create a pallete
		html_text += '<iframe width="154" height="104" id="cp' + rte + '" src="' + includesPath + 'palette.htm" marginwidth="0" marginheight="0" scrolling="no" style="visibility:hidden; position: absolute;"></iframe>';
		html_text += '<input type="hidden" id="hdn' + rte + '" name="' + rte + '" value="">';
		html_text += '</div>';
		setInnerHTML(textarea_display,html_text);
		var hdnRte = getObjectByID('hdn' + rte);
		hdnRte.value = html;
		enableDesignMode(rte, html, readOnly);
	} else {
		if (!readOnly) {
			html_text += '<textarea name="' + rte + '" id="' + rte + '" style="width: ' + width + 'px; height: ' + height + 'px;">' + html + '</textarea>';
		} else {
			html_text += '<textarea name="' + rte + '" id="' + rte + '" style="width: ' + width + 'px; height: ' + height + 'px;" readonly>' + html + '</textarea>';
		}
		setInnerHTML(textarea_display,html_text);
	}
}

function enableDesignMode(rte, html, readOnly) {
	var frameHtml = "<html id=\"" + rte + "\">\n";
	frameHtml += "<head>\n";
	//to reference your stylesheet, set href property below to your stylesheet path and uncomment
	if (cssFile.length > 0){
		frameHtml += '<link rel="Stylesheet" type="text/css" href="'+cssFile+'">';
	}
	frameHtml += "</head>\n";
	frameHtml += "<body>\n";
	frameHtml += html + "\n";
	frameHtml += "</body>\n";
	frameHtml += "</html>";
	
	if (document.all) {
		var oRTE = frames[rte].document;
		oRTE.open();
		oRTE.write(frameHtml);
		oRTE.close();
		if (!readOnly) {
			oRTE.designMode = "On";
			frames[rte].document.attachEvent("onkeypress", function evt_ie_keypress(event) {ieKeyPress(event, rte);});
		}
	} else {
		try {
			var elem = getObjectByID(rte);
			if (!readOnly) elem.contentDocument.designMode = "on";
			try {
				oRTE = elem.contentWindow.document;
				oRTE.open();
				oRTE.write(frameHtml);
				oRTE.close();
				if (isGecko && !readOnly) {
					//attach a keyboard handler for gecko browsers to make keyboard shortcuts work
					oRTE.addEventListener("keypress", geckoKeyPress, true);
				}
			} catch (e) {
				//alert("Error preloading content.");
			}
		} catch (e) {
			//gecko may take some time to enable design mode.
			//Keep looping until able to set.
			if (isGecko) {
				setTimeout("enableDesignMode('" + rte + "', '" + html + "', " + readOnly + ");", 10);
			} else {
				return false;
			}
		}
	}
}
function updateRTE(rte) {
	if (!isRichText) return;
	
	//check for readOnly mode
	var readOnly = false;
	if (document.all) {
		if (frames[rte].document.designMode != "On") readOnly = true;
	} else {
		if (getObjectByID(rte).contentDocument.designMode != "on") readOnly = true;
	}
	
	if (isRichText && !readOnly) {
		//if viewing source, switch back to design view
		if (rte_md_checked) rte_md_checked = !rte_md_checked;
		setHiddenVal(rte);
	}
}
function setHiddenVal(rte) {
	//set hidden form field value for current rte
	var oHdnField = getObjectByID('hdn' + rte);
	if (oHdnField.value == null) oHdnField.value = "";
	if (document.all) {
		oHdnField.value = frames[rte].document.body.innerHTML;
	} else {
		oHdnField.value = getObjectByID(rte).contentWindow.document.body.innerHTML;
	}
	
	/*//if there is no content (other than formatting) set value to nothing
	if (stripHTML(oHdnField.value.replace(" ", "")) == "" &&
		oHdnField.value.toLowerCase().search("<hr") == -1 &&
		oHdnField.value.toLowerCase().search("<img") == -1) oHdnField.value = "";*/
}

function updateRTEs() {
	var vRTEs = allRTEs.split(";");
	for (var i = 0; i < vRTEs.length; i++) {
		updateRTE(vRTEs[i]);
	}
}

function rteCommand(rte, command, option) {
	//function to perform command
	var oRTE;
	if (document.all) {
		oRTE = frames[rte];
	} else {
		oRTE = getObjectByID(rte).contentWindow;
	}
	
	try {
		oRTE.focus();
	  	oRTE.document.execCommand(command, false, option);
		oRTE.focus();
	} catch (e) {
//		alert(e);
//		setTimeout("rteCommand('" + rte + "', '" + command + "', '" + option + "');", 10);
	}
}
var toggleTemp = null;
function toggleHTMLSrc(rte) {
	var oHdnField = getObjectByID('hdn' + rte);
	if (rte_md_checked) {
		//show html as source
		//showHideElement("Buttons1_" + rte, "hide");
		//showHideElement("Buttons2_" + rte, "hide");
		//display buttons
		toggleTemp = getInnerHTML(buttons_display);
		setInnerHTML(buttons_display,toggle_html_text);
		setHiddenVal(rte);
		if (document.all) {
			frames[rte].document.body.innerText = oHdnField.value;
		} else {
			var oRTE = getObjectByID(rte).contentWindow.document;
			var htmlSrc = oRTE.createTextNode(oHdnField.value);
			oRTE.body.innerHTML = "";
			oRTE.body.appendChild(htmlSrc);
		}
	} else {
		//we are unchecking the box
		//showHideElement("Buttons1_" + rte, "show");
		//showHideElement("Buttons2_" + rte, "show");
		//buttons_display.style.display = 'inline';
		setInnerHTML(buttons_display,toggleTemp);
		toggleTemp = null;
		if (document.all) {
			//fix for IE
			var output = escape(frames[rte].document.body.innerText);
			output = output.replace("%3CP%3E%0D%0A%3CHR%3E", "%3CHR%3E");
			output = output.replace("%3CHR%3E%0D%0A%3C/P%3E", "%3CHR%3E");
			frames[rte].document.body.innerHTML = unescape(output);
		} else {
			var oRTE = getObjectByID(rte).contentWindow.document;
			var htmlSrc = oRTE.body.ownerDocument.createRange();
			htmlSrc.selectNodeContents(oRTE.body);
			oRTE.body.innerHTML = htmlSrc.toString();
		}
	}
}

function dlgColorPalette(rte, command) {
	//function to display or hide color palettes
	setRange(rte);
	iLeftPos = mouse_pos[0];
	iTopPos = mouse_pos[1];
	//get dialog position
	var oDialog = getObjectByID('cp' + rte);
	var buttonElement = getObjectByID(command + '_' + rte);
	//var iLeftPos = getOffsetLeft(buttonElement);
	//var iTopPos = getOffsetTop(buttonElement) + (buttonElement.offsetHeight + 4);
	oDialog.style.left = (iLeftPos) + "px";
	oDialog.style.top = (iTopPos+10) + "px";
	
	if ((command == parent.command) && (rte == currentRTE)) {
		//if current command dialog is currently open, close it
		if (oDialog.style.visibility == "hidden") {
			showHideElement(oDialog, 'show');
		} else {
			showHideElement(oDialog, 'hide');
		}
	} else {
		//if opening a new dialog, close all others
		var vRTEs = allRTEs.split(";");
		for (var i = 0; i < vRTEs.length; i++) {
			showHideElement('cp' + vRTEs[i], 'hide');
		}
		showHideElement(oDialog, 'show');
	}
	
	//save current values
	parent.command = command;
	currentRTE = rte;
}
function dlgInsertLink(rte, command) {
	if (InsertLink!=null){
		return;
	}
	removeInsertImage();
	//function to open/close insert table dialog
	//save current values
	parent.command = command;
	currentRTE = rte;
	InsertLink = document.createElement('DIV');
	InsertLink.id = 'insert_link_div'+rte;
	InsertLink.style.backgroundColor  = '#FEFEFE';
	InsertLink.style.borderStyle = 'solid';
	InsertLink.style.borderWidth = '1px';
	InsertLink.style.borderColor = '#000000';
	InsertLink.style.position = 'absolute';
	InsertLink.style.left = mouse_pos[0];
	InsertLink.style.top = mouse_pos[1]-100;
	InsertLink = document.body.appendChild(InsertLink);
	//get currently highlighted text and set link text value
	setRange(rte);
	if (isIE) {
		linkText = stripHTML(rng.htmlText);
	} else {
		linkText = stripHTML(rng.toString());
	}
	var text = '<table cellpadding="4" cellspacing="0" border="0">'
		+'<tr><td colspan="2" class="error" id="error_link+'+rte+'" style="display:none" nowrap></td></tr>'
		+'<tr><td align="right" style="font-size:11px;">URL:</td><td><input type="text" id="urlfield_'+rte+'" size="40"></td></tr>'
		+'<tr><td align="right" style="font-size:11px;">Text:</td><td><input type="text" id="linkText_'+rte+'" size="40" value="'+linkText+'"></td></tr>'
		+'<tr><td align="right" style="font-size:11px;">Target:</td><td align="left"><select id="linkTarget_'+rte+'">'
		+'<option value="_blank">_blank</option><option value="_parent">_parent</option>'
		+'<option value="_self" selected>_self</option><option value="_top">_top</option></select>'
		+'</td></tr><tr><td colspan="2" align="center">'
		+'<input type="button" class="custButton" value="Insert Link" onClick="AddLink(\''+rte+'\');" />&nbsp;'
		+'<input type="button" class="custButton" value="Cancel" onClick="removeInsertLink();" />'
		+'</td></tr></table>';
	setInnerHTML(InsertLink,text);
	var linkField = getObjectByID('urlfield_'+rte);
	linkField.focus();
}
function removeInsertLink(){
	if (InsertLink != null){
		try{
			document.body.removeChild(InsertLink);
		}catch(e){
		}
		InsertLink = null;
	}
}
function AddLink(rte) {
	var linkField = getObjectByID('urlfield_'+rte);
	var textField = getObjectByID('linkText_'+rte);
	var targetField = getObjectByID('linkTarget_'+rte);
	//validate form
	if (linkField.value == '') {
		var errorField = getObjectByID('error_link+'+rte);
		errorField.style.display = 'inline';
		errorField.innerHTML = '<b>Error:</b> please enter the url';
		return false;
	}
	if (textField.value == '') {
		var errorField = getObjectByID('error_link+'+rte);
		errorField.style.display = 'inline';
		errorField.innerHTML = '<b>Error:</b> please enter the link text';
		return false;
	}
	
	var html = '<a href="'+linkField.value+'" target="'
		+targetField.options[targetField.selectedIndex].value
		+'">'+textField.value+'</a>';
	insertHTML(html);
	removeInsertLink();
	return true;
}

function popUpWin (url, win, width, height, options) {
	var leftPos = (screen.availWidth - width) / 2;
	var topPos = (screen.availHeight - height) / 2;
	options += 'width=' + width + ',height=' + height + ',left=' + leftPos + ',top=' + topPos;
	return window.open(url, win, options);
}

function setColor(color) {
	//function to set color
	var rte = currentRTE;
	var parentCommand = parent.command;
	
	if (document.all) {
		if (parentCommand == "hilitecolor") parentCommand = "backcolor";
		
		//retrieve selected range
		rng.select();
	}
	
	rteCommand(rte, parentCommand, color);
	showHideElement('cp' + rte, "hide");
}

function addImage(rte) {
	if (InsertImage!=null){
		return;
	}
	removeInsertLink();
	currentRTE = rte;
	InsertImage = document.createElement('DIV');
	InsertImage.id = 'insert_image_div'+rte;
	InsertImage.style.backgroundColor  = '#FEFEFE';
	InsertImage.style.borderStyle = 'solid';
	InsertImage.style.borderWidth = '1px';
	InsertImage.style.borderColor = '#000000';
	InsertImage.style.position = 'absolute';
	InsertImage.style.left = mouse_pos[0];
	InsertImage.style.top = mouse_pos[1]-100;
	InsertImage = document.body.appendChild(InsertImage);
	var text = '<table cellpadding="4" cellspacing="0">'
		+'<tr><td colspan="2" nowrap><span style="font-size:11px;">please note: you must specify width and height of the image</span></td></tr>'
		+'<tr><td colspan="2" class="error" id="error_link+'+rte+'" style="display:none"></td></tr>'
		+'<tr><td align="right" style="font-size:11px;">URL:</td><td><input type="text" id="imagefield_'+rte+'" size="40"></td></tr>'
		+'<tr><td colspan="2"><table cellpadding="3" cellspacing="0">'
		+'<tr><td style="font-size:11px;">Width:</td><td><input type="text" id="width_'+rte+'" size="3" maxlength="3"></td>'
		+'<td style="font-size:11px;">Height:</td><td><input type="text" id="height_'+rte+'" size="3" maxlength="3"></td>'
		+'<td style="font-size:11px;">Border:</td><td><input type="checkbox" id="border_'+rte+'"></td></tr>'
		+'</td></tr></table>'
		+'<tr><td colspan="2" align="center">position: <select name="float_'+rte+'"><option value="none"></option><option value="left">left</option><option value="right">right</option></select></td></tr>'
		+'<tr><td colspan="2" align="center">'
		+'<input type="button" class="custButton" value="Insert Image" onClick="VerifyImage(\''+rte+'\');" />&nbsp;'
		+'<input type="button" class="custButton" value="Cancel" onClick="removeInsertImage();" />'
		+'</td></tr></table>';
	setInnerHTML(InsertImage,text);
	//set focus to avoid weird text editing in bkground
	var linkField = getObjectByID('imagefield_'+rte);
	linkField.focus();
}
function removeInsertImage(){
	if (InsertImage != null){
		try{
			document.body.removeChild(InsertImage);
		}catch(e){
		}
		InsertImage = null;
	}
}
function VerifyImage(rte) {
	var linkField = getObjectByID('imagefield_'+rte);
	var widthField = getObjectByID('width_'+rte);
	var heightField = getObjectByID('height_'+rte);
	var borderField = getObjectByID('border_'+rte);
	var floatField = getObjectByID('float_'+rte);
	//validate form
	if (linkField.value == '') {
		var errorField = getObjectByID('error_link+'+rte);
		errorField.style.display = 'inline';
		errorField.innerHTML = '<b>Error:</b> please enter the url of the image';
		return false;
	}
	if (widthField.value == '' || !(new RegExp('^[0-9]{1,3}$').test(widthField.value))){
		var errorField = getObjectByID('error_link+'+rte);
		errorField.style.display = 'inline';
		errorField.innerHTML = '<b>Error:</b> please enter the width';
		return false;
	}
	if (heightField.value == '' || !(new RegExp('^[0-9]{1,3}$').test(widthField.value))){
		var errorField = getObjectByID('error_link+'+rte);
		errorField.style.display = 'inline';
		errorField.innerHTML = '<b>Error:</b> please enter the height';
		return false;
	}
	
	var html = '<img src="'+linkField.value+'" width="'+widthField.value+'" height="'+heightField.value+'"'
		+((borderField.checked)?' border="1" style="border-color:#000000;float:'+floatField.value+'"':'border="0"')+'/>';
	insertHTML(html);
	removeInsertImage();
	return true;
}
// Ernst de Moor: Fix the amount of digging parents up, in case the RTE editor itself is displayed in a div.
// KJR 11/12/2004 Changed to position palette based on parent div, so palette will always appear in proper location regardless of nested divs
function getOffsetTop(elm) {
	var mOffsetTop = elm.offsetTop;
	var mOffsetParent = elm.offsetParent;
	var parents_up = 2; //the positioning div is 2 elements up the tree
	
	while(parents_up > 0) {
		mOffsetTop += mOffsetParent.offsetTop;
		mOffsetParent = mOffsetParent.offsetParent;
		parents_up--;
	}
	
	return mOffsetTop;
}

// Ernst de Moor: Fix the amount of digging parents up, in case the RTE editor itself is displayed in a div.
// KJR 11/12/2004 Changed to position palette based on parent div, so palette will always appear in proper location regardless of nested divs
function getOffsetLeft(elm) {
	var mOffsetLeft = elm.offsetLeft;
	var mOffsetParent = elm.offsetParent;
	var parents_up = 2;
	
	while(parents_up > 0) {
		mOffsetLeft += mOffsetParent.offsetLeft;
		mOffsetParent = mOffsetParent.offsetParent;
		parents_up--;
	}
	
	return mOffsetLeft;
}

function selectFont(rte, selectname) {
	//function to handle font changes
	var idx = getObjectByID(selectname).selectedIndex;
	// First one is always a label
	if (idx != 0) {
		var selected = getObjectByID(selectname).options[idx].value;
		var cmd = selectname.replace('_' + rte, '');
		rteCommand(rte, cmd, selected);
		//set index of the select to default
		getObjectByID(selectname).selectedIndex = 0;
	}
}

function insertHTML(html) {
	var rte = currentRTE;
	
	var oRTE;
	if (document.all) {
		oRTE = frames[rte];
	} else {
		oRTE = getObjectByID(rte).contentWindow;
	}
	
	oRTE.focus();
	if (document.all) {
		var oRng = oRTE.document.selection.createRange();
		oRng.pasteHTML(html);
		oRng.collapse(false);
		oRng.select();
	} else {
		oRTE.document.execCommand('insertHTML', false, html);
	}
}

function showHideElement(element, showHide) {
	//function to show or hide elements
	//element variable can be string or object
	if (getObjectByID(element)) {
		element = getObjectByID(element);
	}
	
	if (showHide == "show") {
		element.style.visibility = "visible";
	} else if (showHide == "hide") {
		element.style.visibility = "hidden";
	}
}

function setRange(rte) {
	//function to store range of current selection
	var oRTE;
	if (document.all) {
		oRTE = frames[rte];
		var selection = oRTE.document.selection; 
		if (selection != null) rng = selection.createRange();
	} else {
		oRTE = getObjectByID(rte).contentWindow;
		var selection = oRTE.getSelection();
		rng = selection.getRangeAt(selection.rangeCount - 1).cloneRange();
	}
	return rng;
}

function stripHTML(oldString) {
	//function to strip all html
	var newString = oldString.replace(/(<([^>]+)>)/ig,"");
	
	//replace carriage returns and line feeds
   newString = newString.replace(/\r\n/g," ");
   newString = newString.replace(/\n/g," ");
   newString = newString.replace(/\r/g," ");
	
	//trim string
	newString = trim(newString);
	
	return newString;
}

function trim(inputString) {
   // Removes leading and trailing spaces from the passed string. Also removes
   // consecutive spaces and replaces it with one space. If something besides
   // a string is passed in (null, custom object, etc.) then return the input.
   if (typeof inputString != "string") return inputString;
   var retValue = inputString;
   var ch = retValue.substring(0, 1);
	
   while (ch == " ") { // Check for spaces at the beginning of the string
      retValue = retValue.substring(1, retValue.length);
      ch = retValue.substring(0, 1);
   }
   ch = retValue.substring(retValue.length - 1, retValue.length);
	
   while (ch == " ") { // Check for spaces at the end of the string
      retValue = retValue.substring(0, retValue.length - 1);
      ch = retValue.substring(retValue.length - 1, retValue.length);
   }
	
	// Note that there are two spaces in the string - look for multiple spaces within the string
   while (retValue.indexOf("  ") != -1) {
		// Again, there are two spaces in each of the strings
      retValue = retValue.substring(0, retValue.indexOf("  ")) + retValue.substring(retValue.indexOf("  ") + 1, retValue.length);
   }
   return retValue; // Return the trimmed string back to the user
}

//********************
//Gecko-Only Functions
//********************
function geckoKeyPress(evt) {
	//function to add bold, italic, and underline shortcut commands to gecko RTEs
	//contributed by Anti Veeranna (thanks Anti!)
	var rte = evt.target.id;
	field_updated = true;
	if (evt.ctrlKey) {
		var key = String.fromCharCode(evt.charCode).toLowerCase();
		var cmd = '';
		switch (key) {
			case 'b': cmd = "bold"; break;
			case 'i': cmd = "italic"; break;
			case 'u': cmd = "underline"; break;
		};

		if (cmd) {
			rteCommand(rte, cmd, null);
			
			// stop the event bubble
			evt.preventDefault();
			evt.stopPropagation();
		}
 	}
}

//*****************
//IE-Only Functions
//*****************
function ieKeyPress(evt, rte) {
	var key = (evt.which || evt.charCode || evt.keyCode);
	var stringKey = String.fromCharCode(key).toLowerCase();
	field_updated = true;
//the following breaks list and indentation functionality in IE (don't use)
//	switch (key) {
//		case 13:
//			//insert <br> tag instead of <p>
//			//change the key pressed to null
//			evt.keyCode = 0;
//			
//			//insert <br> tag
//			currentRTE = rte;
//			insertHTML('<br>');
//			break;
//	};
}

function raiseButton(e) {
	if(!e)e=window.event;
	mouse_pos = [e.clientX,e.clientY];
	var el = e.srcElement;
	
	className = el.className;
	if (className == 'rteImage' || className == 'rteImageLowered') {
		el.className = 'rteImageRaised';
	}
}

function normalButton(e) {
	if(!e)e=window.event;
	mouse_pos = [e.clientX,e.clientY];
	var el = e.srcElement;
	
	className = el.className;
	if (className == 'rteImageRaised' || className == 'rteImageLowered') {
		el.className = 'rteImage';
	}
}

function lowerButton(e) {
	if(!e)e=window.event;
	mouse_pos = [e.clientX,e.clientY];
	var el = e.srcElement;
	
	className = el.className;
	if (className == 'rteImage' || className == 'rteImageRaised') {
		el.className = 'rteImageLowered';
	}
}
function submitRte(rte,field){
	if (rte_md_checked){
		changeView(rte);
	}
	//formatHTML(rte);
	updateRTEs();
	getObjectByID(field).value = getObjectByID('hdn' + rte).value;
	return true;
}
/*function formatHTML(rte){
	//not format if the key wasn't pressed yet
	if (!field_updated){
		return;
	}
	field_updated = false;
	var rte_obj = getObjectByID(rte);
	var pos = getXY(rte_obj);
	//create loading image
	/*var LoadingImage = document.createElement('DIV');
	LoadingImage.style.backgroundColor  = '#FEFEFE';
	LoadingImage.style.borderStyle = 'solid';
	LoadingImage.style.borderWidth = '1px';
	LoadingImage.style.borderColor = '#000000';
	LoadingImage.style.position = 'absolute';
	LoadingImage.style.left = pos.x+100;
	LoadingImage.style.top = pos.y+200;
	LoadingImage = document.body.appendChild(LoadingImage);
	setInnerHTML(LoadingImage,'<img src="/images/loading.gif">');*/
	//begin formating
	/*var text =  getInnerHTML(rte_obj);
	text = removeNewlinesFromTags(text);
	text = convertAllFontsToSpans(text);
	text = convertTagAttributeToStyleValue(text);
	text = cleanNodesAndAttributes(text);
	setInnerHTML(rte_obj,text);
	//document.body.removeChild(LoadingImage);
}*/