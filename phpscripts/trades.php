<?php
	require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/main.php");
	require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/trades/trade_interface.inc");
	require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/trades/itemCoder.inc");
	auth_authCheck();
	drawMain_1("Trading Space!");
?>
<style type="text/css">
.mrktBtn{
	text-decoration:none;color:#999999;font-weight:bold;font-size:10px;
}
</style>
<?php
	drawMain_2();
	?>
	<table width="100%" cellpadding="2" cellspacing="0" bgcolor="#336699">
	<tr>
		<td align="center">
	<a style="color:#FFFFFF" href="/trades/marketplace?activity=1">My Recent Activity</a>
	&nbsp;<font color="#FFFFFF">|</font>&nbsp;
	<a style="color:#FFFFFF" href="/trades/marketplace?feedback=1&id=<?=AUTH_USERID?>">My Feedback</a>
	&nbsp;<font color="#FFFFFF">|</font>&nbsp;
	<a style="color:#FFFFFF" href="/trades/marketplace?history=1">My History</a>
	&nbsp;<font color="#FFFFFF">|</font>&nbsp;
	<a style="color:#FFFFFF" href="/trades/marketplace?feedback=1">Marketplace</a>
	&nbsp;<font color="#FFFFFF">|</font>&nbsp;
	<a style="color:#FFFFFF" href="/trades/marketplace?categ=<?=TYPE_ITEM_TEXTBOOK?>">Book Trade</a>
	&nbsp;<font color="#FFFFFF">|</font>&nbsp;
	<a style="color:#FFFFFF" href="/trades/post">Post New</a>
		</td>
	</tr>
	</table>
	<?php
	if (preg_match('|/trades/post|i',$_SERVER['REQUEST_URI'])){
		$file = $_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/trades/post/post.inc";
	}elseif (true || preg_match('|/trades/marketplace|i',$_SERVER['REQUEST_URI'])){
		$file = $_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/trades/marketplace/marketplace.inc";
	}else{
		header("Location: http://".$_SERVER['HTTP_HOST']."/home");
		exit;
	}
	require_once($file);
	drawMain_3(false);
?>