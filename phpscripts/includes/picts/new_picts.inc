<?php if (!isset($_GET['frm'])):?>
<!-- actual content frame -->
<html>
<head>
<link rel="Stylesheet" type="text/css" href="/styles/styles.css">
<title>partyHub - My Photos</title>
<style>
.sides{
	border-left-width:1px;border-left-style:solid;border-left-color:#C5C5C5;
		border-right-width:1px;border-right-style:solid;border-right-color:#C5C5C5;
}
.albm_btn{
	font-family:tahoma,arial,sans-serif;font-size: 10px;
	font-weight: normal;font-Style: normal;font-variant: normal;
	border-width:1px;
	color: #ffffff;background-color:#CC9966;
}
.list_file{
	padding-left:5px;border-bottom-color:#FF9999;border-bottom-width:1px;border-bottom-style:solid;
	padding-right:5px;
}
.sel_line{
	color:#FFFFFF;background-color:#336699;padding-left:5px;padding-right:5px;
}
</style>
<script language="javascript" src="/html/utils.js"></script>
<script language="javascript">
<!--
var imgs = new Array();
var file_elem = new Array();
var valid_elems = 0;
var sel_elem = -1;
var max_elems = 7;
var KEY_UP = 38;
var KEY_DOWN = 40;
var isnotify = true;
var awaitUpload = false;
var iTimerID = null;
function notify(error){
	if (awaitUpload){
		awaitUpload = false;
		ld_flag = false;
		document.location = '/mypictures?newpict=<?=$_GET['newpict']?>&m=1'+((error == false)?'':'&er=1');
	}
}
var lastLoaded = true;
function clkNotify(){
	if (!isnotify){
		return;
	}
	var elem = getObjectByID('file_'+file_elem.length,parent.form.document);
	var capt = getObjectByID('caption_'+file_elem.length,parent.form.document);
	if (elem != null && capt != null && allowed(elem.value)){
		ldtimes = 0;
		var str = filename(elem.value);
		capt.value = str.substr(0,str.lastIndexOf('.'));
		file_elem.push([elem,capt]);
		imgs.push(new Array(3));
		imgs[imgs.length-1][0] = new Image();
		imgs[imgs.length-1][0].src = elem.value;
		//wait until finished loading
		iTimerID = setInterval('isLoaded('+(imgs.length-1)+')',100);
	}
}
//is called by isLoaded once file is loaded
function clkNotify2(){
	clearInterval(iTimerID);
	//resize image
	var dmns = resizeImage(80,imgs[imgs.length-1][0].width,imgs[imgs.length-1][0].height);
	imgs[imgs.length-1][1] = dmns[0];
	imgs[imgs.length-1][2] = dmns[1];
	valid_elems++;
	updateList(true);
}
var waitNotifier = false;
function wait(){
	waitNotifier = !waitNotifier;
}
function isLoaded(indx){
	if (imgs[indx][0].complete && imgs[indx][0].width != 0 && imgs[indx][0].height != 0){
		clkNotify2();
	}
}
function allowed(str){
	var extension = ext(str);
	return ((extension!= null && (extension == 'jpeg' || extension == 'jpg'))?true:false);
}
function ext(str){
	var indx = str.lastIndexOf('.');
	if (indx == -1){
		return null;
	}
	return str.substr(indx+1).toLowerCase();
}
function updateList(ch_sel){
	var list = getObjectByID('pic_list',this.document);
	var text = '<table width="100%" cellpadding="0" cellspacing="0">';
	var count =0;
	for (var i=0;i<max_elems;i++){
		if (i<file_elem.length && allowed(file_elem[i][0].value)){
			text += '<tr><td onClick="msChs('+i+');">'
				+'<table class="list_file" width="100%" cellpadding="0" cellspacing="0">'
				+'<tr><td id="line_'+i+'_0" align="left">'+filename(file_elem[i][0].value)
				+'</td><td id="line_'+i+'_1" align="right">'+imgs[i][0].width+' x '+imgs[i][0].height+'</td></tr></table></td></tr>';
				++count;
		}
	}
	for (;count<max_elems;count++){
		text += '<tr><td class="list_file">&nbsp;</td></tr>';
	}
	text += '</table>';
	setInnerHTML(list,text);
	//change selected to the last image
	if (ch_sel){
		sel_elem = file_elem.length-1;
		drawSel();
	}
}
function drawSel(){
	if (sel_elem != -1){
		var obj = getObjectByID('line_'+sel_elem+'_0');
		obj.className = 'sel_line';
	 	obj = getObjectByID('line_'+sel_elem+'_1');
		obj.className = 'sel_line';
		obj = getObjectByID('img_preview');
		var text = '<table width="100" height="100" cellpadding="0" cellspacing="0">'
			+'<tr><td align="center" style="border-width:1px;border-color:#000000;border-style:solid;">'
			+'<img id="img_view" src="'+imgs[sel_elem][0].src+'" width="'+imgs[sel_elem][1]+'" height="'+imgs[sel_elem][2]+'" border=1></td></tr>'
			+'</table>';
		setInnerHTML(obj,text);
		obj = getObjectByID('m_cptn');
		obj.value = file_elem[sel_elem][1].value;
	}else{
		var elem = getObjectByID('img_preview');
		var text = '<table width="100" height="100" cellpadding="0" cellspacing="0">'
			+'<tr><td align="center" style="border-width:1px;border-color:#000000;border-style:solid;">'
			+'<span style="font-size:11px;">image preview</span></td></tr>'
			+'</table>';
		setInnerHTML(elem,text);
		elem = getObjectByID('m_cptn');
		elem.value = "";
	}
}
function selectFile(){
	if (valid_elems >= max_elems){
		return;
	}
	var frmobj = getObjectByID('uploadFrm',parent.form.document);
	var name = "file_"+file_elem.length;
	var caption_name = "caption_"+file_elem.length;
	var elem = null;
	//do not create a new filebox if one already exists
	if (!(elem = getObjectByID(name,frmobj))){
		//frmobj.removeChild(getObjectByID(name,frmobj));
		elem = parent.form.document.createElement('INPUT');
		elem.name = name;
		elem.id = name;
		elem.type = "FILE";
		elem = frmobj.appendChild(elem);
		var caption_elem = parent.form.document.createElement('INPUT');
		caption_elem.type = "TEXT";
		caption_elem.name = caption_name;
		caption_elem.id = caption_name;
		frmobj.appendChild(caption_elem);
		if (elem.attachEvent){
			elem.attachEvent('onreadystatechange',clkNotify);
			elem.attachEvent('onpropertychange',clkNotify);
			elem.attachEvent('onbeforedeactivate',clkNotify);
		}else if(elem.addEventListener){
			elem.addEventListener('onreadystatechange',clkNotify,false);
			elem.addEventListener('onpropertychange',clkNotify,false);
			elem.addEventListener('onbeforedeactivate',clkNotify,false);
			var box = document.createElement('DIV');
			box.style.backgroundColor  = '#FEFEFE';
			box.style.borderStyle = 'solid';
			box.style.borderWidth = '1px';
			box.style.borderColor = '#000000';
			box.style.position = 'absolute';
			box.style.padding = '10px';
			box.style.left = 150;
			box.style.top = 200;
			box = document.body.appendChild(box);
			setInnerHTML(box,'<p align="center">We currently do not support mozilla :-(</p>');
		}
	}
	if (elem.click){
		elem.click();
	}else if(elem.fireEvent){
		elem.fireEvent('onclick');
	}
}
function msChs(elem){
	sel_elem = elem;
	updateList(false);
	drawSel();
}
function keyPressed(){
	if (event.keyCode){
		var cptn = getObjectByID('m_cptn');
		if (event.keyCode == KEY_UP){
			sel_elem = (sel_elem>0)?sel_elem-1:sel_elem;
			updateList(false);
			drawSel();
		}else if (event.keyCode == KEY_DOWN){
			sel_elem = (sel_elem<(file_elem.length-1))?sel_elem+1:sel_elem;
			updateList(false);
			drawSel();
		}
	}
}
function init(){
	imgs = new Array();
	file_elem = new Array();
	if (document.attachEvent){
		document.attachEvent("onkeydown",keyPressed);
	}else if(document.addEventListener){
		document.addEventListener("onkeydown",keyPressed,false);
	}
	drawSel();
	updateList(true);
}
function resizeImage(target,width,height){
	if (width<=target && height<=target){
		return [width,height];
	}
	//resize by width first
	var percentage = (target / width); 
	width = Math.round(width * percentage); 
	height = Math.round(height * percentage); 
	percentage = (target / height); 
	width = Math.round(width * percentage); 
	height = Math.round(height * percentage);
	return [width,height];
}
function remove(){
	if (sel_elem != -1){
		//turn off notify
		isnotify = false;
		var frmobj = getObjectByID('uploadFrm',parent.form.document);
		frmobj.removeChild(getObjectByID('file_'+sel_elem,frmobj));
		frmobj.removeChild(getObjectByID('caption_'+sel_elem,frmobj));
		--valid_elems;
		var oldlength = file_elem.length;
		//remove one elemen fro imgs - works fine
		imgs.splice(sel_elem,1);
		file_elem.splice(sel_elem,1);
		//rename all fileboxes after this one
		for (var i=(sel_elem+1);i<oldlength;i++){
			var elem = getObjectByID('file_'+i,frmobj);
			elem.name = 'file_'+(i-1);
			elem.id = 'file_'+(i-1);
			elem = getObjectByID('caption_'+i,frmobj);
			elem.name = 'caption_'+(i-1);
			elem.id = 'caption_'+(i-1); 
		}
		sel_elem--;
		updateList(true);
		isnotify = true;
	}
}
var ld_flag = true;
function chkSbmt(){
	if (valid_elems>0){
		//check wherever we need to delete last invalid/dummy filebox/caption
		var frmobj = getObjectByID('uploadFrm',parent.form.document);
		var name = "file_"+file_elem.length;
		if (elem = getObjectByID(name,frmobj)){
			frmobj.removeChild(elem);
			frmobj.removeChild(getObjectByID('caption_'+file_elem.length,frmobj));
		}
		awaitUpload = true;
		//prepare new view
		var text = '<table width="100%" height="300"><tr><td><table width="100%">'
			+'<tr><td><p align="center">Upload is currently in progress. The time might vary depending on your connection speed and file size</p></td></tr>'
			+'<tr><td align="center" valign="middle">'
			+'<table width="171" height="48" bgcolor="#FFFFFF" style="border-width:1px;border-color:#000000;border-style:solid;" '
			+'cellpadding="0" cellspacing="0">'
			+'<tr><td background="/images/loading.jpg">'
			+'&nbsp;<img id="ld_bar" src="/images/loading_bar.gif" '
			+'style="position:absolute;"</td></tr></table></td></tr></table>'
			+'</td></tr></table>';
		setInnerHTML(getObjectByID('m_f'),text);
		frmobj.submit();
		moveBar();
	}
}
var tim = null;
var left_min = 210;
var left_max = 350;
var cur_hor = left_min;
var top = 255;
var spd = 60;
function moveBar(){
	if (ld_flag){
		cur_hor+=1;
		if (cur_hor>left_max)cur_hor=left_min;
		tim = setTimeout("moveBar()", spd);
		var ld_bar = getObjectByID('ld_bar');
		ld_bar.style.top = top;
		ld_bar.style.left = cur_hor;
	}
} 
function filename(str){
	var indx = Math.max(Math.max(str.lastIndexOf('/'),str.lastIndexOf('<?='\\\\'?>')),0);
	return str.substr(indx+1);
}
function saveCptn(){
	if (sel_elem != -1){
		var obj = getObjectByID('m_cptn');
		file_elem[sel_elem][1].value = obj.value;
	}
}
//-->
</script>
</head>
<body onLoad="init();">
<div id="temp"></div>
<table width="100%" cellpadding="0" cellspacing="0">
  <tr><td style="padding:5px;">
  <table width="100%" cellpadding="0" cellspacing="0">
  	<tr>
		<td align="left"><h1><font style="color:#993333;font-size:16px;">Upload Multiple Pictures</font></h1></td>
	</tr>
	<?php if(isset($_GET['m'])):?>
	<tr><td>
	<table width="500" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
		<td class="roundedge" width="100%">&nbsp;</td>
		<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
	</tr>
	<tr><td colspan="3" class="sides" style="padding-left:5px;">
	<?php if(isset($_GET['er'])):?>
	<font class="error"><b>Error: </b>we were unable to process one or more files</font>
	<?php else:?>
	<font class="dimmedTxt"><b>Status: </b>photos uploaded successfully</font>
	<?php endif;?>
	</td></tr>
	<tr>
		<td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
		<td class="roundedgebt" width="100%">&nbsp;</td>
		<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
	</tr>
	</table>
	</td></tr>
	<?php endif;?>
	<tr><td style="padding:10px;">
	<table width="520" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
		<td class="roundedge" width="100%">&nbsp;</td>
		<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
	</tr>
	<tr><td colspan="3" class="sides" style="padding-left:5px;"><font color="#006633"><b>Easy Upload</b></font></td></tr>
	<tr><td colspan="3" class="sides"><table width="100%" cellpadding="0" cellspacing="0"><tr><td style="font-size:4px;" bgcolor="#663333">&nbsp;</td></tr></table></td></tr>
	<tr><td colspan="3" class="sides">
	<div id="m_f">
	<table width="100%" cellpadding="0" cellspacing="3">
	<tr><td style="padding:5px;"><p>Select multiple photos and enter their corresponding description (caption). Please note: only jpeg files are allowed (jpeg,jpg).</p>
	<input type="button" onClick="selectFile();" value="Select Photo" class="albm_btn" 
		style="background-color:#996666;font-size:11px;font-weight:bold;">
	<!--lsit and picture begin-->
	<table width="100%"><tr valign="top"><td align="left">
	<table width="350" cellpadding="0" cellspacing="0" 
		style="border-color:#666666;border-style:solid;border-width:1px;">
	<tr><td align="center" bgcolor="#666699" style="font-family:Verdana, Arial, Helvetica, sans-serif;
		padding:2px;padding-left:5px;font-size:12px;font-weight:bold;color:#FFFFFF">Photo Selection</td></tr>
	<tr><td bgcolor="#CCCCCC" style="border-style:solid;border-color:#666666;
		border-width:1px;border-right-width:0px;">
		<table width="100%" cellpadding="0" cellspacing="0"><tr><td align="left" 
		style="font-weight:bold;color:#FFFFFF;padding-left:5px;">File</td>
		<td align="right"><input type="button" onClick="remove();" value="remove" class="albm_btn" 
		style="background-color:#999966;font-size:10px;font-weight:bold;"></td></tr></table>
		</td></tr>
	<tr><td>
		<div id="pic_list"></div>
	</td></tr>
	<tr><td bgcolor="#CCCCCC" style="padding-left:5px;">
	Caption: &nbsp;<input type="text" size="40" maxlength="100" id="m_cptn" name="m_cptn">&nbsp;&nbsp;
	<input type="button" value="save" class="albm_btn" style="background-color:#999966;font-size:10px;font-weight:bold;" onClick="saveCptn()">
	</td></tr>
	</table>
	</td><td align="right">
	<div id="img_preview"></div>
	</td></tr></table>
	<!--lsit and picture end-->
	</td></tr>
	</table>
	<p align="center" style="padding-bottom:5px;">
	<input type="button" value="upload" class="custButton" onClick="chkSbmt();">&nbsp;
	<input type="button" value="close" class="custButton" onClick="parent.close();"></p>
	</div>
	</td></tr>
	<tr><td colspan="3" class="sides"><table width="100%" cellpadding="0" cellspacing="0"><tr><td style="font-size:4px;" bgcolor="#663333">&nbsp;</td></tr></table></td></tr>
	<tr>
		<td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
		<td class="roundedgebt" width="100%">&nbsp;</td>
		<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
	</tr>
	</table>
	</td></tr>
  </table>
  </td></tr>
</table>
</body>
</html>
<?php else:?>
<!-- hidden frame content -->
<html>
<head>
<script language="javascript">
function ntfy(){
	if (parent.main.notify)parent.main.notify(<?=((is_null($control->error))?'false':'true')?>);
}
</script>
</head>
<body onLoad="ntfy();">
<form id="uploadFrm" name="uploadFrm" method="post" enctype="multipart/form-data" 
	action="/mypictures?frm=1&submit=1&newpict=<?=$_GET['newpict']?>">
</form>
</body></html>
<?php endif;?>