<? require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/interface.inc");?>
<? require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/DBconnect.inc");?>
<?php
/**authCheck sets the following global constants
 *AUTH_COLLEGESHORT - college breviation
 *AUTH_USERID - id of the user
 *AUTH_COLLEGEID - college id
 *AUTH_USER_NAME - user's name
 *AUTH_PERMISSIONS - user's main permission string
 *AUTH_USER_STORAGE - the rest of the storage (without the above, can be empty)
 *AUTH_STYLES - style sheet to use for all pages
 *user's name - user's name
**/
function auth_authCheck($strict = true){
	//check only if haven't checked before
	if (defined('AUTH_CHECKED')){
		return AUTH_CHECKED;
	}
	//first do basic check of cookie
	if (!isset($_COOKIE['session']) || empty($_COOKIE['session'])
		|| !preg_match('/^[a-z1-9]{20}$/',$_COOKIE['session'])){
		if ($strict){
			header("Location: http://".$_SERVER['HTTP_HOST']."/");
			exit;
		}else{
			define('AUTH_CHECKED',false);
			return false;
		}
	}
	db::connect();
	//now do a database check
	$sessionTable = new table_session();
	$usersTable = new table_users();
	$sessionID = $_COOKIE['session'];
	//get userid and college short associated with this id
	$query = 'SELECT '
				.TABLE_SESSION.'.'.$sessionTable->field_userid.','
				.TABLE_SESSION.'.'.$sessionTable->field_storage.','
				.TABLE_SESSION.'.'.$sessionTable->field_time.','
				.TABLE_USERS.'.'.$usersTable->field_permissions.','
				.TABLE_USERS.'.'.$usersTable->field_name.','
				.TABLE_USERS.'.'.$usersTable->field_style
				.' FROM '.TABLE_SESSION.','.TABLE_USERS.' WHERE '
				.TABLE_USERS.'.'.$usersTable->field_id
				.' = '.TABLE_SESSION.'.'.$sessionTable->field_userid
				.' AND '.$sessionTable->field_sessionid
				." = '".$_COOKIE['session']."' LIMIT 1";
	$result = db::query($query); 
	if (!($row = db::fetch_assoc($result))){
		if ($strict){
			header("Location: http://".$_SERVER['HTTP_HOST']."/");
			db::close();
			exit;
		}else{
			define('AUTH_CHECKED',false);
			return false;
		}
	}
	$storage = explode(':',$row[$sessionTable->field_storage]);
	define("AUTH_USERID",$row[$sessionTable->field_userid]);
	define("AUTH_PERMISSIONS",$row[$usersTable->field_permissions]);
	define("AUTH_COLLEGEFULL",$storage[2]);
	define("AUTH_COLLEGESHORT",$storage[1]);
	define("AUTH_COLLEGEID",$storage[0]);
	define("AUTH_STYLE",$row[$usersTable->field_style]);
	define("AUTH_USER_NAME",$row[$usersTable->field_name]);
	//save storage for other than already set parameters
	define("AUTH_USER_STORAGE",
		preg_replace('/^[^:]+:[^:]+:[^:]+/','',
			$row[$sessionTable->field_storage]));
	//get time the session was last updated
	$time = $row[$sessionTable->field_time];
	$date_class = new Mydate(mktime());
	$difference = $date_class->difference($time);
	/*if last date update was longer than 10 min ago, then update
	 *this will decrease the # of sql queries made since
	 *every session is only deleted after 30 min or so of inactivity
	 */
	if (($difference/60)>10){
		$query = "UPDATE ".TABLE_SESSION." SET "
			.$sessionTable->field_time." = NOW() WHERE "
			.$sessionTable->field_sessionid." = '".$_COOKIE['session']."' LIMIT 1";
		$result = db::query($query); 
	}
	define('AUTH_CHECKED',true);
	return true;
}
/**1)checks database for user with this email and pass
   2)creates session using user's id
   3)creates cookie with user's session id
   4)creates cookie with user's email address (for convinience)*/
function auth_signin($email, $pass){
	db::connect();
	//do a general check of fields
	if (empty($email) || empty($pass)
		|| !eregi('^[a-zA-Z0-9_\-\.]+@[a-zA-Z0-9\-]+\.[a-zA-Z0-9\-\.]+$'
				,trim($email)) || !eregi('^[a-zA-Z0-9]+$',$pass)){
		return false;
	}
	$usersTable = new table_users();
	$collegeTable = new table_colleges();
	$eventsTable = new table_events();
	$fields = array();
	$fields[] = TABLE_USERS.'.'.$usersTable->field_id;
	$fields[] = TABLE_USERS.'.'.$usersTable->field_school;
	$fields[] = TABLE_COLLEGES.'.'.$collegeTable->field_shortname;
	$fields[] = TABLE_COLLEGES.'.'.$collegeTable->field_fullname;
	/*check for athentication and while at it grab a few values to be
	 *stored for quick access*/
	$encryptedPass = substr(md5(strtolower(trim($pass))),0,10);
	$query = 'SELECT '.array_tostring($fields).' FROM '
		.TABLE_USERS.','.TABLE_COLLEGES
		.' WHERE '.TABLE_USERS.'.'.$usersTable->field_email 
		." = '".trim($email)."' AND "
		.TABLE_USERS.'.'.$usersTable->field_password
		." = '".$encryptedPass."' AND "
		.TABLE_USERS.'.'.$usersTable->field_school.' = '
		.TABLE_COLLEGES.'.'.$collegeTable->field_id
		.' LIMIT 1';
	$result = db::query($query); 
	if ($row = db::fetch_row($result)){
		/*if found user, then add to session, set cookie with session id
		 *clean buffer, and redirect to to home, and quit
		**/
		$sessionTable = new table_session();
		$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$genid = substr($genid, 0, $sessionTable->field_sessionid_length);
		setcookie('session',$genid);
		$userid = $row[0];
		$school_id = $row[1];
		$school_short = $row[2];
		$school_full = $row[3];
		/*store certain values for quick access upon 
		 *authentication no every page*/
		$toBeAddedtoStorage = $school_id.':'.$school_short.':'.$school_full;
		$query = 'REPLACE '.TABLE_SESSION.' (' 
			.$sessionTable->field_sessionid.',' 
			.$sessionTable->field_userid.',' 
			.$sessionTable->field_time.','
			.$sessionTable->field_storage
				. ") values ('" 
			.$genid."', '" 
			.$userid."', NOW(),'"
			.$toBeAddedtoStorage."')";
		$result = db::query($query); 
		//now put a long lasting cookie that stores email address
		setcookie('quickSignin',$_POST['email'],time()+60*60*24*7);
		//finally update last visit data
		$query = "UPDATE " . TABLE_USERS . " set " 
			. $usersTable->field_lastseen . " = CURDATE() WHERE "
			. $usersTable->field_id . " = '" . $userid . "' LIMIT 1";
		$result = db::query($query);
		return true;
	}
	return false;
}
/***/
function auth_signout(){
	db::connect();
	$sessionTable = new table_session();
	if (!isset($_COOKIE['session']) 
		|| strlen($_COOKIE['session']) != $sessionTable->field_sessionid_length
		|| !preg_match('/^[a-z0-9]+$/i',$_COOKIE['session'])){
		return false;
	}
	$query = "DELETE FROM " . TABLE_SESSION . " WHERE " 
		. $sessionTable->field_sessionid . " = '" . $_COOKIE['session'] . "' LIMIT 1";
	setcookie('session','',0); //send cookie to expire
	$success = db::query($query); 
	return ($success);
}
?>