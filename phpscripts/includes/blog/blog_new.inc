<?php
$fields = $control->response_codes['fields'];
drawMain_1("Blog Home");
?>
<style type="text/css">
.sides{
	border-left-width:1px;border-left-style:solid;border-left-color:#C5C5C5;
		border-right-width:1px;border-right-style:solid;border-right-color:#C5C5C5;
}
/* rich editing*/
	
.rteImage {
	background-repeat:no-repeat;background-image: url(/images/edit/bg.gif);
	cursor: pointer;cursor: hand;
}
.rteImageRaised, .rteImage:hover {
	background-repeat:no-repeat;background-image: url(/images/edit/hoverbg.gif);
	cursor: pointer;
	cursor: hand;
}

.rteImageLowered, .rteImage:active {
	background-repeat:no-repeat;background-image: url(/images/edit/bg.gif);
	cursor: pointer;
	cursor: hand;
}

.rteVertSep {
	margin: 0 4px 0 4px;
}
/* buttons */
.rteBack {
	
}
.rteDiv {
	display: block;
	position: relative;
}
.rteTextArea{
	background-color:#FFFFFF;
}
</style>
<script language="javascript">
<!--
var toggle_html_text = '<p style="color:#666666;">almost all html/css tags are O.K. (no javascript)</p>';
-->
</script>
<script src="/html/utils.js" type="text/javascript" language="javascript"></script>
<script src="/html/richtext.js" type="text/javascript" language="javascript"></script>
<script language="javascript">
<!--
function handlesubmit(){
	if (submitRte('jsblog','text_field')){
		document.frm_blg.submit();
	}
}
//-->
</script>
<?php
drawMain_2();
$blog_t = new table_blog();
$stngs_t = new table_blogsettings();
?>
<table cellpadding="5" cellspacing="5" width="100%">
<tr><td valign="top">
	<span class="roundHdr">Posting to</span> <b><?=toHTML($control->response_codes['blgtitle'])?></span>
	<hr color="#666633">
<?php if(!is_null($control->error)):?>
<font class="error"><b>Error: </b><?=$control->error?></font>
<?php endif;?>
<?php ob_start('funcs_fillForm');?>
<form name="frm_blg" action="/blogger?frm=1&crtblg=<?=$control->response_codes['blgid']?>" method="post">
<input type="hidden" name="postid">
<!-- create a field for input transfer-->
<input id="text_field" type="hidden" name="text_field" value="">
<table width="100%" cellpadding="0" cellspacing="0">
<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
<table width="100%" cellpadding="3" cellspacing="0">
<tr valign="top">
	<td width="50" align="right" style="font-weight:bold;">Title:</td>
	<td>
	<input type="text" name="title" size="40" maxlength="40"></td>
</tr>
<?php ob_end_flush();?>
<tr><td>&nbsp;</td>
<td>
<div id="btns_div_dsp"></div>
</td></tr>
<tr valign="top">
	<td width="50" align="right" style="font-weight:bold;">Blog:</td>
	<td height="100">
	<div id="txt_div_dsp"></div>
	<script language="javascript" type="text/javascript">
	<!--
	initRTE("/images/rte/", "/html/", "",'btns_div_dsp','txt_div_dsp');
	//-->
	</script>
	<script language="javascript" type="text/javascript">
	<!--
	writeRichText('jsblog',getObjectByID('text_field').value, 440, 200, true, false);
	//-->
	</script>
	</td>
</tr>
<tr><td colspan="2" align="center">
<table width="100%"><tr><td align="left" nowrap><input type="checkbox" onClick="changeView('jsblog');"><font style="font-weight:bold"> - edit html</font></td>
<td width="100%" align="center">
<input type="button" class="custButton" style="background-color:#CC6600" value="submit" onClick="handlesubmit()">&nbsp;
<input type="button" class="custButton" style="background-color:#999999" value="cancel" onClick="javascript:document.location = '/blogger';">
<?php if (isset($_GET['edt'])):?>
<input type="button" class="custButton" style="background-color:#999999" value="delete" onClick="javascript:document.location = '/blogger?delpst=<?=$_GET['edt']?>';">
<?php endif;?>
</td>
</tr></table>
</td></tr>
</table>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</form>
</td></tr>
</table>
<?php
drawMain_3(true,true);
?>