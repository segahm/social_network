<?php
require_once($_SERVER['DOCUMENT_ROOT']
	."/phpscripts/includes/events/event_control.php");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/trades/trade_interface.inc");
define('WHATSEARCH','w');
define('WHATSEARCH_EMAIL',1);
define('WHATSEARCH_FNAME',2);
define('WHATSEARCH_LNAME',3);
define('WHATSEARCH_FULLNAME',4);
define('SEARCH_RESULTS_PER_PAGE',10);
class search{
	public $query;
	public $type;	//type of search (people, books...)
	public $user_id;	//viwer's id
	public $options;	//additional search parameters (i.e. for advanced search)
	public $start_results;	//0~
	public $page;
	public $numb_results;	//# of results per page
	public $result;	//result link
	public $total;	//total found
	public $college_id;
	public $invalid_request = false;
	public $random_seed;
	function search($user_id,$college_id,$options){
		$parser = new parser($options);
		$this->type = $parser->getInt('type',0);
		if ($this->type == 0 || $this->type > SEARCH_TYPE_MAX 
			|| $this->type < 1){
			$this->invalid_request = true;
			return;
		}
		$this->page = $parser->getInt('page',1);
		$this->numb_results = $parser->getInt('limit',SEARCH_RESULTS_PER_PAGE);
		if ($this->numb_results>99 || $this->numb_results<2){
			$this->numb_results = SEARCH_RESULTS_PER_PAGE;
		}
		$this->start_results = ($this->page-1)*$this->numb_results;
		$this->query = strtolower(trim($parser->getString('q','')));
		//cut string if too long
		if (strlen($this->query)>255){
			$this->query = substr($this->query,0,255);
		}
		//create random seed if one wasn't passed
		$this->random_seed = $parser->getInt('r',rand(1,100));
		$this->user_id = $user_id;
		$this->college_id = $college_id;
		$this->options = $options;
		switch($this->type){
			case SEARCH_TYPE_PEOPLE:
				$this->search_people();
			break;
			case SEARCH_TYPE_MARKET:
				$this->search_market();
			break;
			case SEARCH_TYPE_EVENT:
				$this->search_event();
			break;
			case SEARCH_TYPE_PARTY:
				$this->search_event('party');
			break;
			case SEARCH_TYPE_PHOTO:
				$this->search_photo();
			break;
			case SEARCH_TYPE_RIDE:
				$this->search_ride();
			break;
		}
	}
	function getQuery(){
		return $this->query;
	}
	function getRandomSeed(){
		return $this->random_seed;
	}
	function is_invalidRequest(){
		return $this->invalid_request;
	}
	function getResult(){
		return $this->result;
	}
	/**returns total # of results not just on this page*/
	function getTotal(){
		return $this->total;
	}
	function getStart(){
		if ($this->total != 0){
			return $this->start_results+1;
		}else{
			return $this->start_results;
		}
	}
	function getResultsPerPage(){
		return $this->numb_results;
	}
	/**by default searches in first & last names, and email
	 *else it searches only in the fields specified*/
	function search_people(){
		$query = $this->query;
		$conditions = array();	//will store independent (OR) cond.
		$and_conditions = array(); //will store independent (AND) cond.
		$tables = array(); //will store table for 'from' sql part
		$emails = null;	//an array that will store found email in a query
		$users_table = new table_users();
		$profile_table = new table_profile();
		$friends_table = new table_friends();
		$college_table = new table_colleges();
		/*apply advanced search (name/email formating) only if query is
		 *not empty*/
		if (!empty($query)){
			//if no advanced options on "what" are specified, 
			//then do a default search,
			//these conditions should be treated as "OR" conditions
			if (!isset($this->options[WHATSEARCH]) || $this->options[WHATSEARCH]>5
				|| $this->options[WHATSEARCH]<1){			
				$words = preg_split('/[^a-z\.@\-_]/i',$query);
				foreach($words as $aword){
					if (funcs_isValidEmail($aword)){
						$emails[] = $aword;
						$query = str_replace($aword,'',$query);
					}
				}
				//now get rid of all invalid characters
				$query = preg_replace('/(?:[^a-z]|(?:[ ]+))/i',' ',$query);
				//$query = preg_replace('/([a-z]{4,})(([ ])|($))/i','\\1*\\2',$query);
				if (empty($query) && empty($emails)){
					$this->noResults();
					return false;
				}	
				if (!empty($query)){
					//search each individual word separately
					$words = preg_split('/[ ]+/',$query);
					foreach($words as $word){
						$conditions[] = TABLE_USERS.'.'.$users_table->field_name
							.' LIKE '.toSQL($word.',%').' OR '
							.TABLE_USERS.'.'.$users_table->field_name
							.' LIKE '.toSQL('%, '.$word);
					}
				}
			}elseif ($this->options[WHATSEARCH] == WHATSEARCH_EMAIL){
				$words = preg_split('/[^a-z\.@\-_]/i',$query);
				foreach($words as $aword){
					if (funcs_isValidEmail($aword)){
						$emails[] = $aword;
						$query = str_replace($aword,'',$query);
					}
				}
			}elseif($this->options[WHATSEARCH] == WHATSEARCH_FNAME){
				//now get rid of all invalid characters
				$query = preg_replace('/[^a-z]/i',' ',$query);
				$query = preg_replace('/[ ]+/',' ',trim($query));
				//spliting into words
				$words = preg_split('/[ ]/',$query);
				foreach($words as $aword){
					//adding each word as a separate (independent) entry
					$conditions[] .= TABLE_USERS.'.'.$users_table->field_name
						." LIKE '%, $aword'";
				}
			}elseif($this->options[WHATSEARCH] == WHATSEARCH_LNAME){
				//now get rid of all invalid characters
				$query = preg_replace('/[^a-z]/i',' ',$query);
				$query = preg_replace('/[ ]+/',' ',trim($query));
				//spliting into words
				$words = preg_split('/[ ]/',$query);
				foreach($words as $aword){
					//adding each word as a separate (independent) entry
					$conditions[] .= TABLE_USERS.'.'.$users_table->field_name
					." LIKE '$aword, %'";
				}
			}elseif($this->options[WHATSEARCH] == WHATSEARCH_FULLNAME){
				//now get rid of all invalid characters
				$query = preg_replace('/[^a-z]/i',' ',$query);
				$query = preg_replace('/[ ]+/',' ',trim($query));
				$query = preg_replace('/([a-z])(([ ])|($))/i','\\1*\\2',$query);
				$conditions[] = 'MATCH ('
					.TABLE_USERS.'.'.$users_table->field_name
					.") AGAINST (".toSQL($query)." IN BOOLEAN MODE)";
			}
		}
		if (!empty($emails)){
			$conditions[] = TABLE_USERS.'.'.$users_table->field_email
				.' IN ('.toSQL($emails).')';
		}
		/*now append "AND" conditions if thus exist*/
		if (!empty($this->options['city'])){
			$and_conditions[] = 'MATCH ('
					.TABLE_PROFILE.'.'.$profile_table->field_city
					.") AGAINST (".toSQL($this->options['city'])." IN BOOLEAN MODE)";
		}
		if (!empty($this->options['locat'])){
			$and_conditions[] = 'MATCH ('
					.TABLE_PROFILE.'.'.$profile_table->field_address.','
					.TABLE_PROFILE.'.'.$profile_table->field_zip.','
					.TABLE_PROFILE.'.'.$profile_table->field_city
					.") AGAINST (".toSQL($this->options['locat'])." IN BOOLEAN MODE)";
		}
		if (!empty($this->options['state'])){
			$and_conditions[] = TABLE_PROFILE.'.'.$profile_table->field_state
								." = ".toSQL($this->options['state']);
		}
		if (!empty($this->options['status'])){
			$and_conditions[] = TABLE_PROFILE.'.'.$profile_table->field_status
								." = ".toSQL($this->options['status']);
		}
		if (!empty($this->options['zip'])){
			$and_conditions[] = TABLE_PROFILE.'.'.$profile_table->field_zip
								." = ".toSQL($this->options['zip']);
		}
		if (!empty($this->options['year'])){
			$and_conditions[] =  TABLE_PROFILE.'.'.$profile_table->field_year
								." = ".toSQL($this->options['year']);
		}
		//major's number
		if (!empty($this->options['major'])){
			$and_conditions[] = '('.TABLE_PROFILE.'.'.$profile_table->field_major
								." = ".toSQL($this->options['major']).' OR '
								.TABLE_PROFILE.'.'.$profile_table->field_majorsecond
								." = ".toSQL($this->options['major']).')';
		}
		//house (college) name
		if (!empty($this->options['house'])){
			$and_conditions[] = TABLE_PROFILE.'.'.$profile_table->field_house
								." = ".toSQL($this->options['house']);
		}
		//room number
		if (!empty($this->options['room'])){
			$and_conditions[] = TABLE_PROFILE.'.'.$profile_table->field_room
				.' LIKE '.toSQL('%'.$this->options['room'].'%');
		}
		//high school name string
		if (!empty($this->options['hs'])){
			$and_conditions[] = 'MATCH ('
				.TABLE_PROFILE.'.'.$profile_table->field_hs
				.") AGAINST (".toSQL($this->options['hs'])." IN BOOLEAN MODE)";
		}
		//interests
		if (!empty($this->options['intrs'])){
			$and_conditions[] = 'MATCH ('
				.TABLE_PROFILE.'.'.$profile_table->field_interests
				.") AGAINST (".toSQL($this->options['intrs'])." IN BOOLEAN MODE)";
		}
		//course title string
		if (!empty($this->options['course'])){
			$courses_t = new table_mycourses();
			$tables[] = TABLE_MYCOURSES;
			$and_conditions[] = 'MATCH ('
				.TABLE_MYCOURSES.'.'.$courses_t->field_course
				.') AGAINST ('.toSQL($this->options['course']).' IN BOOLEAN MODE) AND '
				.TABLE_MYCOURSES.'.'.$courses_t->field_myid.' = '
				.TABLE_USERS.'.'.$users_table->field_id;
		}
		/*by default search in my college only, else use the college id 
		 *provided;
		 *also check if friends id is specified and if yes then disregard
		 *college id unless one is specified explicitly*/
		if (!empty($this->options['friends'])){
			$tables[] = TABLE_FRIENDS.' as shfriends';
			$and_conditions[] = 'shfriends.'.$friends_table->field_user1
				.' = '.toSQL($this->options['friends']).' AND '
				.'shfriends.'.$friends_table->field_user2
				.' = '.TABLE_USERS.'.'.$users_table->field_id;
				
		}
		if (isset($this->options['col']) && $this->options['col'] !=-1){
			$and_conditions[] = TABLE_USERS.'.'.$users_table->field_school
								.' = '.toSQL($this->options['col']);
		}elseif(empty($this->options['friends'])){
			$and_conditions[] = TABLE_USERS.'.'.$users_table->field_school
					." = '".$this->college_id."'";
		}
		$count  = '';
		if (!isset($this->options['count'])){
			$count = 'SQL_CALC_FOUND_ROWS ';
		}
		
		$sqlFields = array();
		$sqlFields[] = TABLE_USERS.'.'.$users_table->field_id;
		$sqlFields[] = TABLE_USERS.'.'.$users_table->field_name;
		$sqlFields[] = TABLE_USERS.'.'.$users_table->field_permissions;
		$sqlFields[] = TABLE_USERS.'.'.$users_table->field_school;
		$sqlFields[] = TABLE_PROFILE.'.'.$profile_table->field_picture;
		$sqlFields[] = TABLE_FRIENDS.'.'.$friends_table->field_user2.' as buddy';
		$sqlFields[] = TABLE_COLLEGES.'.'.$college_table->field_fullname;
		
		$tables[] = TABLE_USERS;
		$tables[] = TABLE_PROFILE;
		$tables[] = TABLE_COLLEGES;
		
		$sqlQuery = 'SELECT DISTINCT '.$count.array_tostring($sqlFields).' FROM '
					.array_tostring($tables).' LEFT JOIN '
					.TABLE_FRIENDS.' ON '.TABLE_USERS.'.'.$users_table->field_id.' = '
					.TABLE_FRIENDS.'.'.$friends_table->field_user1.' AND '
					.TABLE_FRIENDS.'.'.$friends_table->field_user2
					." = '$this->user_id' WHERE "
					.TABLE_USERS.'.'.$users_table->field_id.' = '
					.TABLE_PROFILE.'.'.$profile_table->field_id.' AND '
					.TABLE_USERS.'.'.$users_table->field_school.' = '
					.TABLE_COLLEGES.'.'.$college_table->field_id;
		if (!empty($conditions)){
			$sqlQuery .= ' AND (';
			$firstCond = true;
			foreach ($conditions as $condition){
				(!$firstCond) ? $sqlQuery .= ' OR ' : $firstCond = false;
				$sqlQuery .= $condition;
			}
			$sqlQuery .= ')';
		}
		if (!empty($and_conditions)){
			foreach ($and_conditions as $condition){
				$sqlQuery .= ' AND ';
				$sqlQuery .= $condition;
			}
		}
		$sqlQuery .= " ORDER BY RAND(".$this->random_seed.") LIMIT $this->start_results, $this->numb_results";
		$this->result = db::query($sqlQuery);
		$this->total = sqlFunc_getCount();
	}
	function search_market(){
		$query = $this->query;
		$cond = array();
		$items_t = new table_tradeitems();
		$isbn_t = new table_isbns();
		$users_t = new table_users();
		if (!empty($query)){
			$query = preg_replace('/[^a-z0-9]/i',' ',$query);
			if (trim($query) != ''){
				$query = preg_replace('/(.)([\s]|$)/i','\\1*\\2',$query);
				$cond[] = 'MATCH ('.TABLE_TRADEITEMS.'.'.$items_t->field_content
					.') AGAINST (\''.$query.'\' IN BOOLEAN MODE)';
			}
		}
		if (isset($this->options['categ']) && is_numeric($this->options['categ'])){
			$cond[] = $items_t->field_type." = '".$this->options['categ']."'";
		}
		$count = ' SQL_CALC_FOUND_ROWS ';
		$fields = array();
		$fields[] = TABLE_TRADEITEMS.'.*';
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_USERS.'.'.$users_t->field_feedback;
		$fields[] = TABLE_ISBNS.'.*';
		$query = 'SELECT '.$count.implode(',',$fields).' FROM '.TABLE_TRADEITEMS.','.TABLE_USERS
			.' LEFT JOIN '.TABLE_ISBNS.' ON '
			.TABLE_TRADEITEMS.'.'.$items_t->field_type.' = '.TYPE_ITEM_TEXTBOOK.' AND '
			.'SUBSTRING_INDEX('.TABLE_TRADEITEMS.'.'.$items_t->field_content.", ' :', 1)"
			.' = '.TABLE_ISBNS.'.'.$isbn_t->field_isbn.' WHERE '
			.TABLE_USERS.'.'.$users_t->field_id.' = '.TABLE_TRADEITEMS.'.'.$items_t->field_user
			.((!empty($cond))?' AND '.implode(' AND ',$cond):'').' AND '
			.TABLE_USERS.'.'.$users_t->field_school." = '".$this->college_id."'"
			." ORDER BY RAND($this->random_seed) LIMIT $this->start_results, $this->numb_results";
		$this->result = db::query($query);		
		$this->total = sqlFunc_getCount();
	}
	function search_event($type = null){
		$query = $this->query;
		$query = preg_replace('/[^a-z ]/i',' ',$query);
		$query = preg_replace('/[ ]+/',' ',$query);
		$query = trim($query);
		$event = new eventster($this->user_id,$this->college_id);
		if (empty($query)){
			$query = null;
		}
		$this->result = $event->getEvents(null,null,null,null,true,true,false,$this->numb_results,
						$this->start_results,true,$query,$type);
		$this->total = sqlFunc_getCount();
	}
	/**searches for pictures only belonging to this user*/
	function search_photo(){
		if ($this->numb_results < 19){
			$this->numb_results = 19;
		}
		$pict_t = new table_pictures();
		$albm_t = new table_albums();
		$fields = array();
		$fields[] = TABLE_PICTURES.'.*';
		$sqlQuery = 'SELECT SQL_CALC_FOUND_ROWS '.implode(',',$fields).' FROM '.TABLE_PICTURES.','.TABLE_ALBUMS
			.' WHERE '.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' = '
			.TABLE_PICTURES.'.'.$pict_t->field_albumid.' AND '
			.TABLE_ALBUMS.'.'.$albm_t->field_ownerid.' = '.toSQL($this->user_id)
			.((!empty($this->query))?' AND MATCH ('.TABLE_PICTURES.'.'.$pict_t->field_title
			.') AGAINST ('.toSQL($this->query).' IN BOOLEAN MODE)':'')
			.' ORDER BY RAND('.$this->random_seed.')'
			." LIMIT $this->start_results, $this->numb_results";
		$this->result = db::query($sqlQuery);
		$this->total = sqlFunc_getCount();
	}
	/*searches rideboard for matches in to and from fields, however
	 *if in the query it finds a word to or from it will restrict the search
	 *based on those fields*/
	function search_ride(){
		$query = $this->query;
		//initialize tables
		$ride_t = new table_rideboard();
		$users_t = new table_users();
		$profile_t = new table_profile();
		/*type of search (what rides are available == 2
		 *rides people are looking for == 1)*/
		if (!isset($this->options['kind']) 
			|| ($this->options['kind'] != 1 && $this->options['kind'] != 2)){
			$kind = 2;
		}else{
			$kind = $this->options['kind'];
		}
		$query = preg_replace('/[^a-z0-9 ]/i','',$query);
		$query = preg_replace('/[ ]+/i',' ',$query);
		$query = trim($query);
		if (empty($query)){
			$andcondition = '';
		}elseif (preg_match('/to/i',$query)){
			$query = preg_replace('/to/i','',$query);
			$andcondition = ' AND MATCH ('
				.TABLE_RIDEBOARD.'.'.$ride_t->field_to
			.') AGAINST ('.toSQL($query).' IN BOOLEAN MODE)';
		}elseif(preg_match('/from/i',$query)){
			$query = preg_replace('/from/i','',$query);
			$andcondition = ' AND MATCH ('
				.TABLE_RIDEBOARD.'.'.$ride_t->field_from
			.') AGAINST ('.toSQL($query).' IN BOOLEAN MODE)';
		}else{
			$andcondition = ' AND MATCH ('
				.TABLE_RIDEBOARD.'.'.$ride_t->field_to.','
				.TABLE_RIDEBOARD.'.'.$ride_t->field_from
			.') AGAINST ('.toSQL($query).' IN BOOLEAN MODE)';
		}
		
		//fields we need to get
		$fields_array = array();
		$fields_array[] = TABLE_RIDEBOARD.'.*';
		$fields_array[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields_array[] = TABLE_PROFILE.'.'.$profile_t->field_picture;
		
		$sqlQuery = "SELECT SQL_CALC_FOUND_ROWS ".array_tostring($fields_array).' FROM '
			.TABLE_RIDEBOARD.','.TABLE_USERS.','.TABLE_PROFILE.' WHERE '
			.TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_RIDEBOARD.'.'.$ride_t->field_user.' AND '
			.TABLE_PROFILE.'.'.$profile_t->field_id.' = '
			.TABLE_USERS.'.'.$users_t->field_id.' AND '
			.TABLE_RIDEBOARD.'.'.$ride_t->field_kind." = '".$kind."' AND "
			.TABLE_RIDEBOARD.'.'.$ride_t->field_school." = '".$this->college_id."'"
			.$andcondition
			." LIMIT $this->start_results, $this->numb_results";
		$this->result = db::query($sqlQuery);
		$this->total = sqlFunc_getCount();
	}
	function noResults(){
		$this->result = null;
		$this->total = 0;
		$this->invalid_request = true;
	}
}
?>