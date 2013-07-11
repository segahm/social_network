<?php
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/interface.php");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/DBconnect.php");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/auth.php");
ob_start();
header('Content-type: text/html');
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache"); 
function drawMain_1($title){
?>
<html>
<head>
<?php include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/meta.html");?>
<title>[site:name] - <?php echo $title;?></title>
<?php
}
function drawMain_2($page,$onload=""){
?>
</head>
<body onLoad="<?php echo $onload?>">
<!-- school/site image -->
<?php include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/schoolImagePanel.php");?>

<table width="750" cellpadding=0 cellspacing=0 border=0 align="center">
<tr><td valign=top>
	<table width=100% cellpadding="0" cellspacing="0">
	<!-- search panel-->
	<tr><td colspan=2 valign>
		<? require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/searchPanel.php");?>
	</td></tr>
	<tr valign=top>
		<!-- left panel -->
		<td width=140>
			<table width="140" style="padding-left:10px" cellpadding=2 cellspacing=0>
				<tr height="20">
					<?php if ($page==1){?><td class="mls">
					<?php }else{ ?><td class="mlns">
					<?php }?>
						<a class="ml" href="/profile">My Profile</a>
					</td>
				</tr>
				<tr height="20">
					<?php if ($page==2){?><td class="mls">
					<?php }else{ ?><td class="mlns">
					<?php }?>
						<a class="ml" href="/mypictures">My Photo Album</a>
					</td>
				</tr>
				<tr height="20">
					<?php if ($page==3){?><td class="mls">
					<?php }else{ ?><td class="mlns">
					<?php }?>
						<a class="ml" href="/myaccount">My Account</a>
					</td>
				</tr>
				<tr height="20">
					<?php if ($page==4){?><td class="mls">
					<?php }else{ ?><td class="mlns">
					<?php }?>
						<a class="ml" href="/myprivacy">My Privacy</a>
					</td>
				</tr>
				<tr height="20">
					<?php if ($page==5){?><td class="mls">
					<?php }else{ ?><td class="mlns">
					<?php }?>
						<a class="ml" href="/myconnections">My Connections</a>
					</td>
				</tr>
				<tr height="20">
					<?php if ($page==6){?><td class="mls">
					<?php }else{ ?><td class="mlns">
					<?php }?>
						<a class="ml" href="/mytrades">Trading space</a>
					</td>
				</tr>
				<tr height="20">
					<?php if ($page==7){?><td class="mls">
					<?php }else{ ?><td class="mlns">
					<?php }?>
						<a class="ml" href="/rides">It's my Ride!!!</a>
					</td>
				</tr>
				<tr height="20">
					<?php if ($page==8){?><td class="mls">
					<?php }else{ ?><td class="mlns">
					<?php }?>
						<a class="ml" href="/parties">Parties, yo!</a>
					</td>
				</tr>
				<tr height="20">
					<?php if ($page==9){?><td class="mls">
					<?php }else{ ?><td class="mlns">
					<?php }?>
						<a class="ml" href="/events">Events</a>
					</td>
				</tr>
			</table>
			<br>
			<table width=120 cellpadding=0 cellspacing=0>
				<tr height=20>
					<td background="/images/leftbar.gif" style="background-repeat:no-repeat;" width=20>&nbsp;</td>
					<td align=left class="title" style="padding-left:0px">random...</td>
					<td background="/images/rightbar.gif" style="background-repeat:no-repeat;" width=20>&nbsp;</td>
				</tr>
				<tr>
					<td style="border-width:1px;border-style:solid;border-color:#336699;padding-left:10px" colspan="3" align="left">
						<img src="" height=200 width=100>
					</td>
				</tr>
			</table>
		<!--place for banner-->
		</td>
		<!--center panel-->
		<td width="100%" align="left" style="border-top-width:1px;border-top-style:solid;border-top-color:#000000;border-right-width:1px;border-right-style:solid;border-right-color:#000000;">
<?php
}
function drawMain_3($calendar){
?>
		</td>
		<?php if($calendar == true){ ?>
		<td style="padding-left:10px;">
		<?php require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/calendar.php");?>
		</td>
		<?php } ?>
	</tr>
	</table>
<?php include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>
</td></tr>
</table>
</body>

</html>
<?php
	db_close();
	ob_end_flush();
}
?>