<?php
define('PICT_ID',0);
define('PICT_TITLE',1);
define('PICT_ISPUBLIC',2);
define('PICT_DATE',3);
define('PICT_IMAGE',4);
define('PICT_WIDTH',5);
define('PICT_HEIGHT',6);
define('PICT_DISP','pict_dip');
define('HOW_MANY_PER_LINE',4);
define('HOW_MANY_PER_PAGE',19);
//get the name of the owner of these albums
if (isset($control->response_codes['user'])){
	$users_t = new table_users();
	$user_name = funcs_getNames($control->response_codes['user'][$users_t->field_name]);
	$user_id = $control->response_codes['user'][$users_t->field_id];
	$user_school = $control->response_codes['user'][$users_t->field_school];
	$is_owner = false;
}else{
	$user_name = funcs_getNames(AUTH_USER_NAME);
	$is_owner = true;
}
$albums_t = new table_albums();
$pictures_t = new table_pictures();
$albminfo = $control->response_codes['albm_info'];
$picts_result = $control->getPictures();
$first_picture = null;
drawMain_1("My Photos");
?>
<script language="javascript" src="/html/utils.js"></script>
<script language="javascript">
<!--
<?php
	echo 'var isowner = '.(($is_owner)?'true;':'false;var userid = "'.$user_id.'";');
	//print arrays
	echo "var picts = new Array();\n";
	echo "function initPictures(){\n";

	if ($picture = db::fetch_assoc($picts_result)){
		$pictures_total = -1;
		$first_picture = $picture;
		$i = -1;
		do{
			$img_file = $picture[$pictures_t->field_pictureid];
			//compare times and see if a thumbnail has been created that we can use instead
			$i++;
			if (!Mydate::isBeforeCron(funcs_getTimeStamp($picture[$pictures_t->field_datetime]))){
				$img_file = Picts::getPictureUrl($img_file);
				//resize image to the target
				$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$img_file);
				$width = $array[0];
				$height = $array[1];
				$resizeByWidth = true;
				funcs_imageResize($width,$height,Picts::THUMB_WIDTH,$resizeByWidth);
				$resizeByWidth = false;
				funcs_imageResize($width,$height,Picts::THUMB_HEIGHT,$resizeByWidth);
			}else{
				//do not resize the image since it has been physically resized to
				//the right dimensions;
				//also to limit the number of request to disk using getimagesize
				//do not specify width or height in the <img tag
				$img_file = Picts::getSmallUrl($img_file);
				$width = 'null';
				$height = 'null';
			}
			echo 'picts['.$i.'] = new Array(7);';
			echo "\n";
			echo 'picts['.$i.']['.PICT_ID.'] = "'.$picture[$pictures_t->field_pictureid].'";';
			echo "\n";
			echo 'picts['.$i.']['.PICT_TITLE.'] = "'.toHTML($picture[$pictures_t->field_title]).'";'; 
			echo "\n";
			echo 'picts['.$i.']['.PICT_ISPUBLIC.'] = '.((is_null($picture[$pictures_t->field_public]))?'0':'1').';'; 
			echo "\n";
			echo 'picts['.$i.']['.PICT_DATE.'] = "'.date('jS F, Y',funcs_getTimeStamp($picture[$pictures_t->field_datetime])).'";';  
			echo "\n";
			echo 'picts['.$i.']['.PICT_IMAGE.'] = new Image;'; 
			echo "\n";
			echo 'picts['.$i.']['.PICT_IMAGE.'].src = "'.$img_file.'";';
			echo "\n";
			echo 'picts['.$i.']['.PICT_WIDTH."] = $width;";
			echo "\n";
			echo 'picts['.$i.']['.PICT_HEIGHT."] = $height;\n";
			}while($picture = db::fetch_assoc($picts_result));
		}
		echo "print_picts(0,".HOW_MANY_PER_PAGE.");}\n";
		?>
var newWin = null;
function windOpn(id){
	if (document.all){
		newWin = window.open("","_blank","directories=no,status=no,toolbar=no,menubar=no,location=no,width=600,height=450");
		newWin.document.write('<frameset rows=\"100%,0%\" border="0">'
			+'<frame name=main src="/mypictures?newpict='+id+'" frameborder="0" noresize '
			+'scrolling=no><frame name=form src="/mypictures?newpict='+id+'&frm=1" frameborder=0 noresize></frameset>');
	}else{
		newWin = window.open("/mypictures?newpict="+id+"&mozilla=1","_blank","directories=no,status=no,toolbar=no,menubar=no,location=no,width=600,height=550");
	}
	newWin.document.close();
	setTimeout('refreshThis()',600);
}
function refreshThis(){
	if (newWin != null){
		if (!newWin.closed){
			setTimeout('refreshThis()',1000);
		}else{
			window.location = location.href;
		}
	}
}
function print_picts(start,limit){
	var text = "";
	text += '<table width="100%" cellpadding="2" cellspacing="0"><tr><td align="left"><font style="color:#336699;font-size:10px;">&nbsp;showing '
		+Math.min(start+1,picts.length)+' ~ '+Math.min(start+limit,picts.length)+' out of '+picts.length
		+'</font></td><td align="right">'+((start>0)?'<a href="javascript:print_picts('+(start-limit)+','
		+limit+');">&#60;&#60;</a>&nbsp;&nbsp;':'')
		+(((start+limit)<picts.length)?'<a href="javascript:print_picts('+(start+limit)+','+limit+');">&#62;&#62;</a>':'')+'</td></tr></table>';
	text += '<table cellpadding="3" cellspacing="3"><tr valign="top">';
	var count = 0;
	<?php
		//if i'm browsing myself display create new picture image
		if ($is_owner){
			$albmid = $albminfo[$albums_t->field_albumid];
			echo <<<EOD
			text += '<td><table width="102" cellpadding="0" cellspacing="0" height="80"' 
			+'background="/images/albm_bg.gif" style="background-repeat:no-repeat;">'
			+'<tr><td align="center">'
			+'<a href="javascript:windOpn(\'$albmid\')"><img src="/images/pics_new.gif" border="0"></a></td></tr></table></td>';
			count++;
EOD;
		}
	?>
	var i = start;
	for (;i<Math.min(start+limit,picts.length);i++){
		count++;
		text += (count%<?=HOW_MANY_PER_LINE?> == 1)?'<tr valign="top">':'';
		text += '<td><table width="100%" cellpadding="0" cellspacing="0">'
			+'<tr><td align="center">'
			+'<a href="/mypictures?pic='+picts[i][<?=PICT_ID?>]+((isowner)?'':'&usr='+userid)
			+'"><img alt="dated: '+picts[i][<?=PICT_DATE?>]+'"'
			+((picts[i][<?=PICT_WIDTH?>] != null)?' width="'+picts[i][<?=PICT_WIDTH?>]+'"':'')+((picts[i][<?=PICT_HEIGHT?>] != null)?' height="'+picts[i][<?=PICT_HEIGHT?>]+'"':'')
			+' src="'+picts[i][<?=PICT_IMAGE?>].src+'" border="1" style="border-color:#000000"></a></td></tr>'
			+'<tr><td align="center"><span style="color:#008000;">'+((picts[i][<?=PICT_TITLE?>] != "")?cutLength(picts[i][<?=PICT_TITLE?>],15):'&nbsp;')+'</span></td></tr>'
			+((isowner)?'<tr><td align="center"><input name="chk_pict" type="radio" onClick="checkPict(this);" value="'+i+'"></td></tr>':'')
			+'</table></td>';
		text += (count%<?=HOW_MANY_PER_LINE?> == 0)?'</tr>':'';
	}
	if ((r = (count%<?=HOW_MANY_PER_LINE?>)) != 0){
		for(i=0;i<r;i++){text += '<td>&nbsp;</td>'};text += '</tr>';
	}
	text += "</table>";
	setInnerHTML(<?=PICT_DISP?>,text);
}
var chk_pict = null;
<?php if ($is_owner):?>
function checkPict(obj){
	chk_pict = obj;
	<?=(($is_owner)?'dispstngs();':'')?>
}
function dispstngs(){
	var dsparea = getObjectByID('settings');
	var text = '<form style="padding:0px;margin:0px;" action="/mypictures?cptn=1" method="post">'
		+'Caption: <input style="color:#006633" type="text" maxlength="100" value="'+picts[chk_pict.value][<?=PICT_TITLE?>]+'" size="45" name="caption">'
		+'&nbsp;<input type="submit" class="albm_btn" style="background-color:#666666" value="save settings"><br>'
		+'party pic: <input type="checkbox" name="public" value="1" '+((picts[chk_pict.value][<?=PICT_ISPUBLIC?>] == 1)?'checked':'')+'> - <a href="javascript:windscr(1);">read more</a>'
		+'<input type="hidden" name="id" value="'+picts[chk_pict.value][<?=PICT_ID?>]+'">'
		+'<input type="hidden" name="albmid" value="<?=$albminfo[$albums_t->field_albumid]?>">'
		+'</form>';
	setInnerHTML(dsparea,text);
}
function windscr(actn){
	if (actn == 1){
		var fld = getObjectByID('dscr');
		fld.style.display = 'block'
	}else if(actn == 2){
		var fld = getObjectByID('dscr');
		fld.style.display = 'none'
	}
}
function del_pict(){
	if (!chk_pict || !(chk_pict.checked)){
		return;
	}
	document.location = '/mypictures?delpict='+picts[chk_pict.value][<?=PICT_ID?>];
}
<?php endif;?>
//-->
</script>
<style>
.sides{
	border-left-width:1px;border-left-style:solid;border-left-color:#C5C5C5;
		border-right-width:1px;border-right-style:solid;border-right-color:#C5C5C5;
}
.albm_btn{
	font-family:tahoma,arial,sans-serif;font-size: 10px;
	font-weight: normal;font-Style: normal;font-variant: normal;
	border-width:1px;
	color: #ffffff;background-color:#999999;
}
</style>
<?php
drawMain_2("initPictures();");
?>
<table width="500" align="center" cellpadding="0" cellspacing="0">
<?php if ($is_owner):?>
<tr><td style="padding:10px;">
	<table width="100%" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
		<td class="roundedge" width="100%">&nbsp;</td>
		<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
	</tr>
	<tr><td id="settings" colspan="3" class="sides" style="padding-left:5px;">
	Change the corresponding caption by first selecting the photo
	</td></tr>
	<tr>
		<td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
		<td class="roundedgebt" width="100%">&nbsp;</td>
		<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
	</tr>
	</table>
	<div id="dscr" style="display:none;">
	<br>
	<table width="100%" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
		<td class="roundedge" width="100%">&nbsp;</td>
		<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
	</tr>
	<tr><td colspan="3" class="sides" style="padding-left:5px;">
	<p>Marking this photo as a party picture will automatically make it accessible to the rest of the student population. 
	It will be featured in our "party picture" section and will be displayed at different colleges. 
	We recomend using this section only if you have something funny (keep it appropriate!) 
	that you think will cheer up people a bit.<a href="javascript:windscr(2);">close</a></p>
	</td></tr>
	<tr>
		<td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
		<td class="roundedgebt" width="100%">&nbsp;</td>
		<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
	</tr>
	</table>
	</div>
</td></tr>
<?php endif;?>
<tr><td style="padding:10px;">
	<table width="100%" cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="sides" style="padding-top:3px;padding-bottom:3px;"><font style="color:#006633;"><b>&nbsp;Viewing: </b><?=toHTML($albminfo[$albums_t->field_title])?></font></td></tr>
	<tr><td colspan="3" class="sides" style="padding-left:5px;padding-right:5px;">
		<table width="100%" cellpadding="0" cellspacing="0" bgcolor="#996666"><tr><td>
			<?php if ($is_owner):?>
			<table cellpadding="0" cellspacing="3">
			<tr>
			<td><input type="button" onclick="del_pict();" class="albm_btn" style="background-color:#666666" value="delete picture"></td>
			</tr>
			</table>
			<?php else:
			//display last album created date
			if (!empty($first_picture)){
				$date = funcs_getTimeStamp($first_picture[$pictures_t->field_datetime]);
				echo '<span style="color:#FFFFFF;font-size:12px;padding-left:5px;">last picture uploaded on the '.(date('jS \o\f F, Y',$date)).'</span>';
			}
			endif;?>
		</td><td align="right" style="padding-right:5px;"><a style="color:#FFFFFF;text-decoration:none" href="/mypictures<?=(($is_owner)?'':'?usr='.$user_id)?>">back to albums</a></td></tr></table>
	</td></tr>
	<tr><td colspan="3" class="sides">
		<form name="picts_frm">
		<div id="<?=PICT_DISP?>">
		</div>
		</form>	
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
	</table>
</td></tr>
</table>
<?php
drawMain_3(true);
?>