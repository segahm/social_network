<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/home/home_controller.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/events/event_control.php");
auth_authCheck();
$controller = new home_controller(AUTH_USERID,AUTH_COLLEGEID,$_GET,$_POST);
$event = new eventster(AUTH_USERID,AUTH_COLLEGEID);
drawMain_1("Home");
drawMain_2();
?>
<!-- main panels/elements go into this table -->
<table width="100%" cellspacing="5" cellpadding="5">
	<?php if($controller->show(HOME_AWAITING_FRIENDS)):?>
	<tr><td>
	<table width="100%" cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr>
		<td colspan="3" class="roundsides" style="padding:5px;">
		&nbsp;<font style="color:#006633;text-decoration:underline"><b>Friendship Request[s]</b></font><hr color="#999966">
	<?php require_once($_SERVER['DOCUMENT_ROOT']
		.'/phpscripts/includes/home/friends.inc');?>
		</td>
	</tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
	</table>
	</td></tr>
	<?php endif;?>
	<?php if($controller->show(HOME_ANYMAIL)):?>
	<tr><td>
	<?php require_once($_SERVER['DOCUMENT_ROOT']
		.'/phpscripts/includes/home/mail.inc');?>
	</td></tr>
	<?php endif;?>
	<tr><td>
		&nbsp;<font style="color:#006633;text-decoration:underline"><b>Upcoming Events</b></font><hr color="#999966">
	<?php require_once($_SERVER['DOCUMENT_ROOT']
		."/phpscripts/includes/events/upcoming.inc");?>
	</td></tr>
</table>
<?php
drawMain_3(true);
?>