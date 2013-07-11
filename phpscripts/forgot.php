<?php 
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/DBconnect.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/interface.inc");
ob_start();
$error = null;
//if form was submited
if (isset($_GET['submit'])){
	$error = checkForm();
}
function checkForm(){
	if (empty($_POST['email'])){
		return "please enter something for your email address";
	}
	if (!funcs_isValidEmail($_POST['email'])){
		return "please enter your email address correctly";
	}
	$email = trim($_POST['email']);
	$usersTable = new table_users();
	db::connect();
	$query = "SELECT ".$usersTable->field_id.",".$usersTable->field_name." FROM ".TABLE_USERS
			." WHERE ".$usersTable->field_email
			." = ".toSQL($email)." LIMIT 1";
	$result = db::query($query); 
	if (!($array = db::fetch_row($result))){
		return "we were unble to find a user with this email address";
	}
	//user found now do actions for reseting password
	//get userid
	$userid = $array[0];
	//get first name
	$fname = $array[1];
	$fname = funcs_getNames($fname);
	$fname = $fname[0];
	//generate temporary password
	$genpass = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
	$genpass = substr($genpass, 0, 10);
	//encrypt pass
	$encrypted_genpass = substr(md5($genpass),0,10);
	//reset password
	$query = "UPDATE ".TABLE_USERS." SET "
		.$usersTable->field_password." = '".$encrypted_genpass."'"
		." WHERE ".$usersTable->field_id
		." = '".$userid."' LIMIT 1";
	if (db::query($query) === false){
		die('failed to update forgotten password');
	} 
	//send mail
	$subject = 'password reset service requested';
	//customize the mail message for this user
	$filename = $_SERVER['DOCUMENT_ROOT'].MAIL_MESSAGES.'passwordreset.txt';
	$message =& file_get_contents($filename);
	$message = eregi_replace('<<name>>',$fname,$message);
	$message = eregi_replace('<<pass>>',$genpass,$message);
	$success = myMail(false, $_POST['email'], $subject, $message);
	db::close();
	return null;
}
?>
<html>
<head>
<? include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/meta.html");?>
<title>partyHub - Password Reset</title>
</head>
<body>
<table align="center" width="810" cellpadding="0" cellspacing="0" border="0">
<tr><td><img src="/images/logo1.jpg"></td></tr>
<tr valign="top">
	<td align="left" style="color:#FFFFFF;font-size:14px;font-weight:bold;padding-left:10px;" bgcolor="#336699">
		Password Reset
	</td>
</tr>
<tr>
	<td style="padding:10px;">
	<form action="/forgot?submit=1" method="post">
	<p>If you have forgotten your partyHub password, please enter your school email address (partyHub login) 
	and we will send you a new temporary password</p>
	<input type="text" name="email" value="" size="40">
	<input type="submit" value="submit" class="custButton">
	<?php
	if (isset($_GET['submit']) && is_null($error)){
		echo '<p><font class="error">Important: </font>we have successfully reset your password and you should receive it in a few short minutes</p>';
	}elseif(isset($_GET['submit']) && !is_null($error)){
		echo '<p><font class="error">Error: </font>'.$error.'</p>';
	}
	?>
	</form>
	</td>
</tr>
<tr>
	<td>
	<? include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>
	</td>
</tr>
</table>
</body>
</html>
<?php
ob_end_flush();
?>