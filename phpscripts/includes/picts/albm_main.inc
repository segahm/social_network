<?php
define('ALBUMS_DISP','albm_disp');
define('SCRIPT_ALBID','0');
define('SCRIPT_TITLE','1');
define('SCRIPT_PERM','2');
define('SCRIPT_ISDEF','3');
define('SCRIPT_TIMESTAMP','4');
define('SCRIPT_PICT','5');
define('SCRIPT_WIDTH','6');
define('SCRIPT_HEIGHT','7');
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
$album_array = array(); 	//will store all albums in here for php processing
$albums_t = new table_albums();
$pictures_t = new table_pictures();
$albums = $control->getAlbums();
drawMain_1("My Photos");
?>
<script language="javascript" src="/html/utils.js"></script>
<script language="javascript">
<!--
<?php
	echo 'var isowner = '.(($is_owner)?'true;':'false;var userid = "'.$user_id.'";');
	//print arrays
	echo "var albums = new Array();\n";
	echo "function initAlbum(){\n";
	if ($album = db::fetch_assoc($albums)){
		echo "var i=-1;";
		$pictures = array(); //for inner keeping
		$pictures_total = -1;
		do{
			$album_array = $album;
			$picture = $album[$pictures_t->field_pictureid];
			echo "i++;";
				if (!is_null($picture)){
					if (!Mydate::isBeforeCron(funcs_getTimeStamp($album[$pictures_t->field_datetime]))){
						$picture = Picts::getPictureUrl($picture);
						//resize image to the target
						$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture);
						$width = $array[0];
						$height = $array[1];
						$resizeByWidth = true;
						funcs_imageResize($width,$height,Picts::THUMB_WIDTH,$resizeByWidth);
						$resizeByWidth = false;
						funcs_imageResize($width,$height,Picts::THUMB_HEIGHT,$resizeByWidth);
					}else{
						//do not resize the image since it has been physically resized
						$picture = Picts::getSmallUrl($picture);
						$width = 'null';
						$height = 'null';
					}
					$pictures_total++;
					$pictures[] = $picture;
				}
				echo 'albums[i] = ["'.$album[$albums_t->field_albumid].'","'
					.toHTML(funcs_cutLength($album[$albums_t->field_title],15,null)).'","'
					.$album[$albums_t->field_permissions].'","'
					.$album[$albums_t->field_isdalbum].'",new Date('
					.funcs_getTimeStamp($album[$albums_t->field_date]).'),'
					.((is_null($picture))?'null':'new Image').'];';
				if (!is_null($picture)){
					echo "albums[i][5].src = '$picture';"
						."albums[i][6] = $width;"
						."albums[i][7] = $height;\n";
				}
			}while($album = db::fetch_assoc($albums));
		}
		echo "print_albums(0,24);}\n";
		?>
function print_albums(start,limit){
	var text = "";
	text += '<table width="100%" cellpadding="2" cellspacing="0"><tr><td align="left"><font style="color:#336699;font-size:10px;">&nbsp;showing '
		+Math.min(start+1,albums.length)+' ~ '+Math.min(start+limit,albums.length)+' out of '+albums.length
		+'</font></td><td align="right">'+((start>0)?'<a href="javascript:print_albums('+(start-limit)+','
		+limit+');">&#60;&#60;</a>&nbsp;&nbsp;':'')
		+(((start+limit)<albums.length)?'<a href="javascript:print_albums('+(start+limit)+','+limit+');">&#62;&#62;</a>':'')+'</td></tr></table>';
	text += '<table cellpadding="3" cellspacing="3"><tr valign="top">';
	var count = 0;
	<?php
		//if i'm browsing myself display create new album image
		if ($is_owner){
			echo <<<EOD
			text += '<td><table width="102" cellpadding="0" cellspacing="0" height="80"' 
			+'background="/images/albm_bg.gif" style="background-repeat:no-repeat;">'
			+'<tr><td align="center">'
			+'<a href="/mypictures?newalbm=1"><img src="/images/albm_new.gif" border="0"></a></td></tr></table></td>';
			count++;
EOD;
		}
	?>
	var i = start;
	for (;i<Math.min(start+limit,albums.length);i++){
		count++;
		text += (count%4 == 1)?'<tr valign="top">':'';
		text += '<td><table cellpadding="0" cellspacing="0">'
			+'<tr><td align="center"><table width="102" cellpadding="0" cellspacing="0" height="80" ' 
			+'background="/images/albm_bg.gif" style="background-repeat:no-repeat;"><tr><td align="center">'
			+((albums[i][<?=SCRIPT_PICT?>] != null)?'<a href="/mypictures?showalbm='+albums[i][<?=SCRIPT_ALBID?>]+((isowner)?'':'&usr='+userid)+'"><img'
			+((albums[i][<?=SCRIPT_WIDTH?>] != null)?' width="'+albums[i][<?=SCRIPT_WIDTH?>]+'"':'')+((albums[i][<?=SCRIPT_HEIGHT?>] != null)?' height="'+albums[i][<?=SCRIPT_HEIGHT?>]+'"':'')
			+' src="'+albums[i][<?=SCRIPT_PICT?>].src+'" border="0"></a>':((isowner)?'<a href="/mypictures?showalbm='
			+albums[i][<?=SCRIPT_ALBID?>]+'"><img src="/images/albm_newpics.gif" border="0"></a>':'<img src="/images/albm_empty.gif" border="0">'))
			+'</a></td></tr></table></td></tr>'
			+'<tr><td align="center"><font style="color:#008000;">'+albums[i][<?=SCRIPT_TITLE?>]+'</font></td></tr>'
			+'<tr><td align="center"><input name="chk_albm" type="radio" onClick="checkAlbm(this);"'
			+' value="'+i+'"></td></tr></table></td>';
		text += (count%4 == 0)?'</tr>':'';
	}
	<?php if ($is_owner):?>
		if (count == 1){
			//if no albums
			text += '<td><span style="color:#CC6633;font-size:16px;font-weight:bold;">Uploading pictures is easy!'
				+'</span><br><p style="color:#006633;font-size:14px;margin:5px;padding-left:10px;">-unlimited storage with fast upload</p>'
				+'<p style="color:#993300;font-size:14px;margin:5px;padding-left:10px;">-set share settings on all your photos</p></td>';
		}
	<?php endif;?>
	if ((r = (count%4)) != 0){
		for(i=0;i<r;i++){text += '<td>&nbsp;</td>'};text += '</tr>';
	}
	text += "</table>";
	setInnerHTML(<?=ALBUMS_DISP?>,text);
}
var chk_albm = null;
function checkAlbm(obj){
	chk_albm = obj;
}
function edit_albm(){
	if (!chk_albm || !(chk_albm.checked)){
		return;
	}
	document.location = '/mypictures?edit=1&albm='+albums[chk_albm.value][<?=SCRIPT_ALBID?>];
}
function del_albm(){
	if (!chk_albm || !(chk_albm.checked)){
		return;
	}
	document.location = '/mypictures?delalbm='+albums[chk_albm.value][<?=SCRIPT_ALBID?>];
}
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
drawMain_2("initAlbum();");
?>
<table width="100%" cellpadding="0" cellspacing="0">
<tr><td style="padding:10px;">
	<table width="500" cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="sides"><font style="color:#006633;"><b>&nbsp;<?=((isset($user_id))?$user_name[0].'\'s Photo Albums':'My Photo Albums')?></b></font></td></tr>
	<tr><td colspan="3" class="sides" style="padding-left:5px;padding-right:5px;">
		<table width="100%" cellpadding="0" cellspacing="0" bgcolor="#996666"><tr><td align="left">
			<?php if ($is_owner):?>
			<table cellpadding="0" cellspacing="3">
			<tr>
			<td><input type="button" onclick="edit_albm();" class="albm_btn" style="background-color:#CC9966" value="edit album"></td>
			<td><input type="button" onclick="del_albm();" class="albm_btn" style="background-color:#9999FF" value="delete album"></td>
			</tr>
			</table>
			<?php else:
			//display last album created date
			echo '<span style="color:#FFFFFF;font-size:12px;padding-left:5px;">';
			if (!empty($album_array)){
				$date = funcs_getTimeStamp($album_array[$albums_t->field_date]);
				echo 'last album created on the '.(date('jS \o\f F, Y',$date));
			}else{
				echo $user_name[0]." doesn't have a photo gallery that you can see";
			}
			echo '</span>';
			endif;?>
		</td></tr></table>
	</td></tr>
	<tr><td colspan="3" class="sides">
		<form name="albm_frm">
		<div id="<?=ALBUMS_DISP?>">
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