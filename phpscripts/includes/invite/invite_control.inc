<?php
define('INVITE_FILE',$_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/messages/invite.html');
define('FACEBOOK_INVITE',$_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/messages/facebookinv.html');
class InviteControl{
	private $userid;
	private $get;
	private $post;
	public $response_codes = array();
	const REDIRECT = 'redirect';
	const SENT = 'sent';
	function __construct($userid,$get,$post){
		$this->userid = $userid;
		$this->get = $get;
		$this->post = $post;
		if (isset($get['facebook'])){
			if (isset($get['sbm'])
				&& !empty($post['usr']) && !empty($post['pass'])){
				$this->submitFacebook($post['usr'],$post['pass']);
				$this->response_codes[self::REDIRECT] = '/doc/invite';
			}else{
				$this->response_codes['fields'] = array();
			}
			return;
		}
		if (isset($get['submit'])){
			$code = $this->submit();
			if ($code === true){
				//if we need to redirect somewhere
				if (!empty($post['rdct'])){
					$this->response_codes[self::REDIRECT] = $post['rdct'];
				}else{
					$this->response_codes[self::SENT] = 1;
				}
			}elseif($code === false){
				$this->response_codes[self::REDIRECT] = '/home';
				return;
			}else{
				//error
				$this->response_codes['error'] = $code;
				$this->response_codes['fields'] = $post;
			}
		}
		//if still here then we need to set fields
		$fields = array();
		if (isset($get['rdct'])){
			$fields['rdct'] = $get['rdct'];
		}
		$this->response_codes['fields'] = $fields;
		$my_name = funcs_getNames(AUTH_USER_NAME);
		$search = array('(fname)','(lname)','<<link>>');
		$replace = array($my_name[0],$my_name[1],'http://'.USED_DOMAIN_NAME.'/register');
		$this->response_codes['msg'] = str_replace($search,$replace,file_get_contents(INVITE_FILE));
	}
	private function submitFacebook($usr,$pass){
		$fb_t = new table_facebook();
		if (!empty($this->post['body'])){
			$text = toHTML($this->post['body']);
			$replace = array('/  /','/\n/');
			$replace_with = array(' &nbsp;',' <br>');
			$text = preg_replace($replace,$replace_with,$text);
		}else{
			$text = file_get_contents(FACEBOOK_INVITE);
		}
		$fields = array();
		$fields[$fb_t->field_userid] = $this->userid;
		$fields[$fb_t->field_fb_user] = $usr;
		$fields[$fb_t->field_fb_pass] = $pass;
		$fields[$fb_t->field_message] = $text;
		$query = 'INSERT DELAYED IGNORE INTO '.TABLE_FACEBOOK.' ('
			.sqlFields(array_keys($fields)).',`'.$fb_t->field_time.'`) VALUES '
			.' ('.toSQL($fields).',NOW()) ';
		db::query($query); 
	}
	private function submit(){
		if (empty($this->post['emails']) || trim($this->post['emails']) == ''){
			return 'please enter email address[es] of your friends separated by commas';
		}
		//parse emails
		$email_array = split(',',$this->post['emails']);
		$valid_emails = array();
		$count = 0;
		foreach ($email_array as $email){
			if ($count == 20) break;
			$email = trim($email);
			if (funcs_isValidEmail($email)){
				$count++;
				$valid_emails[] = $email;
			}
		}
		$valid_emails = array_unique($valid_emails);
		if (empty($valid_emails)){
			return 'none of the emails you\'ve entered were found to be valid';
		}
		$inv_t = new table_invite_emails();
		$fields = array();
		$fields[] = $inv_t->field_id;
		$fields[] = $inv_t->field_email;
		$fields[] = $inv_t->field_userid;
		$fields[] = $inv_t->field_time;
		//mark this as read since we are sending emails immediately
		$fields[] = $inv_t->field_isread;
		$query = 'INSERT DELAYED IGNORE INTO '.TABLE_INVITE_EMAILS.' ('
			.sqlFields($fields).') VALUES ';
		$flag = false;
		foreach ($valid_emails as $anemail){
			if ($flag) $query .= ',';
			$flag = true;
			$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
			$genid = substr($genid, 0, $inv_t->field_id_length);
			$query .= "('".$genid."',".toSQL($anemail).",'".$this->userid."',NOW(),1)";
		}
		db::query($query);
		//construct a message
		$my_name = funcs_getNames(AUTH_USER_NAME);
		$subject = 'your friend '.$my_name[0].' '.$my_name[1].' has invited you to join partyHub';
		$message = '';
		if (!empty($this->post['message'])){
			$replace = array('/  /','/\n/');
			$replace_with = array(' &nbsp;',' <br>');
			$this->post['message'] = funcs_cutLength($this->post['message'],253); 
			$message = preg_replace($replace,$replace_with,toHTML($this->post['message'])).'<br>';
		}
		$filename = INVITE_FILE;
		$search = array('(fname)','(lname)','<<link>>');
		$replace = array($my_name[0],$my_name[1],'http://'.USED_DOMAIN_NAME.'/register?invite='.$this->userid);
		$message .= str_replace($search,$replace,file_get_contents($filename));
		myMail(true, $valid_emails, $subject,$message);
		return true;
	}
}
?>
