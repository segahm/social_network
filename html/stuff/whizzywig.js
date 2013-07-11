var whizzywig_version = 'Whizzywig v45'; 
//Copyright © 2005, John Goodman - john.goodman(at)unverse.net  *date 050524
//This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
//This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//A copy of the GNU General Public License can be obtained at: http://www.gnu.org/licenses/gpl.html

//CONFIG VARIABLES (please set outside script)
var buttonPath;  //path to toolbar button images;  "textbuttons" means don't use images
var cssFile;     //url of CSS stylesheet to attach to edit area
var imageBrowse; //path to page for image browser
var linkBrowse;  //path to page for link browser
var idTa;        //id of the textarea to whizzywig (param to makeWhizzyWig)
//OTHER GLOBALS
var oWhizzy; //object of Whizzy
var rng;  //range at current selection
var sel;  //current selection
var papa; //parent of current selection IE only
var trail = ''; //DOM tree path IE only

function makeWhizzyWig(txtArea, controls){ // make a WhizzyWig from the textarea
 if ((navigator.userAgent.indexOf('Safari') != -1 ) || !document.getElementById || !document.designMode ) {//no designMode
  alert("Whizzywig "+t("editor not available for your browser"));
  return;
 }
 idTa = txtArea;
 var taContent = o(idTa).defaultValue ? o(idTa).defaultValue : o(idTa).innerHTML ? o(idTa).innerHTML: ''; //anything in the textarea?
 taWidth = o(idTa).style.width ? o(idTa).style.width : o(idTa).cols + "em";  //grab the width and...
 taHeight = o(idTa).style.height ? o(idTa).style.height : o(idTa).rows + "em";  //...height from the textarea
 o(idTa).style.display = 'none';

 w('<style type="text/css">.selector{margin:0 5px 6px 0;} .btnImage {cursor:pointer; cursor:hand; margin-top:3px; border:2px outset; border-color: #ccc;}  .ctrl {background:ButtonFace; border:2px outset ButtonShadow; padding:5px;width:'+taWidth+'} #sourceTa{color:#060;font:mono small;}</style>');
 var sels = 'formatblock fontname fontsize';
 var buts = ' bold italic underline | left center right | number bullet indent outdent | undo redo  | color hilite rule | link image table | clean html spellcheck';
 var tbuts = ' tstart add_row_above add_row_below delete_row | add_column_before add_column_after delete_column | table_in_cell';
 var t_end = ''; //table controls end, if needed
 buts += tbuts;
 controls = controls.toLowerCase();
 if (!controls || controls == "all") controls = sels +' newline '+ buts +' source';
 else controls += tbuts;
 w('<div id="CONTROLS" class="ctrl">');
 gizmos = controls.split(' ');
 for (var i = 0; i < gizmos.length; i++) {
   if (gizmos[i]){ //make buttons and selects for toolbar, in order requested
      if (gizmos[i] == 'tstart') {
        w('<div id="TABLE_CONTROLS" style="display:none" unselectable="on">');
        t_end = '</div>';
      }
      else if (gizmos[i] == '|') w(' <big style="padding-bottom:2em">|</big> ');
      else if (gizmos[i] == 'newline') w('<br>');
    else if (sels.indexOf(gizmos[i]) != -1) makeSelect(gizmos[i])
    else if (buts.indexOf(gizmos[i]) != -1) makeButton(gizmos[i]);
   }
 }
 w(t_end) //table controls end
 w(fGo('LINK'));
 if (linkBrowse) w('<input type="button" onclick=doWin("'+linkBrowse+'"); value="'+t("Browse")+'"> ');
 w('Link address (URL): <input type="text" id="lf_url" size="60" value="http://">'+fNo(t("OK"),"insertLink()")+'</div>');//LINK_FORM end
 w(fGo('IMAGE'));
 if (imageBrowse) w('<input type="button" onclick=doWin("'+imageBrowse+'"); value="'+t("Browse")+'"> ');
 w(t('Image address (URL)')+': <input type="text" id="if_url" size="50" value="http://"> <label title='+t("to display if image unavailable")+'><br>'+t("Alternate text")+':<input id="if_alt" type="text" size="50"></label><br>'+t("Align")+':<select id="if_side"><option value="none">'+t("normal")+'</option><option value="left">'+t("left")+'</option><option value="right">'+t("right")+'</option></select> '+t("Border")+':<input type="text" id="if_border" size="20" value="none" title="'+t("number or CSS e.g. 3px maroon outset")+'"> '+t("Margin")+':<input type="text" id="if_margin" size="20" value="0" title="'+t("number or CSS e.g. 5px 1em")+'">'+fNo(t("Insert Image"),"insertImage()")+'</div>');//IMAGE_FORM end
 w(fGo('TABLE')+t("Rows")+':<input type="text" id="tf_rows" size="2" value="3"> <select id="tf_head"><option value="0">'+t("No header row")+'</option><option value="1">'+t("Include header row")+'</option></select> '+t("Columns")+':<input type="text" id="tf_cols" size="2" value="3"> '+t("Border width")+':<input type="text" id="tf_border" size="2" value="1"> '+fNo(t("Insert Table"),"makeTable()")+'</div>');//TABLE_FORM end
 w(fGo('COLOR')+'<input type="hidden" id="cf_cmd"><div style="background:#000;padding:1px;height:22px;width:125px;float:left"><div id="cPrvw" style="background-color:red; height:100%; width:100%"></div></div> <input type=text id="cf_color" value="red" size=17 onpaste=vC(value) onblur=vC(value)> <button type="button" onmouseover=vC() onclick=sC()>'+t("OK")+'</button>  <button type="button" onclick="hideDialogs();">'+t("Cancel")+' </button><br> '+t("click below or enter a")+' <a href="http://www.unverse.net/colortable.htm" target="_blank">'+t("color name")+'</a><br clear=all> <table border=0 cellspacing=1 cellpadding=0 width=480 bgcolor="#000000">');
 var webSafe = new Array("00","33","66","99","CC","FF")  //color table
 for (i=0; i<webSafe.length; i++){
  w("<tr>");
  for (j=0; j<webSafe.length; j++){
   for (k=0; k<webSafe.length; k++){
    var clr = webSafe[i]+webSafe[j]+webSafe[k];
    w('<td style="background:#'+clr+';height:12px;width:12px" onmouseover=vC("#'+clr+'") onclick=sC("#'+clr+'")></td>');
   }
  }
  w('</tr>');
 }
 w('</table></div>'); //end color table,COLOR_FORM
  w('</div>'); //controls end
  w('<div id="sourceBox" class="ctrl" style="width:'+taWidth+';height:'+taHeight+';position:absolute;z-index:21;left:0;top:30px;display:none;"><button id="showWYSIWYG" type="button" onclick="showDesign();">'+t("Hide HTML")+'</button><br> <textarea id="sourceTa" style="width:99%;height:95%">'+taContent+'</textarea></div>');

 w('<iframe style="width:'+taWidth+';height:'+taHeight+'" id="whizzyWig"></iframe><br>');

 w('<a href="http://www.unverse.net" style="font-size:xx-small;color:buttonface">'+whizzywig_version+'</a> ');
 var startHTML = "<html>\n";
 startHTML += "<head>\n";
 if (cssFile) {
  startHTML += "<link media=\"all\" type=\"text/css\" href=\"" + cssFile + "\" rel=\"stylesheet\">\n";
 }
 startHTML += "</head>\n";
 startHTML += "<body style='background-color:#fff'>\n";
 startHTML += taContent;
 startHTML += "</body>\n";
 startHTML += "</html>";
 if (document.frames) { //IE
  oWhizzy = frames['whizzyWig'];
 } else { //Moz
  oWhizzy = o("whizzyWig").contentWindow;
  try {
    o("whizzyWig").contentDocument.designMode = "on";
  } catch (e) { //not set? try again
    setTimeout('o("whizzyWig").contentDocument.designMode = "on";', 100);
  }
  oWhizzy.addEventListener("keypress", kb_handler, true); //make keyboard shortcuts work for Moz
 }
 oWhizzy.document.open();
 oWhizzy.document.write(startHTML);
 oWhizzy.document.close();
 if (document.all) oWhizzy.document.designMode = "On";
 oWhizzy.focus();
 whereAmI();
} //end makeWhizzyWig

function makeButton(button){  // assemble the button requested
 var ucBut = button.substring(0,1).toUpperCase();
 ucBut += button.substring(1);
 ucBut = ucBut.replace(/_/g,' ');
 if (!buttonPath || buttonPath == "textbuttons" || buttonPath == "" )
   var butHTML = '<input type="button"  unselectable="On" value="'+t(ucBut)+'" onClick=makeSo("'+button+'")>';
 else
   var butHTML = '<img class="btnImage" src="'+buttonPath+button+'.gif"  onMouseDown="selDown(this)" onMouseUp="selUp(this)" alt="'+t(ucBut)+'" title="'+t(ucBut)+'" onClick=makeSo("'+button+'")>';
 w(butHTML);
}

function fGo(id){ return '<div id="'+id+'_FORM" style="display:none"><hr> '; }//new form

function fNo(txt,go){ //form do it/cancel buttons
 return ' <input type="button" onclick="'+go+'" value="'+txt+'"> <input type="button" onclick="hideDialogs();" value='+t("Cancel")+'>';
}

function makeSelect(select){ // assemble the <select> requested
 var bH ="insHTML<span style=font-size:";
 var eH = ">";
 if (select == 'formatblock') {
  var h = "Heading";
 var values = ["<p>", "<p>", "<h1>", "<h2>", "<h3>", "<h4>", "<h5>", "<h6>", "<address>", "insHTML<big>",  "insHTML<small>", "<pre>"];
 var options = [t("Choose style")+":", t("Paragraph"), t(h)+" 1 ", t(h)+" 2 ", t(h)+" 3 ", t(h)+" 4 ", t(h)+" 5 ", t(h)+" 6", t("Address"), t("Big"), t("Small"), t("Fixed width<pre>")];
 } else if (select == 'fontname') {
 var values = ["Verdana, Arial, Helvetica, sans-serif", "Arial, Helvetica, sans-serif", "Comic Sans MS, fantasy", "Courier New, Courier, mono", "Georgia, serif", "Times New Roman, Times, serif", "Verdana, Arial, Helvetica, sans-serif"];
 var options = [t("Font")+":", "Arial", "Comic", "Courier New", "Georgia", "Times New Roman", "Verdana"];
 } else if (select == 'fontsize') {
  var values = ["3", "1", "2", "3", "4", "5", "6", "7"];
  var options = [t("Font size")+":", "1 "+t("Small"), "2", "3", "4", "5", "6", "7 "+t("Big")];
 } else { return }
 w('<select class="selector" id="' + select + '" onchange="doSelect(this.id);">');
 for (var i = 0; i < values.length; i++) {
  w('<option value="' + values[i] + '">' + options[i] + '</option>');
 }
 w('</select>');
}

function makeSo(command, option) {  //format selected text or line in the whizzy
 whereAmI();
 hideDialogs();
 if ("leftrightcenterjustify".indexOf(command) !=-1) command = "justify" + command;
 else if (command == "number") command = "insertorderedlist";
 else if (command == "bullet") command = "insertunorderedlist";
 else if (command == "rule") command = "inserthorizontalrule";
 else if ((command == "clean") && (sel != '')) command = "removeformat";
 switch (command) {
  case "color": o('cf_cmd').value="forecolor"; if (textSel()) o('COLOR_FORM').style.display = 'block'; break;
  case "hilite" : o('cf_cmd').value="backcolor"; if (textSel()) o('COLOR_FORM').style.display = 'block'; break;
  case "image" : o('IMAGE_FORM').style.display = 'block'; break;
  case "link" : if (textSel()) o('LINK_FORM').style.display = 'block'; break;
  case "html" : showHTML(); break;
  case "table" : doTable(); break;
  case "delete_row" : doRow('delete','0'); break;
  case "add_row_above" : doRow('add','0'); break;
  case "add_row_below" : doRow('add','1'); break;
  case "delete_column" : doCol('delete','0'); break;
  case "add_column_before" : doCol('add','0'); break;
  case "add_column_after" : doCol('add','1'); break;
  case "table_in_cell" : hideDialogs(); o('TABLE_FORM').style.display = 'block'; break;
  case "clean" : cleanUp(); break;
  case "spellcheck" : spellCheck(); break;
  default: oWhizzy.document.execCommand(command, false, option);
 }
 oWhizzy.focus;
}

function doSelect(selectname) {  //select on toolbar used - do it
 whereAmI();
 var idx = o(selectname).selectedIndex;
 var selected = o(selectname).options[idx].value;
 if ((selected.indexOf('insHTML') === 0)) {
  if (textSel()) insHTML(selected.replace(/insHTML/,''));
 } else {
 var cmd = selectname;
 oWhizzy.document.execCommand(cmd, false, selected);
 }
 o(selectname).selectedIndex = 0;
 oWhizzy.focus();
}

function vC(colour) { // view Colour
 if (!colour) colour = o('cf_color').value;
 o('cPrvw').style.backgroundColor = colour;
 o('cf_color').value = colour;
}

function sC(color) {  //set Color for text or background
 hideDialogs();
 var cmd = o('cf_cmd').value;
 if ((cmd == "backcolor") && (!document.all)) cmd = "hilitecolor";
 if  (!color) color = o('cf_color').value;
 if (document.selection) rng.select(); //else IE gets lost
 oWhizzy.document.execCommand(cmd, false, color);
 oWhizzy.focus();
}

function insertLink(URL) {
 hideDialogs();
 if (!URL) URL = o("lf_url").value;
 cmd = URL ? "createlink" : "unlink";
 if (document.selection) rng.select(); //else IE gets lost
 oWhizzy.document.execCommand(cmd, false, URL);
 oWhizzy.focus();
}

function insertImage(URL, side, border, margin, alt) { // insert image as specified
 hideDialogs();
 if (!URL) URL = o("if_url").value;
 if (URL) {
 if (!alt) alt = o("if_alt").value ? o("if_alt").value: URL.replace(/.*\/(.+)\..*/,"$1");
 img = '<img alt="' + alt + '" src="' + URL +'" ';
 if (!side) side = o("if_side").value;
 if ((side == "left") || (side == "right")) img += 'align="' + side + '"';
 if (!border)  border = o("if_border").value;
 if (border.match(/^\d+$/)) border+='px solid';
 if (!margin) margin = o("if_margin").value;
 if (margin.match(/^\d+$/)) margin+='px';
 //be clever with this
 if (border || margin) img += 'style="border:' + border + ';margin:' + margin + ';"';
 img += '/>';
  insHTML(img);
 }
 oWhizzy.focus();
}

function doTable(){ //show table controls if in a table, else make table
 if (trail.indexOf('TABLE') > 0){
  o('TABLE_CONTROLS').style.display = "block";
 }
 else o('TABLE_FORM').style.display = 'block';;
}

function doRow(toDo,below) { //insert or delete a table row
 var paNode = papa;
 while (paNode.tagName != "TR") paNode = paNode.parentNode;
 var tRow = paNode.rowIndex;
 var tCols = paNode.cells.length;
 while (paNode.tagName != "TABLE") paNode = paNode.parentNode;
 if (toDo == "delete") paNode.deleteRow(tRow);
 else {
  var newRow = paNode.insertRow(tRow+parseInt(below)); //1=below  0=above
   for (i = 0; i < tCols; i++){
    var newCell = newRow.insertCell(i);
    newCell.innerHTML = "#";
   }
 }
}

function doCol(toDo,after) { //insert or delete a column
 var paNode = papa;
 while (paNode.tagName != 'TD') paNode = paNode.parentNode;
 var tCol = paNode.cellIndex;
 while (paNode.tagName != "TABLE") paNode = paNode.parentNode;
 var tRows = paNode.rows.length;
 for (i = 0; i < tRows; i++){
  if (toDo == "delete") paNode.rows[i].deleteCell(tCol);
  else {
   var newCell = paNode.rows[i].insertCell(tCol+parseInt(after)); //if after = 0 then before
   newCell.innerHTML = "#";
  }
 }
}

function makeTable() { //insert a table
 hideDialogs();
 var rows = o('tf_rows').value;
 var cols = o('tf_cols').value;
 var border = o('tf_border').value;
 var head = o('tf_head').value;
 if ((rows > 0) && (cols > 0)) {
  var table = '<table border="' + border + '">';
  for (var i=1; i <= rows; i++) {
   table = table + "<tr>";
   for (var j=1; j <= cols; j++) {
    if (i==1) {
     if (head=="1") table += "<th>Title"+j+"</th>"; //Title1 Title2 etc.
     else table += "<td>"+j+"</td>";
    }
    else if (j==1) table += "<td>"+i+"</td>";
   else table += "<td>#</td>";
   }
   table += "</tr>";
  }
  table += " </table>";
  insHTML(table);
 }
}

function doWin(URL) {  //popup  for browse function
 window.open(URL,'popWhizz','toolbar=no,location=no,status=no,menubar=no,scrollbars=yes,resizable=yes,width=640,height=480,top=100');
}

function spellCheck() {  //check spelling with plugin if available
 if (document.all) {
 try {
  var tmpis = new ActiveXObject("ieSpell.ieSpellExtension");
  tmpis.CheckAllLinkedDocuments(document);
 }
 catch(exception) {
  if(exception.number==-2146827859) {
  if (confirm("ieSpell is not installed on your computer. \n Click [OK] to go to download page."))
    window.open("http://www.iespell.com/download.php","DownLoad");
  } else {
   alert("Error Loading ieSpell: Exception " + exception.number);
  }
 }
 } else {
   if (confirm("Click [OK] for instructions to download and install SpellBound on your computer. \n If you already have it, click [Cancel] then right click in the edit area and select 'Check Spelling'."))
     window.open("http://spellbound.sourceforge.net/install","DownLoad");
 }
}

function cleanUp(){  //tidy up crud inserted by Micro$oft Orifice
 whereAmI();
 var iHTML = oWhizzy.document.body.innerHTML;
 iHTML = iHTML.replace(/<\/?DIR>/gi, "");
 iHTML = iHTML.replace(/<\/?FONT[^>]*>/gi, "");
 iHTML = iHTML.replace(/<\/?SPAN[^>]*>/gi, "");
 iHTML = iHTML.replace(/ CLASS=\"[^\"]*\"/gi, "");
 iHTML = iHTML.replace(/ STYLE=\"[^\"]*\"/gi, "");
 iHTML = iHTML.replace(/<\/?o:[^>]*>/gi, "");
 iHTML = iHTML.replace(/<\/?st1:[^>]*>/gi, "");
 iHTML = iHTML.replace(/[‘]/g, "&lsquo;"); //smartquotes
 iHTML = iHTML.replace(/[“]/g, '&ldquo;');
 iHTML = iHTML.replace(/[”]/g, '&rdquo;');
 iHTML = iHTML.replace(/align=justify/gi, "");
 iHTML = iHTML.replace(/<([^>]+)><\/\1>/gi, ""); //empty tag
 oWhizzy.document.body.innerHTML = iHTML;
}

function hideDialogs() {
 o('LINK_FORM').style.display='none';
 o('IMAGE_FORM').style.display='none';
 o('COLOR_FORM').style.display='none';
 o('TABLE_FORM').style.display='none';
 o('TABLE_CONTROLS').style.display='none';
}

function showDesign() {
 oWhizzy.document.body.innerHTML = o('sourceTa').value;
 o('sourceBox').style.display = 'none';
 o('CONTROLS').style.display = 'block';
}

function showHTML() {
 var b = oWhizzy.document.body;
 b.innerHTML = b.innerHTML.replace(/<([^>]+)><\/\1>/gi, ""); //empty tag
 o('sourceTa').value =  (window.get_xhtml) ? get_xhtml(b) : b.innerHTML;
 o('CONTROLS').style.display = 'none';
 o('sourceBox').style.display = 'block';
}

function syncTextarea() { //tidy up before we go-go
 if (o('sourceBox').style.display == 'block') showDesign();
 var b = oWhizzy.document.body;
 b.innerHTML = b.innerHTML.replace(/<([^>]+)><\/\1>/gi, ""); //empty tag
 o(idTa).value = (window.get_xhtml) ? get_xhtml(b) : b.innerHTML;
}

function kb_handler(evt) { // keyboard controls for Mozilla
 var w = evt.target.id;
 if (evt.ctrlKey) {
  var key = String.fromCharCode(evt.charCode).toLowerCase();
  var cmd = '';
  switch (key) {
   case 'b': cmd = "bold"; break;
   case 'i': cmd = "italic"; break;
   case 'u': cmd = "underline"; break;
  };
  if (cmd) {
   makeSo(cmd, true);
   evt.preventDefault();  // stop the event bubble
   evt.stopPropagation();
  }
 }
}

function insHTML(html) { // insert arbitrary HTML at current selection
 whereAmI();
 if (!html) html = prompt("Enter some HTML to insert:", "");
 if (!html) return;
 if (document.all) { //IE
  html = html + rng.htmlText;
  try { oWhizzy.document.selection.createRange().pasteHTML(html); } //
  catch (e) { }// catch error if range is bad for IE
 } else { //Moz
  if (sel) html = html + sel;
  var fragment = oWhizzy.document.createDocumentFragment();
  var div = oWhizzy.document.createElement("div");
  div.innerHTML = html;
  while (div.firstChild) {
   fragment.appendChild(div.firstChild);
  }
  sel.removeAllRanges();
  rng.deleteContents();
  var node = rng.startContainer;
  var pos = rng.startOffset;
  switch (node.nodeType) {
   case 3: if (fragment.nodeType == 3) {
    node.insertData(pos, fragment.data);
    rng.setEnd(node, pos + fragment.length);
    rng.setStart(node, pos + fragment.length);
   } else {
    node = node.splitText(pos);
    node.parentNode.insertBefore(fragment, node);
    rng.setEnd(node, pos + fragment.length);
    rng.setStart(node, pos + fragment.length);
   }
   break;
   case 1: node = node.childNodes[pos];
    node.parentNode.insertBefore(fragment, node);
    rng.setEnd(node, pos + fragment.length);
    rng.setStart(node, pos + fragment.length);
   break;
  }
  sel.addRange(rng);
 }
 oWhizzy.focus();
}

function whereAmI() {//get current selected range if available 
 oWhizzy.focus();
 if (document.all) { //IE
  sel = oWhizzy.document.selection;
  if (sel != null) {
   rng = sel.createRange();
   switch (sel.type) {
    case "Text":case "None":
     papa = rng.parentElement(); break;
    case "Control":
     papa = rng.item(0); break;
    default:
     papa = frames['whizzyWig'].document.body;
   }
   var paNode = papa;
   trail = papa.tagName + '>' +sel.type;
   while (paNode.tagName != 'BODY') {
    paNode = paNode.parentNode;
    trail = paNode.tagName + '>' + trail;
   }
   window.status = trail;
  }
  sel = rng.text;
 } else { //Moz
  sel = oWhizzy.getSelection();
  if (sel != null) rng = sel.getRangeAt(sel.rangeCount - 1).cloneRange();
 }
}

function textSel() {
	if (sel && (sel != "")) return true; 
	else {alert(t("Select some text first")); return false;}
}
function o(objectName) { return document.getElementById(objectName); } //get element by ID
function w(string) { return document.write(string); } //document write
function t(key) {return (window.language && language[key]) ? language[key] :  key;} //translation
function selDown(ctrl) {ctrl.style.borderStyle = 'inset';}  // mouse down on a button
function selUp(ctrl) {ctrl.style.borderStyle = 'outset';} // mouse up on a button