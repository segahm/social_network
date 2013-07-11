<?php
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/main.php");
auth_authCheck();
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/pagedata/quickAddIn.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/mail/mail_controller.inc");
$controller = new mail_controller(AUTH_USERID,AUTH_COLLEGEID,$_GET,$_POST);
$RESPONSE_CODE = $controller->getResponse();
if ($RESPONSE_CODE == MAIL_RESPONSE_REDIRECT){
	header("Location: http://".$_SERVER['HTTP_HOST']."/home");
	exit;
}
drawMain_1("My Messages");
$SHOW_RIGHT = true;
if ($RESPONSE_CODE == MAIL_RESPONSE_VIEW){
	$file = $_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/mail/view.inc";
	$title = 'Message';
?>
<style>
.hdrCol{
	border-style:solid;border-color:#000000;border-width:1px;
	border-bottom-width:0px;
}
.hdrCol2{
	border-style:solid;border-color:#000000;border-width:1px;
	border-bottom-width:0px;border-left-width:0px;
}
</style>
<?php
}elseif ($RESPONSE_CODE == MAIL_RESPONSE_INBOX){
	$file = $_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/mail/incoming.inc";
	$title = 'Incoming Mail';
?>
<style>
.mail_unread{
	border-bottom-style:solid;
	border-bottom-color:#000000;border-bottom-width:1px;
	padding-right:5px;background-color:#F2EFD7
}
.mail_read{
	border-bottom-style:solid;
	border-bottom-color:#000000;border-bottom-width:1px;
	padding-right:5px;
	background-color:#FFFFFF;
}
</style>
<?php
}elseif($RESPONSE_CODE == MAIL_RESPONSE_SENDFAILED
		|| $RESPONSE_CODE == MAIL_RESPONSE_SENDSUCCESS
		|| $RESPONSE_CODE == MAIL_RESPONSE_OUTBOX){
	$file = $_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/mail/outbox.inc";
	$title = 'Compose Mail';
	$SHOW_RIGHT = false;
	require_once($_SERVER['DOCUMENT_ROOT']
	."/phpscripts/includes/pagedata/quickAddIn.inc");
}
drawMain_2();
?>
<table width="100%" cellpadding="5" cellspacing="5"><tr><td>
		<p class="roundHdr"><?=$title?></p>
		<p style="padding:10px;"><a href="/message">[inbox]</a>&nbsp;&nbsp;&nbsp;<a href="/message?outbox=1">[compose]</a></p>
		<?php require_once($file);?>

</td></tr></table>
<?php 
drawMain_3($SHOW_RIGHT);
function printArrays(){
	$quickAddIn = new quickAddIn(AUTH_USERID);
	return printMatchArrays($quickAddIn->getFriends(),$quickAddIn->getGroups());
}
?>