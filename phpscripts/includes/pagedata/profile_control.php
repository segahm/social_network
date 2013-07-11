<?php
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/permissions.php");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/search/searchEngine.php");
class profile_control{
	public $owner_id;	//owner of the profile
	public $owner_permissions;
	public $owner_college_id;
	public $my_id;
	public $my_college_id;
	//access variables
	public $show_friends = false;
	public $show_login = false;
	public $show_lastupdated = false;
	public $show_email = false;
	public $access_denied = true;
	public $courses;
	public $friends;
	/*allows the use optional constants in case we already are aware of these
	 *values*/
	function profile_control($owner_id,$me,$my_college_id,
		$is_friends = null,$owner_permissions = null,
		$owner_collegeid = null){
		if (empty($me) || empty($owner_id)){
			exit;
		}
		$this->owner_id = $owner_id;
		$this->my_id = $me;
		$this->my_college_id = $my_college_id;
		//setting optional parameters
		$this->owner_permissions = $owner_permissions;
		$this->friends = $is_friends;
		$this->owner_college_id = $owner_collegeid;
		//begin check
		if ($this->checkAccess()){
			$this->access_denied = false;
		}else{
			$this->show_friends = false;
			$this->show_login = false;
			$this->show_lastupdated = false;
			$this->show_email = false;
			$this->access_denied = true;
		}
	}
	function canAccess(){
		return !$this->access_denied;
	}
	function is_me(){
		return ($this->owner_id == $this->my_id);
	}
	function is_same_college(){
		return ($this->owner_college_id == $this->my_college_id);
	}
	function getData(){
		if ($this->access_denied){
			return false;
		}
		$users_table = new table_users();
		$college_table = new table_colleges();
		$profile_table = new table_profile();
		$session_table = new table_session();
		$mycourses_table = new table_mycourses();
		$dep_t = new table_departments();
		//add these fields always if allowed to display
		$fields_array = array();
		$fields_array[] = TABLE_USERS.'.'.$users_table->field_name;
		$fields_array[] = TABLE_USERS.'.'.$users_table->field_school;
		$fields_array[] = TABLE_USERS.'.'.$users_table->field_membersince;
		$fields_array[] = TABLE_COLLEGES.'.'.$college_table->field_fullname;
		$fields_array[] = TABLE_COLLEGES.'.'.$college_table->field_majors;
		$fields_array[] = TABLE_COLLEGES.'.'.$college_table->field_houses;
		$fields_array[] = TABLE_USERS.'.'.$users_table->field_email;
		$fields_array[] = TABLE_USERS.'.'.$users_table->field_preferedemail;
		$fields_array[] = TABLE_PROFILE.'.*';
		$fields_array[] = TABLE_USERS.'.'.$users_table->field_lastseen;
		$fields_array[] = TABLE_SESSION.'.'.$session_table->field_time;
		$fields_array[] = TABLE_MYCOURSES.'.'.$mycourses_table->field_number;
		$fields_array[] = TABLE_MYCOURSES.'.'.$mycourses_table->field_course;
		$fields_array[] = TABLE_DEPARTMENTS.'.'.$dep_t->field_description;
		$fields = array_tostring($fields_array);
		unset($fields_array);	//garbage collect
		$query = 'SELECT '.$fields.' FROM (('.TABLE_USERS.','.TABLE_PROFILE
				.','.TABLE_COLLEGES
				.' LEFT JOIN '.TABLE_SESSION
				.' ON '.TABLE_SESSION.'.'.$session_table->field_userid
					." = '".$this->owner_id."') LEFT JOIN ".TABLE_MYCOURSES
				.' ON '.TABLE_MYCOURSES.'.'.$mycourses_table->field_myid
					." = '".$this->owner_id."') LEFT JOIN ".TABLE_DEPARTMENTS.' ON '
					.TABLE_MYCOURSES.'.'.$mycourses_table->field_department.' = '
					.TABLE_DEPARTMENTS.'.'.$dep_t->field_id
				.' WHERE '.TABLE_USERS.'.'.$users_table->field_id
					." = '".$this->owner_id
				."' AND ".TABLE_PROFILE.'.'.$profile_table->field_id
					." = '".$this->owner_id."' AND "
				.TABLE_COLLEGES.'.'.$college_table->field_id.' = '
				.TABLE_USERS.'.'.$users_table->field_school;
		$result = db::query($query);
		$row = db::fetch_assoc($result);
		$userdata = $row;
		//buffer courses
		$this->courses = array();
		if (!is_null($row[$mycourses_table->field_number])){
			do{
				$tempArray = array();
				$tempArray[$mycourses_table->field_number] = $row[$mycourses_table->field_number];
				$tempArray[$mycourses_table->field_course] = $row[$mycourses_table->field_course];
				$tempArray[$dep_t->field_description] = $row[$dep_t->field_description];
				$this->courses[] = $tempArray;
			}while($row = db::fetch_assoc($result));
		}
		/*now unset data that shouldn't be shown to the user
		 *that was automatically inserted if profileAccess = true
		 */
		if(!$this->show_lastupdated){
			unset($userdata[$profile_table->field_lastupdated]);
		}
		if(!$this->show_login){
			unset($userdata[$users_table->field_lastseen]);
			unset($userdata[$session_table->field_time]);
		}
		return $userdata;
	}
	function getCourses(){
		return $this->courses;
	}
	/**checks if access to the list of friends is allowed and gets the list*/
	function getFriends(&$totalResults,$limit){
		if ($this->access_denied || !$this->show_friends){
			return null;
		}
		$options = array();
		$options['type'] = SEARCH_TYPE_PEOPLE;
		$options['limit'] = $limit;
		$options['friends'] = $this->owner_id;
		$options['count'] = 0;	//we don't want to count all friends
		$search = new search($this->my_id,$this->my_college_id,$options);
		$result = $search->getResult();
		$list = array();
		$is_me = $this->is_me();
		//convert sql result to an array
		while ($array = db::fetch_assoc($result)){
			$list[] = $array;
		}
		/*imitate total count of all elements, however instead simply check if number of friends is at
		 *the limit and if it is, we can assume that it is very likely that there is more*/
		$found = count($list);
		if ($found == $options['limit']){
			$totalResults = $limit+1;
		}else{
			$totalResults = $found;
		}
		return $list;
	}
	/**SETS PERMISSION RIGHTS*/
	function checkAccess(){
		//if me no need to check anything
		if ($this->is_me()){
			$this->show_friends = true;
			$this->show_login = true;
			$this->show_lastupdated = true;
			$this->show_email = true;
			return true;
		}
		//if customary set friends
		if ($this->friends == true){
			$this->access_denied = false;
		}elseif(sqlFunc_is_blocked($this->my_id,$this->owner_id)){
			return false;
		}
		if (is_null($this->owner_permissions) 
			|| is_null($this->owner_college_id)
			|| is_null($this->friends)){
			$users_table = new table_users();
			$friends_table = new table_friends();
			$tempfriends_t = new table_tempfriends();
			/**get permissions of the owner if no optional parameters 
			 *were specified*/
			$query = 'SELECT '
				.TABLE_USERS.'.'.$users_table->field_permissions.', '
				.TABLE_USERS.'.'.$users_table->field_school.','
				/*the following 2 fields are for friendship comparison only
				 *where the first one is the signifier of the acutal friendship
				 *and the second is when owner of the profile has invited me to
				 *become friends 
				 *(i.e. if both null then we are not friends in any way)*/
				.TABLE_FRIENDS.'.'.$friends_table->field_user2.','
				.TABLE_TEMPFRIENDS.'.'.$tempfriends_t->field_invited
				.' FROM ('.TABLE_USERS.' LEFT JOIN '.TABLE_FRIENDS.' ON '
				.TABLE_FRIENDS.'.'.$friends_table->field_user1.' = '
				.TABLE_USERS.'.'.$users_table->field_id.' AND '
				.TABLE_FRIENDS.'.'.$friends_table->field_user2." = '"
				.$this->my_id."') LEFT JOIN ".TABLE_TEMPFRIENDS.' ON '
				.TABLE_TEMPFRIENDS.'.'.$tempfriends_t->field_invited." = '"
				.$this->my_id."' AND ".TABLE_TEMPFRIENDS.'.'
				.$tempfriends_t->field_inviter.' = '.TABLE_USERS.'.'
				.$users_table->field_id.' WHERE '
				.TABLE_USERS.'.'.$users_table->field_id.' = '
				.toSQL($this->owner_id).' LIMIT 1';
			$result = db::query($query);
			if ($row = db::fetch_row($result)){
				$this->owner_permissions = $row[0];
				$this->owner_college_id = $row[1];
				if (is_null($row[2]) && is_null($row[3])){
					$this->friends = false;
				}else{
					if (!is_null($row[2])){
						$this->friends = true;
					}elseif(!is_null($row[3])){
						$futureFriends = true;
					}
				}
			}else{
				//user doesn't exists - no access
				return false;
			}
		}
		if ($this->owner_permissions & Permissions::PERM_PROFILE_SHOWPROFUPDATED){
			$this->show_lastupdated = true;
		}
		if ($this->owner_permissions & Permissions::PERM_PROFILE_SHOWLOGIN){
			$this->show_login = true;
		}
		if ($this->owner_permissions & Permissions::PERM_PROFILE_SHOWFRIENDS){
			$this->show_friends = true;
		}
		if ($this->owner_permissions & Permissions::PERM_PROFILE_SHOWEMAIL){
			$this->show_email = true;
		}
		//if already granted access
		if ($this->access_denied == false){
			return true;
		}
		//if we are in the same college
		if ($this->my_college_id == $this->owner_college_id){
			//if all pref. are set for college - no need to check any further
			if ($this->owner_permissions & Permissions::PERM_PROFILE_ACCESS_PUBLIC){
				return true;
			}
			//else check wherever we are friends
			return ($this->friends || (isset($futureFriends) && $futureFriends));				
		}else{
			//in different colleges (i.e. only can see if both are friends)
			//check for friends
			return ($this->friends || (isset($futureFriends) && $futureFriends));
		}
		//if still here then no access
		return false;
	}
	public function is_friends(){
		return ($this->friends === true);
	}
	public function is_sameCollege(){
		return ($this->owner_college_id === $this->my_college_id);
	}
}
?>