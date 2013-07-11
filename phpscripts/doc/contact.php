<?php
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/DBconnect.inc");
header('Content-type: text/html; charset=iso-8859-1');
class Contact_Form{
	function __construct(){
		if (isset($_GET['sbmt']) && !empty($_POST['subject']) 
			&& !empty($_POST['name']) && !empty($_POST['body'])){
			db::connect();
			myMail(false,'info@partyHub.com','contact form - '.$_POST['subject'],$_POST['body']);
			db::close();
		}
	}
}
$control = new Contact_Form();
?>
<html>
<head>
<title>partyHub - Contact Us</title>
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
</head>
<body bgcolor="#FFFFFF">
<table align="center" width="810" cellpadding="0" cellspacing="0" border="0">
<tr><td><img src="/images/logo1.jpg"></td></tr>
<tr>
	<td class="cntcthdr" style="font-size:14px;" bgcolor="#336699">
		Contact Us
	</td>
</tr>
<tr><td>
	<table width="100%">
	<tr valign="top"><td style="padding-top:30px;padding-left:20px;">
		<table cellpadding="0" cellspacing="0"><tr><td>
			<table width="350" cellpadding="2" cellspacing="2">
			<tr bgcolor="#993333">
				<td class="cntcthdr">About Us</td>
			</tr>
			<tr valign="top">
				<td align="left" class="cntcbrd" style="border-color:#993333;">
					<h1 style="color:#006633;">Who we are</h1> partyHub was founded by Serge Mirkin 
					with a goal to create the one place for all colleges, all schools to interact and connect. 
					The motivation was not to provide another opportunity for young “scholars” to 
					waste their time but to inspire friendships, discussions, and even parties while 
					preserving each person’s privacy and interests. Our goal is to help strengthen the 
					friendships that matter and help create the ones that will.</p>
				</td>
			</tr>
			</table>
		</td></tr>
		<tr><td style="padding-left:40px;padding-top:60px">
		<table width="250" cellpadding="2" cellspacing="2">
		<tr bgcolor="#336699">
			<td class="cntcthdr">Add my College!</td>
		</tr>
		<tr valign="top">
			<td align="left" class="cntcbrd" style="border-color:#336699;">
				To add your college simply contact us providing the following:<p/>
				<ul>
					<li>college name</li>
					<li>school email format</li>
					<li>alternative email format</li>
					<li>school website</li>
					<li>department list</li>
					<li>list of majors<br><font style="font-size:10px">(if different from the above)</font></li>
				</ul>
			</td>
		</tr>
		</table>
		</td></tr>
		</table>
	</td>
	<td style="padding-top:100px;">
		<table cellpadding="2" cellspacing="2">
		<tr bgcolor="#006633">
			<td class="cntcthdr">Drop Us A Line</td>
		</tr>
		<tr valign="top">
			<td align="left" class="cntcbrd" style="border-color:#006633;">
			<form name="cntcfrm" action="/doc/contact?sbmt=1" method="post">
			<table cellpadding="3" cellspacing="0">
			<tr><td>
			Regarding: <select name="subject">
				<option value="Invite">send me an Invite</option>
				<option value="Add my College">Add my College</option>
				<option value="Support">Support</option>
				<option value="Report an Error">Report an Error</option>
				<option value="Business...">Business...</option>
				<option value="User Abuse">User Abuse</option>
				<option value="Other">Other</option>
			</select>
			</td></tr><tr><td>
			<textarea name="body" cols="40" rows="5"></textarea>
			</td></tr>
			<tr><td>
			Your Name: <input type="text" name="name" maxlength="50" size="20"><br>
			<font style="font-size:10px;">and/or email</font>
			</td></tr>
			</table>
			<center><input class="custButton" style="background-color:#999966;padding-left:2px;padding-right:2px;" type="submit" value="send"></center>
			</form>	
			</td>
		</tr>
		</table>
	</td></tr>
	</table>
</td></tr>
<tr><td>
<? include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>
</td></tr>
</td></tr>
</table>
<script type="text/javascript" language="javascript">
document.cntcfrm.body.focus();
</script>
</body>
</html>