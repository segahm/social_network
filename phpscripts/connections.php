<? 
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/pagedata/profile_control.php");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/connections/con_control.inc");
auth_authCheck();

$control = new connection_control(AUTH_USERID,AUTH_COLLEGEID,$_GET,$_POST);
$message = $control->message;
$bodyFunction = null;
$SHOW_RIGHT_PANEL = true;
$SHOW_DRAG_DROP_SCRIPT = false;
drawMain_1("My Buddies");
if (isset($_GET['editgroups'])){
	$include_file = $_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/connections/edit.inc';
	$SHOW_RIGHT_PANEL = false;
}else{
	$include_file = $_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/connections/friends.inc';
}
?>
<script language="javascript" src="/html/utils.js"></script>
<?php
drawMain_2();
?>
<!-- main panels/elements go into this table -->
<table width="100%" cellspacing="5" cellpadding="5">
	<?php if(isset($message)):?>
	<tr>
		<td class="blkBrd">
		<table width="100%">
		<tr>
		<td><img src="/images/exclamation.gif"></td>
		<td align="left"><?=$message?></td>
		</tr>
		</table>
		</td>
	</tr>
	<?php endif;?>
	<tr>
		<td>
		<?php require_once($include_file);?>
		</td>
	</tr>
</table>
<?php
drawMain_3($SHOW_RIGHT_PANEL);
?>