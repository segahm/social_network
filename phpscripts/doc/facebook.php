<?php
	require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/interface.inc");
	require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/DBconnect.inc");
	require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/auth.inc");
	auth_authCheck();
	require_once($_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/invite/invite_control.inc');
$_GET['facebook']=1;
$control = new InviteControl(AUTH_USERID,$_GET,$_POST);
if (isset($control->response_codes[InviteControl::REDIRECT])){
	header("Location: http://".$_SERVER['HTTP_HOST'].$control->response_codes[InviteControl::REDIRECT]);
}else{
$fields = $control->response_codes['fields'];
?>
<html>
<head>
<title>partyHub - Social Network Invite</title>
<link rel="Stylesheet" type="text/css" href="/styles/styles.css">
<style type="text/css">
.cntcthdr{
	color:#FAFAFA;font-weight:bold;padding:0px;padding-left:10px;font-size:12px;
}
.cntcbrd{
	border-width:1px;border-style:solid;padding-left:10px;padding:10px;
}
td{
	font-family:Verdana, Arial, Helvetica, sans-serif
}
</style>
<script language="javascript" type="text/javascript">
<!--
function skip(){
	document.location = '/doc/invite';
}
//-->
</script>
</head>
<body bgcolor="#FFFFFF">
<form action="/doc/invite/social?sbm=1" method="post">
<table align="center" width="810" cellpadding="0" cellspacing="0" border="0">
<tr><td><img src="/images/logo1.jpg"></td></tr>
<tr>
	<td class="cntcthdr" style="font-size:14px;" bgcolor="#336699">
		Invite/Import Friends
	</td>
</tr>
<tr><td>
	<table width="100%">
	<tr valign="top"><td>
		<table><tr><td style="padding-top:30px;padding-left:20px;">
		<table width="250" cellpadding="2" cellspacing="2">
		<tr bgcolor="#006633">
			<td class="cntcthdr">How will this be used?</td>
		</tr>
		<tr valign="top">
			<td align="left" class="cntcbrd" style="border-color:#006633;font-size:11px;">
			We will use your login info only once to check wherever any of your friends are already on partyHub.com. If they are, then we will automatically send them friendship requests. If they are not, then we will send them an email invitation using the message you’ve typed.
			</td>
		</tr>
		</table>
		</td></tr><tr><td style="padding-top:50px;">
			<table width="350" cellpadding="2" cellspacing="2">
				<tr bgcolor="#336699">
					<td class="cntcthdr">theFaceBook friends</td>
				</tr>
				<tr valign="top">
					<td align="left" class="cntcbrd" style="border-color:#336699">
					To import your friends from theFacebook specify your login information and we will do it for you<p/>
					<table>
					<tr><td>Login:</td><td><input type="text" name="usr" size="15"> <span style="font-size:10px;">(school email)</span></tr>
					<tr><td>Password:</td><td><input type="password" name="pass" size="15"></tr>
					</table>
					<p align="center">
					<input type="submit" value="invite" class="custButton" 
			style="background-color:#336699;font-size:12px;">&nbsp;
			<input type="button" onClick="skip()" value="skip" class="custButton" 
			style="background-color:#999999;font-size:12px;">&nbsp;
					</p>
					</td>
				</tr>
			</table>
		</td></tr></table>
		
	</td><td><table>
		<tr><td style="padding-top:20px;">
			<table width="400" cellpadding="1" cellspacing="0"><tr><td bgcolor="#996666" align="right">
			<input type="submit" value="invite" class="custButton" 
			style="background-color:#336699;font-size:12px;">&nbsp;
			<input type="button" onClick="skip()" value="skip" class="custButton" 
			style="background-color:#999999;font-size:12px;">&nbsp;
			</td></tr></table>
		</td></tr>
		<tr><td style="padding-top:50px;padding-left:10px;">
		<table width="470" cellpadding="2" cellspacing="2">
		<tr bgcolor="#663333">
			<td class="cntcthdr">Write your own invite</td>
		</tr>
		<tr valign="top">
			<td align="left" class="cntcbrd" style="border-color:#663333">
			<span style="font-size:11px;font-weight:bold;">Message:</span>
			<p style="font-size:11px;line-height:20px;">
			<?php
			$replace = array('/  /','/\n/');
			$replace_with = array(' &nbsp;',' <br>');
			$message = '';
			ob_start('facebook_getcontents');
			include_once(FACEBOOK_INVITE);
			ob_end_flush();
			function facebook_getcontents(){
				$message = ob_get_contents();
			}
			preg_replace($replace,$replace_with,$message);
			?>
			</p>
			<?php ob_start('funcs_fillForm');?>
			<table><tr valign="top"><td>
			<span style="font-size:11px;font-weight:bold;">Or:</span>
			</td><td>
			<textarea name="body" cols="45" rows="8"></textarea><br>
			<?php
			$name = funcs_getNames(AUTH_USER_NAME);
			?>
			<p style="font-size:10px;">cheers,<br><b><?=$name[0].' '.$name[1]?></b></p>
			</td></tr></table>
			<?php ob_end_flush();?>
			</td>
		</tr>
		</table>
		</td></tr></table>
	</td>
	</table>
</td></tr>
<tr><td>
<? include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>
</td></tr>
</td></tr>
</table>
</form>
</body>
</html>
<?php }?>