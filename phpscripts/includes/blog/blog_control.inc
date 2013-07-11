<?php
define('USER_BLOG_DISPLAY_LIMIT',10);
define('USER_POST_DISPLAY_LIMIT',10);
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/utils/richformat.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/pagedata/quickAddIn.inc");
class Blog{
	private $userid;
	private $collegeid;
	private $get;
	private $post;
	public $page;
	public $error;
	public $response_codes = array();
	const PAGE_NEWBLOG = 'blog_new.inc';
	const PAGE_NEWSETTING = 'blog_settings.inc';
	const PAGE_HOME = 'blog_home.inc';
	const PAGE_USER = 'blog_user.inc';
	const REDIRECT = 'redirect';
	const PERM_PRIVATE = 0;
	const PERM_PUBLIC = 1;
	//the following will be only active if private
	const PERM_ALLFRIENDS = 2;
	const PERM_SPECIFIED = 4;
	const PERM_SCHOOL = 8;
	const PERM_ALLSCHOOLS = 16;
	//editing permissions
	//if allow edit
	const PERM_EDIT_ALLSCHOOLS = 32;
	const PERM_EDIT_MYSCHOOL = 64;
	const PERM_EDIT_FRIENDS = 128;
	function __construct($userid,$collegeid,$get,$post){
		$this->userid = $userid;
		$this->collegeid = $collegeid;
		$this->get = $get;
		$this->post = $post;
		$stngs_t = new table_blogsettings();
		//if creating a new blog
		if (isset($get['delcmnt']) && isset($get['blgid'])){
			if ($this->deleteComment($get['delcmnt'],$get['blgid'])){
				//redirect to blog
				$this->response_codes[self::REDIRECT] = '/findblog?id='.$get['blgid'].'&isme=1#cmnts';
			}else{
				$this->response_codes[self::REDIRECT] = '/home';
			}
			return;
		}
		if (isset($get['delpst'])){
			$this->deletePost($get['delpst']);
			$this->response_codes[self::REDIRECT] = '/blogger';
			return;
		}
		if (isset($get['delblg'])){
			$this->deleteSettings($get['delblg']);	
			$this->page = self::PAGE_HOME;
			return;
		}
		if (isset($get['usrblgs'])){
			if ($this->prepareUserBlogs($get['usrblgs']) === true){
				$this->page = self::PAGE_USER;
				return;
			}else{
				//continue to our home page but set bad access
				$this->error = 'you do not have permissions to access this person\'s blogs';
			}
		}
		if (isset($get['crtblg'])){
			if (isset($get['frm'])){
				$response = $this->newPost($get['crtblg']);
				if ($response === true){
					//redirect to the newly created blog
					$this->response_codes[self::REDIRECT] = '/findblog?id='.$get['crtblg'].'&isme=1#pst_'.$post['postid'];
					return;
				}elseif($response === false){
					//if invalid request
					$this->response_codes[self::REDIRECT] = '/home';
					return;
				}
				//error!
				$this->error = $response;
			}elseif(isset($get['edt'])){
				if (!$this->getForEditPost($get['edt'])){
					$this->response_codes[self::REDIRECT] = '/home';
					return;
				}
				$this->page = self::PAGE_NEWBLOG;
				return;
			}
			if (!$this->prepareForPostingToSettings($get['crtblg'])){
				//if invalid request
				$this->response_codes[self::REDIRECT] = '/home';
				return;
			}
			$this->page = self::PAGE_NEWBLOG;
			return;
		}
		if (isset($get['crtstngs'])){
			if (isset($get['frm'])){
				$response = $this->newSettings();
				if ($response === true){
					//redirect to the new post creation page
					$this->response_codes[self::REDIRECT] = '/blogger?crtblg='.$get['id'];
				}elseif($response !== false){
					//if error
					$this->page = self::PAGE_NEWSETTING;
					$this->error = $response;
				}else{
					//if invalid request
					$this->response_codes[self::REDIRECT] = '/home';
				}
			}elseif(isset($get['edt'])){
				if ($this->getForEditBlog($get['edt'])){
					$this->page = self::PAGE_NEWSETTING;
				}else{
					$this->response_codes[self::REDIRECT] = '/home';
				}
			}else{
				$def['title'] = date('jS F Y');
				$def['perm'] = self::PERM_PUBLIC;
				$this->response_codes['fields'] = $def;
				$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
				$genid = substr($genid, 0, $stngs_t->field_id_length);
				$this->response_codes['blgid'] = $genid;
				$this->page = self::PAGE_NEWSETTING;
			}
			return;
		}
		//if access restricted to the blog
		if (isset($get['rstctdacs'])){
			$this->error = 'you do not have permissions to access this blog';
		}
		//if still here set default page
		$this->page = self::PAGE_HOME;
	}
	private function prepareUserBlogs($userid){
		$users_t = new table_users();
		$profile_t = new table_profile();
		if (strlen($userid) != $users_t->field_id_length){
			return false;
		}
		//get user information
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_USERS.'.'.$users_t->field_school;
		$fields[] = TABLE_PROFILE.'.'.$profile_t->field_picture;
		$query = 'SELECT '.implode(',',$fields).' FROM '
			.TABLE_USERS.','.TABLE_PROFILE.' WHERE '
			.TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_PROFILE.'.'.$profile_t->field_id.' AND '
			.TABLE_USERS.'.'.$users_t->field_id.' = '.toSQL($userid).' LIMIT 1';
		$result = db::query($query);
		if (!($row = db::fetch_assoc($result))){
			return false;
		}
		if (sqlFunc_is_blocked($this->userid,$userid) !== false){
			return false;
		}
		$this->response_codes['userinfo'] = $row;
		$this->response_codes['userinfo'][$users_t->field_id] = $userid;
		return true;
	}
	private function deleteComment($cmntid,$blogid){
		$comment_t = new table_blogcomments();
		if (strlen($cmntid) != $comment_t->field_commentid_length){
			return false;
		}
		$stngs_t = new table_blogsettings();
		if (strlen($blogid) != $stngs_t->field_id_length){
			return false;
		}
		$query = 'DELETE FROM '.TABLE_BLOGCOMMENTS.' WHERE '.TABLE_BLOGCOMMENTS
			.'.'.$comment_t->field_commentid.' = '.toSQL($cmntid).' AND '
			.TABLE_BLOGCOMMENTS.'.'.$comment_t->field_blogid.' = '.toSQL($blogid)
			.' AND EXISTS (SELECT 1 FROM '.TABLE_BLOGSETTINGS.' WHERE '
				.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' = '.toSQL($blogid).' AND '
				.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner." = '".$this->userid."') LIMIT 1";
		db::query($query);
		return true;
	}
	private function deletePost($postid){
		$blog_t = new table_blog();
		if (strlen($postid) != $blog_t->field_id_length){
			return;
		}
		$stngs_t = new table_blogsettings();
		$query = 'DELETE FROM '.TABLE_BLOG.' USING '.TABLE_BLOG.','.TABLE_BLOGSETTINGS.' WHERE '
			.TABLE_BLOG.'.'.$blog_t->field_id
			.' = '.toSQL($postid).' AND '.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id
			.' = '.TABLE_BLOG.'.'.$blog_t->field_settingsid.' AND '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner." = '".$this->userid."'";
		db::query($query);
	}
	private function deleteSettings($id){
		$strngs_t = new table_blogsettings();
		if (strlen($id) != $strngs_t->field_id_length){
			return;
		}
		//delete only blogsettings and leave the rest of the job for cronjob
		//to save some time for the user's request
		$query = 'DELETE FROM '.TABLE_BLOGSETTINGS.' WHERE '
			.TABLE_BLOGSETTINGS.'.'.$strngs_t->field_id.' = '.toSQL($id).' AND '
			.TABLE_BLOGSETTINGS.'.'.$strngs_t->field_owner." = '".$this->userid."' LIMIT 1";
		db::query($query);
	}
	private function getForEditBlog($blogid){
		$strngs_t = new table_blogsettings();
		if (strlen($blogid) != $strngs_t->field_id_length){
			return false;
		}
		//get all information about the blog
		$ids_t = new table_ids();
		//left join ids table only if permissions are for specified
		$query = 'SELECT '.TABLE_BLOGSETTINGS.'.*,'.TABLE_IDS.'.'.$ids_t->field_subjectid
			.' FROM '.TABLE_BLOGSETTINGS.' LEFT JOIN '.TABLE_IDS.' ON '
			.'('.TABLE_BLOGSETTINGS.'.'.$strngs_t->field_permissions.' & '
			.self::PERM_SPECIFIED.') AND '.TABLE_BLOGSETTINGS.'.'.$strngs_t->field_id
			.' = '.TABLE_IDS.'.'.$ids_t->field_ownerid.' WHERE '
			.TABLE_BLOGSETTINGS.'.'.$strngs_t->field_id.' = '.toSQL($blogid).' AND '
			.TABLE_BLOGSETTINGS.'.'.$strngs_t->field_owner." = '".$this->userid."'";
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			$this->response_codes['blgid'] = $blogid;
			//set permissions
			$permissions = $row[$strngs_t->field_permissions];
			if (!($permissions & self::PERM_PUBLIC)){
				$this->response_codes['fields']['perm'] = 0;
				if($permissions & self::PERM_ALLSCHOOLS){
					$this->response_codes['fields']['perm_school2'] = self::PERM_ALLSCHOOLS;
				}else{
					if ($permissions & self::PERM_SPECIFIED){
						$this->response_codes['fields']['perm_friends2'] = self::PERM_SPECIFIED;
					}elseif($permissions & self::PERM_ALLFRIENDS){
						$this->response_codes['fields']['perm_friends1'] = self::PERM_ALLFRIENDS;
					}
					if($permissions & self::PERM_SCHOOL){
						$this->response_codes['fields']['perm_school1'] = self::PERM_SCHOOL;
					}
				}
			}else{
				$this->response_codes['fields']['perm'] = self::PERM_PUBLIC;
			}
			if ($permissions & self::PERM_EDIT_ALLSCHOOLS){
				$this->response_codes['fields']['perm_edit3'] = self::PERM_EDIT_ALLSCHOOLS;
			}
			if ($permissions & self::PERM_EDIT_FRIENDS){
				$this->response_codes['fields']['perm_edit1'] = self::PERM_EDIT_FRIENDS;
			}
			if ($permissions & self::PERM_EDIT_MYSCHOOL){
				$this->response_codes['fields']['perm_edit2'] = self::PERM_EDIT_MYSCHOOL;
			}
			$this->response_codes['fields']['title'] = $row[$strngs_t->field_header];
			$this->response_codes['fields']['style'] = $row[$strngs_t->field_custom];
			$this->response_codes['fields']['is_not_new'] = 1;
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
			//blog doesn't exists
			return false;
		}
		return true;
	}
	private function newSettings(){
		$stngs_t = new table_blogsettings();
		if (strlen($this->get['id']) != $stngs_t->field_id_length){
			return false;
		}
		if (empty($this->post['title'])){
			$this->response_codes['fields'] = $this->post;
			$this->response_codes['blgid'] = $this->get['id'];
			return 'please enter the short title to be used to identify this blog';
		}
		$perm = 0;
		foreach($this->post as $key => $value){
			if (substr($key,0,4) == 'perm'){
				if (!isset($this->post['allow_edit']) && $key == 'perm_edit'){
					continue;
				}
				$perm = $perm | $value;
			}
		}
		if (($perm & self::PERM_PUBLIC) && 
				(($perm & self::PERM_ALLFRIENDS) || ($perm & self::PERM_SPECIFIED)
					|| ($perm & self::PERM_SCHOOL)|| ($perm & self::PERM_ALLSCHOOLS))){
			$this->response_codes['fields'] = $this->post;
			$this->response_codes['blgid'] = $this->get['id'];
			return 'you can only specify people in your school or friends by making these settings private';
		}
		//correct user's error if specified private but granted edit
		if ((($perm & self::PERM_EDIT_ALLSCHOOLS) || ($perm & self::PERM_EDIT_MYSCHOOL) || ($perm & self::PERM_EDIT_FRIENDS)) 
			 && !($perm & self::PERM_ALLFRIENDS) 
			&& !($perm & self::PERM_SPECIFIED) 
			&& !($perm & self::PERM_SCHOOL) 
			&& !($perm & self::PERM_ALLSCHOOLS)
			&& !($perm & self::PERM_PUBLIC)){
			$this->response_codes['fields'] = $this->post;
			$this->response_codes['blgid'] = $this->get['id'];
			return 'you cannot set comment permissions since registered users will not have access';	
		}
		//check for illegal permissions
		if ((($perm & self::PERM_ALLFRIENDS) 
				&& ($perm & self::PERM_SPECIFIED))
			|| (($perm & self::PERM_ALLSCHOOLS)
				&& ($perm & self::PERM_ALLFRIENDS))
			|| (($perm & self::PERM_ALLSCHOOLS)
				&& ($perm & self::PERM_SPECIFIED))
			|| (($perm & self::PERM_ALLSCHOOLS)
				&& ($perm & self::PERM_SCHOOL))){
			return false;
		}
		//work with the style sheet if one is submited
		if (!isset($this->post['style'])){
			$style_text = null;
		}else{
			$style_text = strip_tags($this->post['style']);
			//replace all invalid properties
			$illegal_styles = array();
			$illegal_styles[] = 'z-index';
			$illegal_styles[] = 'display';
			$illegal_styles[] = 'visibility';
			$illegal_styles[] = 'position';
			$illegal_styles[] = 'ACCELERATOR';
			$illegal_styles[] = 'zoom';
			$style_text = preg_replace('/(?:'.implode('|',$illegal_styles).'):[^;}]*/','',$style_text);
			$style_text = preg_replace('/[\s]+/',' ',$style_text);
		}
		$fields = array();
		$fields[$stngs_t->field_id] = $this->get['id'];
		$fields[$stngs_t->field_owner] = $this->userid;
		$fields[$stngs_t->field_permissions] = $perm;
		$fields[$stngs_t->field_header] = $this->post['title'];
		$query = 'REPLACE INTO '.TABLE_BLOGSETTINGS.' ('.sqlFields(array_keys($fields))
			.',`'.$stngs_t->field_custom.'`,`'.$stngs_t->field_date
			.'`) values ('.toSQL($fields).','
			.((is_null($style_text))?'NULL':toSQL($style_text)).',CURDATE())';
		db::query($query);
		$ids_t = new table_ids();
		//delete all old entries in ids of all the users previously specified
		$query = 'DELETE QUICK FROM '.TABLE_IDS.' USING '.TABLE_BLOGSETTINGS.','.TABLE_IDS
			.' WHERE '.TABLE_IDS.'.'.$ids_t->field_ownerid.' = '
			.toSQL($this->get['id']).' AND '.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' = '
			.TABLE_IDS.'.'.$ids_t->field_ownerid.' AND '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner." = '".$this->userid."'";
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
		 				.') values ('.toSQL($this->get['id']).',?)';
		 			$stmt = db::prepare($query);
		 			foreach($mergedids as $id){
		 				$stmt->bind_param('s',$id);
						$stmt->execute();
		 			}
		 		}else{
		 			foreach($mergedids as $id){
		 				$query = 'REPLACE DELAYED INTO '.TABLE_IDS.' ('
		 					.$ids_t->field_ownerid.','.$ids_t->field_subjectid
		 					.') values ('.toSQL($this->get['id']).",'".$id."')";
		 				db::query($query);
		 			}
		 		}
		 	}
		}
		return true;	
	}
	/**returns a result-set of blogs posted by me from latest to earliest using 
	 *specified limit & start if such are present in the $_GET*/
	public function getMine($limit = null){
		if (is_null($limit) && (!isset($this->get['limit']) || !is_numeric($this->get['limit'])
			|| $this->get['limit'] < 2 || $this->get['limit']>20)){
			$this->get['limit'] = 10;
		}elseif(is_null($limit)){
			$limit = $this->get['limit'];
		}
		if (!isset($this->get['start']) || !is_numeric($this->get['start'])
			|| $this->get['start'] < 0){
			$start = 0;
		}else{
			$start = $this->get['start'];
		}
		$blog_t = new table_blog();
		$stngs_t = new table_blogsettings();
		$query = 'SELECT '.TABLE_BLOG.'.* FROM '.TABLE_BLOG.','.TABLE_BLOGSETTINGS.' WHERE '
			.TABLE_BLOG.'.'.$blog_t->field_settingsid." = "
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' AND '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner." = '"
			.$this->userid."' ORDER BY ".TABLE_BLOG.'.'.$blog_t->field_timedate
			." DESC LIMIT $start, $limit";
		return db::query($query);
	}
	/**returns a result set with recent blogs posted by other users*/
	public function getRecent($limit = null){
		if (is_null($limit) && (!isset($this->get['limit']) || !is_numeric($this->get['limit'])
			|| $this->get['limit'] < 2 || $this->get['limit']>20)){
			$limit = 10;
		}elseif(is_null($limit)){
			$limit = $this->get['limit'];
		}
		if (!isset($this->get['start']) || !is_numeric($this->get['start'])
			|| $this->get['start'] < 0){
			$start = 0;
		}else{
			$start = $this->get['start'];
		}
		$blog_t = new table_blog();
		$stngs_t = new table_blogsettings();
		$users_t = new table_users();
		$friends_t = new table_friends();
		$groups_t = new table_friends_in_groups();
		$ids_t = new table_ids();
		$fields = array();
		$fields[] = TABLE_BLOG.'.*';
		$fields[] = TABLE_USERS.'.'.$users_t->field_id.' as userid';
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_USERS.'.'.$users_t->field_school;
		$query = 'SELECT DISTINCT '.implode(',',$fields).' FROM '.TABLE_BLOG.','
			.TABLE_BLOGSETTINGS.','.TABLE_USERS.' WHERE '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner." != '".$this->userid."' AND "
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' = '
			.TABLE_USERS.'.'.$users_t->field_id
			.' AND '.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' = '
			.TABLE_BLOG.'.'.$blog_t->field_settingsid;
		$orcondition = array();
		//public blogs
		$orcondition[] = '('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
			.' & '.self::PERM_PUBLIC.')';
		//for my college
		$orcondition[] = '(('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
			.' & '.self::PERM_SCHOOL.') AND '
				.TABLE_USERS.'.'.$users_t->field_school." = '".$this->collegeid."')";
		//for all colleges
		$orcondition[] = '('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
			.' & '.self::PERM_ALLSCHOOLS.')';
		//get all blogs for all friends where i'm a friend
		$orcondition[] = '(('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
			.' & '.self::PERM_ALLFRIENDS
			.') AND EXISTS (SELECT 1 FROM '.TABLE_FRIENDS.' WHERE '
					.TABLE_FRIENDS.'.'.$friends_t->field_user1.' = '
					.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' AND '
					.TABLE_FRIENDS.'.'.$friends_t->field_user2." = '".$this->userid."' LIMIT 1))";
		//get all blogs for specified friends where I'm specified
		$orcondition[] = '(('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
			.' & '.self::PERM_SPECIFIED.') AND EXISTS (SELECT 1 FROM '
			.TABLE_IDS.','.TABLE_FRIENDS_IN_GROUPS
					.' WHERE '.TABLE_IDS.'.'.$ids_t->field_ownerid.' = '
					.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' AND ('
					.TABLE_IDS.'.'.$ids_t->field_subjectid." = '".$this->userid."' OR ("
					.TABLE_IDS.'.'.$ids_t->field_subjectid.' = '
					.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' AND '
					.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_userid." = '".$this->userid
					."')) LIMIT 1))";
		$query .= ' AND ('.implode(' OR ',$orcondition).')';
		$query .= ' ORDER BY '.TABLE_BLOG.'.'.$blog_t->field_timedate." DESC LIMIT $start, $limit";
		return db::query($query);
	}
	public function getUsersRecent($blog_id_array,$userid){
		if (empty($blog_id_array)){
			return null;
		}
		$blog_t = new table_blog();
		$stngs_t = new table_blogsettings();
		$query = 'SELECT '.TABLE_BLOG.'.* FROM '.TABLE_BLOG.','.TABLE_BLOGSETTINGS
			.' WHERE '.TABLE_BLOG.'.'.$blog_t->field_settingsid." = "
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' AND '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' = '.toSQL($userid).' AND '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' IN ('.toSQL($blog_id_array).')'
			.' ORDER BY RAND() DESC LIMIT '.USER_POST_DISPLAY_LIMIT;
		return db::query($query);
	}
	public function getUsersBlogs($userid,$schoolid){
		$limit = USER_BLOG_DISPLAY_LIMIT;
		if (!isset($this->get['blgstart']) || !is_numeric($this->get['blgstart'])
			|| $this->get['blgstart'] < 0){
			$start = 0;
		}else{
			$start = $this->get['blgstart'];
		}
		$stngs_t = new table_blogsettings();
		$friends_t = new table_friends();
		$groups_t = new table_friends_in_groups();
		$ids_t = new table_ids();
		$fields = array();
		$fields[] = TABLE_BLOGSETTINGS.'.'.$stngs_t->field_header;
		$fields[] = TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id;
		$query = 'SELECT DISTINCT '.implode(',',$fields).' FROM '
			.TABLE_BLOGSETTINGS.' WHERE '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' = '.toSQL($userid);
		//if not the owner check for permissions
		if ($userid !== $this->userid){
			$orcondition = array();
			//public blogs
			$orcondition[] = '('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_PUBLIC.')';
			//for my college
			if ($this->collegeid == $schoolid){
				$orcondition[] = '('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_SCHOOL.')';
			}
			//for all colleges
			$orcondition[] = '('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_ALLSCHOOLS.')';
			//get all blogs for all friends where i'm a friend
			$orcondition[] = '(('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_ALLFRIENDS
				.') AND EXISTS (SELECT 1 FROM '.TABLE_FRIENDS.' WHERE '
						.TABLE_FRIENDS.'.'.$friends_t->field_user1.' = '
						.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' AND '
						.TABLE_FRIENDS.'.'.$friends_t->field_user2." = '".$this->userid
						."' LIMIT 1))";
			//get all blogs for specified friends where I'm specified
			$orcondition[] = '(('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_SPECIFIED.') AND EXISTS (SELECT 1 FROM '
				.TABLE_IDS.' LEFT JOIN '.TABLE_FRIENDS_IN_GROUPS.' ON '
				.TABLE_IDS.'.'.$ids_t->field_subjectid.' = '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' AND '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_userid." = '".$this->userid
				."' WHERE ".TABLE_IDS.'.'.$ids_t->field_ownerid.' = '
					.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' AND (('
					.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' IS NOT NULL) OR ('
					.TABLE_IDS.'.'.$ids_t->field_subjectid." = '".$this->userid."'))"
					." LIMIT 1))";
			$query .= ' AND ('.implode(' OR ',$orcondition).')';
		}
		$query .= ' ORDER BY '.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_date." DESC LIMIT $start, $limit";
		return db::query($query);
	}
	/**for profile display*/
	public static function getProfileBlog($my_id,$user_id,$my_collegeid,$user_college){
		$stngs_t = new table_blogsettings();
		$blog_t = new table_blog();
		$fields = array();
		$fields[] = TABLE_BLOG.'.*';
		$fields[] = TABLE_BLOGSETTINGS.'.'.$stngs_t->field_header;
		$query = 'SELECT '.implode(',',$fields).' FROM '
			.TABLE_BLOGSETTINGS.','.TABLE_BLOG.' WHERE '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' = '.toSQL($user_id).' AND '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' = '.TABLE_BLOG.'.'.$blog_t->field_settingsid;
		//if not the owner check for permissions
		if ($my_id !== $user_id){
			$friends_t = new table_friends();
			$groups_t = new table_friends_in_groups();
			$ids_t = new table_ids();
			$orcondition = array();
			//public blogs
			$orcondition[] = '('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_PUBLIC.')';
			//for my college
			if ($my_collegeid === $user_college){
				$orcondition[] = '('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_SCHOOL.')';
			}
			//for all colleges
			$orcondition[] = '('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_ALLSCHOOLS.')';
			//get all blogs for all friends where i'm a friend
			$orcondition[] = '(('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_ALLFRIENDS
				.') AND EXISTS (SELECT 1 FROM '.TABLE_FRIENDS.' WHERE '
					.TABLE_FRIENDS.'.'.$friends_t->field_user1.' = '
					.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' AND '
					.TABLE_FRIENDS.'.'.$friends_t->field_user2." = '".$my_id."' LIMIT 1))";
			//get all blogs for specified friends where I'm specified
			$orcondition[] = '(('.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_permissions
				.' & '.self::PERM_SPECIFIED.') AND EXISTS (SELECT 1 FROM '
				.TABLE_IDS.','.TABLE_FRIENDS_IN_GROUPS
				.' WHERE '.TABLE_IDS.'.'.$ids_t->field_ownerid.' = '
				.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' AND ('
				.TABLE_IDS.'.'.$ids_t->field_subjectid." = '".$my_id."' OR ("
				.TABLE_IDS.'.'.$ids_t->field_subjectid.' = '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' AND '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_userid." = '".$user_id
				."')) LIMIT 1))";
			$query .= ' AND ('.implode(' OR ',$orcondition).')';
		}
		$query .= ' ORDER BY '.TABLE_BLOG.'.'.$blog_t->field_timedate.' DESC LIMIT 1';
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			return $row;
		}
		return null;
	}
	//returns a list of all of my blog settings (i.e. blogs not posts)
	public function getMySettingsList(){
		$blog_t = new table_blog();
		$stngs_t = new table_blogsettings();
		$fields = array();
		$fields[] = 'count(*) as blog_posts';
		$fields[] = TABLE_BLOGSETTINGS.'.*';
		$fields[] = TABLE_BLOG.'.'.$blog_t->field_id.' as postid';
		$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_BLOGSETTINGS
			.' LEFT JOIN '.TABLE_BLOG
			.' ON '.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' = '
			.TABLE_BLOG.'.'.$blog_t->field_settingsid.' WHERE '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner." = '".$this->userid
			."' GROUP BY ".TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' ORDER BY '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_date;
		return db::query($query);
	}
	/*set response codes for first time posts*/
	private function prepareForPostingToSettings($id){
		$stngs_t = new table_blogsettings();
		if (strlen($id) != $stngs_t->field_id_length){
			return false;
		}
		$query = 'SELECT '.$stngs_t->field_header.' FROM '.TABLE_BLOGSETTINGS.' WHERE '
			.$stngs_t->field_id.' = '.toSQL($id).' AND '
			.$stngs_t->field_owner." = '".$this->userid."' LIMIT 1";
		$result = db::query($query);
		if (!($row = db::fetch_row($result))){
			return false;
		}
		$this->response_codes['blgtitle'] = $row[0];
		$this->response_codes['blgid'] = $id;
		$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$blog_t = new table_blog();
		$genid = substr($genid, 0, $blog_t->field_id_length);
		$this->response_codes['fields']['postid'] = $genid;
		return true;
	}
	/**sets response variables for editing posts*/
	private function getForEditPost($postid){
		$blog_t = new table_blog();
		if (strlen($postid) != $blog_t->field_id_length){
			return false;
		}
		$stngs_t = new table_blogsettings();
		$fields = array();
		$fields[] = TABLE_BLOG.'.'.$blog_t->field_title;
		$fields[] = TABLE_BLOG.'.'.$blog_t->field_text;
		$fields[] = TABLE_BLOG.'.'.$blog_t->field_settingsid;
		$fields[] = TABLE_BLOGSETTINGS.'.'.$stngs_t->field_header;
		$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_BLOG.','.TABLE_BLOGSETTINGS.' WHERE '
			.TABLE_BLOG.'.'.$blog_t->field_id.' = '.toSQL($postid).' AND '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id.' = '
			.TABLE_BLOG.'.'.$blog_t->field_settingsid.' AND '
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner." = '".$this->userid."' LIMIT 1";
		$result = db::query($query);
		if (!($row = db::fetch_row($result))){
			return false;
		}
		$this->response_codes['fields']['title'] = $row[0];
		$this->response_codes['fields']['text_field'] = $row[1];
		$this->response_codes['blgid'] = $row[2];
		$this->response_codes['blgtitle'] = $row[3];
		$this->response_codes['fields']['postid'] = $postid;
		return true;
	}
	/**processes a new/old post*/
	private function newPost($blogid){
		$stngs_t = new table_blogsettings();
		if (strlen($blogid) != $stngs_t->field_id_length){
			return false;
		}
		$blog_t = new table_blog();
		//check hidden post id
		if (!isset($this->post['postid']) 
			|| strlen($this->post['postid']) != $blog_t->field_id_length){
			return false;
		}
		if (!isset($this->post['title']) || trim($this->post['title']) == ''){
			$this->response_codes['fields'] = $this->post;
			return 'please enter the title in order to continue';
		}
		if (!isset($this->post['text_field']) || trim($this->post['text_field']) == ''){
			$this->response_codes['fields'] = $this->post;
			return 'post text is empty';
		}
		$domFormater = new RichFormat($this->post['text_field']);
		if (!is_null($domFormater->getError())){
			$this->response_codes['fields'] = $this->post;
			return $domFormater->getError();
		}
		//make sure that this blog indeed belongs to the user
		$query = 'SELECT 1 FROM '.TABLE_BLOGSETTINGS.' WHERE '.$stngs_t->field_id.' = '
			.toSQL($blogid).' AND '.$stngs_t->field_owner." = '".$this->userid."' LIMIT 1";
		$result = db::query($query);
		if (!(db::fetch_row($result))){
			return false;
		}
		$fields = array();
		$fields[$blog_t->field_id] = $this->post['postid'];
		$fields[$blog_t->field_text] = $domFormater->getHtml();
		$fields[$blog_t->field_settingsid] = $blogid;
		$fields[$blog_t->field_title] = $this->post['title'];
		$query = 'REPLACE INTO '.TABLE_BLOG.' ('.sqlFields(array_keys($fields))
			.',`'.$blog_t->field_timedate.'`) values ('.toSQL($fields).',NOW())';
		db::query($query);
		//update date for last updated
		$query = 'UPDATE '.TABLE_BLOGSETTINGS.' SET '.$stngs_t->field_date.' = CURDATE() '
			.' WHERE '.$stngs_t->field_id.' = '.toSQL($blogid)
			.' AND '.$stngs_t->field_owner." = '".$this->userid."' LIMIT 1";
		db::query($query);
		return true;
	}
}
?>