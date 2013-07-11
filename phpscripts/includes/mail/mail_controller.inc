<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/utils/richformat.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/trades/itemCoder.inc");
define('MAIL_RESPONSE_REDIRECT','1');
define('MAIL_RESPONSE_SENDFAILED','2');
define('MAIL_RESPONSE_SENDSUCCESS','3');
define('MAIL_RESPONSE_INBOX','4');
define('MAIL_RESPONSE_OUTBOX','5');
define('MAIL_RESPONSE_VIEW','6');
define('MAIL_OUTBOX_TYPE_ID','1');
define('MAIL_OUTBOX_TYPE_GROUP','2');
define('MAIL_OUTBOX_TYPE_SPECIFIED','3');
class mail_controller{
	public $user_id;
	public $college_id;
	public $get_data;
	public $post_data;
	public $response = null;
	public $error = null;
	public $outType = null;
	/**name and school will be used for deirect request to send a message to a specified user*/
	public $name = null;
	public $school = null;
	public $message;
	function mail_controller($user_id,$college_id,$get,$post,$ignore = false){
		//if no processing is needed
		$this->user_id = $user_id;
		$this->college_id = $college_id;
		$this->get_data = $get;
		$this->post_data = $post;
		if ($ignore){
			return;
		}
		//ig to address was somehow specified
		if (isset($this->get_data['id']) 
		 		|| isset($this->get_data['groupid'])
				|| !empty($this->post_data['users'])
				|| !empty($this->post_data['groups'])){
			//if sending a message, then do an actual check for access
			//to this user's messaging service	
			if (isset($this->get_data['send'])){
		   		if (isset($this->get_data['id'])){
					$this->setOutboxType(MAIL_OUTBOX_TYPE_ID);
					//if id of the user is specified, check wherever we can send 
		 			//a message to this user
		 			if (!$this->canAccess($this->get_data['id'])){
		 				$this->setResponse(MAIL_RESPONSE_SENDFAILED);
						$this->error = 'you are prohibited from sending messages to this user';
						return;
					}
					$ids = $this->get_data['id'];
				}elseif(isset($this->get_data['groupid'])){
					$this->setOutboxType(MAIL_OUTBOX_TYPE_GROUP);
					//if this is a group id check that we can send a 
					//message to this group
					if (is_null($ids = $this->getUserIds($this->get_data['groupid']))){
						$this->setResponse(MAIL_RESPONSE_REDIRECT);
						return;
					}
				}else{
					$this->setOutboxType(MAIL_OUTBOX_TYPE_SPECIFIED);
					$ids = array();
					if (!empty($this->post_data['users'])){
						$users = quickAddIn::getListOfIds($this->post_data['users'],$this->user_id);
						$ids = array_merge($ids,$users);
					}
					if (!empty($this->post_data['groups'])){
						$groups = $this->getListOfUsersInGroups($this->post_data['groups']);
						$ids = array_merge($ids,$groups);
					}
				}
				//send message
				if (!empty($ids)){ 
					if ($this->sendMessage($ids)){
						$this->setResponse(MAIL_RESPONSE_SENDSUCCESS);
					}else{
						$this->setResponse(MAIL_RESPONSE_SENDFAILED);
					}
					return;
				}else{
					$this->error = 'please specify to whom will this message be send';
					$this->setResponse(MAIL_RESPONSE_SENDFAILED);
					return;
				}
			}else{
				/*if we are not sending a message yet simply set the name
				 *thus checking that such user or group exist*/
				if (isset($this->get_data['id'])){
					$this->setOutboxType(MAIL_OUTBOX_TYPE_ID);
					$name = $this->getName();
					//user wasn't found
					if (is_null($name)){
						$this->setResponse(MAIL_RESPONSE_REDIRECT);
					}
				}elseif(isset($this->get_data['groupid'])){
					$this->setOutboxType(MAIL_OUTBOX_TYPE_GROUP);
					$name = $this->getName();
					//group wasn't found
					if (is_null($name)){
						$this->setResponse(MAIL_RESPONSE_REDIRECT);
					}
				}else{
					$this->setOutboxType(MAIL_OUTBOX_TYPE_SPECIFIED);
				}
			}
			//else work with inbox data
		}else{
			if(isset($this->get_data['view'])){
				//check that message exists
				if (is_null($this->getMessage())){
					$this->setResponse(MAIL_RESPONSE_REDIRECT);
					return;
				}
				$this->setResponse(MAIL_RESPONSE_VIEW);
			}
			if(isset($this->get_data['delm'])){
				$this->removeMessages($this->get_data['delm']);
				$this->setResponse(MAIL_RESPONSE_INBOX);
				return;
			}elseif(isset($this->get_data['markread'])){
				$this->markAsRead($this->get_data['markread']);
				$this->setResponse(MAIL_RESPONSE_VIEW);
				$this->viewMessage = 'message was marked as read';
				return;
			}elseif(isset($this->get_data['markunread'])){
				$this->markAsUnRead($this->get_data['markunread']);
				$this->setResponse(MAIL_RESPONSE_VIEW);
				$this->viewMessage = 'message was marked as not read';
				return;
			}elseif(isset($this->post_data['action'])){
				//if one of the above actions is specified on the selected 
				//messages then get all message ids and perform this action on
				//them
				$action = $this->post_data['action'];
				$ids = array();
				$i = 1;
				while ($i <= $this->post_data['totalmessages']){
					if (isset($this->post_data['selected'.$i])){
						$ids[] = $this->post_data['selected'.$i];
					}
					$i++;
				}
				if (!empty($ids)){
					if ($action == 'delm'){
						$this->removeMessages($ids);
					}elseif($action == 'markread'){
						$this->markAsRead($ids);
					}elseif($action == 'markunread'){
						$this->markAsUnRead($ids);
					}
				}
				$this->setResponse(MAIL_RESPONSE_INBOX);
				return;
			}
		}
		if(isset($this->get_data['outbox'])){
			$this->setResponse(MAIL_RESPONSE_OUTBOX);
			return;
		}
		//set default response code
		$this->setResponse(MAIL_RESPONSE_INBOX);
	}
	function getFormData(){
		//if sending failed
		if ($this->response == MAIL_RESPONSE_SENDFAILED){
			return $this->post_data;
		}
		/*first create a message id that will be submitted with the form, so that if the user submits the form
		 *multiple times we won't send multiple messages*/
		$message_t = new table_messages();
		$messageid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$messageid = substr($messageid, 0, $message_t->field_messageid_length);
		$fields = array('mid' => $messageid);
		//if user is specified, then may be something else is available
		if ($this->response == MAIL_RESPONSE_OUTBOX
				&& $this->outType == MAIL_OUTBOX_TYPE_ID){
			if (isset($this->get_data['rideid'])){
				$fields['subject'] = 'regarding your posting on the ride board';
				return $fields;
			}
			//if replying and subject & body are set
			if (isset($this->get_data['reply']) && isset($this->post_data['subject']) 
				&& isset($this->post_data['time']) && isset($this->post_data['body'])){
				$fields['subject'] = 'RE: '.$this->post_data['subject'];
				$fields['body'] = "<br>------------------------------------------------------------------------> At "
					.date('g\:i a, n-j-Y',$this->post_data['time'])
					." original message was:\n\n".$this->post_data['body'];
				return $fields;
			}
			//if sending a message regading an item
			if (isset($this->get_data['item']) && isset($this->get_data['type'])){
				$history_t = new table_tradehistory();
				$query = 'SELECT '.$history_t->field_content.' FROM '.TABLE_TRADEHISTORY.' WHERE '
					.$history_t->field_id.' = '.toSQL($this->get_data['item']).' LIMIT 1';
				$result = db::query($query);
				if ($row = db::fetch_row($result)){
					$coder = new itemCoder($row[0],$this->get_data['type']);
					$array = $coder->getResult();
					$face_var = new face_var();
					$fields['subject'] = 'regarding your listing on Trading Space';
					if (isset($array[$face_var->common_title])){
						$fields['body'] = "<p style=\"padding-bottom:3px;margin:0px;\">I'm writting to you with regard to your \""
							.toHTML($array[$face_var->common_title])."\" listing on trading space."
							." I would like to arrange time and place for us to meet. </p><br>How about sometime before: <br>"
							."My buying preferences are: <br>";
					}
					//set message
					$this->error = 'The listing was removed and its owner was successfully notified.';
				}
			}
		}
		return $fields;
	}
	/*gets a name of the user or group*/
	function getName(){
		//if name has already been set
		if (!is_null($this->name) && !is_null($this->school)){
			return array($this->name,$this->school);
		}
		//if send failed then get name and schoolid from the form instead of sending a query to db
		if ($this->response == MAIL_RESPONSE_SENDFAILED
			&& ($this->outType == MAIL_OUTBOX_TYPE_ID
					|| $this->outType == MAIL_OUTBOX_TYPE_GROUP)){
			$this->name = $this->post_data['name'];
			$this->school = $this->post_data['school'];
		}elseif ($this->outType == MAIL_OUTBOX_TYPE_ID){
			$users_t = new table_users();
			$query = 'SELECT '.$users_t->field_name.','.$users_t->field_school.' FROM '.TABLE_USERS
				.' WHERE '.$users_t->field_id." = '".$this->get_data['id']."' LIMIT 1";
			$result = db::query($query);
			if ($row = db::fetch_row($result)){
				$names = funcs_getNames($row[0]);
				$this->name = $names[0].' '.$names[1];
				$this->school = $row[1];
			}else{
				$this->name = null;
				$this->school = null;
			}
		}elseif($this->outType == MAIL_OUTBOX_TYPE_GROUP){
			$groups_t = new table_friendgroups();
			$query = 'SELECT '.$groups_t->field_title.' FROM '.TABLE_FRIENDGROUPS
				.' WHERE '.$groups_t->field_groupid
				." = '".$this->get_data['groupid']."' LIMIT 1";
			$result = db::query($query);
			if ($row = db::fetch_row($result)){
				$this->name = $row[0];
			}else{
				$this->name = null;
			}
		}
		return array($this->name,$this->school);
	}
	function setOutboxType($code){
		if (is_null($this->outType)){
			$this->outType = $code;
			return true;
		}else{
			return false;
		}
	}
	function getOutboxType(){
		return $this->outType;
	}
	function getResponse(){
		return $this->response;
	}
	function setResponse($code){
		if (is_null($this->response) || $code == MAIL_RESPONSE_REDIRECT){
			$this->response = $code;
			return true;
		}else{
			return false;
		}
	}
	/*marks message[s] as read*/
	function markAsRead($ids){
		if (empty($ids)){
			return;
		}
		$message_t = new table_messages();
		$query = 'UPDATE '.TABLE_MESSAGES.' SET `'
			.$message_t->field_isread."` = '1' WHERE `"
			.$message_t->field_to."` = '".$this->user_id."' AND "
			.$message_t->field_messageid.' IN ('.toSQL($ids).')';
		db::query($query);
	}
	/*marks message[s] as unread*/
	function markAsUnRead($ids){
		if (empty($ids)){
			return;
		}
		$message_t = new table_messages();
		$query = 'UPDATE '.TABLE_MESSAGES.' SET `'
			.$message_t->field_isread."` = '0' WHERE `"
			.$message_t->field_to."` = '".$this->user_id."' AND "
			.$message_t->field_messageid.' IN ('.toSQL($ids).')';
		db::query($query);
	}
	/*removes user message[s]*/
	function removeMessages($ids){
		$message_t = new table_messages();
		$query = 'DELETE FROM '.TABLE_MESSAGES.' WHERE `'
			.$message_t->field_messageid.'` IN ('.toSQL($ids).') AND `'
			.$message_t->field_to."` = '".$this->user_id."' LIMIT "
			.count($ids);
		db::query($query);
	}
	/**gets a list of user ids based on the friend's group specified*/
	function getUserIds($groupid){
		$table = new table_friends_in_groups();
		$query = 'SELECT '.$table->userid.' FROM '.TABLE_FRIENDS_IN_GROUPS
			.' WHERE '.$table->groupid.' = '.toSQL($groupid).' AND '
			.$table->ownerid." = '".$this->user_id."'";
		$result = db::query($query);
		$ids = array();
		if ($row = db::fetch_row($result)){
			do{
				$ids[] = $row[0];
			}while($row = db::fetch_row($result));
			return $ids;
		}else{
			return null;
		}
	}
	/*allows access if in the same college or friends and not blocked*/
	function canAccess($id){
		if (sqlFunc_is_blocked($this->user_id,$id)){
			return false;
		}
		if (sqlFunc_isSameCollege($this->college_id,$id)){
			return true;
		}
		if (sqlFunc_isFriends($this->user_id,$id)){
			return true;
		}
		return false;
	}
	private function sendMessage($to,$data = null){
		if (empty($to)){
			return false;
		}
		if (is_null($data)){
			$data =& $this->post_data;
		}
		//make sure that no response codes were sent previously
		if (!is_null($this->response)){
			return false;
		}
		$message_t = new table_messages();
		if (empty($data['mid']) || strlen($data['mid']) != $message_t->field_messageid_length){
			$this->setResponse(MAIL_RESPONSE_REDIRECT);
			return false;
		}else{
			$messageid = $data['mid'];
		}
		$parser = new parser($data);
		if (($subject = $parser->getString('subject',false)) === false){
			$this->error = 'please enter something for the subject of this message';
			return false;
		}
		if (($body = $parser->getString('body',false)) === false){
			$this->error = 'please enter text for the body of this message';
			return false;
		}
		$richFormat = new RichFormat($body);
		if (!is_null($richFormat->getError())){
			$this->error = $richFormat->getError();
			return false;
		}
		//insert data
		$fields = array();
		$fields[$message_t->field_from] = $this->user_id;
		$fields[$message_t->field_subject] = strip_tags($subject);
		$fields[$message_t->field_body] = $richFormat->getHtml();
		$fields[$message_t->field_isread] = '0';	//0 - false, 1 - true
		$fields[$message_t->field_messageid] = null;
		$fields[$message_t->field_to] = null;
		//if many users, then message id will be generated each time
		//hence repeated message are possible
		$query = 'REPLACE DELAYED INTO '.TABLE_MESSAGES.' ('
			.sqlFields(array_keys($fields)).',`'
			.$message_t->field_date.'`) VALUES ';
		if (!is_array($to))$to = array($to);
		$values = array();
		foreach ($to as $person){
			$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
			$genid = substr($genid, 0, $message_t->field_messageid_length);
			$fields[$message_t->field_messageid] = $genid;
			$fields[$message_t->field_to] = $person;
			$values[] = '('.toSQL($fields).",NOW())";
		}
		$query .= implode(',',$values);
		db::query($query);
		return true;
	}
	public function customSend($to,$message,$subject){
		$data = array();
		$message_t = new table_messages();
		$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$genid = substr($genid, 0, $message_t->field_messageid_length);
		$data['mid'] = $genid;
		$data['body'] = $message;
		$data['subject'] = $subject;
		return $this->sendMessage($to,$data);
	}
	/*gets a result of messages*/
	function getMessages(){
		$message_t = new table_messages();
		$users_t = new table_users();
		$query = 'SELECT '.TABLE_MESSAGES.'.*,'
			.TABLE_USERS.'.'.$users_t->field_name.','
			.TABLE_USERS.'.'.$users_t->field_school.' FROM '.TABLE_USERS.','
			.TABLE_MESSAGES.' WHERE '.TABLE_USERS.'.'.$users_t->field_id
			.' = '.TABLE_MESSAGES.'.'.$message_t->field_from.' AND '
			.TABLE_MESSAGES.'.'.$message_t->field_to." = '".$this->user_id."'"
			.' ORDER BY '.TABLE_MESSAGES.'.'.$message_t->field_date.' DESC';
		$result = db::query($query);
		return $result;
	}
	function getMessage(){
		if (!isset($this->get_data['view'])){
			return null;
		}
		if (!is_null($this->message)){
			return $this->message;
		}
		$message_t = new table_messages();
		$users_t = new table_users();
		$query = 'SELECT '.TABLE_MESSAGES.'.*,'
			.TABLE_USERS.'.'.$users_t->field_name.','
			.TABLE_USERS.'.'.$users_t->field_school.' FROM '.TABLE_USERS.','
			.TABLE_MESSAGES.' WHERE '.TABLE_USERS.'.'.$users_t->field_id
			.' = '.TABLE_MESSAGES.'.'.$message_t->field_from.' AND '
			.TABLE_MESSAGES.'.'.$message_t->field_to." = '".$this->user_id."' AND "
			.TABLE_MESSAGES.'.'.$message_t->field_messageid.' = '
			.toSQL($this->get_data['view']).' LIMIT 1';
		$result = db::query($query);
		if (!($row = db::fetch_assoc($result))){
			return null;
		}
		if (isset($this->get_data['read']) && $row[$message_t->field_isread] == '0'){
			//set as read
			$query = 'UPDATE '.TABLE_MESSAGES.' SET '.$message_t->field_isread.' = 1 WHERE '
				.$message_t->field_messageid.' = '.toSQL($this->get_data['view']).' LIMIT 1';
			db::query($query);
		}
		$this->message = $row;
		return $this->message;
	}
	/**works just like quickAddIn's getListOfGroupIds but instead of getting group ids it
	 *get individual user ids belonging to those groups*/
	function getListOfUsersInGroups($groups_in_string){
		$groups= preg_split('/[ ]*,[ ]*/',$groups_in_string);
		if (empty($groups)){
			return array();
		}
		$groups = array_unique($groups);
		$groups_t = new table_friendgroups();
		$ingroups_t = new table_friends_in_groups();
		$groups_string = toSQL($groups);
		//count max # of results found
		$query = 'SELECT '.TABLE_FRIENDS_IN_GROUPS.'.'.$ingroups_t->field_userid
				.' FROM '.TABLE_FRIENDGROUPS.','.TABLE_FRIENDS_IN_GROUPS
				.' WHERE '.TABLE_FRIENDGROUPS.'.'.$groups_t->field_title
				.' IN ('.$groups_string.') AND '
				.TABLE_FRIENDGROUPS.'.'.$groups_t->field_ownerid." = '"
				.$this->user_id."' AND "
				.TABLE_FRIENDGROUPS.'.'.$groups_t->field_groupid.' = '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$ingroups_t->field_groupid;
		$result = db::query($query);
		//buffer all ids into an array
		if ($row = db::fetch_row($result)){
			$ids = array();
			do{
				$ids[] = $row[0];
			}while($row = db::fetch_row($result));
			return $ids;
		}else{
			return null;
		}
	}
}
?>