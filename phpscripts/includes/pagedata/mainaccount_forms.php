<?php
class myaccount{
	public $id;	//id (in users' table) of the user
	function myaccount($id){
		$this->id = $id;
	}
	/**gets all the fields for the account form, from the users table
	  *returns null if no entry was found, fields otherwise*/
	function &getFields(){
		$utable = new table_users();
		$ctable = new table_colleges();
		$query = "SELECT "
			.TABLE_USERS.".".$utable->field_name.", "
			.TABLE_USERS.".".$utable->field_email.", "
			.TABLE_USERS.".".$utable->field_preferedemail.", "
			.TABLE_COLLEGES.".".$ctable->field_emailformat
			." FROM ".TABLE_USERS.",".TABLE_COLLEGES
			." WHERE "
				.TABLE_USERS.".".$utable->field_id
			." = '"
				.$this->id."'"
			." AND "
				.TABLE_USERS.".".$utable->field_school
			." = "
				.TABLE_COLLEGES.".".$ctable->field_id
				." LIMIT 1";
		$result = db::query($query);
		if ($fields = db::fetch_assoc($result)){
			//separate name into first and last
			$names =& funcs_getNames($fields[$utable->field_name]);
			$fields['fname'] =& $names[0];
			$fields['lname'] =& $names[1];
			return $fields;
		}else{
			return null;
		}
	}
	/**first checks for erros and returns an error message if any found
	  *then adds an entry to the tempemails table for later confirmation
	  *of the email address and send an email message to the new address with
	  *the link to confirmation page
	  *returns: error if bad email format, null if success*/
	function changeEmail(&$form){
		$utable = new table_users();
		if (empty($form[$utable->field_email])){
			return "nothing was entered for primary email";
		}
		//check wherever email format is valid 
		//(attach @something.edu so that the function would work)
		if (!funcs_isValidEmail($form[$utable->field_email]
				.'@something.edu')){
			return "please enter a valid school email address (without @school)";
		}
		//do a password check
		if (!$this->isValidPass($form)){
			return 'please enter your current password correctly';
		}
		/**do several things:
		  *1) get user's name for email purposes
		  *2) get current school email to prevent the change to be made
		  *to the same email
		*/
		$ctable = new table_colleges();
		$query = 'SELECT '
			.$utable->field_name.', '
			.$utable->field_email
			.' FROM '.TABLE_USERS
			.' WHERE '
				.TABLE_USERS.".".$utable->field_id
			." = '"
				.$this->id."' LIMIT 1";
		$result = db::query($query);
		$row = db::fetch_row($result);
		$currentEmail = $row[1];
		//check if this is the same email
		if (preg_replace('/^([^@]+)@.+$/','\\1',$currentEmail) == $form[$utable->field_email]){
			return 'newly entered email address is the same as the old one';
		}
		$emailFormat = preg_replace('/^[^@]+(@.+)$/','\\1',$currentEmail);
		//get user's name
		$fName = $row[0];
		$fName =& funcs_getNames($fName);
		$fName = $fName[0];
		//now insert temporary email to table for further confirmation
		$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$genid = substr($genid, 0, 10);
		//construct actual email address
		$email = $form[$utable->field_email].$emailFormat;
		$temail_table = new table_tempemails();
		//add ab entry for further email confirmation
		$query = "INSERT DELAYED INTO ".TABLE_TEMPEMAILS
		." ("
			.$temail_table->field_genid.","
			.$temail_table->field_date.","
			.$temail_table->field_email.","
			.$temail_table->field_userid
		.") VALUES ('"
			.$genid."',CURDATE(),"
			.toSQL($email).",'"
			.$this->id
		."')";
		$success = db::query($query);
		if (!$success){
			die;
		}
		//now add a message to mail that will be send to a constructed email
		$subject = 'email verification is required';
		//customize the mail message for this user
		$filename = $_SERVER['DOCUMENT_ROOT'].MAIL_MESSAGES.'emailchange.txt';
		$message =& file_get_contents($filename);
		$message = str_replace('<<name>>',$fName,$message);
		$link = 'http://'.USED_DOMAIN_NAME.'/confirm?'.CONFIRM_ID.'='.$genid.'&'.CONFIRM_WHAT.'='.CONFIRM_EMAIL;
		$message = str_replace('<<link>>',$link,$message);
		myMail(false, $email, $subject, $message);
		return null; //no error
	}
	/**works the same way change email does, except this email can be
	  *anything other than school's email
	  *returns: error if bad email format, null if success*/
	function changeAltEmail(&$form){
		$utable = new table_users();
		//do a password check
		if (!$this->isValidPass($form)){
			return 'please enter your current password correctly';
		}
		//check wherever field was empty, if yes, then remove an enrty
		if (empty($form[$utable->field_preferedemail])){
			$users_table = new table_users();
			$query = "UPDATE ".TABLE_USERS." SET "
					.$users_table->field_preferedemail." = NULL"
					." WHERE ".$users_table->field_id." = '".$this->id
					."' LIMIT 1";
			$success = db::query($query);
			return 2;
		}
		//check wherever email format is valid 
		if (!funcs_isValidEmail($form[$utable->field_preferedemail])){
			return "please enter a valid alternative email address";
		}
		/**get the name of the person and an old email address*/
		$query = 'SELECT '
			.$utable->field_name.','
			.$utable->field_preferedemail
			.' FROM '.TABLE_USERS
			.' WHERE '
				.$utable->field_id
			." = '"
				.$this->id."' LIMIT 1";
		$result = db::query($query);
		$row = db::fetch_row($result);
		//check wherever email is the same
		if (strcmp($row[1],$form[$utable->field_preferedemail]) == 0){
			return 'in order to change your alternative email address, please enter a new one';
		}
		$fName = $row[0];
		$fName =& funcs_getNames($fName);
		$fName = $fName[0];
		//now insert temporary email to table for further confirmation
		$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$genid = substr($genid, 0, 10);
		$email = $form[$utable->field_preferedemail];
		$temail_table = new table_tempemails();
		$query = "INSERT DELAYED INTO ".TABLE_TEMPEMAILS
		." ("
			.$temail_table->field_genid.","
			.$temail_table->field_date.","
			.$temail_table->field_email.","
			.$temail_table->field_userid
		.") VALUES ('"
			.$genid."',CURDATE(),"
			.toSQL($email).",'"
			.$this->id
		."')";
		$success = db::query($query);
		if (!$success){
			die;
		}
		//now add a message to mail that will be send to an email
		$subject = 'email verification is required for your alternative email address';
		//customize the mail message for this user
		$filename = $_SERVER['DOCUMENT_ROOT'].MAIL_MESSAGES.'altemailchange.txt';
		$message =& file_get_contents($filename);
		$message = str_replace('<<name>>',$fName,$message);
		$link = 'http://'.USED_DOMAIN_NAME.'/confirm?'.CONFIRM_ID.'='.$genid.'&'.CONFIRM_WHAT.'='.CONFIRM_PREFERREDEMAIL;
		$message = str_replace('<<link>>',$link,$message);
		myMail(false, $email, $subject, $message);
		return null; //no error
	}
	/**changes name without sending a notification email
	  *returns error - false or null - true*/
	function changeName(&$form){
		if (empty($form['fname']) || empty($form['lname'])
			|| !preg_match('/^[a-z ]+$/i',$form['fname'])
			|| !preg_match('/^[a-z ]+$/i',$form['lname'])){
			return 'please enter your actual first and last names';
		}
		//do a password check
		if (!$this->isValidPass($form)){
			return 'please enter your current password correctly';
		}
		$name = strtolower(trim($form['lname']).", ".trim($form['fname']));
		$users_table = new table_users();
		$query = 'UPDATE '.TABLE_USERS.' SET '
				.$users_table->field_name." = '".$name."'"
				.' WHERE '
				.$users_table->field_id." = '".$this->id."' LIMIT 1";
		$success = db::query($query);
		return null;
	}
	/**changes password and sends a notification to the user regarding
	  *account update*/
	function changePass(&$form){
		if (empty($form['newpass'])){
			return 'please enter your new password';
		}
		if (empty($form['newpass2'])){
			return 'please confirm your new password';
		}
		if ($form['newpass'] != $form['newpass2']){
			return 'the new password and its confirmation do not match';
		}
		if (!funcs_isValidPass($form['newpass'])){
			return 'please enter a 4-10 character long password';
		}
		//do a password check
		if (!$this->isValidPass($form)){
			return 'please enter your current password correctly';
		}
		$encryptedPass = substr(md5(strtolower(trim($form['newpass']))),0,10);
		$users_table = new table_users();
		$query = "UPDATE ".TABLE_USERS." SET "
				.$users_table->field_password." = '".$encryptedPass."'"
				." WHERE ".$users_table->field_id." = '".$this->id."' LIMIT 1";
		$success = db::query($query);
		//get the name and an email of the person
		$query = "SELECT ".$users_table->field_name.','
			.$users_table->field_email.','
			.$users_table->field_preferedemail
			." FROM ".TABLE_USERS
			." WHERE "
				.$users_table->field_id
			." = '"
				.$this->id."' LIMIT 1";
		$result = db::query($query);
		$row = db::fetch_row($result);
		//convert to just first name
		$fName = $row[0];
		$fName =& funcs_getNames($fName);
		$fName = $fName[0];
		if (!empty($row[2])){
			$email = array($row[1],$row[2]);
		}else{
			$email = $row[1];
		}
		//send account update message
		$subject = 'notification about a recent update of your account';
		//customize the mail message for this user
		$filename = $_SERVER['DOCUMENT_ROOT'].MAIL_MESSAGES.'passwordchange.txt';
		$message =& file_get_contents($filename);
		$message = str_replace('<<name>>',$fName,$message);
		$message = str_replace('<<password>>',strtolower(trim($form['newpass'])),$message);
		myMail(false, $email, $subject, $message);
		return null;
	}
	/**checks wherever current password is valid, should be used
	  *always priot to any account changes
	  *password check is case-insensitive*/
	function isValidPass(&$form){
		//first do a general check without accessing database
		if (empty($form['password'])){
			return false;
		}
		if (!ereg('^[a-zA-Z0-9]+$',$form['password'])){
			return false;
		}
		$usersTable = new table_users();
		//encrypt password for comparison purposes
		$encryptedPass = substr(md5(strtolower(trim($form['password']))),0,10);
		$query = "SELECT ".$usersTable->field_id
				." FROM ".TABLE_USERS." WHERE " 
					.$usersTable->field_id 
				." = '"
					.$this->id."' AND "
					.$usersTable->field_password 
				." = '".$encryptedPass."' LIMIT 1";
		$result = db::query($query); 
		if (db::fetch_row($result)){
			return true;
		}else{
			return false;
		}
	}
}
?>