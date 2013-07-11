<?php drawMain_1("My Photos");
$data = $control->response_codes['pic_info'];
$pict_t = new table_pictures();
$albm_t = new table_albums();
//resize image
$img_file = $data[$pict_t->field_pictureid];
$img_file = Picts::getPictureUrl($img_file);
//resize image to the target
$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$img_file);
$width = $array[0];
$height = $array[1];
$resizeByWidth = true;
funcs_imageResize($width,$height,300,$resizeByWidth);
?>
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
</style>
<?php drawMain_2();?>
<table width="100%" cellpadding="0" cellspacing="0">
  <tr><td style="padding:5px;">
  <table width="100%" cellpadding="0" cellspacing="0">
  	<tr>
		<td align="left"><h1><font style="color:#993333;font-size:16px;">My Pictures</font></h1></td>
	</tr>
	<tr><td style="padding:10px;">
	<form name="crtalbm_frm" action="/mypictures?delpict=<?=$_GET['delpict']?>" method="post">
	<input type="hidden" name="albid" value="<?=$data[$pict_t->field_albumid]?>">
	<input type="hidden" name="pictid" value="<?=$_GET['delpict']?>">
	<table width="500" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
		<td class="roundedge" width="100%">&nbsp;</td>
		<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
	</tr>
	<tr><td colspan="3" class="sides" style="padding-left:5px;"><font color="#006633"><b>Confirm Delete</b></font></td></tr>
	<tr><td colspan="3" class="sides"><table width="100%" cellpadding="0" cellspacing="0"><tr><td style="font-size:4px;" bgcolor="#663333">&nbsp;</td></tr></table></td></tr>
	<tr><td colspan="3" class="sides" style="padding:5px;">
	<p style="font-family:Verdana, Arial, Helvetica, sans-serif;font-size:14px;color:#666666">Please confirm that you would indeed like to delete this picture.</p>
	<p class="dimmedTxt" style="font-weight:bold">Selected Picture: <font style="color:#006633"><?=toHTML($data[$pict_t->field_title])?></font></p>
	<p align="center"><img src="<?=$img_file?>" width="<?=$width?>" height="<?=$height?>" border=1></p>
	</td></tr>
	<tr><td colspan="3" style="padding:5px;" class="sides">
	<table width="100%" cellpadding="0" cellspacing="0"><tr><td bgcolor="#663333" align="right">
	<input type="button" onclick="document.location = '/mypictures?showalbm=<?=$data[$pict_t->field_albumid]?>'"
		class="albm_btn" style="background-color:#999966" value="cancel">&nbsp;
	<input type="submit" class="albm_btn" value="confirm delete">
	</td></tr></table>
	</td></tr>
	<tr>
		<td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
		<td class="roundedgebt" width="100%">&nbsp;</td>
		<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
	</tr>
	</table>
	</form>
	</td></tr>
  </table>
  </td></tr>
</table>
<?php
drawMain_3(true);
?>