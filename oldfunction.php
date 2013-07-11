<?php
/**checks if two users are friends or if optional condition is set to true
 *also checks if I was invited by the other user*/
function sqlFunc_isFriends($my_id,$user_id,$check_temp = false){
	/*checks wherever one of the users exists in the other ones
	 *group*/
	$friends_table = new table_friends();
	$query = 'SELECT '.$friends_table->field_user1.' FROM '.TABLE_FRIENDS
			.' WHERE '.$friends_table->field_user1." = '".$my_id."' AND "
			.$friends_table->field_user2." = '".$user_id."' LIMIT 1";
	$result = db::query($query);
	//if found an enty return true
	if (db::fetch_row($result)){
		return true;
	}elseif ($check_temp){
		$tempfriends_t = new table_tempfriends();
		/*if false and check_temp == true, then check temp table
		 *for it seems reasonable to enable access for me to see an 
		 *inviter's profile*/
		$query = 'SELECT '.$tempfriends_t->field_inviter.' FROM '.TABLE_TEMPFRIENDS
			.' WHERE '.$tempfriends_t->field_invited." = '".$my_id."' AND "
			.$tempfriends_t->field_inviter." = '".$user_id."' LIMIT 1";
		$result = db::query($query);
		if (db::fetch_row($result)){
			return true;
		}
	}
	return false;
}
/**finds if either of the 2 users have the same major (first or second
 **assuming both are in the same college*/
function sqlFunc_sameMajors($user_id1,$user_id2){
	$profile_table = new table_profile();
	$query = 'SELECT t1.'.$profile_table->field_major.' FROM '
			.TABLE_PROFILE.' AS t1,'.TABLE_PROFILE.' AS t2'
			.' WHERE '
			.'t1.'.$profile_table->field_id." = '".$user_id1."' AND "
			.'t2.'.$profile_table->field_id." = '".$user_id2."' AND "
			.'(('
				.'t1.'.$profile_table->field_major
				.' IN (t2.'.$profile_table->field_major.','
				.'t2.'.$profile_table->field_majorsecond
			.')) OR ('
				.'t1.'.$profile_table->field_majorsecond
				.' IN (t2.'.$profile_table->field_major.','
				.'t2.'.$profile_table->field_majorsecond
			.'))) LIMIT 1';
	$result = db::query($query);
	//if found an enty return true
	if (db::fetch_row($result)){
		return true;
	}else{
		return false;
	}
}
/**finds wherever 2 users have the same course together 
 **based on course number and department name (note: assumes same college)*/
function sqlFunc_sameCourses($user_id1,$user_id2){
	$ctable = new table_mycourses();
	$query = 'SELECT t1.'.$ctable->field_id.' FROM '
		.TABLE_MYCOURSES.' as t1, '.TABLE_MYCOURSES.' as t2'
		.' WHERE t1.'.$ctable->field_myid." = '".$user_id1."' AND "
		.'t2.'.$ctable->field_myid." = '".$user_id2."' AND "
		.'t1.'.$ctable->field_number.' = t2.'.$ctable->field_number
		.' AND t1.'.$ctable->field_department.' = t2.'.$ctable->field_department
		.' LIMIT 1';
	$result = db::query($query);
	//if found an enty return true
	if (db::fetch_row($result)){
		return true;
	}else{
		return false;
	}
}
?>