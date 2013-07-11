<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/auth.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/mail/mail_controller.inc");
?>
<?php
db::connect();
ob_start();
if (!isset($_GET[CONFIRM_ID]) || !isset($_GET[CONFIRM_WHAT])
	|| $_GET[CONFIRM_WHAT]>3 || $_GET[CONFIRM_WHAT]<1){
	header("Location: http://".$_SERVER['HTTP_HOST']."/");
	exit;
}
$error = null;
$majorError = false;
$usersTable = new table_users();
//find out what are we confirming
switch ($_GET[CONFIRM_WHAT]) {
case CONFIRM_NEWEMAIL:
   	newEmail();
    break;
case CONFIRM_EMAIL:
    existingEmail();
    break;
case CONFIRM_PREFERREDEMAIL:
    preferredEmail();
    break;
}
/**gets temporary user from database and adds him to the users table
   with the specified new password*/
function newEmail(){
	global $error, $majorError, $usersTable;
	$tempusersTable = new table_tempusers();
	$collegeTable = new table_colleges();
	if (strlen($_GET[CONFIRM_ID])!= $tempusersTable->field_id_length){
		header("Location: http://".$_SERVER['HTTP_HOST']."/");
		exit;
	}
	/*adds slashes just in case this is an invalid id 
	(i.e. no slashes will be added if valid request)*/
	$tempUserID = addslashes($_GET[CONFIRM_ID]);
	//query database for this user in temporary table
	$query = "SELECT ". TABLE_TEMPUSERS.".*,"
			.TABLE_COLLEGES.".".$collegeTable->field_fullname." FROM "
			.TABLE_TEMPUSERS.",".TABLE_COLLEGES." WHERE " 
			.TABLE_TEMPUSERS.".".$tempusersTable->field_id 
			. " = '" . $tempUserID
			."' AND "
			.TABLE_TEMPUSERS.".".$tempusersTable->field_school
			." = "
			.TABLE_COLLEGES.".".$collegeTable->field_id
			. " LIMIT 1";
	$result = db::query($query);
	if (!($values = db::fetch_assoc($result))){
		$majorError = true;
		return false;
	}else{
		$GLOBALS['collegefullname'] = $values[$collegeTable->field_fullname];
		$email = $values[$tempusersTable->field_email];
		$name = $values[$tempusersTable->field_name];
		$school = $values[$tempusersTable->field_school];
		if(isset($_GET['proceed'])){
			if (!funcs_isValidPass($_POST['pass']) 
				|| !funcs_isValidPass($_POST['pass2'])){
				$error = "please enter a 4-10 character long password";
				return false;
			}
			if (strcmp(strtolower($_POST['pass']),strtolower($_POST['pass2']))!=0){
				$error = "please make sure that the two passwords match";
				return false;
			}
			$encryptedPass = substr(md5(strtolower(trim($_POST['pass']))),0,10);
			do{
				//loop while no user exists with this id
				//generate user's id
				$id = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
				$id = substr($id, 0, $usersTable->field_id_length);
				//make sure that no user exists with the same id
				$query = 'SELECT '.$usersTable->field_id.' FROM '.TABLE_USERS.' WHERE '
					.$usersTable->field_id." = '".$id."' LIMIT 1";
				$result = db::query($query);
			}while (db::fetch_row($result));
			//move user to the users table
			$query = "INSERT INTO " . TABLE_USERS . " (" 
					.$usersTable->field_id.",".$usersTable->field_name
					.",".$usersTable->field_email.",".$usersTable->field_password
					.",".$usersTable->field_school.",".$usersTable->field_membersince
					.",".$usersTable->field_lastseen
				.") values ('"
					.$id."','"
					.$name."','"
					.$email."','"
					.$encryptedPass."','"
					.$school."',"
					." CURDATE(), CURDATE()"
				.")";
			$result = db::query($query);
			//create a new profile for this user
			$profileTable = new table_profile();
			$query = "INSERT INTO ".TABLE_PROFILE
					." (".$profileTable->field_id
					.") values ('".$id."')";
			$result = db::query($query);
			//delete user from temporary table
			$query = "DELETE FROM " . TABLE_TEMPUSERS . " WHERE " 
				. $tempusersTable->field_id . " = '" . $tempUserID . "' LIMIT 1";
			$result = db::query($query);
			//make me and the user friends
			$friends_t = new table_friends();
			$grp_t = new table_friends_in_groups();
			//my id is always aaaaa... depending on the length
			$myid = substr('aaaaaaaaaaaaaaaaaaa',0,$friends_t->field_user2_length);
			$fields = array();
			$fields[$friends_t->field_user1] = $id;
			$fields[$friends_t->field_user2] = $myid;
			$query = 'INSERT DELAYED INTO '.TABLE_FRIENDS.' ('.sqlFields(array_keys($fields))
				.') VALUES ('.toSQL($fields).')';
			$fields[$friends_t->field_user1] = $myid;
			$fields[$friends_t->field_user2] = $id;
			$query .= ', ('.toSQL($fields).')';
			db::query($query);
			//add this user to my default group
			$fields = array();
			$fields[$grp_t->field_ownerid] = $myid;
			$fields[$grp_t->field_userid] = $id;
			//my default group id is always aaaaa... depending on the length
			$fields[$grp_t->field_groupid] = substr('aaaaaaaaaaaaaaaaaa',0,$grp_t->field_groupid_length);
			$query = 'INSERT DELAYED INTO '.TABLE_FRIENDS_IN_GROUPS.' ('.sqlFields(array_keys($fields))
				.') VALUES ('.toSQL($fields).')';
			db::query($query);
			//send the user a welcome message
			$message_t = new table_messages();
			$message = <<<EOD
<p><font face="lucida grande" size="2">How is it going there? Just wanted to welcome you to partyHub.&nbsp;<strong>
<font color="#006600">Welcome!</font></strong>&nbsp;</font></p>
<p>We suggest that you take a look at our help page to find out how to change you profile colors, fonts...
</p>
<p><font color="#660000">and don't forget to invite your friends 
asap to get this thing going,</font></p> <p><font color="#000099">partyHub team</font></p>
EOD;
			$fields = array();
			$fields[$message_t->field_body] = $message;
			$fields[$message_t->field_from] = 'aaaaa';
			$fields[$message_t->field_isread] = '0';
			$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
			$genid = substr($genid, 0, $message_t->field_messageid_length);
			$fields[$message_t->field_messageid] = $genid;
			$fields[$message_t->field_subject] = 'Welcome to partyHub!!!';
			$fields[$message_t->field_to] = $id;
			$query = 'INSERT DELAYED INTO '.TABLE_MESSAGES.' ('.sqlFields(array_keys($fields)).',`'.$message_t->field_date
				.'`) VALUES ('
				.toSQL($fields).',NOW())';
			db::query($query);
			auth_signin($email,$_POST['pass']);
			header("Location: http://".$_SERVER['HTTP_HOST']."/doc/invite/social");
			exit;
			}
	}
	//else print default
}
/**confirms user's school email address and changes it to a new one*/
function existingEmail(){
	global $error, $majorError, $usersTable;
	$tempemail_table = new table_tempemails();
	//first do a general check
	if (strlen($_GET[CONFIRM_ID])!= $tempemail_table->field_genid_length){
		header("Location: http://".$_SERVER['HTTP_HOST']."/");
		exit;
	}
	/*adds slashes just in case this is an invalid id 
	(i.e. no slashes will be added if valid request)*/
	$tempID = addslashes($_GET[CONFIRM_ID]);
	//now get data from database
	$query = "SELECT * FROM ".TABLE_TEMPEMAILS." WHERE "
			.$tempemail_table->field_genid." = '".$tempID."' LIMIT 1";
	$result = db::query($query);
	$array = null;
	if (!($array = db::fetch_array($result))){
		$majorError = true;
		return false;
	}
	$userid = $array[$tempemail_table->field_userid];
	$email = $array[$tempemail_table->field_email];
	//now update email address of the user with the new one
	$query = "UPDATE ".TABLE_USERS." SET "
			.$usersTable->field_email." = '".$email."'"
			." WHERE ".$usersTable->field_id." = '".$userid."' LIMIT 1";
	$success = db::query($query);
	//finally delete an entry in tempemails table
	$query = "DELETE FROM ".TABLE_TEMPEMAILS." WHERE "
			.$tempemail_table->field_genid." = '".$tempID."' LIMIT 1";
	$success = db::query($query);

}
/**confirms user's alternative email address and changes it*/
function preferredEmail(){
	global $error, $majorError, $usersTable;
	$tempemail_table = new table_tempemails();
	//first do a general check
	if (strlen($_GET[CONFIRM_ID])!= $tempemail_table->field_genid_length){
		header("Location: http://".$_SERVER['HTTP_HOST']."/");
		exit;
	}
	/*adds slashes just in case this is an invalid id 
	(i.e. no slashes will be added if valid request)*/
	$tempID = addslashes($_GET[CONFIRM_ID]);
	//now get data from database
	$query = "SELECT * FROM ".TABLE_TEMPEMAILS." WHERE "
			.$tempemail_table->field_genid." = '".$tempID."' LIMIT 1";
	$result = db::query($query);
	$array = null;
	if (!($array = db::fetch_array($result))){
		$majorError = true;
		return false;
	}
	$userid = $array[$tempemail_table->field_userid];
	$email = $array[$tempemail_table->field_email];
	//now update alternative email address of the user with the new one
	$query = "UPDATE ".TABLE_USERS." SET "
			.$usersTable->field_preferedemail." = '".$email."'"
			." WHERE ".$usersTable->field_id." = '".$userid."' LIMIT 1";
	$success = db::query($query);
	//finally delete an entry in tempemails table
	$query = "DELETE FROM ".TABLE_TEMPEMAILS." WHERE "
			.$tempemail_table->field_genid." = '".$tempID."' LIMIT 1";
	$success = db::query($query);
}
?>
<html>
<head>
<link rel="Stylesheet" type="text/css" href="/styles/styles.css">
<title>partyHub - Confirmation</title>
<style type="text/css">

.confirmButton {
	font-family:tahoma,arial,sans-serif;font-size: 10pt;
	font-weight: normal;font-Style: normal;font-variant: normal;
	color: #ffffff;background-color:#339966;
}
td{
	font-family:Verdana, Arial, Helvetica, sans-serif
}
</style>
</head>
<body>
<table align="center" width="810" cellpadding="0" cellspacing="0" border="0">
<tr><td><img src="/images/logo1.jpg"></td></tr>
<tr valign=top>
	<td height="20" style="color:#FFFFFF;font-size:14px;font-weight:bold;padding-left:10px;" bgcolor="#336699">
		Email Confirmation
	</td>
</tr>
<tr><td align="center" style="padding-top:10px">
<?php
if($majorError){
?>
<table width="400" style="border-width:1px;border-color:#000000;border-style:solid;">
<tr valign="top">
	<td style="padding-left:10px">
<p><font class="error"><b>Important: </b></font>Unfortunately it appears that we have no record of your confirmation 
id in our records. It is possible that we have automatically deleted 
your email confirmation record due to its expiration period. However, 
if you believe that an error occurred, you are more than welcome to contact us. 
</p>		
	</td>
</tr>
</table>
<?php
}else{ 
	switch ($_GET[CONFIRM_WHAT]) {
		case CONFIRM_NEWEMAIL:
			print_new_registration();
   			break;
		case CONFIRM_EMAIL:
    		print_email_confirmation();
    		break;
		case CONFIRM_PREFERREDEMAIL:
    		print_altemail_confirmation();
    		break;
	}
}
?>
</td></tr>
<tr><td>
<?php include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>
</td></tr>
</table>
</body>
</html>
<?php
db::close();
ob_end_flush();
function print_new_registration(){
global $usersTable, $error;
?>
<table style="border-width:1px;border-color:#000000;border-style:solid;">
<tr valign="top">
	<td style="border-right-width:1px;border-right-style:solid;border-right-color:#CCCCCC;padding-left:10px;padding-top:10px;" width="250">
	What is ahead?<br>
	Once you complete these fields you will be taken to the page where you can 
	create your profile, upload picture[s], message your friends and 
	do whatever else you feel like (as long as it's within our terms of use :-).
	</td>
	<td style="padding-left:10px;padding-top:10px;">
	Please complete the following<br>
	<form method="post" action="<?='/confirm?proceed=1&'.CONFIRM_ID.'='.$_GET[CONFIRM_ID].'&'.CONFIRM_WHAT.'='.CONFIRM_NEWEMAIL?>">
	<table>
	<?php
	if (!empty($error)){
		echo '<tr><td colspan=2><font color="#CC3300"><b>error: </b>'.$error.'</font></td></tr>';
	}
	?>
		<tr valign="top">
			<td nowrap align="right">New Password: </td>
			<td>
				<input type="password" name="pass" value="" size=15 maxlength="<?=$usersTable->field_password_length?>">
				<br><span class="dimmedTxt">case-insensitive, 4-10 characters long</span>
			</td>
		</tr>
		<tr valign="top">
			<td nowrap align="right">Repeat Above: </td>
			<td><input type="password" name="pass2" value="" size=15 maxlength="<?=$usersTable->field_password_length?>"></td>
		</tr>
		<tr valign="top">
			<td align="right">College: </td>
			<td><input disabled type="text" value="<?=$GLOBALS['collegefullname']?>" size=40></td>
		</tr>
		<tr>
			<td colspan="2" align="center">
				<input type="submit" value="proceed" class="confirmButton">
			</td>
		</tr>
	</table>
	</form>
	</td>
</tr>
</table>
<?php
}
function print_email_confirmation(){
?>
<table width="400" style="border-width:1px;border-color:#000000;border-style:solid;">
<tr valign="top">
	<td style="padding-left:10px">
	<p>Your account has been updated successfully with the new email address. <a href="/">please click here to be redirected to the home page</a></p>		
	</td>
</tr>
</table>
<?php
}
function print_altemail_confirmation(){
?>
<table width="400" style="border-width:1px;border-color:#000000;border-style:solid;">
<tr valign="top">
	<td style="padding-left:10px">
	<p>Your account has been updated successfully with the new alternative/preferred email address. <a href="/">please click here to be redirected to the home page</a></p>		
	</td>
</tr>
</table>
<?php
}
?>