<?php 
define('PAGE_DEFAULT',1);
define('PAGE_SUBMIT',2);
define('PAGE_NO_INVITE',3);
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/DBconnect.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/interface.inc");
ob_start();
db::connect();
$page = PAGE_DEFAULT;
/*
//find out if we are invited
$users_t = new table_users();
if (!isset($_GET['invite']) || strlen($_GET['invite']) != $users_t->field_id_length){
	$page = PAGE_NO_INVITE;
}else{
	$query = 'SELECT 1 FROM '.TABLE_USERS.' WHERE '.$users_t->field_id.' = '
		.toSQL($_GET['invite']).' LIMIT 1';
	$result = db::query($query);
	if (!db::fetch_row($result)){
		$page = PAGE_NO_INVITE;
	}
}*/
/**checks for errors and wherever the action is 'legal'*/
$tempusersTable = new table_tempusers();
$usersTable = new table_users();
$collegeTable = new table_colleges();
function checkForErrors(){
	global $page;
	if ($page === PAGE_NO_INVITE){
		return;
	}
	global $tempusersTable, $usersTable, $collegeTable;
	//do a general format check
	if (!isset($_POST['first']) || !isset($_POST['last']) 
		|| !isset($_POST['email'])
		|| empty($_POST['first']) || empty($_POST['last'])
		|| empty($_POST['email'])){
		return "you must complete all the fields";
	}
	if (!preg_match('/^[a-zA-Z[:space:]-]+$/i',trim($_POST['first']))
		|| !preg_match('/^[a-zA-Z[:space:]-]+$/i',trim($_POST['last']))){
		return "please enter your real name";
	}
	if (!isset($_POST['agree']) || $_POST['agree'] != '1'){
		return 'you must must agree to our "terms of use" in order to continue';
	}
	if (!funcs_isValidEmail($_POST['email'])){
		return "please enter a valid email address";
	}
	//check wherever this school exists
	$query = "SELECT " . $collegeTable->field_id . " FROM " . TABLE_COLLEGES
		. " WHERE " . $collegeTable->field_emailformat . " = '" 
		. preg_replace("/^[^@]+@(.+)$/i",'\\1',trim($_POST['email'])) . "' LIMIT 1";
	$result = db::query($query); 
	if (!($row = db::fetch_row($result))){
		return "our services currently do not extend to your school, but perhaps, you could take some time to let us know about your school (do it like a real rockstar)";
	}
	$GLOBALS['schoolID'] = $row[0];
	//now check that no one else is using this email
	$query = "SELECT " . $tempusersTable->field_id . " FROM " 
		. TABLE_TEMPUSERS . " WHERE " . $tempusersTable->field_email
		. " = '" . strtolower(trim($_POST['email'])) . "' LIMIT 1";
	$result = db::query($query); 
	if (db::fetch_row($result)){
		return "it seems like someone else is already using this email address - well, ain't that interesting?";
	}
	$query = "SELECT " . $usersTable->field_id . " FROM " 
		. TABLE_USERS . " WHERE " . $usersTable->field_email
		. " = '" . strtolower(trim($_POST['email'])) . "' LIMIT 1";
	$result = db::query($query); 
	if (db::fetch_row($result)){
		return "it seems like someone else is already using this email address - well, ain't that interesting?";
	}
	return null;
}
if (isset($_GET['proceed'])){
	$error = checkForErrors();
	if (!isset($error)){
		//add user to the temporary table
		$id = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$id = substr($id, 0, $tempusersTable->field_id_length);
		$name = strtolower(trim($_POST['last'])) . ', ' . strtolower(trim($_POST['first']));
		$email = trim($_POST['email']);
		$query = "INSERT DELAYED INTO " . TABLE_TEMPUSERS . " (" 
			. $tempusersTable->field_id . ","
			. $tempusersTable->field_name . ","
			. $tempusersTable->field_email . ","
			. $tempusersTable->field_school . ","
			. $tempusersTable->field_date 
			. ") values ('"
			. $id . "','" . $name . "','" . $email . "','" 
			. $GLOBALS['schoolID']. "', CURDATE())";
		$result = db::query($query);
		//send email confirmation letter (i.e. add it to the queue)
		$subject = "email confirmation letter from partyHub";
		$filename = $_SERVER['DOCUMENT_ROOT'].MAIL_MESSAGES.'registration.txt';
		$message =& file_get_contents($filename);
		$message = eregi_replace('<<name>>',$_POST['first'],$message);
		$link = 'http://'.USED_DOMAIN_NAME.'/confirm?'.CONFIRM_ID.'='.$id.'&'.CONFIRM_WHAT.'='.CONFIRM_NEWEMAIL;
		$message = eregi_replace('<<link>>',$link,$message);
		//(bool)$ishtml, $to, $subject, $message
		myMail(false,$email,$subject,$message);
		$page = PAGE_SUBMIT; 
	}
}
?>
<html>
<head>
<link rel="Stylesheet" type="text/css" href="/styles/styles.css">
<title>partyHub - Registration</title>
<style type="text/css">
.registerButton {
	font-family:tahoma,arial,sans-serif;font-size: 10pt;
	font-weight: normal;font-Style: normal;font-variant: normal;
	color: #ffffff;background-color:#006699;
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
<body>
<table align="center" width="810" cellpadding="0" cellspacing="0" border="0">
<tr><td><img src="/images/logo1.jpg"></td></tr>
<tr valign="top">
	<td align="left" style="color:#FFFFFF;font-size:14px;font-weight:bold;padding-left:10px;" bgcolor="#336699">
		Register
	</td>
</tr>
<tr><td>
	<table width="100%" cellpadding="5" cellspacing="5">
	<tr valign="top">
		<td>
			<table width="250" cellpadding="2" cellspacing="2">
			<tr class="cntcthdr" bgcolor="#993333">
				<td style="color:#FFFFFF">Some of the Cool feautes</td>
			</tr>
			<tr valign="top">
				<td align="left" valign="top" class="cntcbrd" style="border-color:#993333">
					<ul>
						<li>personal style sheets</li>
						<li>group discussions</li>
						<li>blogs</li>
						<li>photo albums</li>
						<li>partyHub inside mail</li>
					</ul>
				</td>
			</tr>
			</table>
		</td>
		<td style="padding-top:10px;">
			<table width="400" cellpadding="2" cellspacing="2">
			<tr class="cntcthdr" bgcolor="#3399CC">
				<td style="color:#FFFFFF">What's next?</td>
			</tr>
			<tr valign="top">
				<td align="left" valign="top" class="cntcbrd" style="border-color:#3399CC">
				<p>Once you complete the form below you will receive an email from partyHub.com with the 
				confirmation link in it. Please click on that link as soon as possible since it will only be 
				active for a few days. Afterwards, you will be redirected to your profile page where you can begin 
				customizing your page any way you like (including colors, pictures, and more)</p>
				</td>
			</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td align="center" colspan="2">
<?php
if ($page === PAGE_DEFAULT){
//&invite=<?=$_GET['invite']
?>
	<form method=post action="/register?proceed=1?>">
			<table width=430 class="cntcbrd" style="border-color:#000000;">
		<?php
		if (isset($error) && !empty($error)){
			echo '<tr><td colspan="2" style="padding-left:10px;"><font color="#ff0000">' . $error . '</font></td></tr>';
		}
		?>
			<tr>
				<td align="right">First Name: </td>
				<td><input type=text name="first" value="<?php if (isset($_POST['first'])){echo toHTML($_POST['first']);}?>" size="20" maxlength="<?=($tempusersTable->field_name_length/2)?>"></td>
			</tr>
			<tr>
				<td align="right">Last Name: </td>
				<td><input type=text name="last" value="<?php if (isset($_POST['last'])){echo toHTML($_POST['last']);}?>" size="20" maxlength="<?=($tempusersTable->field_name_length/2)?>"></td>
			</tr>
			<tr>
				<td align="right">email (school): </td>
				<td><input type=text name="email" value="<?php if (isset($_POST['email'])){echo toHTML($_POST['email']);}?>" size="20" maxlength="<?=$tempusersTable->field_email_length?>"></td>
			</tr>
			<tr><td colspan="2" style="font-size:10px;"><input type="checkbox" name="agree" value="1"> I acknowledge my understanding and acceptance of the <a href="/doc/terms" style="color:#003399">Terms of Use</a></td></tr>
			<tr><td colspan="2" align="center"><input type="submit" value="submit" class="registerButton"></td></tr>
			</table>
		</form>
<?php }elseif($page === PAGE_SUBMIT){?>
<table width="430" style="border-width:1px;border-color:#000000;border-style:solid;">
	<tr>
		<td style="padding-left:10px;">
		<p>
		<font class="error"><b>Important: </b></font>in a few minutes you 
		should receive an email from partyHub with a confirmation link 
		attached. It is very important that you confirm your registration 
		using that link as soon as possible, since we only store temporary registrations 
		for a few days.
		</p>
		</td>
	</tr>
</table>
<?php }else{?>
	<table width=430 style="padding:4px;"><tr><td>
	Unfortunately at the moment we only allow invite based registration – this is so that 
	we can continue changing things here and there. However, if you know someone who 
	is a registered user, then feel free to ask them for an invite. You can also ask us - 
	in this case we will make an exception :-). If you do get an invite, keep in mind that 
	all feedback is appreciated and is most likely to be implemented in the site within a 
	short period of time.<p/>
	cheers,<br>
	partyHub team</td></tr>
	</table>
<?php }?>
	</td></tr>
	<tr><td colspan="2" style="padding-left:120px">
		<table width="200" cellpadding="2" cellspacing="2">
			<tr class="cntcthdr" bgcolor="#006633">
				<td style="color:#FFFFFF">Don't see your college?</td>
			</tr>
			<tr valign="top">
				<td align="left" valign="top" class="cntcbrd" style="border-color:#006633">
				<p>If you find that for some strange reason (like a humongous number of colleges out there) 
				we currently do not support your college, then let us know <a href="/doc/contact">here</a></p>
				</td>
			</tr>
		</table>
	</td></tr>
	</table>
<? include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>
</td></tr>
</table>
</body>
</html>
<?php
db::close();
ob_end_flush();
?>