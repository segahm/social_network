<?php
class Picts{
	private $userid;
	private $collegeid;
	private $get;
	private $post;
	public $page;
	public $error;
	public $response_codes = array();
	const MAIN_ALBUM_PAGE = 'albm_main.inc';
	const DEL_ALBUM_PAGE = 'albm_del.inc';
	const CREATE_ALBUM = 'crt_albm.inc';
	const MAIN_PICTS = 'picts_main.inc';
	const NEW_PICTS = 'new_picts.inc';
	const MOZILLA_NEW_PICTS = 'new_picts_mozilla.inc';
	const DEL_PICT_PAGE = 'pic_del.inc';
	const REDIRECT = 'redirect';
	const PICT_PAGE = 'pic.inc';
	const PERM_PUBLIC = 1;
	//the following two overwrite each other (use carefully)
	const PERM_ALLFRIENDS = 2;
	const PERM_SPECIFIED = 4;
	//public might overwrite school
	const PERM_SCHOOL = 8;
	const MAX_WIDTH_IMAGE = 400;	//maximum image size that we allow
	const THUMB_WIDTH = 80;
	const THUMB_HEIGHT = 60;
	function __construct($userid,$collegeid,$get,$post){
		$this->userid = $userid;
		$this->collegeid = $collegeid;
		$this->get = $get;
		$this->post = $post;
		//set default page 
		$this->page = self::MAIN_ALBUM_PAGE;
		$album_t = new table_albums();
		$users_t = new table_users();
		if (isset($get['usr']) && $get['usr'] != $userid 
			&& strlen($get['usr']) == $users_t->field_id_length){
			//find out who this is and wherever that person has blocked us
			$result = $this->whois($get['usr']);
			if (is_null($result)){
				$this->response_codes[self::REDIRECT] = '/home';
				return;
			}
			if (sqlFunc_is_blocked($this->userid,$result[$users_t->field_id]) !== false){
				$this->response_codes[self::REDIRECT] = '/blocked?usr='.$result[$users_t->field_id];
				return;
			}
			$this->response_codes['user'] = $result;
		}
		if (isset($get['cptn'])){
			if ($this->chgPictSettings()){
				$this->response_codes[self::REDIRECT] = '/mypictures?'
					.((isset($post['albmid']))?'showalbm='.$post['albmid']:'pic='.$post['id']);
			}else{
				$this->response_codes[self::REDIRECT] = '/home';
			}
			return;
		}
		if (isset($get['newalbm'])){
			if (isset($get['pf']) && isset($get['albm'])){
				$result = $this->new_albm();
				if ($result === true){
					$this->response_codes[self::REDIRECT] = '/mypictures?showalbm='.$get['albm'];
					return;
				}elseif($result !== false){
					$this->error = $result;
				}else{
					$this->response_codes[self::REDIRECT] = '/home';
					return;
				}
			}else{
				//set default fields
				$def = array();
				$def['name'] = date('m-d-Y');
				$def['perm'] = 0;
				$this->response_codes['fields'] = $def;
				$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
				$genid = substr($genid, 0, $album_t->field_albumid_length);
				$this->response_codes['albmid'] = $genid;
			}
			$this->page = self::CREATE_ALBUM;
			return;
		}
		if (isset($get['edit'])){
			if ($this->getForEdit($get['albm'])){
				$this->page = self::CREATE_ALBUM;
			}else{
				$this->response_codes[self::REDIRECT] = '/home';
			}
			return;
		}
		if (isset($get['delalbm']) && strlen($get['delalbm']) == $album_t->field_albumid_length){
			if (isset($post['albm'])){
				$this->delete_albm($post['albm']);
				$this->page = self::MAIN_ALBUM_PAGE;
			}else{
				if ($this->getAlbmStats($get['delalbm'])){
					$this->page = self::DEL_ALBUM_PAGE;
				}else{
					$this->response_codes[self::REDIRECT] = '/home';
				}
			}
			return;
		}
		if (isset($get['showalbm']) 
				&& strlen($get['showalbm']) 
					== $album_t->field_albumid_length){
			if ($info = $this->getAlbmInfo($get['showalbm'])){
				$this->response_codes['albm_info'] = $info;
				$this->page = self::MAIN_PICTS;
			}else{
				$this->response_codes[self::REDIRECT] = '/home';
			}
			return;
		}
		if (isset($get['delpict'])){
			if (isset($post['pictid']) && isset($post['albid'])){
				$this->delete_pict($post['pictid']);
				$this->response_codes[self::REDIRECT] = '/mypictures?showalbm='.$post['albid'];
			}else{
				$data = $this->getPicture($get['delpict']);
				if (!empty($data)){
					$this->response_codes['pic_info'] = $data;
					$this->page = self::DEL_PICT_PAGE;
				}else{
					$this->response_codes[self::REDIRECT] = '/home';
				}
			}
			return;
		}
		if (isset($get['newpict'])){
			$pict_t = new table_pictures();
			if (strlen($get['newpict']) != $album_t->field_albumid_length){
				$this->response_codes[self::REDIRECT] = '/home';
			}elseif(isset($get['submit'])){
				//upload pictures and notify parent
				$this->uploadPics($get['newpict']);
				if (isset($get['mozilla'])){
					$this->page = self::MOZILLA_NEW_PICTS;
				}else{
					$this->page = self::NEW_PICTS;
				}
			}else{
				if (isset($get['mozilla'])){
					$this->page = self::MOZILLA_NEW_PICTS;
				}else{
					$this->page = self::NEW_PICTS;
				}
			}
			return;
		}
		if (isset($get['pic'])){
			$data = $this->getPicture($get['pic']);
			if (!empty($data)){
				$this->response_codes['pic_info'] = $data;
				$this->page = self::PICT_PAGE;
			}else{
				$this->response_codes[self::REDIRECT] = '/home';
			}
		}
	}
	private function uploadPics($albmid){
		$pict_t = new table_pictures();
		$pictures = array();
		$i=-1;
		foreach ($_FILES as $file){
			$i++;
			if (empty($file['type']) || !preg_match('/jpg|jpeg/i',$file['type'])){
				continue;
			}
			$type = '.jpg';
			$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
			$genid = substr($genid, 0, $pict_t->field_pictureid_length-strlen($type));
			$pict = array($genid.$type);
			$uploadfile = $_SERVER['DOCUMENT_ROOT'].$this->getPictureUrl($pict[0]);
			if (!(move_uploaded_file($file['tmp_name'], $uploadfile))){
				//this doesn't actually set the text of the error but simply notifies that there
				//was an error
				$this->error = 'we were unable to process one or more files';
				continue;
			}
			if (!chmod($uploadfile,0777)){
				log_writeLog('chmod failed on:'.$uploadfile);
			}
			if (isset($this->post['caption_'.$i])){
				$pict[] = $this->post['caption_'.$i];
			}else{
				$pict[] = null;
			}
			$pictures[] = $pict;
		}
		if (empty($pictures)){
			return;
		}
		if (count($pictures)>1){
			//prepare statement
			$fields = array();
			$fields[] = $pict_t->field_albumid;
			$fields[] = $pict_t->field_datetime;
			$fields[] = $pict_t->field_pictureid;
			$fields[] = $pict_t->field_title;
			$query = 'INSERT INTO '.TABLE_PICTURES.' ('.sqlFields($fields)
				.") VALUES ";
			$str = array();
			foreach($pictures as $pict){
				$str[] =  "('".$albmid."',NOW(),".toSQL($pict[0]).','.toSQL($pict[1]).')';
			}
			$query .= implode(',',$str);
			log_writeLog($query);
		}else{
			$fields = array();
			$fields[$pict_t->field_albumid] = $albmid;
			$fields[$pict_t->field_pictureid] = $pictures[0][0];
			$fields[$pict_t->field_title] = $pictures[0][1];
			$query = 'INSERT INTO '.TABLE_PICTURES.' ('.sqlFields(array_keys($fields)).',`'
				.$pict_t->field_datetime.'`) values ('.toSQL($fields).',NOW())';
		}
		log_writeLog(((db::query($query))?'true':'false'));		
	}
	private function chgPictSettings(){
		$pict_t = new table_pictures();
		if (!isset($this->post['id']) 
			|| strlen($this->post['id']) != $pict_t->field_pictureid_length){
			return false;
		}
		//update actions
		$actions = array();
		if (isset($this->post['caption'])){
			$actions[] = TABLE_PICTURES.'.'.$pict_t->field_title.' = '.toSQL($this->post['caption']);
		}
		//if updating not from album do not update public status
		if (isset($this->post['albmid'])){
			if (isset($this->post['public']) && $this->post['public'] == 1){
				$actions[] = TABLE_PICTURES.'.'.$pict_t->field_public.' = "0/0"';
			}else{
				$actions[] = TABLE_PICTURES.'.'.$pict_t->field_public.' = NULL';
			}
		}
		if (!empty($actions)){
			$albm_t = new table_albums();
			$query = 'UPDATE '.TABLE_PICTURES.','.TABLE_ALBUMS.' SET '
				.implode(',',$actions).' WHERE '
				.TABLE_PICTURES.'.'.$pict_t->field_pictureid.' = '.toSQL($this->post['id'])
				.' AND '.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' = '
				.TABLE_PICTURES.'.'.$pict_t->field_albumid.' AND '
				.TABLE_ALBUMS.'.'.$albm_t->field_ownerid." = '".$this->userid."'";
			db::query($query);
		}
		return true;
	}
	/**queries db for the picture and returns picture's data*/
	private function getPicture($picid){
		$pict_t = new table_pictures();
		if (strlen($picid) != $pict_t->field_pictureid_length){
			return null;
		}
		$albm_t = new table_albums();
		$query = 'SELECT '.TABLE_PICTURES.'.*'
			.' FROM '.TABLE_PICTURES.','.TABLE_ALBUMS.' WHERE '
			.TABLE_PICTURES.'.'.$pict_t->field_pictureid.' = '.toSQL($picid).' AND '
			.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' = '
			.TABLE_PICTURES.'.'.$pict_t->field_albumid;
		if (isset($this->response_codes['user'])){
			$users_t = new table_users();
			//get owner's parameters
			$userid = $this->response_codes['user'][$users_t->field_id];
			$schoolid = $this->response_codes['user'][$users_t->field_school];
			$is_friend = (isset($this->response_codes['isfriends']))?true:false;
			//where picture is a public picture
			$conditions[] = '('.TABLE_PICTURES.'.'.$pict_t->field_public.' IS NOT NULL)';
			//where albums pemissions are for public
			$conditions[] = '('.TABLE_ALBUMS.'.'.$albm_t->field_permissions
				.' & '.self::PERM_PUBLIC.')';
			//if from the same college
			if ($schoolid == $this->collegeid){
				//get all for people in the same school
				$conditions[] = '('.TABLE_ALBUMS.'.'.$albm_t->field_permissions
					.' & '.self::PERM_SCHOOL.')';
			}
			if ($is_friend){
				//get all albums for friends only
				$conditions[] = '('.TABLE_ALBUMS.'.'.$albm_t->field_permissions
					.' & '.self::PERM_ALLFRIENDS.')';
				$ids_t = new table_ids();
				//get all for specified friends where I'm a specified friend
				$conditions[] = '(('.TABLE_ALBUMS.'.'.$albm_t->field_permissions
					.' & '.self::PERM_SPECIFIED.') AND EXISTS '
					.'(SELECT 1 FROM '.TABLE_IDS.','.TABLE_ALBUMS.' WHERE '
					.TABLE_IDS.'.'.$ids_t->field_ownerid.' = '
					.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' AND '
					.TABLE_ALBUMS.'.'.$albm_t->field_ownerid." = '".$userid."' AND "
					.TABLE_IDS.'.'.$ids_t->field_subjectid." = '".$this->userid."' LIMIT 1))";
			} 			
			//combine all conditions					
			$query .= ' AND ('.implode(' OR ',$conditions).')';
		}else{
			//if looking own picture
			$query .= ' AND '.TABLE_ALBUMS.'.'.$albm_t->field_ownerid." = '".$this->userid."'";
		}	
		$query .= ' LIMIT 1';
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			return $row;
		}
		return null;
	}
	/**deletes a picture and returns true if successful and legal-else false*/
	private function delete_pict($pictid){
		$pict_t = new table_pictures();
		if (strlen($pictid) != $pict_t->field_pictureid_length){
			return false;
		}
		$albm_t = new table_albums();
		$query = 'DELETE FROM '.TABLE_PICTURES.' USING '.TABLE_PICTURES.','
			.TABLE_ALBUMS.' WHERE '.$pict_t->field_pictureid.' = '
			.toSQL($pictid).' AND '.TABLE_PICTURES.'.'.$pict_t->field_albumid.' = '
			.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' AND '
			.TABLE_ALBUMS.'.'.$albm_t->field_ownerid
			." = '".$this->userid."'";
		db::query($query);
		//make sure that the picture was delete so that we can delete the associative file
		if (db::affected_rows() != 1){
			return false;
		}
		//first unlink the big file
		//note: to save time don't check that file exists - since is must
		unlink($_SERVER['DOCUMENT_ROOT'].$this->getPictureUrl($pictid));
		//also unlink the small file if one exists
		if (file_exists($_SERVER['DOCUMENT_ROOT'].$this->getSmallUrl($pictid))){
			unlink($_SERVER['DOCUMENT_ROOT'].$this->getSmallUrl($pictid));
		}
		return true;
	}
	private function getAlbmInfo($albm){
		$albm_t = new table_albums();
		$query = 'SELECT * FROM '.TABLE_ALBUMS.' WHERE '
			.$albm_t->field_albumid.' = '.toSQL($albm).' LIMIT 1';
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			return $row;
		}else{
			return null;
		}
	}
	/**gets a brief description fo the album for display on the delete page*/
	private function getAlbmStats($albm){
		$album_t = new table_albums();
		if (strlen($albm) != $album_t->field_albumid_length){
			return false;
		}
		$picts_t = new table_pictures();
		$query = 'SELECT SQL_CALC_FOUND_ROWS '.TABLE_ALBUMS.'.'.$album_t->field_title.','
			.TABLE_PICTURES.'.'.$picts_t->field_pictureid.' FROM '.TABLE_ALBUMS
			.' LEFT JOIN '.TABLE_PICTURES.' ON '.TABLE_ALBUMS.'.'.$album_t->field_albumid.' = '
			.TABLE_PICTURES.'.'.$picts_t->field_albumid.' WHERE '
			.TABLE_ALBUMS.'.'.$album_t->field_albumid.' = '.toSQL($albm).' AND '
			.TABLE_ALBUMS.'.'.$album_t->field_ownerid." = '".$this->userid."' LIMIT 1";
		$result = db::query($query);
		if (!($row = db::fetch_row($result))){
			return false;
		}
		$this->response_codes['title'] = $row[0];
		$count = sqlFunc_getCount();
		//if count is 1 check that there indeed existed a picture under this album
		if ($count == 1 && is_null($row[1])){
			$count = 0;
		}
		$this->response_codes['photos'] = $count;
		return true;
	}
	private function getForEdit($albmid){
		$album_t = new table_albums();
		if (strlen($albmid) != $album_t->field_albumid_length){
			return false;
		}
		//get all information about the album
		$ids_t = new table_ids();
		//left join ids table only if permissions are for specified
		$query = 'SELECT '.TABLE_ALBUMS.'.*,'.TABLE_IDS.'.'.$ids_t->field_subjectid
			.' FROM '.TABLE_ALBUMS.' LEFT JOIN '.TABLE_IDS.' ON '
			.'('.TABLE_ALBUMS.'.'.$album_t->field_permissions.' & '
			.self::PERM_SPECIFIED.') AND '.TABLE_ALBUMS.'.'.$album_t->field_albumid
			.' = '.TABLE_IDS.'.'.$ids_t->field_ownerid.' WHERE '
			.TABLE_ALBUMS.'.'.$album_t->field_albumid.' = '.toSQL($albmid).' AND '
			.TABLE_ALBUMS.'.'.$album_t->field_ownerid." = '".$this->userid."'";
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			$this->response_codes['albmid'] = $row[$album_t->field_albumid];
			//set permissions
			$permissions = $row[$album_t->field_permissions];
			if ($permissions & self::PERM_PUBLIC){
				$this->response_codes['fields']['perm'] = self::PERM_PUBLIC;
			}else{
				$this->response_codes['fields']['perm'] = 0;
				if ($permissions & self::PERM_SPECIFIED){
					$this->response_codes['fields']['perm_friends2'] = self::PERM_SPECIFIED;
				}elseif($permissions & self::PERM_ALLFRIENDS){
					$this->response_codes['fields']['perm_friends1'] = self::PERM_ALLFRIENDS;
				}
				if($permissions & self::PERM_SCHOOL){
					$this->response_codes['fields']['perm_school'] = self::PERM_SCHOOL;
				}
			}
			$this->response_codes['fields']['name'] = $row[$album_t->field_title];
			$ids = array();
			do{
				$ids[] = $row[$ids_t->field_subjectid];
			}while($row = db::fetch_assoc($result));
			if (!empty($ids)){
				$users = quickAddIn::getListOfUsers($ids);
				$groups = quickAddIn::getListOfGroups($ids);
				$this->response_codes['fields']['users'] = implode(',',$users);
				$this->response_codes['fields']['groups'] = implode(',',$groups);
			}
		}else{
			//album wasn't found
			return false;
		}
		return true;
	}
	private function delete_albm($albmid){
		$album_t = new table_albums();
		if (strlen($albmid) != $album_t->field_albumid_length){
			return;
		}
		/*first try to delete the album so that we know that the records stored
		 *in ids table do indeed belong to this user*/
		$query = 'DELETE FROM '.TABLE_ALBUMS.' WHERE '.$album_t->field_albumid
			.' = '.toSQL($albmid).' AND '.$album_t->field_ownerid
			." = '".$this->userid."' LIMIT 1";
		db::query($query);
		//if still here than this album indeed exists (existed) and belongs 
		//to this user, however
		//everything else will be deleted later by cron job to minimize time for
		//individual user request
	}
	private function new_albm(){
		$album_t = new table_albums();
		if (strlen($this->get['albm']) != $album_t->field_albumid_length){
			return false;
		}
		if (empty($this->post['name'])){
			$this->response_codes['fields'] = $this->post;
			$this->response_codes['albmid'] = $this->get['albm'];
			return 'please enter the name to be used for this album';
		}
		$name = $this->post['name'];
		$perm = 0;
		foreach($this->post as $key => $value){
			if (substr($key,0,4) == 'perm'){
				$perm = $perm ^ $value;	
			}
		}
		if (($perm & self::PERM_PUBLIC) && 
				(($perm & self::PERM_ALLFRIENDS) || ($perm & self::PERM_SPECIFIED)
					|| ($perm & self::PERM_SCHOOL))){
			$this->response_codes['fields'] = $this->post;
			$this->response_codes['albmid'] = $this->get['albm'];
			return 'you can only specify people in your school or friends by making this album private';
		}
		//check for illegal permissions
		if ((($perm & self::PERM_ALLFRIENDS) 
				&& ($perm & self::PERM_SPECIFIED))
			||
			(($perm & self::PERM_SPECIFIED) 
				&& ($perm & self::PERM_ALLFRIENDS))){
			return false;
		}
		//insert album
		$fields = array();
		$fields[$album_t->field_albumid] = $this->get['albm'];
		$fields[$album_t->field_isdalbum] = 0;
		$fields[$album_t->field_ownerid] = $this->userid;
		$fields[$album_t->field_title] = $name;
		$fields[$album_t->field_permissions] = $perm;
		$query = 'REPLACE INTO '.TABLE_ALBUMS.' ('.sqlFields(array_keys($fields))
			.',`'.$album_t->field_date.'`) values ('.toSQL($fields).',CURDATE())';
		db::query($query);
		
		$ids_t = new table_ids();
		//delete all old entries in ids of all the users previously specified
		$query = 'DELETE QUICK FROM '.TABLE_IDS.' USING '.TABLE_ALBUMS.','.TABLE_IDS
			.' WHERE '.TABLE_IDS.'.'.$ids_t->field_ownerid.' = '
			.toSQL($this->get['albm']).' AND '.TABLE_ALBUMS.'.'.$album_t->field_albumid.' = '
			.TABLE_IDS.'.'.$ids_t->field_ownerid.' AND '
			.TABLE_ALBUMS.'.'.$album_t->field_ownerid." = '".$this->userid."'";
		db::query($query);
		//now insert user ids if permissions were specified to do so
		
		if ($perm & self::PERM_SPECIFIED){
			$userids = quickAddIn::getListOfIds($this->post['users'],$this->userid);
			$groupids = quickAddIn::getListOfGroupIds($this->post['groups'],$this->userid);
			//also get ids selected using scrolling panel
			$userids2 = array();
			if (!empty($this->post['selfriends'])){
				$userids2 = quickAddIn::getScrollingIds($this->post['selfriends']);
			}
		 	$mergedids = array_merge($userids,$userids2);
		 	$mergedids = array_unique($mergedids);
		 	$mergedids = array_merge($groupids,$mergedids);
		 	if (!empty($mergedids)){
		 		//insert ids
		 		if (count($mergedids) >2){
		 			//prepare statement
		 			$query = 'REPLACE DELAYED INTO '.TABLE_IDS.' ('
		 				.$ids_t->field_ownerid.','.$ids_t->field_subjectid
		 				.') values ('.toSQL($this->get['albm']).',?)';
		 			$stmt = db::prepare($query);
		 			foreach($mergedids as $id){
		 				$stmt->bind_param('s',$id);
						$stmt->execute();
		 			}
		 		}else{
		 			foreach($mergedids as $id){
		 				$query = 'REPLACE DELAYED INTO '.TABLE_IDS.' ('
		 					.$ids_t->field_ownerid.','.$ids_t->field_subjectid
		 					.') values ('.toSQL($this->get['albm']).",'".$id."')";
		 				db::query($query);
		 			}
		 		}
		 	}
		}
		return true;	
	}
	private function whois($id){
		$users_t = new table_users();
		$friends_t = new table_friends();	//for checking wherever we are friends
		if (strlen($id) != $users_t->field_id_length){
			return null;
		}
		$query = 'SELECT '.TABLE_USERS.'.'.$users_t->field_name.','
			.TABLE_USERS.'.'.$users_t->field_school.','
			.TABLE_FRIENDS.'.'.$friends_t->field_user1.' as isfriends'
			.' FROM '.TABLE_USERS.' LEFT JOIN '.TABLE_FRIENDS.' ON '
							.TABLE_FRIENDS.'.'.$friends_t->field_user1." = '".$this->userid
							."' AND ".TABLE_FRIENDS.'.'.$friends_t->field_user2.' = '
							.TABLE_USERS.'.'.$users_t->field_id
			.' WHERE '.TABLE_USERS.'.'.$users_t->field_id.' = '.toSQL($id).' LIMIT 1';
		$result = db::query($query);
		if (!($row = db::fetch_assoc($result))){
			return null;
		}
		if (!is_null($row['isfriends'])){
			$this->response_codes['isfriends'] = true;
		}
		//clean garbage
		unset($row['isfriends']);
		//add user's id since it wasn't returned in the query
		$row[$users_t->field_id] = $id;
		return $row;
	}
	/**gets a list of pictures for album ($_GET[albm])
	 *based on the permissions of the album and the connection between 
	 *the owner of the album
	 *and the user*/
	public function getPictures(){
		$albumid = $this->get['showalbm'];
		$picts_t = new table_pictures();
		$fields = array();
		$fields[] = TABLE_PICTURES.'.*';
		if (isset($this->response_codes['user'])){
			//if not the owner we will need to check permissions
			$album_t = new table_albums();
			$users_t = new table_users();
			//get owner's parameters
			$userid = $this->response_codes['user'][$users_t->field_id];
			$schoolid = $this->response_codes['user'][$users_t->field_school];
			$is_friend = (isset($this->response_codes['isfriends']))?true:false;
			
			$query =  'SELECT DISTINCT '.implode(',',$fields)
				.' FROM '.TABLE_PICTURES.','.TABLE_ALBUMS.' WHERE '
				.TABLE_PICTURES.'.'.$picts_t->field_albumid.' = '.toSQL($albumid).' AND '
				.TABLE_ALBUMS.'.'.$album_t->field_albumid.' = '.toSQL($albumid).' AND '
				.TABLE_ALBUMS.'.'.$album_t->field_ownerid." = '".$userid."' AND ";
			$conditions = array();
			
			//get all public albums
			$conditions[] = '('.TABLE_ALBUMS.'.'.$album_t->field_permissions
				.' & '.self::PERM_PUBLIC.')';
				
			//if from the same college
			if ($schoolid == $this->collegeid){
				//get all for people in the same school
				$conditions[] = '('.TABLE_ALBUMS.'.'.$album_t->field_permissions
					.' & '.self::PERM_SCHOOL.')';
			}
			if ($is_friend){
				$groups_t = new table_friends_in_groups();
				//get all albums for friends only
				$conditions[] = '('.TABLE_ALBUMS.'.'.$album_t->field_permissions
					.' & '.self::PERM_ALLFRIENDS.')';
				$ids_t = new table_ids();
				//get all for specified friends where I'm a specified friend
				$conditions[] = '(('.TABLE_ALBUMS.'.'.$album_t->field_permissions
					.' & '.self::PERM_SPECIFIED.') AND EXISTS '
					.'(SELECT 1 FROM '.TABLE_IDS.' LEFT JOIN '.TABLE_FRIENDS_IN_GROUPS.' ON '
					.TABLE_IDS.'.'.$ids_t->field_subjectid.' = '
					.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' AND '
					.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_userid." = '".$this->userid."'"
					.' WHERE '.TABLE_IDS.'.'.$ids_t->field_ownerid.' = '
					.TABLE_ALBUMS.'.'.$album_t->field_albumid.' AND (('
					.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' IS NOT NULL) OR ('
					.TABLE_IDS.'.'.$ids_t->field_subjectid." = '".$this->userid."')) LIMIT 1))";
			} 			
			//combine all conditions					
			$query .= '('.implode(' OR ',$conditions).')';	
			$query .= ' ORDER BY '
				.TABLE_PICTURES.'.'.$picts_t->field_datetime." DESC";
		}else{
			$query = 'SELECT '.implode(',',$fields)
				.' FROM '.TABLE_PICTURES.' WHERE '
				.TABLE_PICTURES.'.'.$picts_t->field_albumid.' = '.toSQL($albumid).' ORDER BY '
				.TABLE_PICTURES.'.'.$picts_t->field_datetime.' DESC';
		}
		$result = db::query($query);
		return $result;
	}
	/**gets a list of albums using start and limit keywords
	 *based on the permissions of each album and the connection between the owner of these albums*/
	public function getAlbums(){
		$album_t = new table_albums(); 
		$picts_t = new table_pictures();
		$fields = array();
		$fields[] = TABLE_ALBUMS.'.*';
		$fields[] = TABLE_PICTURES.'.'.$picts_t->field_pictureid;	//default picture
		$fields[] = TABLE_PICTURES.'.'.$picts_t->field_datetime;	//default picture's timedate
		if (isset($this->response_codes['user'])){
			//if not the owner we will need to check permissions
			$users_t = new table_users();
			//get owner's parameters
			$userid = $this->response_codes['user'][$users_t->field_id];
			$schoolid = $this->response_codes['user'][$users_t->field_school];
			//$is_same_major = (isset($this->response_codes['issamemajor']))?true:false;
			//$is_same_course = (isset($this->response_codes['issamecourse']))?true:false;
			$is_friend = (isset($this->response_codes['isfriends']))?true:false;
			
			$query = 'SELECT DISTINCT '.implode(',',$fields).' FROM '.TABLE_ALBUMS
				.' LEFT JOIN '.TABLE_PICTURES.' ON '
				.TABLE_PICTURES.'.'.$picts_t->field_albumid.' = '
				.TABLE_ALBUMS.'.'.$album_t->field_albumid.' AND '
				.TABLE_PICTURES.'.'.$picts_t->field_pictureid.' = (SELECT '
				.TABLE_PICTURES.'.'.$picts_t->field_pictureid.' FROM '.TABLE_PICTURES.' WHERE '
				.TABLE_PICTURES.'.'.$picts_t->field_albumid
				.' = '.TABLE_ALBUMS.'.'.$album_t->field_albumid.' LIMIT 1)'
				.' WHERE '
				.TABLE_ALBUMS.'.'.$album_t->field_ownerid." = '".$userid."' AND ";
			$conditions = array();
			
			//get all public albums
			$conditions[] = '('.TABLE_ALBUMS.'.'.$album_t->field_permissions
				.' & '.self::PERM_PUBLIC.')';
				
			//if from the same college
			if ($schoolid == $this->collegeid){
				//get all for people in the same school
				$conditions[] = '('.TABLE_ALBUMS.'.'.$album_t->field_permissions
					.' & '.self::PERM_SCHOOL.')';
			}
			if ($is_friend){
				$groups_t = new table_friends_in_groups();
				//get all albums for friends only
				$conditions[] = '('.TABLE_ALBUMS.'.'.$album_t->field_permissions
					.' & '.self::PERM_ALLFRIENDS.')';
				$ids_t = new table_ids();
				//get all for specified friends where I'm a specified friend					
				$conditions[] = '(('.TABLE_ALBUMS.'.'.$album_t->field_permissions
					.' & '.self::PERM_SPECIFIED.') AND EXISTS '
					.'(SELECT 1 FROM '.TABLE_IDS.' LEFT JOIN '.TABLE_FRIENDS_IN_GROUPS.' ON '
					.TABLE_IDS.'.'.$ids_t->field_subjectid.' = '
					.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' AND '
					.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_userid." = '".$this->userid."'"
					.' WHERE '.TABLE_IDS.'.'.$ids_t->field_ownerid.' = '
					.TABLE_ALBUMS.'.'.$album_t->field_albumid.' AND (('
					.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' IS NOT NULL) OR ('
					.TABLE_IDS.'.'.$ids_t->field_subjectid." = '".$this->userid."')) LIMIT 1))";
			} 			
			//combine all conditions					
			$query .= '('.implode(' OR ',$conditions).')';	
			$query .= ' ORDER BY '.TABLE_ALBUMS.'.'.$album_t->field_date." DESC ";
		}else{
			//select all albums with possibly one picture if that picture exists
			$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_ALBUMS
				.' LEFT JOIN '.TABLE_PICTURES.' ON '
				.TABLE_PICTURES.'.'.$picts_t->field_albumid.' = '
				.TABLE_ALBUMS.'.'.$album_t->field_albumid.' AND '
				.TABLE_PICTURES.'.'.$picts_t->field_pictureid.' = (SELECT '
				.TABLE_PICTURES.'.'.$picts_t->field_pictureid.' FROM '.TABLE_PICTURES.' WHERE '
				.TABLE_PICTURES.'.'.$picts_t->field_albumid
				.' = '.TABLE_ALBUMS.'.'.$album_t->field_albumid.' LIMIT 1)'
				.' WHERE '.TABLE_ALBUMS.'.'.$album_t->field_ownerid." = '".$this->userid
				."' ORDER BY ".TABLE_ALBUMS.'.'.$album_t->field_date.' DESC';
		}
		$result = db::query($query);
		return $result;
	}
	public static function getPictureUrl($id){
		return '/img/picts/'.$id;
	}
	public static function getSmallUrl($id){
		return '/img/picts/sml/'.$id;
	}
	public static function getRandomPic(){
		$pic_t = new table_pictures();
		$user_t = new table_users();
		$albm_t = new table_albums();
		$fields = array();
		$fields[] = TABLE_PICTURES.'.'.$pic_t->field_pictureid;
		$fields[] = TABLE_PICTURES.'.'.$pic_t->field_title;
		$fields[] = TABLE_PICTURES.'.'.$pic_t->field_public;
		$fields[] = TABLE_USERS.'.'.$user_t->field_name;
		$fields[] = TABLE_USERS.'.'.$user_t->field_id;
		$query = 'SELECT '.implode(',',$fields).' FROM '
			.TABLE_PICTURES.','.TABLE_USERS.','.TABLE_ALBUMS
			.' WHERE '.TABLE_PICTURES.'.'.$pic_t->field_public.' IS NOT NULL AND '
			.TABLE_PICTURES.'.'.$pic_t->field_albumid.' = '
			.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' AND '
			.TABLE_USERS.'.'.$user_t->field_id.' = '
			.TABLE_ALBUMS.'.'.$albm_t->field_ownerid
			.' ORDER BY RAND() LIMIT 1';
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			return $row;
		}else{
			return null;
		}
	}
	public static function updatePartyPicScore($id,$is_up,$score){
		$pict_t = new table_pictures();
		if ($pict_t->field_pictureid_length != strlen($id)){
			return;
		}
		$score = base64_decode($score);
		if (!preg_match('#[0-9]{1,3}/[0-9]{1,3}#',$score)){
			return;
		}
		$rating = explode('/',$score);
		($is_up)?$rating[0]++:$rating[1]++;
		$query = 'UPDATE '.TABLE_PICTURES.' SET '.$pict_t->field_public." = '"
			.$rating[0].'/'.$rating[1]."'"
			.' WHERE '.$pict_t->field_pictureid.' = '.toSQL($id).' AND '
			.$pict_t->field_public.' IS NOT NULL LIMIT 1';
		db::query($query);
	}
}
?>