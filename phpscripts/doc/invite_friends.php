<?php
	require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/main.php");
	auth_authCheck();
	require_once($_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/invite/invite_control.inc');
$control = new InviteControl(AUTH_USERID,$_GET,$_POST);
if (isset($control->response_codes[InviteControl::REDIRECT])):
	header("Location: http://".$_SERVER['HTTP_HOST']
		.$control->response_codes[InviteControl::REDIRECT]);
else:
	drawMain_1('Friend Invite');
	drawMain_2();
	$fields = $control->response_codes['fields'];
?>
<table width="100%" cellpadding="0" cellspacing="0"><tr><td style="padding:10px;">
<table cellpadding="0" cellspacing="0" width="100%">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
	<span class="roundHdr">Invite Friends</span>
	</td></tr>
	<tr><td colspan="3" class="roundsides"><hr color="#666633"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:10px;">
	<table width="100%"><tr valign="top"><td>
	<?php if (isset($control->response_codes['error'])):?>
	<span class="error" style="font-size:11px;"><b>Error:</b> <?=$control->response_codes['error']?></span>
	<?php elseif(isset($control->response_codes[InviteControl::SENT])):?>
	<span style="color:#006600;font-weight:bold;">your invite was sent successfully</span>
	<?php endif;?>
	<form action="/doc/invite?submit=1" method="post">
	<?php ob_start('funcs_fillForm');?>
	<input type="hidden" name="rdct">
	<span style="font-size:11px;font-weight:bold;">Emails:</span><br>
	<textarea name="emails" cols="40" rows="5"></textarea><br>
	<span style="font-size:11px;">separate multiple addresses using commas</span><br><br>
	<span style="font-size:11px;font-weight:bold;">Optional Message:</span><br>
	<textarea name="message" cols="40" rows="5"></textarea><br>
	<span style="font-size:11px;">max 250 characters (no html)</span>
	<center><input type="submit" class="custButton" value="send"></center>
	<?php ob_end_flush();?>
	</form>
	</td>
	<td style="padding:5px;" align="center">
	<p style="color:#006600">Feel free to let them know about these <b>cool</b> feautures:<br></p>
	<span style="color:#996666">Blog</span><br>
	<span style="color:#CC3300">Photo Album</span><br>
	<span style="color:#996633">Trading Space</span><br>
	<span style="color:#999999">Discussion Board</span><br>
	<span style="color:#3366CC">Events & Parties</span><br>
	<span style="color:#663333">Ride Board</span><br>
	<span style="font-size:11px;">...</span>
	</td>
	</tr></table>
	<?php
	$replace = array('/\s\s/','/[\n]/');
	$replace_with = array(' &nbsp;',' <br>');
	?>
	<span style="font-size:11px;font-weight:bold;">Main Body Message:</span>
	<?=$control->response_codes['msg']?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</td></tr></table>
<?php
	drawMain_3(true);
	endif;
?>