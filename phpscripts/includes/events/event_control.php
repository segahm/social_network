<?php
require_once($_SERVER['DOCUMENT_ROOT']
	."/phpscripts/includes/permissions.php");
define('EVENT_PERM_SCHOOL',1);
define('EVENT_PERM_FRIENDS',2);
define('EVENT_PERM_SPECIFIED',3);
define('EVENT_PERM_MAJOR',4);
define('EVENT_PERM_COURSES',5);
define('EVENT_RIGHTS_PUBLIC',1);
define('EVENT_RIGHTS_PRIVATE',2);
define('EVENT_RIGHTS_MY',3);
define('EVENT_RIGHTS','rights');
class eventster{
	public $user_id;
	public $school_id;
	function eventster($id,$schoolid){
		$this->user_id = $id;
		$this->school_id = $schoolid;
	}
	/*removes any events from my event's list; if this is my event
	 *then it will be removed completely, otherwise its id will be
	 *added to my deleted events list and will not be displayed in the future*/
	function removeGeneralEvent($event_id){
		$events_t = new table_events();
		//first try to remove it from my events (in case this is my event)
		if(strlen($event_id) != $events_t->field_id_length){
			return false;
		}elseif ($this->removeEvent($event_id)){
			return true;	//success delting my event
		}else{
			//now check that this event exists
			$query = 'SELECT '.$events_t->field_date.' FROM '.TABLE_EVENTS
				.' WHERE '.$events_t->field_id.' = '.toSQL($event_id).' LIMIT 1';
			$result = db::query($query);
			if ($row = db::fetch_row($result)){
				$delevents_t = new table_deletedevents();
				$query = 'REPLACE INTO '.TABLE_DELETEDEVENTS
					.' ('.$delevents_t->field_eventid.','
					.$delevents_t->field_myid.','
					.$delevents_t->field_date.") values ('"
					.$event_id."','".$this->user_id."','".$row[0]."')";
				$rowsAffected = db::query($query);
				return ($rowsAffected == 1);
			}else{
				return false; //event wasn't found
			}
		}
	}
	function removeEvent($event_id){
		$events_t = new table_events();
		$inevents_t = new table_ids();
		if (empty($event_id) || strlen($event_id) != $events_t->field_id_length){
			return false;
		}
		$query = 'DELETE FROM '.TABLE_EVENTS.' WHERE '
				.TABLE_EVENTS.'.'.$events_t->field_id
				.' = '.toSQL($event_id).' AND '
				.TABLE_EVENTS.'.'.$events_t->field_postedby
				." = '".$this->user_id."' LIMIT 1";
		db::query($query);
		/*make sure that 1 row was deleted otherwise it might be
		 *that the user doesn't own the event*/
		if (db::affected_rows() == 1){
			$query = 'DELETE LOW_PRIORITY FROM '.TABLE_IDS.' WHERE '
				.TABLE_IDS.'.'.$inevents_t->field_ownerid
				." = '".$event_id."'";
			db::query($query);
			//also delete all entries that 
			$delevents_t = new table_deletedevents();
			$query = 'DELETE LOW_PRIORITY FROM '.TABLE_DELETEDEVENTS.' WHERE '
				.TABLE_DELETEDEVENTS.'.'.$delevents_t->field_eventid
				." = '".$event_id."'";
			db::query($query);
			return true;
		}else{
			return false;
		}
	}
	/*returns null on success, error otherwise, false if invalid request*/
	function post_form($form,&$date_string){
		$parser = new parser($form);
		$events_t = new table_events();
		$inevents_t = new table_ids();
		$fields = array();	//fields to be inserted
		//begin checking input data
		$fields[$events_t->field_title] = $parser->getString('title','');
		if (empty($fields[$events_t->field_title])){
			return 'please enter the title for this event';
		}
		$field_month = $parser->getInt('month',-1);
		$field_day = $parser->getInt('day',-1);
		if ($field_month<1 || $field_month>12
			|| $field_day<1 || $field_day>31){
			return 'please select a valid date for this event';
		}
		/*check if posting to the date that we passed in this year
		 *and if yes then increment a year*/
		 if ((int)mktime(0,0,0,$field_month,$field_day)<(int)mktime(0,0,0)){
			$nextYear = (int)date('Y',time())+1;
			$fields[$events_t->field_date] = date('Y-m-d',
						mktime(0,0,0,$field_month,$field_day,$nextYear));
		 }else{
			$fields[$events_t->field_date] = date('Y-m-d',
						mktime(0,0,0,$field_month,$field_day));
		}
		$date_string = $fields[$events_t->field_date];
		$fields[$events_t->field_type] =  $parser->getString('type','');
		if ($fields[$events_t->field_type] == 'other'){
			$fields[$events_t->field_type] = $parser->getString('type_other',null);
		}
		$fields[$events_t->field_type] = preg_replace('/[^a-z0-9 ]/i','',$fields[$events_t->field_type]);
		if (empty($fields[$events_t->field_type])){
			return 'please select the type of event this is';
		}
		if ($fields[$events_t->field_type] == 'party'){
			$fields[$events_t->field_location] = $parser->getString('location',null);
			$fields[$events_t->field_host] = $parser->getString('host',null);
			$fields[$events_t->field_time] = $parser->getString('time',null);
			if (is_null($fields[$events_t->field_location])){
				return 'please enter the location for this party';
			}
			if (is_null($fields[$events_t->field_host])){
				return 'please enter the names of people who will be hosting this party';
			}
			if (is_null($fields[$events_t->field_time])){
				return 'please enter the time this party will begin';
			}
		}
		$fields[$events_t->field_permissions] = $parser->getInt('view',1);
		if ($fields[$events_t->field_permissions]<EVENT_PERM_SCHOOL 
			|| $fields[$events_t->field_permissions]>EVENT_PERM_COURSES){
			return false;
		}
		if ($fields[$events_t->field_permissions] == EVENT_PERM_SPECIFIED){
			$field_users = $parser->getString('users','');
			$field_groups = $parser->getString('groups','');
			$field_ids = $parser->getString('selfriends','');
			//get a list of these users's ids
			$field_users = quickAddIn::getListOfIds($field_users,$this->user_id);
			$field_groups = quickAddIn::getListOfGroupIds($field_groups,$this->user_id);
			$field_users = array_merge($field_users,quickAddIn::getScrollingIds($field_ids));
			if (empty($field_users) && empty($field_groups)){
				return 'please enter friends/groups you wish to grant access to this event';
			}
		}elseif($fields[$events_t->field_permissions] == EVENT_PERM_COURSES){
			$field_courses = $parser->getString('courses','');
			$field_courses = quickAddIn::getListOfCourseIds($field_courses,$this->user_id);
			if (empty($field_courses)){
				return 'please enter course titles that will be granted access to this event';
			}
		}
		$fields[$events_t->field_text] = $parser->getString('description',NULL);
		//generate new event id
		$fields[$events_t->field_id] = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$fields[$events_t->field_id] = substr($fields[$events_t->field_id], 0, $events_t->field_id_length);
		
		/*now delete any old entries if old id was specified*/
		$old_eventid = $parser->getString('oldid',null);
		$this->removeEvent($old_eventid);
		//finally append owner's id
		$fields[$events_t->field_postedby] = $this->user_id;
		
		$query = 'INSERT INTO '.TABLE_EVENTS.' ('.sqlFields(array_keys($fields))
				.') values ('.toSQL($fields).')';
		db::query($query);
		//now insert ids of users and groups that are allowed to see this
		if (!empty($field_users) || !empty($field_groups) || !empty($field_courses)){
			if (empty($field_courses)){
				$ids = array_merge($field_users,$field_groups);
			}else{
				$ids = $field_courses;
			}
			//don't wait for this to complete since it onlu effects other users
			$query = 'INSERT DELAYED INTO '.TABLE_IDS.' ('.$inevents_t->field_ownerid.','.$inevents_t->field_subjectid
					.') VALUES ';
			$values = array();
			foreach($ids as $id){
				$values[] = "('".$fields[$events_t->field_id]."','".$id."')";
			}
			$query .= implode(',',$values);
			db::query($query);
		}
	}
	function getMyEventTitles(){
		$events_t = new table_events();
		$fields = array();
		$fields[] = $events_t->field_id;
		$fields[] = $events_t->field_title;
		$query = 'SELECT '.array_tostring($fields).' FROM '.TABLE_EVENTS.' WHERE '
				.$events_t->field_postedby." = '".$this->user_id
				."' ORDER BY ".$events_t->field_title;
		$result = db::query($query);
		$my_events = array();
		while ($row = db::fetch_row($result)){
			$my_events[$row[0]] = $row[1];
		}
		return $my_events;
	}
	function getEvent($eventid){
		$events_t = new table_events();
		$users_t = new table_users();
		$profile_t = new table_profile();
		$college_t = new table_colleges();
		$fields_array = array();
		$fields_array[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields_array[] = TABLE_EVENTS.'.*';
		$fields_array[] = TABLE_COLLEGES.'.'.$college_t->field_fullname.' as college_name';
		$fields_array[] = TABLE_USERS.'.'.$users_t->field_school;
		$fields = array_tostring($fields_array);
		$query = 'SELECT '.$fields.' FROM '.TABLE_EVENTS.','.TABLE_USERS
			.','.TABLE_PROFILE.','.TABLE_COLLEGES.' WHERE '.TABLE_EVENTS.'.'.$events_t->field_id
			.' = '.toSQL($eventid).' AND '
			.TABLE_EVENTS.'.'.$events_t->field_postedby.' = '
			.TABLE_USERS.'.'.$users_t->field_id.' AND '
			.TABLE_COLLEGES.'.'.$college_t->field_id.' = '
			.TABLE_USERS.'.'.$users_t->field_school.' LIMIT 1';
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			return $row;
		}else{
			return null;
		}
	}
	function getEventData($id = null){
		$events_t = new table_events();
		if (is_null($id) || strlen($id)!=$events_t->field_id_length){
			return $this->getDefaultData();
		}
		$inevents_t = new table_ids();
		$query = 'SELECT * FROM '.TABLE_EVENTS.' LEFT JOIN '
			.TABLE_IDS.' ON '
			.TABLE_EVENTS.'.'.$events_t->field_id.' = '
			.TABLE_IDS.'.'.$inevents_t->field_ownerid.' WHERE '
			.TABLE_EVENTS.'.'.$events_t->field_id.' = '.toSQL($id).' AND '
			.TABLE_EVENTS.'.'.$events_t->field_postedby." = '"
			.$this->user_id."'";
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			/*first, create fields array that contains 1 to 1 correspondense between
			 *data stored in db and form*/
			$fields = array();
			$fields['event_select'] = $row[$events_t->field_id]; 
			$fields['oldid'] = $row[$events_t->field_id];
			$fields['title'] = $row[$events_t->field_title];
			$fields['description'] = $row[$events_t->field_text];
			$fields['view'] = $row[$events_t->field_permissions];
			$fields['location'] = $row[$events_t->field_location];
			$fields['host'] = $row[$events_t->field_host];
			$fields['time'] = $row[$events_t->field_time];
			/*set select type to this type since it will simply
			 *be ignored if not in the select and then set other to it also*/
			$fields['type'] = $row[$events_t->field_type];
			$fields['type_other'] = $row[$events_t->field_type];
			//now work with date
			$timeStamp = funcs_getTimeStamp($row[$events_t->field_date]);
			$fields['month'] = date('n',$timeStamp);
			$fields['day'] = date('j',$timeStamp);
			//now iterate to find out all ids used in this event
			$ids = array();
			do{
				$ids[] = $row[$inevents_t->field_subjectid];
			}while($row = db::fetch_assoc($result));
			//now get a list of names and group title
			if (!empty($ids)){
				$users = array();
				$groups = array();
				$courses = array();
				quickAddIn::getListOfNames_Titles($ids,$users,$groups,$courses);
				if (!empty($users)){
					$fields['users'] = array_tostring($users);
				}
				if (!empty($groups)){
					$fields['groups'] = array_tostring($groups);
				}
				if (!empty($courses)){
					$fields['courses'] = array_tostring($courses);
				}
			}
			return $fields;
		}else{
			return $this->getDefaultData();
		}
				
	}
	/**sets default values for the form data*/
	function getDefaultData(){
		$fields = array();
		$fields['view'] = 1;
		$fields['day'] = date('j');
		$fields['month'] = date('n');
		return $fields;
	}
	function toString($value){
		if (is_null($value)){
			return '';
		}else{
			return $value;
		}
	}
	/**returns an array with days (1-{28,30,31}) as keys and
	 *arrays of names, titles, event_rights for this data as values
	*/
	function calendarEvents($year,$month,$perm_string){
		$result = $this->getEvents($year,$month,null,$perm_string,
			false,false,false,null,0,false,null,null,true);
		if (is_null($result)){
			return array();
		}
		$events_t = new table_events;
		$users_t = new table_users();
		$dates = array();
		//create an array with dates as keys
		while (($row = db::fetch_assoc($result))){
			$date = date('j',funcs_getTimeStamp($row[$events_t->field_date]));
			$names = funcs_getNames($row[$users_t->field_name]);
			//only add 1 value for each date
			if (!isset($dates[$date])){
				$dates[$date] = array(
						'title' => toHTML($row[$events_t->field_title]),
						'name' => $names[0],
						EVENT_RIGHTS => $row[EVENT_RIGHTS]);
			}
		}
		return $dates;
	}
	/*gets events that are meant for this user to see depending
	 *on the conditions specified 
	 *(default settings are for the calendar display)*/
	function getEvents($year,$month,$days_array = null,$perm_string,
						$all_fields = false,
						$order = false,$byTitle = false,
						$limit = null, $start = 0,$count = false,
						$query = null,$type = null,$withRights = false){
		//set permissions
		if (!is_null($perm_string)){
			$privateEvents = Permissions::PERM_EVENT_PRIVATE & $perm_string;				
			$publicEvents = Permissions::PERM_EVENT_PUBLIC & $perm_string;	
		}else{
			$privateEvents = true;			
			$publicEvents = true;
		}
		$events_t = new table_events;
		$users_t = new table_users();
		$friends_t = new table_friends();
		$ingroups_t = new table_friends_in_groups();
		$inevents_t = new table_ids();
		$delevents_t = new table_deletedevents();
		$profile_t = new table_profile();
		$fields = array();
		/*where this is a brief search (i.e calendar) or full*/
		if ($all_fields){
			$fields_array[] = TABLE_EVENTS.'.*';
			$fields_array[] = TABLE_USERS.'.'.$users_t->field_school;
		}else{
			$fields_array[] = TABLE_EVENTS.'.'.$events_t->field_date;
			$fields_array[] = TABLE_EVENTS.'.'.$events_t->field_title;
		}
		$fields_array[] = TABLE_USERS.'.'.$users_t->field_name;
		if ($count){
			$count = ' SQL_CALC_FOUND_ROWS ';
		}else{
			$count = '';
		}
		$fields = array_tostring($fields_array);
		if (!is_null($year) && !is_null($month)){
			$year = date('Y',mktime(1,1,1,1,1,(int)$year));
			$month = date('n',mktime(1,1,1,$month));
			if (!empty($days_array)){
				//construct dateformat string
				$dateFormat = 'DATE_FORMAT('
					.TABLE_EVENTS.'.'.$events_t->field_date
					.",'%Y-%c') = '$year-$month' AND "
					.'DATE_FORMAT('.TABLE_EVENTS.'.'.$events_t->field_date
					.",'%d') IN (".toSQL($days_array).') AND ';
			}else{
				$dateFormat = 'DATE_FORMAT('
					.TABLE_EVENTS.'.'.$events_t->field_date
					.",'%Y-%c') = '$year-$month' AND ";
			}
		}else{
			$dateFormat = '';
		}
		if (!is_null($query)){
			$queryClause = ' AND MATCH ('
				.TABLE_EVENTS.'.'.$events_t->field_title.','
				.TABLE_EVENTS.'.'.$events_t->field_type
				.') AGAINST ('.toSQL($query).' IN BOOLEAN MODE)';
		}else{
			$queryClause = '';
		}
		if (!is_null($type)){
			$queryClause .= ' AND '.TABLE_EVENTS.'.'.$events_t->field_type
				.' = '.toSQL($type);
		}
		//construct string for my deleted events
		$deletedJoin = ' LEFT JOIN '.TABLE_DELETEDEVENTS.' ON '
				.TABLE_DELETEDEVENTS.'.'.$delevents_t->field_eventid
				.' = '.TABLE_EVENTS.'.'.$events_t->field_id.' AND '
				.TABLE_DELETEDEVENTS.'.'.$delevents_t->field_myid
				." = '".$this->user_id."' ";
		$deletedWhereClause = TABLE_DELETEDEVENTS.'.'.$delevents_t->field_date.' IS NULL';
		$unionSelects = array();
		//if we need to find out wherever this is a private event or public
		if ($withRights){
			$aditField = ", '".EVENT_RIGHTS_MY."' as ".EVENT_RIGHTS;
		}else{
			$aditField = '';
		}
		//get events posted by me
		$unionSelects[] = 'SELECT DISTINCT '.$count.$fields.$aditField.' FROM '
			.TABLE_EVENTS.','.TABLE_USERS.' WHERE '.$dateFormat
			.TABLE_EVENTS.'.'.$events_t->field_postedby
			." = '".$this->user_id."' AND "
			.TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_EVENTS.'.'.$events_t->field_postedby.$queryClause;
		/*find events for my school*/
		if ($publicEvents){
			//if we need to find out wherever this is a private event or public
			if ($withRights){
				$aditField = ", '".EVENT_RIGHTS_PUBLIC."' as ".EVENT_RIGHTS;
			}else{
				$aditField = '';
			}
			$unionSelects[] = 'SELECT DISTINCT '.$fields.$aditField.' FROM '
				.TABLE_EVENTS.','.TABLE_USERS.$deletedJoin
				.' WHERE '.$dateFormat
				.TABLE_EVENTS.'.'.$events_t->field_permissions
				." = '".EVENT_PERM_SCHOOL."' AND "
				.TABLE_EVENTS.'.'.$events_t->field_postedby.' = '
				.TABLE_USERS.'.'.$users_t->field_id.' AND '
				.TABLE_USERS.'.'.$users_t->field_school
				." = '".$this->school_id."'".' AND '.$deletedWhereClause.$queryClause;
			//clear count since we already used it
			$count = '';
			/*find all events with permissions for major and that have 
			 *owners in the same major;*/
			$unionSelects[] = 'SELECT DISTINCT '.$fields.$aditField.' FROM '
				.TABLE_EVENTS.','.TABLE_USERS.','.TABLE_PROFILE.' as myprofile,'
				.TABLE_PROFILE.$deletedJoin.' WHERE '.$dateFormat
				.TABLE_EVENTS.'.'.$events_t->field_permissions
				." = '".EVENT_PERM_MAJOR."' AND "
				.TABLE_EVENTS.'.'.$events_t->field_postedby
				.' = '.TABLE_USERS.'.'.$users_t->field_id.' AND '
				.TABLE_USERS.'.'.$users_t->field_id.' = '
				.TABLE_PROFILE.'.'.$profile_t->field_id.' AND '
				.'myprofile.'.$profile_t->field_id
				." = '".$this->user_id."' AND ("
				.'myprofile.'.$profile_t->field_major
				.' IN ('.TABLE_PROFILE.'.'.$profile_t->field_major.','
				.TABLE_PROFILE.'.'.$profile_t->field_majorsecond.') OR '
				.'myprofile.'.$profile_t->field_majorsecond
				.' IN ('.TABLE_PROFILE.'.'.$profile_t->field_major.','
				.TABLE_PROFILE.'.'.$profile_t->field_majorsecond.')) AND '
				.TABLE_USERS.'.'.$users_t->field_school
				." = '".$this->school_id."'".' AND '.$deletedWhereClause.$queryClause;
				/*find people with the same courses by taking id[s] that were gathered from
				 *courses specified on the list and connecting them to their title and then
				 *comparing titles as strings*/
				$courses_t = new table_mycourses();
				$unionSelects[] = 'SELECT DISTINCT '.$fields.$aditField.' FROM '
					.TABLE_EVENTS.','.TABLE_USERS.','.TABLE_IDS.','
					.TABLE_MYCOURSES.','.TABLE_MYCOURSES.' as myc'
					.$deletedJoin.' WHERE '.$dateFormat
					.TABLE_EVENTS.'.'.$events_t->field_permissions
					." = '".EVENT_PERM_COURSES."' AND "
					.TABLE_EVENTS.'.'.$events_t->field_postedby
					.' = '.TABLE_USERS.'.'.$users_t->field_id.' AND '
					.TABLE_USERS.'.'.$users_t->field_school
					." = '".$this->school_id."' AND "
					.TABLE_IDS.'.'.$inevents_t->field_ownerid
					.' = '.TABLE_EVENTS.'.'.$events_t->field_id.' AND '
					.TABLE_MYCOURSES.'.'.$courses_t->field_id.' = '
					.TABLE_IDS.'.'.$inevents_t->field_subjectid
					.' AND myc.'.$courses_t->field_myid
					." = '".$this->user_id."' AND "
					.' STRCMP(LOWER(myc.'.$courses_t->field_course.'),LOWER('
					.TABLE_MYCOURSES.'.'.$courses_t->field_course.')) = 0'
					.' AND '.$deletedWhereClause.$queryClause;
					
		}
		if ($privateEvents){
			//if we need to find out wherever this is a private event or public
			if ($withRights){
				$aditField = ", '".EVENT_RIGHTS_PRIVATE."' as ".EVENT_RIGHTS;
			}else{
				$aditField = '';
			}
			/*get all events that allow the owner's friends to see the event
			 *and where me is the owner's friend*/
			$unionSelects[] = 'SELECT DISTINCT '.$count.$fields.$aditField.' FROM '
				.TABLE_EVENTS.','.TABLE_FRIENDS.','.TABLE_USERS
				.$deletedJoin.' WHERE '.$dateFormat
				.TABLE_EVENTS.'.'.$events_t->field_permissions
				." = '".EVENT_PERM_FRIENDS."' AND "
				.TABLE_EVENTS.'.'.$events_t->field_postedby.' = '
				.TABLE_FRIENDS.'.'.$friends_t->field_user1.' AND '
				.TABLE_FRIENDS.'.'.$friends_t->field_user2." = '"
				.$this->user_id."' AND "
				.TABLE_EVENTS.'.'.$events_t->field_postedby.' = '
				.TABLE_USERS.'.'.$users_t->field_id
				.' AND '.$deletedWhereClause.$queryClause;
			//get all events that specify this user
			$unionSelects[]  = 'SELECT DISTINCT '.$fields.$aditField.' FROM '
				.TABLE_EVENTS.','.TABLE_USERS.','.TABLE_IDS
				.$deletedJoin.' WHERE '.$dateFormat
				.TABLE_EVENTS.'.'.$events_t->field_permissions
				." = '".EVENT_PERM_SPECIFIED."' AND "
				.TABLE_EVENTS.'.'.$events_t->field_postedby.' = '
				.TABLE_USERS.'.'.$users_t->field_id.' AND '
				.TABLE_IDS.'.'.$inevents_t->field_ownerid
				.' = '.TABLE_EVENTS.'.'.$events_t->field_id
				.' AND '
				.TABLE_IDS.'.'.$inevents_t->field_subjectid
				." = '".$this->user_id."'"
				.' AND '.$deletedWhereClause.$queryClause;
			/*get all entries which specify one of the groups that this 
			 *user belongs to*/
			$unionSelects[] = 'SELECT DISTINCT '.$fields.$aditField.' FROM (('
				.TABLE_EVENTS.','.TABLE_USERS.' INNER JOIN '.TABLE_IDS
				.' ON '.TABLE_IDS.'.'.$inevents_t->field_ownerid
				.' = '.TABLE_EVENTS.'.'.$events_t->field_id
				.') INNER JOIN '.TABLE_FRIENDS_IN_GROUPS.' ON '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$ingroups_t->field_groupid.' = '
				.TABLE_IDS.'.'.$inevents_t->field_subjectid.' AND '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$ingroups_t->field_userid
				." = '".$this->user_id."') ".$deletedJoin
				.' WHERE '.$dateFormat
				.TABLE_EVENTS.'.'.$events_t->field_permissions
				." = '".EVENT_PERM_SPECIFIED."' AND "
				.TABLE_EVENTS.'.'.$events_t->field_postedby.' = '
				.TABLE_USERS.'.'.$users_t->field_id
				.' AND '.$deletedWhereClause.$queryClause;
		}
		//combine selectes into a union
		$query = '';
		$first = true;
		foreach ($unionSelects as $single_select){
			if (!$first){
				$query .= ' UNION ';
			}
			$first = false;
			$query .= '('.$single_select.')';
		}
		//construct order by clause
		if ($order){
			$query .= ' ORDER BY ';
			$query .= ($byTitle) ? $events_t->field_title : $events_t->field_date;
		}
		//construct limit clause
		if (!empty($limit) && is_numeric($limit)){
			if (!is_numeric($start) || $start<0){
				$start = 0;
			}
			$query .= " LIMIT $start, $limit";
		}
		$result = db::query($query);
		return $result;
	}
	function findUpcoming($plusHowMany = 1){
		$year = date('Y',time());
		$month = date('m',time());
		$today = date('j',time());
		//add dates within today
		$days_array = array();
		for ($i=0;$i<=$plusHowMany;$i++){
			$days_array[$i] = date('d',mktime(0,0,0,0,$today+$i));
		}
		$result = $this->getEvents($year,$month,$days_array,null,true,true,false,3);
		$events_t = new table_events();
		$eventsByDate = array();
		/*iterate through events and group them into one array by date
		 *so that rows are arranged by date with the offset == 0 at today's
		 *date*/
		while($row = db::fetch_assoc($result)){
			$eventsDate = date('d',funcs_getTimeStamp($row[$events_t->field_date]));
			foreach($days_array as $offset => $day){
				if (strcmp($eventsDate,$day)==0){
					//found offset that matches this date
					if (isset($eventsByDate[$offset])){
						$eventsByDate[$offset][] = $row;
					}else{
						$eventsByDate[$offset] = array($row);
					}
					break;
				}
			}
		}
		return $eventsByDate;
	}
}
?>