<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/interface.inc");
ob_start();
//start beffering in case we will need to send headers
if (isset($_GET['signin'])){
	require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/auth.inc");
	if ($_GET['signin']==1 && isset($_POST['email']) && isset($_POST['pass'])){
		//check for athentication
		if (auth_signin($_POST['email'],$_POST['pass'])){
			header("Location: http://".$_SERVER['HTTP_HOST']."/home");
			db::close();
			exit;
		}else{
			$GLOBALS['badpassword'] = '<font color="#ff0000" style="font-size:11px;"><b>Error:</b> bad email/password</font>';
			db::close();
		}
	}else if($_GET['signin']==0){
		auth_signout();
		db::close();
		header("Location: http://".$_SERVER['HTTP_HOST']."/");
		exit;
	}
}
/**if cookie exists with an email address, then use it for user's conviniencef*/
$email = "";
if (isset($_COOKIE['quickSignin']) && !empty($_COOKIE['quickSignin'])){
	$GLOBALS['email'] = $_COOKIE['quickSignin'];
}
?>
<html>
<head>
<link rel="Stylesheet" type="text/css" href="/styles/styles.css">
<script language="javascript" src="/html/utils.js" type="text/javascript"></script>
<script language="javascript" type="text/javascript">
<!--
var img4 = null;
var img2 = null;
var img3 = null;
function init(){
	img4 = new Image();
	img4.src = '/images/preview/hm_lrg4.jpg';
	img2 = new Image();
	img2.src = '/images/preview/hm_lrg2.jpg';
	img3 = new Image();
	img3.src = '/images/preview/hm_lrg3.jpg';
}
var dispImg = null;
function showImg(which){
	if (dispImg != null){
		return;
	}
	dispImg = document.createElement('DIV');
	dispImg.style.backgroundColor  = '#FEFEFE';
	dispImg.style.borderStyle = 'solid';
	dispImg.style.borderWidth = '1px';
	dispImg.style.borderColor = '#000000';
	dispImg.style.position = 'absolute';
	dispImg.style.left = 100;
	dispImg.style.top = 200;
	dispImg = document.body.appendChild(dispImg);
	if (dispImg.attachEvent){
		dispImg.attachEvent('onmouseout',hideImg);
	}else if(dispImg.addEventListener){
		dispImg.onmouseout = hideImg;
	}
	setInnerHTML(dispImg,'<img src="'+eval('img'+which+'.src')+'">');
}
function hideImg(){
	if (dispImg != null){
		document.body.removeChild(dispImg);
		dispImg = null;
	}
}
//-->
</script>
<link rel="Shortcut Icon" href="/images/">
<title>partyHub</title>
<style type="text/css">
.signinButton {
	font-family:tahoma,arial,sans-serif;font-size: 10pt;
	font-weight: normal;font-Style: normal;font-variant: normal;
	color: #ffffff;background-color:#CC0000;
}
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
</head>
<body bgcolor="#FFFFFF" onLoad="init()">
<table align="center" width="810" cellpadding="0" cellspacing="0" border="0">
<tr><td><img src="/images/logo1.jpg"></td></tr>
<tr>
	<td style="color:#FFFFFF;font-size:14px;font-weight:bold;padding-left:10px;" bgcolor="#336699">
		Welcome!
	</td>
</tr>
<tr><td>
	<table width="100%" cellpadding="5" cellspacing="5">
	<tr valign="top">
		<td style="padding-top:20px;">
			<table width="100%" cellpadding="5"><tr><td style="padding-left:60px;">
				<table width="350" cellpadding="2" cellspacing="2">
				<tr class="cntcthdr" bgcolor="#006633">
					<td style="color:#FFFFFF">What is partyHub?</td>
				</tr>
				<tr valign="top">
					<td class="cntcbrd" style="border-color:006633">
							<p style="font-family:MS Sans Serif, Arial, Helvetica;font-size:10pt;">partyHub is a place for college guys and gals to stay connected and have 
							a good time while at it. Wherever it is partying, blogging, or buying textbooks 
							- partyHub helps you keep in touch with your peers.
							</p>
					</td>
				</tr>
				</table>
			</td></tr>
			<tr><td align="left">
				<table width="350" cellpadding="2" cellspacing="2">
				<tr class="cntcthdr" bgcolor="#336699">
					<td style="color:#FFFFFF">...</td>
				</tr>
				<tr valign="top">
					<td class="cntcbrd" style="border-color:336699">
							<p style="font-family:MS Sans Serif, Arial, Helvetica;font-size:10pt;">Share Your Photos, Post Blogs, Customize Your Own Profile with <font color="#CC9999">css style sheets</font>, 
							and above all - Invite People to Your Parties & Events!
							</p>
					</td>
				</tr>
				</table>
			</td></tr>
			</table>
		</td>
		<td align="center">
			<form method=post action="/signin" name="signin">
			<table height="130" width="250"  style="border-width:1px;border-color:#CCCCCC;border-style:solid;">
			<tr>
				<td align="right">email:</td>
				<td><input type="text" name="email" value="<?=$email?>" size="20"></td>
			</tr>
			<tr>
				<td align="right">password:</td>
				<td><input type="password" name="pass" value="" size="20"></td>
			</tr>
			<?php
				if (isset($GLOBALS['badpassword'])){
					echo '<tr><td colspan=2>' . $GLOBALS['badpassword'] . '</td></tr>';
				}
			?>
			<tr><td colspan=2 align=center><input type=submit value="[signin]" class="signinButton"></td>
			<tr>
				<td align="center" colspan="2">
				<a href="/register">[register]</a><a href="/forgot" 
				style="padding-left:20px">[forgot]</a>
				</td>
			</tr>
			</table>
			</form>
			<br>
			<br>
			<table width="300" cellpadding="2" cellspacing="2">
				<tr><td class="cntcthdr" bgcolor="#663333">partyHub Colleges?</td></tr>
				<tr valign="top">
					<td class="cntcbrd" style="border-color:#663333" align="center">
					<p align="center" style="font-size:10px;font-weight:bold;">
					All Colleges, All Schools!
					</p>
					</td>
				</tr>
			</table>
		</td>
		</tr>
	</table>
<? include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>

</td></tr>
</table>
<script language="javascript">
document.signin.pass.focus();
</script>
</body>
</html>
<?php
ob_end_flush();
?>