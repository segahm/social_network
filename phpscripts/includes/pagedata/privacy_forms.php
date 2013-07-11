<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/permissions.php");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/connections/con_control.inc");
class privacy_forms{
	public $id;	//user's id
	function privacy_forms($id){
		$this->id = $id;
	}
	function &getFields(){
		$utable = new table_users();
		//get permissions of this user
		$query = "SELECT ".$utable->field_permissions." FROM ".TABLE_USERS
				." WHERE ".$utable->field_id." = '".$this->id."' LIMIT 1";
		$result = db::query($query);
		$row = db::fetch_row($result);
		$perm = $row[0];
		$fields = array();
		$fields['perm_prf'] = $perm & Permissions::PERM_PROFILE_ACCESS_PUBLIC;
		$fields['perm_pefsh1'] = $perm & Permissions::PERM_PROFILE_SHOWFRIENDS;
		$fields['perm_pefsh2'] = $perm & Permissions::PERM_PROFILE_SHOWLOGIN;
		$fields['perm_pefsh3'] = $perm & Permissions::PERM_PROFILE_SHOWPROFUPDATED;
		$fields['perm_pefsh4'] = $perm & Permissions::PERM_PROFILE_SHOWEMAIL;
		$fields['perm_ev1'] = $perm & Permissions::PERM_EVENT_PUBLIC;
		$fields['perm_ev2'] = $perm & Permissions::PERM_EVENT_PRIVATE;
		$fields['perm_sv'] = $perm & Permissions::PERM_SAFEVIEW;
		return $fields;
	}
	function submitForm($form){
		$perm = 00000000;
		foreach($form as $key => $mask){
			if (substr($key,0,5) == 'perm_'){
				$perm = $perm | $mask; 
			}
		}
		//now update the table
		$utable = new table_users();
		$query = "UPDATE ".TABLE_USERS." SET "
				.$utable->field_permissions." = '".$perm."'"
				." WHERE ".$utable->field_id." = '".$this->id."' LIMIT 1";
		$success = db::query($query);
		return $success;
	}
	function submitBlock(&$form){
		//first we need to find out which users had been unset
		$keys =& array_keys($form);
		$prev_ids = array_filter($keys,"filterblockedlist");
		$ids_toberemoved = array();
		foreach($prev_ids as $pcount){
			$userid = $form[$pcount];
			//if user had been removed from list
			if (!isset($form[$userid])){
				$ids_toberemoved[] = $userid;
			}
		}
		//if someone had been removed
		if (!empty($ids_toberemoved)){
			$blocked_t = new table_blocklist();
			$query = 'DELETE FROM '.TABLE_BLOCKLIST.' WHERE '
					.$blocked_t->field_blockerid." = '".$this->id."' AND "
					.$blocked_t->field_blockedid.' IN ('
					.toSQL($ids_toberemoved).')';
			$success = db::query($query);
		}
		//now block user[s] entered in emails
		if (!empty($form['emails'])){
			/*limit this to 5 for security reasons we don't want to 
			 *allow to many email to be submitted at once
			 */
			$emailsTemp = split(',',$form['emails'],5);
			$emails = array();
			//now check add valid emails to the list
			foreach($emailsTemp as $key => $value){
				if (funcs_isValidEmail(trim($value)) && !in_array(trim($value), $emails)){
					$emails[$key] = trim($value);
				}
			}
			/*now remove these users from any connections with me 
			 *and put them on blocklist*/
			return $this->blockEmails($emails,5);
		}
		return true;
	}
	/**limit is the max number of users that can be blocked at once*/
	function blockEmails($emails,$limit){
		//make sure that count($email)==$limit
		$list_of_emails = array_slice($emails,0,$limit);
		unset($emails);
		$users_t = new table_users();
		$blocklist_t = new table_blocklist();
		//get ids correspnding to emails
		$query = 'SELECT '.$users_t->field_id.' FROM '.TABLE_USERS
			.' WHERE '.$users_t->field_email.' IN ('
				.toSQL($list_of_emails)
			.") LIMIT ".count($list_of_emails);
		$result = db::query($query);
		$list_of_ids = array();
		while ($row = db::fetch_row($result)){
			//make sure we won't block ourselves
			if ($this->id != $row[0]){
				$list_of_ids[] = $row[0];
			}
		}
		if (empty($list_of_ids)){
			//user[s] were not found
			return false;
		}
		$del_success = connection_control::deleteFriends($this->id,$list_of_ids,count($list_of_ids));
		//assume success in delete
		foreach ($list_of_ids as $idvalue){
			$query = 'REPLACE INTO '.TABLE_BLOCKLIST.' ('
				.$blocklist_t->field_blockerid.','
				.$blocklist_t->field_blockedid.','.$blocklist_t->field_date
				.") values ('".$this->id."','".$idvalue."',CURDATE())";
			$success = db::query($query);
		}
		return true;
	}
	function getBlockedList(){
		$blocked_t = new table_blocklist();
		$users_t = new table_users();
		$fields = array();
		$fields[] = TABLE_USERS.'.'.$users_t->field_id;
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$query = 'SELECT '.array_tostring($fields).' FROM '
			.TABLE_BLOCKLIST.','.TABLE_USERS
			.' WHERE '
			.TABLE_BLOCKLIST.'.'.$blocked_t->field_blockerid
			." ='".$this->id."' AND "
			.TABLE_BLOCKLIST.'.'.$blocked_t->field_blockedid
				.' = '
			.TABLE_USERS.'.'.$users_t->field_id;
		$result = db::query($query);
		$list = array();
		while ($row = db::fetch_row($result)){
			$list[$row[0]] = $row[1];
		}
		return $list;
	}
}
function filterblockedlist($var){
	return eregi('^p[0-9]+$',$var);
}
?>