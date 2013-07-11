<? 
/**gets result of all departments under college id in alphebetical order*/
function sqlFunc_getDepartments($collegeid){
	$table = new table_departments();
	$query = "SELECT * FROM ".TABLE_DEPARTMENTS
		." WHERE ".$table->field_collegeid." = '".$collegeid."'"
		." ORDER BY ".$table->field_description;
	$result = db::query($query);
	return $result;
}
/**gets result of all colleges in alphebetical order*/
function sqlFunc_getColleges($fields = "*"){
	$table = new table_colleges();
	$query = "SELECT ".$fields." FROM ".TABLE_COLLEGES." ORDER BY ".$table->field_fullname;
	$result = db::query($query);
	return $result;
}
/**gets result of all courses under college id and optionaly
  *department id alphebetical order*/
function sqlFunc_getCourses($college,$fieldOfStudy = null){
	$table = new table_courses();
	$query = "SELECT * FROM ".TABLE_COURSES
		." WHERE ".$table->field_collegeid." = '".$college."'";
	if (!is_null($fieldOfStudy)){
		$query .= " AND ".$table->field_fieldid." = '".$fieldOfStudy."'";
	}
	$query .= " ORDER BY ".$table->field_description;
	$result = db::query($query);
	return $result;
}
function sqlFunc_getCourseByNumb($college,$department,$coursenumb){
	$table = new table_courses();
	$query = "SELECT * FROM ".TABLE_COURSES
		." WHERE ".$table->field_collegeid." = '".$college."'"
		." AND ".$table->field_fieldid." = ".toSQL($department)
		." AND ".$table->field_number." = ".toSQL($coursenumb)." LIMIT 1";
	$result = db::query($query);
	if ($row = db::fetch_array($result)){
		return $row;
	}else{
		return null;
	}
}
/**this function puts the mail messages into the queue which will then
  *be executed by the external program*/
function myMail($ishtml, $to, $subject, $message){
	if ($ishtml == true){
		$ishtml = 'true';
	}else{
		$ishtml = 'false';
	}
	$mailTable = new table_mail();
	if (is_array($to)){
		$toEmail = implode(',',$to);
	}else{
		$toEmail = $to;
	}
	if ($ishtml){
		$message = preg_replace('/[\r\n]+/',"\n",$message);
	}
	$query = "INSERT DELAYED INTO " . TABLE_MAIL 
				. " (" . $mailTable->field_message 
					. ") values (".toSQL($ishtml . '<<delim>>' . $toEmail . '<<delim>>' . $subject
					. '<<delim>>' . $message).")";
	return db::query($query);
}
//simply returns count of the last executed statement
function sqlFunc_getCount(){
	$query = "SELECT FOUND_ROWS()";
	$result = db::query($query);
	if ($row = db::fetch_row($result)){
		return (int)$row[0];
	}else{
		return 0;
	}
}

function sqlFunc_is_blocked($viewer_id,$owner_id){
	//double check that not the same user
	if ($viewer_id == $owner_id){
		return false;
	}
	$blocked_t = new table_blocklist();
	$query = 'SELECT '.$blocked_t->field_date.' FROM '.TABLE_BLOCKLIST
			.' WHERE '.$blocked_t->field_blockedid." = '".$viewer_id."' AND "
			.$blocked_t->field_blockerid.' = '.toSQL($owner_id).' LIMIT 1';
	$result = db::query($query);
	if ($row = db::fetch_row($result)){
		return $row[0];
	}else{
		return false;
	}
}
/**gets a list of groups owned by the user (if withtitle is specified, 
 *then it will return wherever group exists )*/
function sqlFunc_getGroups($user_id,$withtitle = null,$withid = null){
	$groups_t = new table_friendgroups();
	$query = 'SELECT * FROM '.TABLE_FRIENDGROUPS.' WHERE '
		.$groups_t->field_ownerid." = ".toSQL($user_id)."";
	if (!is_null($withtitle)){
		$query .= ' AND '.$groups_t->field_title.' = '
					.toSQL($withtitle).' LIMIT 1';
		$result = db::query($query);
		return db::fetch_row($result);
	}elseif (!is_null($withid)){
		$query .= ' AND '.$groups_t->field_groupid.' = '
					.toSQL($withid).' LIMIT 1';
		$result = db::query($query);
		return db::fetch_row($result);
	}else{
		$result = db::query($query);
		return $result;
	}
}
function sqlFunc_isSameCollege($myCollege,$user){
	$users_t = new table_users();
	$query = 'SELECT '.$users_t->field_school.' FROM '.TABLE_USERS.' WHERE '
		.$users_t->field_id." = ".toSQL($user)." LIMIT 1";
	$result = db::query($query);
	if ($row = db::fetch_row($result)){
		return ($row[0] == $myCollege);
	}
	return false;
}
function sqlFunc_isFriends($user1,$user2){
	$friends_t = new table_friends();
	$query = 'SELECT 1 FROM '.TABLE_FRIENDS.' WHERE '.$friends_t->field_user1.' = '
		.toSQL($user1).' AND '.$friends_t->field_user2.' = '
		.toSQL($user2).' LIMIT 1';
	$result = db::query($query);
	if ($row = db::fetch_row($result)){
		return true;
	}
	return false;
}
?>