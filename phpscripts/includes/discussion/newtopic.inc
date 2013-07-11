<?php
$fields = $control->fields;
$error = $control->getError();
?>
<script src="/html/utils.js" type="text/javascript" language="javascript"></script>
<script src="/html/richtext.js" type="text/javascript" language="javascript"></script>
<script type="text/javascript" language="javascript">
<!--
function handlesubmit(){
	if (submitRte('message','fld_message')){
		document.frm_tpc.submit();
	}
}
//-->
</script>
<table width="100%" cellpadding="5" cellspacing="5">
<tr><td>
<?php if(!is_null($error)):?>
<font class="error"><b>Error: </b><?=$error?></font>
<?php endif;?>
<?php ob_start('funcs_fillForm');?>
<form name="frm_tpc" action="/discussion?newtopic=1" method="post">
<input type="hidden" name="tpcid">
<input type="hidden" name="msgid">
<!-- create a field for input transfer-->
<input id="fld_message" type="hidden" name="message" value="">
<table width="100%" cellpadding="3" cellspacing="0" style="border-color:#3399CC">
<tr valign="top">
	<td class="tbltL" style="border-color:#336699" width="50" align="right">Topic:</td>
	<td class="tbltR" style="border-color:#336699;border-left-width:0px;">
	<input type="text" name="header" size="80"></td>
</tr>
<tr><td class="tbltL" style="border-color:#336699" >&nbsp;</td>
<td class="tbltR" style="border-color:#336699">
<div id="btns_div_dsp"></div>
</td></tr>
<tr valign="top">
	<td class="tbltL" style="border-color:#336699" width="50" align="right">Message:</td>
	<td class="tbltR" style="border-color:#336699;padding:5px;" height="100">
	<div id="txt_div_dsp"></div>
	<script language="javascript" type="text/javascript">
	<!--
	initRTE("/images/rte/", "/html/", "/styles/rte.css",'btns_div_dsp','txt_div_dsp');
	//-->
	</script>
	<script language="javascript" type="text/javascript">
	<!--
	writeRichText('message',getObjectByID('fld_message').value, 480, 200, true, false);
	//-->
	</script>
	</td>
</tr>
<tr><td colspan="2" align="center" style="border-color:#336699;border-width:1px;border-style:solid;">
<input type="button" class="custButton" value="submit" onClick="handlesubmit()">&nbsp;
<input type="button" class="custButton" value="cancel" onClick="javascript:document.location = '/discussion';"></td></tr>
<?php ob_end_flush();?>
</table>
</form>
</td></tr></table>