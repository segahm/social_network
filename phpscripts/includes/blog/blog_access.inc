<?php
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/permissions.php");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/blog/blog_control.inc");
class BlogAccess{
	private $get;
	private $post;
	public $error;
	public $response_codes = array();
	const REDIRECT = 'redirect';
	const IS_ACCESS = 'access';
	const SETTINGS = 'settings';
	const IS_EDIT_ACCESS = 'iseditaccess';
	public $blogid;
	function __construct(){
		$this->get = $_GET;
		$this->post = $_POST;
		if (isset($_GET['cmnt']) && isset($_GET['id'])){
			$code = $this->postComment();
			if ($code === false){
				$this->setRedirect('/home');
				return;
			}elseif($code === true){
				$this->setRedirect('/findblog?id='.$_GET['id']
					.((isset($_GET['isme']))?'&isme=1':''));
				return;
			}
			$this->error = $code;
			$this->response_codes['fields'] = $this->post;
		}
		//see first if we are browsing random blog
		if (isset($_GET['rndm'])){
			$code = $this->processRandom();
			//if we couldn't find a single random blog - WHICH MEANS THAT 
			//EITHER THERE IS A MAJOR CODE PROBLEM OR THE SITE HAS DISCOVERED
			//A VERY FAT BLACK HOLE
			if ($code === false){
				$this->setRedirect('/home');
			}else{
				//we only search for public random blogs
				$this->response_codes[self::IS_ACCESS] = Blog::PERM_PUBLIC;
			}
			return;
		}
		if (!isset($_GET['id'])){
			$this->setRedirect('/home');
			return;
		}
		$this->blogid = $_GET['id'];
		$stngs_t = new table_blogsettings();
		if (strlen($this->blogid) != $stngs_t->field_id_length){
			$this->setRedirect('/home');
			return;
		}
		if (!$this->getBlogSettings($this->blogid)){
			//settings were not found
			$this->setRedirect('/home');
			return;
		}
		$perm = $this->response_codes[self::SETTINGS][$stngs_t->field_permissions];
		//check for public access only if the blog wasn't marked with is me
		//so that we can give special permissions to the owner if he is indeed the owner
		if (!isset($_GET['isme']) && ($perm & Blog::PERM_PUBLIC)){
			//grant access
			$this->response_codes[self::IS_ACCESS] = Blog::PERM_PUBLIC;
			return;
		}
		//else check for private access but authenticate first
		auth_authCheck();
		//if still here user newly acquired user id and college id to
		//check wherever we have a private access to this blog
		if (!$this->privateAccesCheck($this->blogid,$perm,AUTH_USERID,AUTH_COLLEGEID)){
			$this->setRedirect('/blogger?rstctdacs=1');
			unset($this->response_codes[self::IS_ACCESS]);
			return;
		}
	}
	private function setRedirect($url){
		if (isset($this->response_codes[self::REDIRECT])){
			return;
		}
		$this->response_codes[self::REDIRECT] = $url;
	}
	/**checks for permissions and */
	private function postComment(){
		$access = false;
		if (!isset($this->post['text']) || trim($this->post['text']) == ''){
			return 'please enter your text comment';
		}
		$stngs_t = new table_blogsettings();
		if (!isset($this->get['id']) 
			|| strlen($this->get['id']) != $stngs_t->field_id_length){
			return false;
		}
		$comments_t = new table_blogcomments();
		if (!isset($this->post['postid']) 
			|| strlen($this->post['postid']) != $comments_t->field_commentid_length){
			return false;
		}
		//now check editing permissions on this blog
		//first attempt to authenticate
		$authorized = auth_authCheck(false);
		if (!$authorized){
			return 'you cannot post a comment since you are not signed in';
		}
		//now check all the usuall stuff regarding permissions
		if (!$this->getBlogSettings($this->get['id'])){
			return false;
		}
		$perm = $this->response_codes[self::SETTINGS][$stngs_t->field_permissions];
		$users_t = new table_users();
		$owner_id = $this->response_codes[self::SETTINGS][$stngs_t->field_owner];
		$owner_school = $this->response_codes[self::SETTINGS][$users_t->field_school];
		//check if I'm the owner
		if ($owner_id == AUTH_USERID){
			$access = true;
		}
		//check that I'm not blocked
		if (sqlFunc_is_blocked(AUTH_USERID,$owner_id)){
			$this->setRedirect('/blocked?usr='.$owner_id);
			return false;
		}
		if (!$access && ($perm & Blog::PERM_EDIT_ALLSCHOOLS)){
			//since we are authenticated then yes access
			$access = true;
		}
		if (!$access && ($perm & Blog::PERM_EDIT_MYSCHOOL)){
			//check if we are in the same school
			if ($owner_school == AUTH_COLLEGEID){
				$access = true;
			}
		}
		if (!$access && ($perm & Blog::PERM_EDIT_FRIENDS)){
			$friends_t = new table_friends();
			$query = 'SELECT 1 FROM '.TABLE_FRIENDS
				.' WHERE '.TABLE_FRIENDS.'.'.$friends_t->field_user1." = '"
				.$owner_id."' AND ".TABLE_FRIENDS.'.'.$friends_t->field_user2
				." = '".AUTH_USERID."' LIMIT 1";
			$result = db::query($query);
			if (db::fetch_row($result)){
				//set response code for further access check
				$this->response_codes['isfriends'] = true;
				$access = true;
			}
			$this->response_codes['isfriends'] = false;
		}
		if (!$access){
			return 'you do not have permissions to post a comment';
		}
		//post a comment
		$fields = array();
		$fields[$comments_t->field_blogid] = $this->get['id'];
		$fields[$comments_t->field_commentid] = $this->post['postid'];
		$fields[$comments_t->field_text] = strip_tags($this->post['text']);
		$fields[$comments_t->field_userid] = AUTH_USERID;
		$query = 'REPLACE INTO '.TABLE_BLOGCOMMENTS.' ('
			.sqlFields(array_keys($fields)).',`'.$comments_t->field_time
			.'`) values ('.toSQL($fields).',NOW())';
		db::query($query);
		return true;
	}
	/**gets a random public event*/
	private function processRandom(){
		$stngs_t = new table_blogsettings();
		$users_t = new table_users();
		$col_t = new table_colleges();
		$profile_t = new table_profile();
		$fields = array();
		$fields[] = TABLE_BLOGSETTINGS.'.*';
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_USERS.'.'.$users_t->field_school;
		$fields[] = TABLE_COLLEGES.'.'.$col_t->field_fullname;
		$fields[] = TABLE_PROFILE.'.'.$profile_t->field_picture;
		$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_BLOGSETTINGS.','
			.TABLE_USERS.','.TABLE_PROFILE.','.TABLE_COLLEGES
			.' WHERE '.TABLE_USERS.'.'.$users_t->field_id
			.' = '.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' AND '
			.TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_PROFILE.'.'.$profile_t->field_id.' AND '
			.TABLE_COLLEGES.'.'.$col_t->field_id.' = '
			.TABLE_USERS.'.'.$users_t->field_school
			.' ORDER BY RAND() LIMIT 1';
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			$this->response_codes[self::SETTINGS] = $row;
			$this->blogid = $this->response_codes[self::SETTINGS][$stngs_t->field_id];
			return true;
		}
		return false;
	}
	public function getComments(){
		$comments_t = new table_blogcomments();
		$users_t = new table_users();
		$fields = array();
		$fields[] = TABLE_BLOGCOMMENTS.'.*';
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_USERS.'.'.$users_t->field_school;
		$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_BLOGCOMMENTS.','.TABLE_USERS
			.' WHERE '.TABLE_BLOGCOMMENTS.'.'.$comments_t->field_blogid
			.' = '.toSQL($this->blogid).' AND '.TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_BLOGCOMMENTS.'.'.$comments_t->field_userid.' ORDER BY '
			.TABLE_BLOGCOMMENTS.'.'.$comments_t->field_time.' DESC';
		return db::query($query);
	}
	public function getPosts(){
		$blog_t = new table_blog();
		$query = 'SELECT * FROM '.TABLE_BLOG.' WHERE '.$blog_t->field_settingsid
			.' = '.toSQL($this->blogid).' ORDER BY '.$blog_t->field_timedate.' DESC';
		return db::query($query);
	}
	public function getOtherBlogsByTheUser(){
		$stngs_t = new table_blogsettings();
		$user_id = $this->response_codes[self::SETTINGS][$stngs_t->field_owner];
		$stngs_t = new table_blogsettings();
		$query = 'SELECT '.$stngs_t->field_header.','.$stngs_t->field_date.','
			.$stngs_t->field_id.' FROM '.TABLE_BLOGSETTINGS.' WHERE '
			.$stngs_t->field_owner." = '".$user_id."' AND "
			.$stngs_t->field_id.' != '.toSQL($this->blogid);
		return db::query($query);
	}
	public function getByUsersFriends(){
		$stngs_t = new table_blogsettings();
		$user_id = $this->response_codes[self::SETTINGS][$stngs_t->field_owner];
		$friends_t = new table_friends();
		$fields = array();
		$fields[] = TABLE_BLOGSETTINGS.'.'.$stngs_t->field_header;
		$fields[] = TABLE_BLOGSETTINGS.'.'.$stngs_t->field_date;
		$fields[] = TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id;
		$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_BLOGSETTINGS.','
			.TABLE_FRIENDS.' WHERE '
			.TABLE_FRIENDS.'.'.$friends_t->field_user1." = '".$user_id."' AND "
			.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' = '
			.TABLE_FRIENDS.'.'.$friends_t->field_user2.' ORDER BY RAND() LIMIT 10';
		return db::query($query);
	}
	private function getBlogSettings($blogid){
		if (isset($this->response_codes[self::SETTINGS])){
			return true;
		}
		$stngs_t = new table_blogsettings();
		$users_t = new table_users();
		$col_t = new table_colleges();
		$profile_t = new table_profile();
		$fields = array();
		$fields[] = TABLE_BLOGSETTINGS.'.*';
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_USERS.'.'.$users_t->field_school;
		$fields[] = TABLE_COLLEGES.'.'.$col_t->field_fullname;
		$fields[] = TABLE_PROFILE.'.'.$profile_t->field_picture;
		$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_BLOGSETTINGS.','
			.TABLE_USERS.','.TABLE_PROFILE.','.TABLE_COLLEGES
			.' WHERE '.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_id
			.' = '.toSQL($blogid).' AND '.TABLE_USERS.'.'.$users_t->field_id
			.' = '.TABLE_BLOGSETTINGS.'.'.$stngs_t->field_owner.' AND '
			.TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_PROFILE.'.'.$profile_t->field_id.' AND '
			.TABLE_COLLEGES.'.'.$col_t->field_id.' = '
			.TABLE_USERS.'.'.$users_t->field_school
			.' LIMIT 1';
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			$this->response_codes[self::SETTINGS] = $row;
			return true;
		}
		return false;
	}
	/**checks access for the blog, assuming that wherever it is public or
	 *not was checked earlier and resulted in it being private*/
	private function privateAccesCheck($blogid,$perm,$userid,$collegeid){
		$stngs_t = new table_blogsettings();
		$users_t = new table_users();
		$owner_id = $this->response_codes[self::SETTINGS][$stngs_t->field_owner];
		$owner_school = $this->response_codes[self::SETTINGS][$users_t->field_school];
		//check if I'm the owner
		if ($owner_id == $userid){
			$this->response_codes[self::IS_ACCESS] = Blog::PERM_PRIVATE;
			return true;
		}
		//check that I'm not blocked
		if (sqlFunc_is_blocked($userid,$owner_id) !== false){
			$this->setRedirect('/blocked?usr='.$owner_id);
			return false;
		}
		//check if we are in the same school and permissions are for the same school
		if ($perm & Blog::PERM_SCHOOL){
			if ($owner_school == $collegeid){
				$this->response_codes[self::IS_ACCESS] =  Blog::PERM_SCHOOL;
				return true;
			}
		}elseif($perm & Blog::PERM_ALLSCHOOLS){
			//if for all schools, then since I'm a registered user it follows that
			//I have access
			$this->response_codes[self::IS_ACCESS] =  Blog::PERM_ALLSCHOOLS;
			return true;
		}
		if($perm & Blog::PERM_ALLFRIENDS){
			$this->response_codes[self::IS_ACCESS] =  Blog::PERM_ALLFRIENDS;
			//check if we have previously checked for friends
			if (isset($this->response_codes['isfriends'])){
				return $this->response_codes['isfriends'];
			}
			//find out if we are friends
			$friends_t = new table_friends();
			$query = 'SELECT 1 FROM '.TABLE_FRIENDS
				.' WHERE '.TABLE_FRIENDS.'.'.$friends_t->field_user1." = '"
				.$owner_id."' AND ".TABLE_FRIENDS.'.'.$friends_t->field_user2
				." = '".$userid."' LIMIT 1";
		}elseif($perm & Blog::PERM_SPECIFIED){
			//check if we have previously checked for friends and found no connection
			if (isset($this->response_codes['isfriends']) 
				&& !$this->response_codes['isfriends']){
				return false;
			}
			//find out if i'm a friend with this person and I was specified in his list
			$groups_t = new table_friends_in_groups();
			$ids_t = new table_ids();
			$query = 'SELECT 1 FROM '.TABLE_IDS.' LEFT JOIN '.TABLE_FRIENDS_IN_GROUPS.' ON '
				.TABLE_IDS.'.'.$ids_t->field_subjectid.' = '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' AND '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_userid." = '".$userid
				."' WHERE "
				.TABLE_IDS.'.'.$ids_t->field_ownerid.' = '.toSQL($blogid).' AND (('
				.TABLE_FRIENDS_IN_GROUPS.'.'.$groups_t->field_groupid.' IS NOT NULL) OR ('
				.TABLE_IDS.'.'.$ids_t->field_subjectid." = '".$userid."')) LIMIT 1";
			$this->response_codes[self::IS_ACCESS] =  Blog::PERM_SPECIFIED;
		}
		if (isset($query)){
			$result = db::query($query);
			if (db::fetch_row($result)){
				return true;
			}
		}
		return false;
	}
}
?>