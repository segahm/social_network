<?php
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/picts/picts_control.inc");
//update score of the last picture if one was rated
if (isset($_GET['prtypicstf']) && isset($_GET['prtypictid'])
	&& isset($_GET['prtypicthmbup'])){
	Picts::updatePartyPicScore($_GET['prtypictid'],($_GET['prtypicthmbup'] == 1),$_GET['prtypicstf']);
}
$picture_data = Picts::getRandomPic();
if (!is_null($picture_data)):
	$pict_t = new table_pictures();
	$users_t = new table_users();
	$name = funcs_getNames($picture_data[$users_t->field_name],false);
	$img_file = Picts::getPictureUrl($picture_data[$pict_t->field_pictureid]);
	//resize image to the target
	$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$img_file);
	$width = $array[0];
	$height = $array[1];
	$resizeByWidth = true;
	funcs_imageResize($width,$height,140,$resizeByWidth);
	//parse url so that we can referece it again
	$url_array = parse_url($_SERVER['REQUEST_URI']);
	$cur_url = $url_array['path'].'?'.((isset($url_array['query']))?$url_array['query'].'&':'');
	//calculate ups/downs
	$rate = explode('/',$picture_data[$pict_t->field_public]);
?>
<table width="100%" cellpadding="0" cellspacing="0">
	<tr><td align="center">
	<table width="150" cellpadding="0" cellspacing="0"><tr><td class="mainPrtyPnl">Party Pic...</td></tr></table>
	</td></tr>
	<tr><td align="center">
	<a title="<?=toHTML('"'.funcs_cutLength($picture_data[$pict_t->field_title],30).'"')?> by <?=$name?>" 
	href="/mypictures?pic=<?=$picture_data[$pict_t->field_pictureid]?>&usr=<?=$picture_data[$users_t->field_id]?>">
	<img width="<?=$width?>" height="<?=$height?>" src="<?=$img_file?>" border="0"></a>			
	<table cellpadding="3" cellspacing="0" align="center">
	<tr>
		<td valign="top"><span style="color:#CC3300;font-weight:bold;"><?=$rate[1]?></span></td>
		<td valign="bottom">
		<a style="text-decoration:none" 
			href="<?=$cur_url.'prtypicthmbup=0&prtypicstf='.base64_encode($picture_data[$pict_t->field_public])
				.'&prtypictid='.$picture_data[$pict_t->field_pictureid]?>">
				<img src="/images/thumbdown.gif" border="0"></a></td>
		<td valign="top">
		<a style="text-decoration:none" 
			href="<?=$cur_url.'prtypicthmbup=1&prtypicstf='.base64_encode($picture_data[$pict_t->field_public])
			.'&prtypictid='.$picture_data[$pict_t->field_pictureid]?>"><img src="/images/thumbup.gif" 
			border="0"></a></td>
		<td valign="baseline"><span style="color:#006633;font-weight:bold;"><?=$rate[0]?></span></td>
	</tr>
	</table>
	</td></tr>
</table>
<?php endif;?>