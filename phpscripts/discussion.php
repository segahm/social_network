<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/mail/mail_controller.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/utils/richformat.inc");
auth_authCheck();
class discussion_control{
	private $get;
	private $post;
	private $userid;
	private $collegeid;
	private $responseCode = null;
	public $storage_array = array();
	private $error = null;
	const RESPONSE_NEWTOPIC = 1;
	const RESPONSE_SHOWTOPICS = 2;
	const RESPONSE_MESSAGES = 3;
	const RESPONSE_REDIRECT = 4;
	const RESPONSE_NEWMESSAGE = 5;
	public $fields = array();
	public $new_location;
	function __construct($userid,$collegeid,$get,$post){
		//set default reidirect location
		$new_location = 'http://' . $_SERVER['HTTP_HOST'] . '/home';
		$this->userid = $userid;
		$this->collegeid = $collegeid;
		$this->post = $post;
		$this->get = $get;
		if (isset($get['newtopic'])){
			if (!$this->newTopic()){
				$this->setResponse(self::RESPONSE_NEWTOPIC);
			}else{
				$this->setResponse(self::RESPONSE_MESSAGES);
			}
			return;
		}
		if (isset($get['newmsg'])){
			if ($this->newMessage()){
				$this->setResponse(self::RESPONSE_MESSAGES);
			}else{
				$this->setResponse(self::RESPONSE_NEWMESSAGE);
			}
			return;
		}
		if (isset($get['msgs'])){
			$result = $this->getMessages();
			if ($result === false){
				//if invalid request
				$this->setResponse(self::RESPONSE_REDIRECT);
			}else{
				$this->storage_array['getMessages'] = $result;
				$this->setResponse(self::RESPONSE_MESSAGES);
				if (isset($get['invite'])){
					$this->inviteFriends($get['tpcid']);
				}
			}
			return;
		}
		//for administrative purposes
		if (isset($get['rmvmsg'])){
			$this->removeMsg($get['rmvmsg']);
		}
		if (isset($get['rmvtpc'])){
			$this->removeTpc($get['rmvtpc']);
		}
		//by default show topics
		$this->setResponse(self::RESPONSE_SHOWTOPICS);
	}
	private function removeMsg($id){
		$msg_t = new table_topicmessages();
		$query = 'DELETE FROM '.TABLE_TOPICMESSAGES.' WHERE '.$msg_t->field_messageid
			.' = '.toSQL($id).' LIMIT 1';
		db::query($query);
	}
	private function removeTpc($id){
		$msg_t = new table_topicmessages();
		$tpc_t = new table_topics();
		$query = 'DELETE FROM '.TABLE_TOPICS.','.TABLE_TOPICMESSAGES.' USING '
			.TABLE_TOPICS.' LEFT JOIN '.TABLE_TOPICMESSAGES.' ON '
			.TABLE_TOPICMESSAGES.'.'.$msg_t->field_topicid.' = '.toSQL($id).' WHERE '
			.TABLE_TOPICS.'.'.$tpc_t->field_id.' = '.toSQL($id);
		db::query($query);
	}
	/**takes ids from the submited list and sends an invite to them*/
	private function inviteFriends($tpcid){
		if (empty($this->post)){
			return;
		}
		$users_t = new table_users();
		$ids = array();
		foreach ($this->post as $id => $value){
			if (!preg_match('/id_[a-z0-9]{'.$users_t->field_id_length.','.$users_t->field_id_length.'}/',$id)){
				continue;
			}
			$ids[] = str_replace('id_','',$id);
		}
		$name = funcs_getNames(AUTH_USER_NAME);
		//now send each one of them an invite
		$message = $name[0].' '.$name[1].' has invited you to the topic '
			.'discussion. Click <A href="http://'.USED_DOMAIN_NAME.'/discussion?msgs=1&amp;tpcid='.$tpcid
			.'">here</A> if you are interested.';
		$subject = "you've been invited to the group discussion";
		$mail = new mail_controller($this->userid,$this->collegeid,array(),array(),true);
		$mail->customSend($ids,$message,$subject);
		$this->storage_array['invited'] = 1;
	}
	public function getMessages(){
		if (isset($this->storage_array['getMessages'])){
			return $this->storage_array['getMessages'];
		}
		if (!isset($this->get['limit']) || !is_numeric($this->get['limit'])
			|| $this->get['limit'] < 2 || $this->get['limit']>99){
			$limit =10;
		}else{
			$limit = $this->get['limit'];
		}
		if (!isset($this->get['page']) || !is_numeric($this->get['page'])
			|| $this->get['page'] < 1){
			$this->get['page'] = 1;
		}
		$start = ($this->get['page']-1)*$limit;
		$this->storage_array['page'] = $this->get['page'];
		$this->storage_array['limit'] = $limit;
		
		$users_t = new table_users();
		$topics_t = new table_topics();
		$topicmsg_t = new table_topicmessages();
		$profile_t = new table_profile;
		//first find topic id/topic title
		$query = 'SELECT '.$topics_t->field_id.','.$topics_t->field_topic.' FROM '.TABLE_TOPICS.' WHERE ';
		if (isset($this->get['tpc'])){
			//if searching by topic title
			$query .= TABLE_TOPICS.'.'.$topics_t->field_topic.' = '.toSQL($this->get['tpc']);
		}elseif(isset($this->get['tpcid'])){
			//if searching by topic id
			$query .= TABLE_TOPICS.'.'.$topics_t->field_id.' = '.toSQL($this->get['tpcid']);
		}else{
			return false;
		}
		$query .= ' LIMIT 1';
		$result = db::query($query);
		if (!($row = db::fetch_row($result))){
			//nothing was found. if topic id was specified then invalid request,
			return (isset($this->get['tpcid']))?false:null;
		}else{
			$this->storage_array['tpcid'] = $row[0];
			$this->storage_array['topic'] = $row[1];
		}	
		//construct a query for checking block list
		$blocked_t = new table_blocklist();
		$query = 'SELECT '.TABLE_TOPICS.'.'.$topics_t->field_userid.' FROM '.TABLE_BLOCKLIST.','.TABLE_TOPICS.' WHERE '
			.TABLE_BLOCKLIST.'.'.$blocked_t->field_blockerid.' = '
			.TABLE_TOPICS.'.'.$topics_t->field_userid.' AND '
			.TABLE_BLOCKLIST.'.'.$blocked_t->field_blockedid." = '".$this->userid."'"
			.' AND '.TABLE_TOPICS.'.'.$topics_t->field_id.' = '.toSQL($this->storage_array['tpcid']).' LIMIT 1';
		$result = db::query($query);
		if ($row = db::fetch_row($result)){
			//no access
			$this->new_location = 'http://' . $_SERVER['HTTP_HOST'].'/blocked?usr='.$row[0];
			return false;
		}
		$fields = array();
		$fields[] = TABLE_TOPICMESSAGES.'.*';
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_USERS.'.'.$users_t->field_school;
		$fields[] = TABLE_PROFILE.'.'.$profile_t->field_picture;
		$query = 'SELECT SQL_CALC_FOUND_ROWS '.implode(',',$fields).' FROM '.TABLE_PROFILE.','.TABLE_USERS
			.','.TABLE_TOPICMESSAGES
			.' WHERE '.TABLE_USERS.'.'.$users_t->field_id.' = '.TABLE_TOPICMESSAGES.'.'.$topicmsg_t->field_userid
			.' AND '.TABLE_USERS.'.'.$users_t->field_id.' = '.TABLE_PROFILE.'.'.$profile_t->field_id
			. ' AND '.TABLE_TOPICMESSAGES.'.'.$topicmsg_t->field_topicid
			.' = '.toSQL($this->storage_array['tpcid']);
		//if only for my college
		if (isset($this->get['mycol'])){
			$query .= ' AND '.TABLE_USERS.'.'.$users_t->field_school." = '".$this->collegeid."'";
		}
		$query .= ' ORDER BY '.TABLE_TOPICMESSAGES.'.'.$topicmsg_t->field_time." DESC LIMIT $start, $limit";
		$result = db::query($query);
		$this->storage_array['totalcount'] = sqlFunc_getCount();
		return $result;
	}
	public function getTopics(){
		if (!isset($this->get['limit']) || !is_numeric($this->get['limit'])
			|| $this->get['limit'] < 2 || $this->get['limit']>99){
			$limit =10;
		}else{
			$limit = $this->get['limit'];
		}
		if (!isset($this->get['page']) || !is_numeric($this->get['page'])
			|| $this->get['page'] < 1){
			$this->get['page'] = 1;
		}
		$start = ($this->get['page']-1)*$limit;
		$this->storage_array['page'] = $this->get['page'];
		$this->storage_array['limit'] = $limit;
		
		$users_t = new table_users();
		$topics_t = new table_topics();
		$topicmsg_t = new table_topicmessages();
		$profile_t = new table_profile;
		$fields = array();
		$fields[] = TABLE_TOPICS.'.*';
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_PROFILE.'.'.$profile_t->field_picture;
		$fields[] = TABLE_USERS.'.'.$users_t->field_school;
		$fields[] = 'count(*) totalcount';
		$fields[] = TABLE_TOPICMESSAGES.'.'.$topicmsg_t->field_topicid.' as messages_check';
		$query = 'SELECT SQL_CALC_FOUND_ROWS '.implode(',',$fields).' FROM '.TABLE_USERS.','.TABLE_TOPICS.','.TABLE_PROFILE.' LEFT JOIN '
			.TABLE_TOPICMESSAGES.' ON '.TABLE_TOPICS.'.'.$topics_t->field_id.' = '
			.TABLE_TOPICMESSAGES.'.'.$topicmsg_t->field_topicid
			.' WHERE '.TABLE_USERS.'.'.$users_t->field_id.' = '.TABLE_TOPICS.'.'.$topics_t->field_userid
			.' AND '.TABLE_USERS.'.'.$users_t->field_id.' = '.TABLE_PROFILE.'.'.$profile_t->field_id;
		//if only for my college
		if (isset($this->get['mycol'])){
			$query .= ' AND '.TABLE_USERS.'.'.$users_t->field_school." = '".$this->collegeid."'";
		}
		$query .= ' GROUP BY '.TABLE_TOPICMESSAGES.'.'.$topicmsg_t->field_topicid." ORDER BY totalcount DESC LIMIT $start, $limit";
		$result = db::query($query);
		$this->storage_array['totalcount'] = sqlFunc_getCount();
		return $result;
	}
	/**given a topic id it will add a message to this topic
	 *NOTE: if topic id doesn't currently exist, this method will add a message that will not be 
	 *linked from anywhere; this of course shouldn't happen unless someone does this on purpose;
	 *so it is best advised that some kind garbage collection checks for not linked messages every now and then*/
	private function newMessage(){
		if (!isset($this->get['tpcid'])){
			$this->setResponse(self::RESPONSE_REDIRECT);
			return false;
		}
		$disc_t = new table_topicmessages();
		//if not yet submited any forms - i.e. acccessing the page first time
		if (!isset($this->post['message'])){
			$tpc_t = new table_topics();
			$query = 'SELECT '.$tpc_t->field_topic.' FROM '.TABLE_TOPICS.' WHERE '.$tpc_t->field_id.' = '
				.toSQL($this->get['tpcid']).' LIMIT 1';
			$result = db::query($query);
			//if nothing found, then it is invalid id -- i.e. redirect
			if (!($row = db::fetch_row($result))){
				$this->setResponse(self::RESPONSE_REDIRECT);
			}else{
				$this->fields['header'] = 'RE:'.$row[0];
				//generate an id for the message id to be inserted
				$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
				$genid = substr($genid, 0, $disc_t->field_messageid_length);
				$this->fields['msgid'] = $genid;
			}
			return false;
		}
		if (!isset($this->post['msgid']) 
			|| strlen($this->post['msgid']) != $disc_t->field_messageid_length){
			$this->setResponse(self::RESPONSE_REDIRECT);
			return false;
		}
		if (empty($this->post['message'])){
			$this->fields = $this->post;
			$this->setError("please enter the message");
			return false;
		}
		$richFormat = new RichFormat($this->post['message']);
		if (!is_null($richFormat->getError())){
			$this->fields = $this->post;
			$this->setError($richFormat->getError());
			return false;
		}
		$fields = array();
		$fields[$disc_t->field_messageid] = $this->post['msgid'];
		$fields[$disc_t->field_message] =  $richFormat->getHtml();
		$fields[$disc_t->field_topicid] = $this->get['tpcid'];
		$fields[$disc_t->field_userid] = $this->userid;
		$query = 'REPLACE INTO '.TABLE_TOPICMESSAGES.' ('.sqlFields(array_keys($fields))
			.',`'.$disc_t->field_time.'`'
			.') values ('.toSQL($fields).',NOW())';
		db::query($query);
		return true;
	}
	/**takes message body and topic header and adds message body to the topic; if topic
	 *doesn't exist it will create a new topic*/
	private function newTopic(){
		$topic_t = new table_topics();
		//if first time accessing the page, create a new id and display form page
		if (!isset($this->post['tpcid']) 
			|| strlen($this->post['tpcid']) != $topic_t->field_id_length){
			$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
			$genid = substr($genid, 0,  $topic_t->field_id_length);
			$this->fields['tpcid'] = $genid;
			//geenerate and message id
			$message_t = new table_topicmessages();
			$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
			$genid = substr($genid, 0,$message_t->field_messageid_length);
			$this->fields['msgid'] = $genid;
			return false;
		}
		if (empty($this->post['message'])){
			$this->fields = $this->post;
			$this->setError("please enter the message");
			return false;
		}
		if (empty($this->post['header'])){
			$this->fields = $this->post;
			$this->setError("please enter the topic header");
			return false;
		}
		$header = trim(strip_tags($this->post['header']));
		//first check wherever this header already exists
		$topic_t = new table_topics();
		$disc_t = new table_topicmessages();
		$query = 'SELECT '.$topic_t->field_id.' FROM '.TABLE_TOPICS.' WHERE STRCMP('
			.$topic_t->field_topic.','.toSQL($header).') = 0 LIMIT 1';
		$result = db::query($query);
		if (!($row = db::fetch_row($result))){
			//topic doesn't yet exist, create a new one
			$fields = array();
			$fields[$topic_t->field_topic] = $header;
			$fields[$topic_t->field_userid] = $this->userid;
			$fields[$topic_t->field_id] = $this->post['tpcid'];
			$query = 'REPLACE INTO '.TABLE_TOPICS.' ('.sqlFields(array_keys($fields)).',`date`'
				.') values ('.toSQL($fields).',CURDATE())';
			db::query($query);
			//use topic id specified by the user for the topic
			$this->get['tpcid'] = $this->post['tpcid'];
		}else{
			//use topic id of the topic previously existing
			$this->get['tpcid'] = $row[0];
		}
		//now insert this message
		return $this->newMessage();
	}
	public function getResponse(){
		return $this->responseCode;
	}
	private function setResponse($code){
		if (!is_null($this->responseCode)){
			return;
		}
		$this->responseCode = $code;
	}
	public function getError(){
		return $this->error;
	}
	private function setError($error){
		if (!is_null($this->error)){
			return;
		}
		$this->error = $error;
	}
}
$control = new discussion_control(AUTH_USERID,AUTH_COLLEGEID,$_GET,$_POST);
$show_scroll_panel = false;
$code = $control->getResponse();
if($code == discussion_control::RESPONSE_MESSAGES){
	$file = 'messages.inc';
	$show_scroll_panel = true;
}elseif($code == discussion_control::RESPONSE_SHOWTOPICS){
	$file = 'topics.inc';
}elseif($code == discussion_control::RESPONSE_NEWMESSAGE){
	$file = 'newmessage.inc';
}elseif($code == discussion_control::RESPONSE_NEWTOPIC){
	$file = 'newtopic.inc';
}else{
	header("Location: ".$control->new_location);
	exit;
}
drawMain_1("Discussions");
drawMain_2(null,$show_scroll_panel);
require_once($_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/discussion/'.$file);
drawMain_3(false);
?>