<html>
<head>
<link rel="Stylesheet" type="text/css" href="/styles/styles.css">
<style type="text/css">
body{
background-color:#F7F9F6;
}
.file_upload{
}
.hdn{
	position: absolute;
	display:none;
}
.list_file{
	padding-left:5px;border-bottom-color:#FF9999;border-bottom-width:1px;border-bottom-style:solid;
	padding-right:5px;
}
.sel_line{
	color:#FFFFFF;background-color:#336699;padding-left:5px;padding-right:5px;
}
</style>
<title>partyHub - My Photos</title>
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
function clkNotify(){
	if (!isnotify){
		return;
	}
	var elem = getObjectByID('file_'+file_elem.length,document);
	var capt = getObjectByID('caption_'+file_elem.length,document);
	if (elem != null && capt != null && allowed(elem.value)){
		//make the filenox invisible
		elem.className = "hdn";
		//move filebox from the div area
		var frmobj = getObjectByID('uploadFrm',document);
		frmobj.appendChild(elem);
		var str = filename(elem.value);
		capt.value = str.substr(0,str.lastIndexOf('.'));
		file_elem.push([elem,capt]);
		imgs.push(new Image());
		imgs[imgs.length-1] = new Image();
		imgs[imgs.length-1].src = elem.value;
		valid_elems++;
		createFilebox();
		updateList(true);
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
			text += '<tr><td onClick="msChs('+i+');" class="list_file" style="cursor:pointer;" id="line_'+i+'">'
			+filename(file_elem[i][0].value)+'</td></tr>';
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
		var obj = getObjectByID('line_'+sel_elem);
		obj.className = 'sel_line';
		obj = getObjectByID('m_cptn');
		obj.value = file_elem[sel_elem][1].value;
	}else{
		var obj = getObjectByID('m_cptn');
		obj.value = "";
	}
}
function createFilebox(){
	if (valid_elems >= max_elems){
		return;
	}
	var frmobj = getObjectByID('uploadFrm',document.document);
	var filebox = getObjectByID('file_upload',document.document);
	var name = "file_"+file_elem.length;
	var caption_name = "caption_"+file_elem.length;
	var elem = null;
	//do not create a new filebox if one already exists
	if (!(elem = getObjectByID(name,filebox))){
		//frmobj.removeChild(getObjectByID(name,frmobj));
		elem = document.createElement('INPUT');
		elem.name = name;
		elem.id = name;
		elem.type = "FILE";
		elem.className = "file_upload";
		elem = filebox.appendChild(elem);
		var caption_elem = document.createElement('INPUT');
		caption_elem.type = "TEXT";
		caption_elem.name = caption_name;
		caption_elem.id = caption_name;
		caption_elem.className = "hdn";
		frmobj.appendChild(caption_elem);
		elem.onclick = clkNotify;
	}
}
function msChs(elem){
	sel_elem = elem;
	updateList(false);
	drawSel();
}
function keyPressed(e){
	if (!e)e = event;
		if (e.keyCode){
			var cptn = getObjectByID('m_cptn');
			if (e.keyCode == KEY_UP){
				sel_elem = (sel_elem>0)?sel_elem-1:sel_elem;
				updateList(false);
				drawSel();
			}else if (e.keyCode == KEY_DOWN){
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
		document.onkeydown = keyPressed;
	}
	createFilebox();
	drawSel();
	updateList(true);
}
function remove(){
	if (sel_elem != -1){
		//turn off notify
		isnotify = false;
		var frmobj = getObjectByID('uploadFrm',document);
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
		if (valid_elems == (max_elems-1)){
			createFilebox();
		}
	}
}
var ld_flag = true;
function chkSbmt(){
	if (valid_elems>0){
		//check wherever we need to delete last invalid/dummy filebox/caption
		var frmobj = getObjectByID('uploadFrm',document);
		var file_upload = getObjectByID('file_upload',document);
		var name = "file_"+file_elem.length;
		if (elem = getObjectByID(name,file_upload)){
			file_upload.removeChild(elem);
			frmobj.removeChild(getObjectByID('caption_'+file_elem.length,frmobj));
		}
		awaitUpload = true;
		//prepare new view
		var text = '<table width="100%" height="300"><tr><td><table width="100%">'
			+'<tr><td><p align="center">Upload is currently in progress. The time might vary depending on your connection speed and file size</p></td></tr>'
			+'<tr><td align="center" valign="middle">'
			+'&nbsp;<img src="/images/loading.gif" border="0">'
			+'</td></tr></table>';
		setInnerHTML(getObjectByID('m_f'),text);
		frmobj.submit();
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
<center>
    <table cellpadding="0" cellspacing="0" border="0">
      <tr>
        <td><img src="/images/firefox/clear.gif" height=20 width=1></td>
      </tr>
      <tr><td>
			<table width="100%" cellpadding=0 cellspacing=0>
				<tr>
				  <td><img alt="Firefox Start" src="/images/firefox/title.gif" width=440 height=116></td>
				  <td style="background: url(/images/firefox/title_bg.gif) repeat-x;" width="100%"></td>
				  <td><img alt="Firefox Logo" src="/images/firefox/fox1.gif" width=105 height=116></td>
				  <td><img alt="Firefox Logo" src="/images/firefox/fox2.gif" width=19 height=85></td>
				</tr>
			  </table>
		</td></tr>
		<?php if(isset($_GET['submit'])):?><tr><td style="padding:5px;">
	<table width="500" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
		<td class="roundedge" width="100%">&nbsp;</td>
		<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
	</tr>
	<tr><td colspan="3" class="roundsides" style="padding-left:5px;">
	<?php if(!is_null($control->error)):?>
	<font class="error"><b>Error: </b><?=$control->error?></font>
	<?php else:?>
	<font class="dimmedTxt"><b>Status: </b>photos uploaded successfully</font>
	<?php endif;?>
	</td></tr>
	<tr>
		<td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
		<td class="roundedgebt" width="100%">&nbsp;</td>
		<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
	</tr>
	</table></td></tr>
	<?php endif;?>
      <tr><td>
	  	<form name="uploadFrm" enctype="multipart/form-data" method="post" action="/mypictures?frm=1&submit=1&newpict=<?=$_GET['newpict']?>&mozilla=1">
		<table width="520" cellpadding="0" cellspacing="0">
			<tr>
				<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
				<td class="roundedge" width="100%">&nbsp;</td>
				<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
			</tr>
			<tr><td colspan="3" class="roundsides" style="padding-left:5px;"><font color="#006633"><b>Easy Upload</b></font></td></tr>
			<tr><td colspan="3" class="roundsides"><table width="100%" cellpadding="0" cellspacing="0"><tr><td style="font-size:4px;" bgcolor="#663333">&nbsp;</td></tr></table></td></tr>
			<tr><td colspan="3" class="roundsides">
			<div id="m_f">
			<table width="100%" cellpadding="0" cellspacing="3">
			<tr><td style="padding:5px;"><p>Select multiple photos and enter their corresponding description (caption). Please note: only jpeg files are allowed (jpeg,jpg).</p>
			<div id="file_upload">
			</div>
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
		
		</form>
	  </td></tr>
    </table>
</center>
</body>
</html>
