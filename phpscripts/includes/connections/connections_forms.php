<?php
class manage_groups{
	public $user_id;
	public $post_fields;
	public $error;
	function manage_groups($user_id,$postfields,$getfields){
		$this->user_id = $user_id;
		$this->post_fields = $postfields;
		if (isset($getfields['newgroup'])){
			$this->new_group();
		}
		if (isset($getfields['mfriends'])){
			$this->manage_friends();
		}
		if (isset($getfields['edtgrp'])){
			$this->redo_group();
		}
		if (isset($getfields['removegroup'])){
			$this->removeGroup($getfields['removegroup']);
		}
	}
	function getError(){
		return $this->error;
	}
	function removeGroup($group_id){
		$grp_table = new table_friendgroups();
		$fr_table = new table_friends_in_groups();
		if (strlen($group_id)!=$grp_table->field_groupid_length){
			return false;
		}
		$query = 'DELETE FROM '.TABLE_FRIENDGROUPS.' WHERE '
				.TABLE_FRIENDGROUPS.'.'.$grp_table->field_groupid
				." = ".toSQL($group_id)." AND "
				.TABLE_FRIENDGROUPS.'.'.$grp_table->field_ownerid
				." = '".$this->user_id."'";
		$success1 = db::query($query);
		$query = 'DELETE FROM '.TABLE_FRIENDS_IN_GROUPS.' WHERE '
				.TABLE_FRIENDS_IN_GROUPS.'.'.$fr_table->field_groupid
				." = ".toSQL($group_id)." AND "
				.TABLE_FRIENDS_IN_GROUPS.'.'.$fr_table->field_ownerid
				." = '".$this->user_id."'";
		$success2 = db::query($query);
		return $success1 && $success2;
	}
	function redo_group(){
		if (!empty($this->post_fields['title']) && !empty($this->post_fields['id'])){
			return $this->addGroup($this->post_fields['title'],$this->post_fields['id']);
		}
	}
	function new_group(){
		if (empty($this->post_fields['title'])){
			$this->error = 'Please enter a title for your group!';
			return false;
		}
		//if still here add a new group
		return $this->addGroup($this->post_fields['title']);
	}
	function manage_friends(){
		//process form only if group is set and != 0 (i.e. not empty)
		//and something is entered for users
		if (!empty($this->post_fields['groupadd']) 
				&& !empty($this->post_fields['usersadd'])){
			$id_list = quickAddIn::getListOfIds($this->post_fields['usersadd'],$this->user_id);
			if (!empty($id_list)){
				return connection_control::addToGroup($this->user_id,$this->post_fields['groupadd'],$id_list);
			}
		}
		/*proceed with remove only if different groups are specified for 
		 *add and remove*/
		if (!empty($this->post_fields['groupremove']) 
				&& !empty($this->post_fields['usersremove'])
				&& (empty($this->post_fields['groupadd'])
					|| ($this->post_fields['groupremove'] 
						!= $this->post_fields['groupadd']))){
			$id_list = quickAddIn::getListOfIds($this->post_fields['usersremove'],$this->user_id);
			if (!empty($id_list)){
				return connection_control::removeFromGroup($this->user_id,$this->post_fields['groupremove'],$id_list);
			}
		}
	}
	/**creates a group for the user with the title specified;
	 *if group id is specified, then it will change the title instead*/
	function addGroup($title,$group_id = null){
		$table = new table_friendgroups();
		//disregard certain invalid characters
		$title = preg_replace('/[,]/','',$title);
		if (is_null($group_id)){
			$group_id = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
			$group_id = substr($group_id, 0, $table->field_groupid_length);
		}elseif(strlen($group_id) != $table->field_groupid_length){
			return false;
		}
		if (sqlFunc_getGroups($this->user_id,$title)){
			$this->error = 'You already have a group with this title!';
			return false;
		}
		$query = 'REPLACE INTO '.TABLE_FRIENDGROUPS
			." ($table->field_ownerid,$table->field_groupid,$table->field_title)" 
			." values ('".$this->user_id."',".toSQL($group_id).','
			.toSQL(trim($title)).')';
		$success = db::query($query);
		return $success;
	}
	/*returns an array of groups that looks like this:
	 *group[group_id] = array("title" => group_title, "users" => array("user_id" => user_name,...))*/
	function getGroups(){
		$friendgr_t = new table_friendgroups();
		$users_t = new table_users();
		$fig_t = new table_friends_in_groups();
		$tables = array();
		$tables[] = TABLE_USERS.' as tu';
		$tables[] = TABLE_FRIENDGROUPS.' as tg';
		$tables[] = TABLE_FRIENDS_IN_GROUPS.' as tfg';
		$fields = array();
		$fields[] = 'tu.'.$users_t->field_id;
		$fields[] = 'tu.'.$users_t->field_name;
		$fields[] = 'tg.'.$friendgr_t->field_groupid;
		$fields[] = 'tg.'.$friendgr_t->field_title;
		$query = 'SELECT '.array_tostring($fields).' FROM '
					.array_tostring($tables).' WHERE '
					.'tfg.'.$fig_t->field_ownerid
					." = '".$this->user_id."' AND "
					.'tg.'.$friendgr_t->field_groupid.' = '
					.'tfg.'.$fig_t->field_groupid.' AND '
					.'tfg.'.$fig_t->field_userid
					.' = tu.'.$users_t->field_id;
		$result = db::query($query);
		$groups = array();
		/*iterate through users adding them to appropriate groups*/
		while ($row = db::fetch_assoc($result)){
			if (!isset($groups[$row[$friendgr_t->field_groupid]])){
				//if group is not set yet, create a group with title 
				//and 1 user id
				$groups[(string)$row[$friendgr_t->field_groupid]] 
					= array('title' => $row[$friendgr_t->field_title],
						'users' => 
							array($row[(string)$users_t->field_id] 
									=> $row[$users_t->field_name]));
			}else{
				/*group already exists, so we need to append the user to it*/
				$groups[$row[$friendgr_t->field_groupid]]
						['users']
						[$row[(string)$users_t->field_id]]
					= $row[$users_t->field_name];
			}
		}
		return $groups;
	}
}
?>