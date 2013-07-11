<?php
class myprofile{
	public $id;
	function myprofile($id){
		$this->id = $id;
	}
	function updateContactFields(&$form){
		$table = new table_profile();
		//if field wasn't selected, then set it to null
		foreach ($form as $key => $value){
			if (trim($value) == ''){
				$form[$key] = null;
			}
		}
		$parser = new parser($form);
		$query = "UPDATE ".TABLE_PROFILE." SET "
			.$table->field_phone." = ".$parser->getSql($table->field_phone).","
			.$table->field_cellphone." = ".$parser->getSql($table->field_cellphone).","
			.$table->field_mailbox." = ".$parser->getSql($table->field_mailbox).","
			.$table->field_AIM." = ".$parser->getSql($table->field_AIM).","
			.$table->field_address." = ".$parser->getSql($table->field_address).","
			.$table->field_city." = ".$parser->getSql($table->field_city).","
			.$table->field_state." = ".$parser->getSql($table->field_state).","
			.$table->field_zip." = ".$parser->getSql($table->field_zip).","
			.$table->field_lastupdated." = CURDATE()"
			." WHERE ".$table->field_id." = '".$this->id."' LIMIT 1";
		db::query($query);
		//assume success and return null
		return null;
	}
	function getContactFields(){
		$table = new table_profile();
		$query = "SELECT ".$table->field_phone.", "
			.$table->field_cellphone.", ".$table->field_mailbox.", "
			.$table->field_AIM.", ".$table->field_address.", "
			.$table->field_city.", ".$table->field_state.", "
			.$table->field_zip
			." FROM ".TABLE_PROFILE." WHERE ".$table->field_id
			." = '".$this->id."' LIMIT 1";
		$result = db::query($query);
		if (db::num_rows($result) != 1){
			return null;
		}else{
			$fields = db::fetch_assoc($result);
			//set default values for null fields
			foreach($fields as $key => $value){
				if (is_null($value)){
					$fields[$key] = "";
				}
			}
			return $fields;
		}
	}
	public function updateStyles($form){
		$illegal_styles = array();
			$illegal_styles[] = 'z-index';
			$illegal_styles[] = 'display';
			$illegal_styles[] = 'position';
			$illegal_styles[] = 'ACCELERATOR';
			$illegal_styles[] = 'zoom';
		if (empty($form['main_style']) || trim($form['main_style']) == ''){
			$main_style = 'NULL';
		}else{
			$main_style = strip_tags($form['main_style']);
			//replace all invalid properties
			$main_style = preg_replace('/(?:'.implode('|',$illegal_styles).'):[^;}]*/','',$main_style);
			$main_style = toSQL(preg_replace('/[\s]+/',' ',$main_style));
		}
		if (empty($form['prf_style']) || trim($form['prf_style']) == ''){
			$prf_style = 'NULL';
		}else{
			$prf_style = strip_tags($form['prf_style']);
			//replace all invalid properties
			$prf_style = preg_replace('/(?:'.implode('|',$illegal_styles).'):[^;}]*/','',$prf_style);
			$prf_style = toSQL(preg_replace('/[\s]+/',' ',$prf_style));
		}
		$users_t = new table_users();
		$profile_t = new table_profile();
		$query = 'UPDATE '.TABLE_USERS.','.TABLE_PROFILE.' SET '
			.TABLE_USERS.'.'.$users_t->field_style.' = '.$main_style.', '
			.TABLE_PROFILE.'.'.$profile_t->field_style.' = '.$prf_style
			.' WHERE '.TABLE_USERS.'.'.$users_t->field_id." = '".$this->id."' AND "
			.TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_PROFILE.'.'.$profile_t->field_id;
		db::query($query);
		//no error
		return null;
	}
	public function getStyles(){
		$profile_t = new table_profile();
		$users_t = new table_users();
		$query = 'SELECT '.TABLE_USERS.'.'.$users_t->field_style.','
			.TABLE_PROFILE.'.'.$profile_t->field_style.' FROM '
			.TABLE_USERS.','.TABLE_PROFILE.' WHERE '
			.TABLE_USERS.'.'.$users_t->field_id." = '".$this->id."' AND "
			.TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_PROFILE.'.'.$profile_t->field_id.' LIMIT 1';
		$result = db::query($query);
		$row = db::fetch_row($result);
		$fields = array();
		$fields['main_style'] = ((is_null($row[0]))?'':$row[0]);
		$fields['prf_style'] = ((is_null($row[1]))?'':$row[1]);
		return $fields;
	}
	function updateGeneralFields($form){
		$table = new table_profile();
		//if field wasn't selected, then set it to null
		foreach ($form as $key => $value){
			if (trim($value) == ''){
				$form[$key] = null;
			}
		}
		$parser = new parser($form);
		//no check on user input is done
		$query = "UPDATE ".TABLE_PROFILE." SET "
			.$table->field_status." = ".$parser->getSql($table->field_status).","
			.$table->field_major." = ".$parser->getSql($table->field_major).","
			.$table->field_majorsecond." = ".$parser->getSql($table->field_majorsecond).","
			.$table->field_house." = ".$parser->getSql($table->field_house).","
			.$table->field_room." = ".$parser->getSql($table->field_room).","
			.$table->field_year." = ".$parser->getSql($table->field_year).","
			.$table->field_hs." = ".$parser->getSql($table->field_hs).","
			.$table->field_sex." = ".$parser->getSql($table->field_sex).","
			.$table->field_aboutme." = ".$parser->getSql($table->field_aboutme).","
			.$table->field_interests." = ".$parser->getSql($table->field_interests).","
			.$table->field_lastupdated." = CURDATE()"
			." WHERE ".$table->field_id." = '".$this->id."' LIMIT 1";
		db::query($query);
		//assume success and return null
		return null;
	}
	function getGeneralFields(){
		$table = new table_profile();
		$query = "SELECT ".$table->field_status.", "
			.$table->field_major.", ".$table->field_majorsecond.", "
			.$table->field_house.", ".$table->field_room.", "
			.$table->field_year.','.$table->field_hs.','.$table->field_sex.','
			.$table->field_aboutme.','.$table->field_interests
			." FROM ".TABLE_PROFILE." WHERE ".$table->field_id
			." = '".$this->id."' LIMIT 1";
		$result = db::query($query);
		if ($row = db::fetch_assoc($result)){
			//set default values for null fields
			foreach($row as $key => $value){
				if (is_null($value)){
					$row[$key] = "";
				}
			}
			return $row;
		}
		return null;
	}
	/**gets a list of courses using user's id*/
	public function getCourses(){
		$cour_t = new table_mycourses();
		$dep_t = new table_departments();
		$fields = array();
		$fields[] = TABLE_MYCOURSES.'.*';
		$fields[] = TABLE_DEPARTMENTS.'.'.$dep_t->field_description;
		$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_MYCOURSES.','.TABLE_DEPARTMENTS." WHERE "
			.TABLE_MYCOURSES.'.'.$cour_t->field_myid." = '".$this->id."' AND "
			.TABLE_DEPARTMENTS.'.'.$dep_t->field_id.' = '.TABLE_MYCOURSES.'.'.$cour_t->field_department
			.' LIMIT 7';
		$result = db::query($query);
		return $result;
	}
	/**adds course to mycourses replacing any course with same number*/
	function addMyCourse($number,$description,$fieldid){
		//first find out the title of the id of fieldOfStudy specified
		$description = preg_replace('/[^a-z0-9 ]/i','',$description);
		$fieldid = preg_replace('/[^0-9a-z]/i','',$fieldid);
		if (empty($description) || empty($fieldid)){
			return false;
		}
		//now insert data into my courses
		$table = new table_mycourses();
		$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$genid = substr($genid,0,$table->field_id_length);
		$query = "REPLACE INTO ".TABLE_MYCOURSES." ("
			.$table->field_id.","
			.$table->field_myid.","
			.$table->field_number.","
			.$table->field_course.","
			.$table->field_department
			.") VALUES ('".$genid."','".$this->id."','".trim($number)."','"
			.preg_replace('/[ ]+/',' ',trim($description))."',".$fieldid.")";
		$result = db::query($query);
		if ($result){
			$this->updateDate();
			return true;
		}else{
			return false;
		}
	}
	/**deletes course based on its course number and user's id*/
	function deleteMyCourse($course_id){
		$table = new table_mycourses();
		$query = "DELETE FROM ".TABLE_MYCOURSES." WHERE "
			.$table->field_id." = '".$course_id."'"
			." AND "
			.$table->field_myid." = '".$this->id."'"
			." LIMIT 1";
		$result = db::query($query);
		if ($result){
			$this->updateDate();
			return true;
		}else{
			return false;
		}
	}
	/**returns relative picture path*/
	function getPicture(){
		$table = new table_profile();
		$query = "SELECT ".$table->field_picture." FROM ".TABLE_PROFILE
			." WHERE ".$table->field_id." = '".$this->id."' LIMIT 1";
		$result = db::query($query);
		//safe to assume that row exists
		$row = db::fetch_assoc($result);
		$picture = $row[$table->field_picture];
		//now simply construct a path to file (the real pth will be rewritten using apache)
		$picture = funcs_getPictureURL($picture);
		return $picture;
	}
	function setPicture($file_name){
		$table = new table_profile();
		//first see if picture already exists
		$picture_file = $this->getPicture();
		if ($picture_file != funcs_getPictureURL(null)){
			//delete old file from disk
			unlink($_SERVER['DOCUMENT_ROOT'] . $picture_file);
		}
		//insert image url into database
		$query = "UPDATE ".TABLE_PROFILE." SET "
			.$table->field_picture." = '".$file_name."', "
			.$table->field_lastupdated." = CURDATE()"
			." WHERE ".$table->field_id." = '".$this->id."' LIMIT 1";
		$result = db::query($query);
		if ($result){
			return true;
		}else{
			return false;
		}
	}
	/**sends an independent update query to simply update the date 
	  *profile was last updated*/
	function updateDate(){
		$table = new table_profile();
		$query = "UPDATE ".TABLE_PROFILE." SET "
			.$table->field_lastupdated." = CURDATE()"
			." WHERE ".$table->field_id." = '".$this->id."' LIMIT 1";
		$success = db::query($query);
		return $success;
	}
}
?>