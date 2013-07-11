<?php
class connection_control{
	private $userid;
	private $collegeid;
	private $get;
	private $post;
	public $message;
	function __construct($userid,$collegeid,$get,$post){
		$this->userid = $userid;
		$this->collegeid = $collegeid;
		$this->get = $get;
		$this->post = $post;
		$users_table = new table_users();
		/*if need to add a person to my friends*/
		if (!empty($get['addfriend']) && strlen($get['addfriend']) 
				== $users_table->field_id_length){
			//add a friend and get a response code back to determine appropriate output
			$resp_code = connection_control::addFriends($userid,$get['addfriend'],$more);
			if ($resp_code == 2){
				//bad user id, redirect
				header("Location: http://".$_SERVER['HTTP_HOST']."/home");
				exit;
			}
			switch($resp_code){
				case 5:
					$this->message = "Our records show that $more is already on your buddy list.";
					break;
				case 3:
					$this->message = "$more was added to your buddy list!";
					break;
				case 1:
					$this->message = "Unfortunately, you were blocked by $more[0] on the ".date('jS \o\f F, Y',funcs_getTimeStamp($more[1]));
					break;
				case 4:
					$this->message = "Your request to add $more to your buddy list is currently pending.";
					break;
				case 6:
					$this->message = "Your request to add $more to your buddy list was successful and $more will be notified immediately.";
					break;
			}
		}
		/*if need to remove a friend from my buddy list*/
		if (isset($get['removefriend']) && strlen($get['removefriend']) 
				== $users_table->field_id_length){
			if (self::deleteFriends($userid,array($get['removefriend']))){
				$this->message = 'You have successfully removed one of your friends';
			}
		}
		/*if need to add a friend to one of my groups*/
		if (isset($get['addtogroup']) && isset($get['user'])){
			self::addToGroup($userid,$get['addtogroup'],$get['user']);
		}
		/*if need to remove a friend from my buddy list*/
		if (isset($get['delufrmgrp']) && isset($get['user'])){
			self::removeFromGroup($userid,$get['delufrmgrp'],$get['user']);
		}
	}
	/**
	 *makes the two users friends
	 *changes more depending on the returning code
	 *codes are:
	 *1 - we are blocked
	 *2 - friend does not exist
	 *3 - became friends
	 *4 - request is currently pending
	 *5 - already are friends
	 *6 - just invited to become friends
	 */
	public static function addFriends($my_id,$friend,&$more){
		//if the joker is trying to add himself
		if ($my_id == $friend){
			return 2;
		}
		//check if users actually exists and get name, email
		$user_t = new table_users();
		$query = 'SELECT u1.'.$user_t->field_name.' as fname'
		 		.',u1.'.$user_t->field_email
				.',u1.'.$user_t->field_preferedemail
				.', u2.'.$user_t->field_name.' as myname'
				.' FROM '.TABLE_USERS.' as u1,'.TABLE_USERS.' as u2'
				.' WHERE u1.'.$user_t->field_id." = ".toSQL($friend)." AND "
				.'u2.'.$user_t->field_id." = '".$my_id."' LIMIT 1";
		$result = db::query($query);
		if ($array = db::fetch_array($result)){
			$friend_name = funcs_getNames($array['fname'],false);
			$my_name = funcs_getNames($array['myname'],false);
			$friend_email = $array[$user_t->field_email];
			$friend_email2 = $array[$user_t->field_preferedemail];
		}else{
			return 2;	//user doesn't exists
		}
		//now check if blocked
		if ($date = sqlFunc_is_blocked($my_id,$friend)){
			$more = array($friend_name,$date);
			return 1;
		}
		$more = $friend_name;
		$friends_t = new table_friends();
		$tempfriends_t = new table_tempfriends();
		//next check if user has been invited earlier
		$query = 'SELECT '.$tempfriends_t->field_invited.' FROM '.TABLE_TEMPFRIENDS.' WHERE '
			.$tempfriends_t->field_inviter." = ".toSQL($friend)." AND "
			.$tempfriends_t->field_invited." = '".$my_id."' LIMIT 1";
		$result = db::query($query);
		if (db::fetch_row($result)){
			//make the two friends and delete temp entry
			$fields = array($friends_t->field_user1,$friends_t->field_user2);
			$values = array($friend,$my_id);
			$values2 = array($my_id,$friend);
			$query = 'INSERT INTO '.TABLE_FRIENDS.' ('.array_tostring($fields)
				.') values ('.toSQL($values).'),('.toSQL($values2).')';
			$success = db::query($query);
			//now delete an entry from temp table
			$query = 'DELETE FROM '.TABLE_TEMPFRIENDS.' WHERE '
					.$tempfriends_t->field_inviter." = ".toSQL($friend)." AND "
					.$tempfriends_t->field_invited." = '".$my_id."' LIMIT 1";
			$result = db::query($query);
			return 3;	//just became friends
		}
		//now check if the request is currently pending
		$query = 'SELECT '.$tempfriends_t->field_invited.' FROM '.TABLE_TEMPFRIENDS.' WHERE '
			.$tempfriends_t->field_inviter." = '".$my_id."' AND "
			.$tempfriends_t->field_invited." = ".toSQL($friend)." LIMIT 1";
		$result = db::query($query);
		if (db::fetch_row($result)){
			return 4; //request is currently pending
		}
		//check wherever we are alredy friends
		$query = 'SELECT '.$friends_t->field_user1.' FROM '.TABLE_FRIENDS
				.' WHERE '.$friends_t->field_user1." = '".$my_id."' AND "
				.$friends_t->field_user2." = ".toSQL($friend)." LIMIT 1";
		$result = db::query($query);
		if (db::fetch_row($result)){
			return 5; //already are friends
		}
		//else add the user to the temp table and send email to friend
		$fields = array($tempfriends_t->field_inviter,
						$tempfriends_t->field_invited,
						$tempfriends_t->field_date);
		$values = array($my_id,$friend);
		$query = 'INSERT DELAYED INTO '.TABLE_TEMPFRIENDS.' ('
			.sqlFields($fields).') values ('
			.toSQL($values).',CURDATE())';
		$success = db::query($query);
		/*delete an entry (if one exists) in the blocked table since if one makes another person
		 *a friends then he shouldn't block him*/
		$blocked_t = new table_blocklist();
		$query = 'DELETE FROM '.TABLE_BLOCKLIST.' WHERE '.$blocked_t->field_blockerid." = '"
			.$my_id."' AND ".$blocked_t->field_blockedid." = '".$friend."' LIMIT 1";
		db::query($query);
		//send email
		if (!is_null($friend_email2)){
			$emails = array($friend_email,$friend_email2);
		}else{
			$emails = $friend_email;
		}
		$subject = "$friend_name has listed you as a friend on partyHub";
		$filename = $_SERVER['DOCUMENT_ROOT'].MAIL_MESSAGES.'addfriend.txt';
		$message =& file_get_contents($filename);
		$message = preg_replace('/<<invited>>/i',$friend_name,$message);
		$message = preg_replace('/<<inviter>>/i',$my_name,$message);
		myMail(false, $emails, $subject, $message);
		return 6;	//successfully invited
	}
	public static function deleteFriends($my_id,$list_of_ids,$limit = 1){
		$limit = $limit*2; //multiply by 2 since deleting friends twice
		//first delete from friends table
		$friends_t = new table_friends();
		$groups_t = new table_friends_in_groups();
		$tempfriends_t = new table_tempfriends();
		$ids = toSQL($list_of_ids);
		$query = 'DELETE FROM '.TABLE_FRIENDS.' WHERE ('
			.$friends_t->field_user1." = '".$my_id."' AND "
			.$friends_t->field_user2.' IN ('.$ids
			.')) OR ('
			.$friends_t->field_user2." = '".$my_id."' AND "
			.$friends_t->field_user1.' IN ('.$ids
			.")) LIMIT $limit";
		db::query($query);
		$limit = $limit/2;
		//now delete from temp table
		$query = 'DELETE FROM '.TABLE_TEMPFRIENDS.' WHERE ('
			.$tempfriends_t->field_inviter." = '".$my_id."' AND "
			.$tempfriends_t->field_invited.' IN ('.$ids
			.')) OR ('
			.$tempfriends_t->field_invited." = '".$my_id."' AND "
			.$tempfriends_t->field_inviter.' IN ('.$ids
			.")) LIMIT $limit";
		db::query($query);
		//now delete from friend's groups
		$query = 'DELETE FROM '.TABLE_FRIENDS_IN_GROUPS.' WHERE ('
				.$groups_t->field_ownerid." = '".$my_id."' AND "
				.$groups_t->field_userid.' IN ('.$ids.')) OR ('
				.$groups_t->field_userid." = '".$my_id."' AND "
				.$groups_t->field_ownerid.' IN ('.$ids.'))';
		db::query($query);
		return true;
	}
	/**returns true if statements were executed, false if group doesn't exists*/
	public static function addToGroup($owner_id,$group_id,$user_id_list){
		//first check if group exists
		if (!sqlFunc_getGroups($owner_id,null,$group_id)){
			return false;
		}
		$groups_t = new table_friends_in_groups();
		//if only one id
		if (count($user_id_list) == 1){
			if (!is_array($user_id_list)){
				$user_id = $user_id_list;
			}else{
				$user_id = array_pop($user_id_list);
			}
			//if user is adding himself to his own group
			if ($user_id == $owner_id){
				return false;
			}
			$query = 'REPLACE INTO '.TABLE_FRIENDS_IN_GROUPS
				.' ('.$groups_t->field_ownerid.','.$groups_t->field_userid.','
				.$groups_t->field_groupid.") values ('$owner_id',"
				.toSQL($user_id).",".toSQL($group_id).")";
			$success = db::query($query);
			return true;
		}else{
			//prepare a statement first
			/*$query = "PREPARE addtogroupstmt FROM 'REPLACE INTO ".TABLE_FRIENDS_IN_GROUPS
				." (".$groups_t->field_ownerid.",".$groups_t->field_userid.","
				.$groups_t->field_groupid.") values (\\'$owner_id\\',?,\\'"
				.$group_id."\\')'";
			$success = db::query($query);*/
			//continue only if prepared this statement
			//if ($success){
				$success2 = array();
				foreach($user_id_list as $user_id){
					//execute only if not the same user as the owner of the group
					if ($user_id != $owner_id){
						//$query = 'EXECUTE addtogroupstmt USING '.toSQL($user_id);
						$query = 'REPLACE INTO '.TABLE_FRIENDS_IN_GROUPS
							.' ('.$groups_t->field_ownerid.','.$groups_t->field_userid.','
							.$groups_t->field_groupid.") values ('$owner_id',"
							.toSQL($user_id).",".toSQL($group_id).")";
						$success2[] = db::query($query);
					}
				}
			//}
			return true;
		}
	}
	public static function removeFromGroup($user_id,$group_id,$id_list){
		if (is_array($id_list)){
			$ids = toSQL($id_list);
		}else{
			$ids = toSQL($id_list);
		}
		$limit = count($ids);
		$groups_t = new table_friends_in_groups();
		$query = 'DELETE FROM '.TABLE_FRIENDS_IN_GROUPS.' WHERE '
			.$groups_t->field_ownerid." = '".$user_id."' AND "
			.$groups_t->field_groupid.' = '.toSQL($group_id).' AND '
			.$groups_t->field_userid.' IN ('.$ids.') LIMIT '.$limit;
		$success = db::query($query);
		return $success;
	}
	public function getAllGroups(){
		//get groups
		$groups_t = new table_friendgroups();
		$result = sqlFunc_getGroups($this->userid);
		$all_groups = array();
		while ($array = db::fetch_assoc($result)){
			$all_groups[$array[$groups_t->field_groupid]] = $array[$groups_t->field_title];
		}
		return $all_groups;
	}
}
?>