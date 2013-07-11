<?php
define('HOME_AWAITING_FRIENDS',1);
define('HOME_ANYMAIL',2);
class home_controller{
	public $userid;
	public $collegeid;
	public $get;
	public $post;
	public $awaitingFriends;
	public $mail;
	function home_controller($user_id,$college_id,$get_form,$post_form){
		$this->userid = $user_id;
		$this->collegeid = $college_id;
		$this->get = $get_form;
		$this->post = $post_form;
		if (isset($this->get['refusefriend'])){
			$this->refuseFriendship($this->userid,$this->get['refusefriend']);
		}
		//set variables for show access
		$this->awaitingFriendConfirm();
		$this->checkMail();
	}
	/**checks wherever any unread mail is in the inbox*/
	function checkMail(){
		if (!is_null($this->mail)){
			return $this->mail;
		}
		$message_t = new table_messages();
		$users_t = new table_users();
		$profile_t = new table_profile();
		$query = 'SELECT '.TABLE_MESSAGES.'.'.$message_t->field_messageid
			.','.TABLE_USERS.'.'.$users_t->field_name.','
			.TABLE_PROFILE.'.'.$profile_t->field_picture.','
			.TABLE_USERS.'.'.$users_t->field_school
			.' FROM '.TABLE_MESSAGES.','.TABLE_USERS.','.TABLE_PROFILE
			.' WHERE '.TABLE_MESSAGES.'.'.$message_t->field_to
			." = '".$this->userid."' AND ".TABLE_MESSAGES.'.'
			.$message_t->field_isread." = '0' AND ".TABLE_USERS.'.'
			.$users_t->field_id.' = '.TABLE_MESSAGES.'.'
			.$message_t->field_from.' AND '.TABLE_USERS.'.'
			.$users_t->field_id.' = '.TABLE_PROFILE.'.'
			.$profile_t->field_id.' LIMIT 1';
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			$this->mail = $row;
		}else{
			$this->mail = null;
		}
		return $this->mail;
		
	}
	/**deletes an entry from tempfriends table where inviter invited me*/
	function refuseFriendship($my_id,$inviter_id){
		$tempfriends_t = new table_tempfriends();
		if(strlen($inviter_id) != $tempfriends_t->field_inviter_length){
			return;
		}
		$query = 'DELETE FROM '.TABLE_TEMPFRIENDS.' WHERE '
			.$tempfriends_t->field_inviter." = '".$inviter_id."' AND "
			.$tempfriends_t->field_invited." = '".$my_id."' LIMIT 1";
		db::query($query);
	}
	function show($code){
		if ($code == HOME_AWAITING_FRIENDS
			&& !empty($this->awaitingFriends)){
			return true;
		}
		if ($code == HOME_ANYMAIL
			&& !empty($this->mail)){
			return true;
		}
		return false;
	}
	/**returns a result set with users who are trying to add me to their
 		*friend's list and are awaiting my confirmation;
 		*returns: an array with inviter = that user's id, name, picture, 
		 *date the request was processed*/
	function awaitingFriendConfirm(){
		if (!is_null($this->awaitingFriends)){
			return $this->awaitingFriends;
		}
		$tempfriends_t = new table_tempfriends();
		$users_t = new table_users();
		$profile_t = new table_profile();
		$query = 'SELECT '
			.TABLE_USERS.'.'.$users_t->field_id.' as inviter,'
			.TABLE_USERS.'.'.$users_t->field_name.' as name,'
			.TABLE_PROFILE.'.'.$profile_t->field_picture.' as picture,'
			.TABLE_TEMPFRIENDS.'.'.$tempfriends_t->field_date.' as date FROM '
				.TABLE_TEMPFRIENDS.','.TABLE_USERS.','.TABLE_PROFILE
			.' WHERE '.TABLE_TEMPFRIENDS.'.'.$tempfriends_t->field_invited
			." = '".$this->userid."' AND ".TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_TEMPFRIENDS.'.'.$tempfriends_t->field_inviter.' AND '
			.TABLE_USERS.'.'.$users_t->field_id
			.' = '.TABLE_PROFILE.'.'.$profile_t->field_id;
		$result = db::query($query);
		$this->awaitingFriends = array();
		while($row = db::fetch_assoc($result)){
			$this->awaitingFriends[] = $row;
		}
		return $this->awaitingFriends;
	}
}
?>