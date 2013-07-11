<?php
class quickAddIn{
	public $user_id;
	function quickAddIn($user){
		$this->user_id = $user;
	}
	/**queries database and returns a sorted array (no regard to case)
	 *of group titles as values*/
	function getGroups(){
		$groups_t = new table_friendgroups();
		$query = 'SELECT '.$groups_t->field_title.' FROM '
			.TABLE_FRIENDGROUPS.' WHERE '
			.$groups_t->field_ownerid." = '".$this->user_id."'";
		$result = db::query($query);
		$groups = array();
		while ($row = db::fetch_row($result)){
			$groups[] = $row[0];
		}
		natcasesort($groups);
		return $groups;
	}
	/**queries database and returns a sorted array (no regard to case)
	 *of course title as values*/
	function getCourses(){
		$courses_t = new table_mycourses();
		$query = 'SELECT '.$courses_t->field_course.' FROM '
			.TABLE_MYCOURSES.' WHERE '
			.$courses_t->field_myid." = '".$this->user_id."'";
		$result = db::query($query);
		$courses = array();
		while ($row = db::fetch_row($result)){
			$courses[] = $row[0];
		}
		natcasesort($courses);
		return $courses;
	}
	/**queries database and returns a sorted array (no regard to case)
	 *of names (first last) as values*/
	function getFriends(){
		$users_t = new table_users();
		$friends_t = new table_friends();
		$query = 'SELECT '.$users_t->field_name.','
				.$users_t->field_email.' FROM '
				.TABLE_USERS.','.TABLE_FRIENDS.' WHERE '
				.TABLE_FRIENDS.'.'.$friends_t->field_user1." = '"
				.$this->user_id."' AND "
				.TABLE_FRIENDS.'.'.$friends_t->field_user2.' = '
				.TABLE_USERS.'.'.$users_t->field_id;
		$result = db::query($query);
		//buffer friends
		$friends = array();
		while ($row = db::fetch_row($result)){
			$names = funcs_getNames($row[0]);
			$friends[] = $names[0].' '.$names[1];
			$friends[] = $row[1];
		}
		natcasesort($friends);
		//an alternative way is:
		/*usort($friends,
			create_function(
				'$a,$b',
				'return strcmp(strtolower($a),strtolower($b));'
			));*/
		return $friends;
	}
	/**takes a string of users separated by commas, where users are either 
	 *emails or
	 *names (first last); it then checks for valid formats and adds to query only
	 *those that are valid; returns: null if nothing is found, array of ids if found;
	 *optionaly: if $friends_with is specified, then it will only return users
	 *that are friends with this person*/
	public static function getListOfIds($users_in_string,$friends_with = false,$inSchool = false){
		$users = preg_split('/[ ]*,[ ]*/',$users_in_string);
		//first get a list of emails
		$emails = array();
		if (!empty($users)){
			$users = array_unique($users);
		}else{
			return array();
		}
		foreach($users as $value){
			if (funcs_isValidEmail($value)){
				$emails[] = trim($value);
				//delete an entry from the string
				$users_in_string = str_replace($value,'',$users_in_string);
			}
		}
		//now get a list of names
		$users = preg_split('/[ ]*,[ ]*/',$users_in_string);
		if (!empty($users)){
			$users = array_unique($users);
		}else{
			return array();
		}
		$names = array();
		foreach($users as $value){
			if (preg_match('/^[a-z ]+ [a-z]+$/i',trim($value))){
				$names[] = trim(strtolower($value));
				//delete an entry from the string
				$users_in_string = str_replace($value,'',$users_in_string);
			}
		}
		//now convert names to appropriate format
		foreach($names as $key => $value){
			$names[$key] = preg_replace('/^([a-z ]+) ([a-z]+)$/','\\2, \\1',$value);
			$names[$key] = preg_replace('/[ ]+/',' ',$names[$key]);
		}
		if (empty($emails) && empty($names)){
			return array();
		}
		$users_t = new table_users();
		$emails_string = toSQL($emails);
		$names_string = toSQL($names);
		//count max # of results found
		$limit = count($emails_string)+count($names_string);
		$tables = array();
		$tables[] = TABLE_USERS;
		//if users need to be friends with $friends_with
		if ($friends_with !== false){
			$tables[] = TABLE_FRIENDS;
		}
		$query = 'SELECT '.$users_t->field_id.' FROM '.array_tostring($tables).' WHERE (';
		if (!empty($emails)){
			$query .= $users_t->field_email.' IN ('.$emails_string.')';
		}
		if (!empty($emails) && !empty($names)){
			$query .= ' OR ';
		}
		if (!empty($names)){
			$query .= $users_t->field_name.' IN ('.$names_string.')';
		}
		$query .= ')';
		if ($friends_with !== false){
			$friends_t = new table_friends();
			$query .= ' AND '.$friends_t->field_user1." = '".$friends_with."' AND "
				.$users_t->field_id.' = '.$friends_t->field_user2;
		}elseif($inSchool !== false){
			$query .= ' AND '.$users_t->field_school." = '".$inSchool."'";
		}
		$query .= ' LIMIT '.$limit;
		$result = db::query($query);
		//buffer all ids into an array
		if ($row = db::fetch_row($result)){
			$ids = array();
			do{
				$ids[] = $row[0];
			}while($row = db::fetch_row($result));
			return $ids;
		}else{
			return array();
		}
	}
	public static function getListOfGroupIds($groups_in_string,$owner_id){
		$groups= preg_split('/[ ]*,[ ]*/',$groups_in_string);
		if (empty($groups)){
			return array();
		}
		$groups = array_unique($groups);
		$groups_t = new table_friendgroups();
		$groups_string = toSQL($groups);
		//count max # of results found
		$limit = count($groups);
		$query = 'SELECT '.$groups_t->field_groupid.' FROM '.TABLE_FRIENDGROUPS
				.' WHERE '.$groups_t->field_title.' IN ('.$groups_string.') AND '
				.$groups_t->field_ownerid." = '".$owner_id."' LIMIT $limit";
		$result = db::query($query);
		//buffer all ids into an array
		if ($row = db::fetch_row($result)){
			$ids = array();
			do{
				$ids[] = $row[0];
			}while($row = db::fetch_row($result));
			return $ids;
		}else{
			return array();
		}
	}
	public static function getListOfCourseIds($courses_in_string,$owner_id){
		$courses= preg_split('/[ ]*,[ ]*/',$courses_in_string);
		if (empty($courses)){
			return array();
		}
		$courses= array_unique($courses);
		$courses_t = new table_mycourses();
		$courses_in_string = toSQL($courses);
		//count max # of results found
		$limit = count($courses);
		$query = 'SELECT '.$courses_t->field_id.' FROM '.TABLE_MYCOURSES
				.' WHERE '.$courses_t->field_course.' IN ('.$courses_in_string.') AND '
				.$courses_t->field_myid." = '".$owner_id."' LIMIT $limit";
		$result = db::query($query);
		//buffer all ids into an array
		if ($row = db::fetch_row($result)){
			$ids = array();
			do{
				$ids[] = $row[0];
			}while($row = db::fetch_row($result));
			return $ids;
		}else{
			return array();
		}
	}
	/*takes a list of ids and returns possible user names and/or group titles
	 *associated with these ids (returns them through the three arrays passed
	 *as parameters)*/
	public static function getListOfNames_Titles($ids,&$user_names,&$group_titles,
									&$course_titles){
		if (!is_array($ids) || !is_array($user_names) || !is_array($group_titles)
			|| !is_array($course_titles)){
			return false; //invalid request
		}
		$users_t = new table_users();
		$groups_t = new table_friendgroups();
		$courses_t = new table_mycourses();
		$users = array();
		$groups = array();
		$courses = array();
		foreach($ids as $value){
			//if this is a user id
			if (strlen($value) == $users_t->field_id_length){
				$users[] = $value;
			}elseif(strlen($value) == $groups_t->field_groupid_length){
				$groups[] = $value;
			}elseif(strlen($value) == $courses_t->field_id_length){
				$courses[] = $value;
			}
		}
		if (!empty($users)){
			$query = 'SELECT '.$users_t->field_name.' FROM '.TABLE_USERS
				.' WHERE '.$users_t->field_id
				.' IN ('.toSQL($users).') LIMIT '.count($users);
			$result = db::query($query);
			while ($row = db::fetch_row($result)){
				$names = funcs_getNames($row[0]);
				$user_names[] = $names[0].' '.$names[1];
			}
		}
		if (!empty($groups)){
			$query = 'SELECT '.$groups_t->field_title.' FROM '.TABLE_FRIENDGROUPS
				.' WHERE '.$groups_t->field_groupid
				.' IN ('.toSQL($groups).') LIMIT '.count($groups);
			$result = db::query($query);
			while ($row = db::fetch_row($result)){
				$group_titles[] = $row[0];
			}
		}
		if (!empty($courses)){
			$query = 'SELECT '.$courses_t->field_course.' FROM '.TABLE_MYCOURSES
				.' WHERE '.$courses_t->field_id
				.' IN ('.toSQL($courses).') LIMIT '.count($courses);
			$result = db::query($query);
			while ($row = db::fetch_row($result)){
				$course_titles[] = $row[0];
			}
		}
		return true;
	}
	public static function getListOfUsers($ids){
		if (!is_array($ids)){
			return false; //invalid request
		}
		$users_t = new table_users();
		$user_names = array();
		foreach($ids as $key => $value){
			//if this is a user id
			if (strlen($value) != $users_t->field_id_length){
				unset($ids[$key]);
			}
		}
		if (!empty($ids)){
			$query = 'SELECT '.$users_t->field_name.' FROM '.TABLE_USERS
				.' WHERE '.$users_t->field_id
				.' IN ('.toSQL($ids).') LIMIT '.count($ids);
			$result = db::query($query);
			while ($row = db::fetch_row($result)){
				$names = funcs_getNames($row[0]);
				$user_names[] = $names[0].' '.$names[1];
			}
		}
		return $user_names;
	}
	public static function getListOfGroups($ids){
		if (!is_array($ids)){
			return false; //invalid request
		}
		$groups_t = new table_friendgroups();
		$group_titles = array();
		foreach($ids as $key => $value){
			//if this is a user id
			if (strlen($value) != $groups_t->field_groupid_length){
				unset($ids[$key]);
			}
		}
		if (!empty($ids)){
			$query = 'SELECT '.$groups_t->field_title.' FROM '.TABLE_FRIENDGROUPS
				.' WHERE '.$groups_t->field_groupid
				.' IN ('.toSQL($ids).') LIMIT '.count($ids);
			$result = db::query($query);
			while ($row = db::fetch_row($result)){
				$group_titles[] = $row[0];
			}
		}
		return $group_titles;
	}
	public static function getListOfCourses($ids){
		if (!is_array($ids)){
			return false; //invalid request
		}
		$courses_t = new table_mycourses();
		$course_titles = array();
		foreach($ids as $key => $value){
			//if this is a user id
			if (strlen($value) != $courses_t->field_id_length){
				unset($ids[$key]);
			}
		}
		if (!empty($courses)){
			$query = 'SELECT '.$courses_t->field_course.' FROM '.TABLE_MYCOURSES
				.' WHERE '.$courses_t->field_id
				.' IN ('.toSQL($ids).') LIMIT '.count($ids);
			$result = db::query($query);
			while ($row = db::fetch_row($result)){
				$course_titles[] = $row[0];
			}
		}
		return $course_titles;
	}
	/**converts a string of user ids received from scrolling panel into 
	 *an array of ids*/
	public static function getScrollingIds($string){
		if (empty($string)){
			return array();
		}
		$users_t = new table_users();
		$userids = explode(',',$string);
		//the following can be used for simple verification
		foreach ($userids as $key => $value){
			//if invalid id
			if (strlen($value) != $users_t->field_id_length
				|| !preg_match('/^[a-z0-9]+$/i',$value)){
				unset($userids[$key]);
			}
		}
		return $userids;
	}
}
?>