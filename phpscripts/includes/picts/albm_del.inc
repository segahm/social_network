<?php drawMain_1("My Photos");?>
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
	<form name="crtalbm_frm" action="/mypictures?delalbm=<?=$_GET['delalbm']?>" method="post">
	<table width="500" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
		<td class="roundedge" width="100%">&nbsp;</td>
		<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
	</tr>
	<tr><td colspan="3" class="sides" style="padding-left:5px;"><font color="#006633"><b>Confirm Delete</b></font></td></tr>
	<tr><td colspan="3" style="padding:5px;" class="sides">
	<table width="100%" cellpadding="0" cellspacing="0"><tr><td style="font-size:4px;" bgcolor="#663333">&nbsp;</td></tr></table>
	</td></tr>
	<tr><td colspan="3" class="sides" style="padding:5px;">
	<input type="hidden" name="albm" value="<?=$_GET['delalbm']?>">
	<p style="font-family:Verdana, Arial, Helvetica, sans-serif;font-size:14px;color:#666666">Please confirm that you would indeed like to delete this album.</p>
	<p style="color:#999999;font-weight:bold">Selected Album:</p>
	<font color="#993333">Album:</font> <?=toHTML($control->response_codes['title'])?> (photos: <?=$control->response_codes['photos']?>)
	</td></tr>
	<tr><td colspan="3" style="padding:5px;" class="sides">
	<table width="100%" cellpadding="0" cellspacing="0"><tr><td bgcolor="#663333" align="right">
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