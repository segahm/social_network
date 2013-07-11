<?php
class ride{
	public $user_id;
	public $school_id;
	function ride($user,$school){
		$this->user_id = $user;
		$this->school_id = $school;
	}
	/*where kind == 1 is looking for a ride, and king == 2 give a ride*/
	function postForm($kind,$form){
		if (empty($form['to']) || empty($form['from']) 
			|| empty($form['day']) || empty($form['month'])
			|| empty($form['year']) || empty($form['time'])
			|| empty($form['number']) || ($kind != 1 && $kind != 2)){
			return false;
		}
		if (!preg_match('/200[5-9]/',$form['year'])
			|| !preg_match('/[0-9]{1,2}/',$form['day'])
			|| !preg_match('/[0-9]{1,2}/',$form['month'])
			|| !is_numeric($form['number'])){
			return false; 
		}
		$ride_t = new table_rideboard();
		$fields[$ride_t->field_to] = $form['to'];
		$fields[$ride_t->field_from] = $form['from'];
		$fields[$ride_t->field_user] = $this->user_id;
		$fields[$ride_t->field_school] = $this->school_id;
		$fields[$ride_t->field_seats] = $form['number'];
		$fields[$ride_t->field_kind] = $kind;
		$fields[$ride_t->field_time] = $form['time'];
		$fields[$ride_t->field_date] = date('Y-m-d',mktime(0,0,0,$form['month'],$form['day'],$form['year']));
		$fields_string = sqlFields(array_keys($fields));
		$values_string = toSQL($fields);
		$query = 'INSERT INTO '.TABLE_RIDEBOARD.' ('.$fields_string
			.') values ('.$values_string .')';
		db::query($query);
		return true;
	}
	function remove($rideid){
		$ride_t = new table_rideboard();
		$query = 'DELETE FROM '.TABLE_RIDEBOARD.' WHERE '.$ride_t->field_uniqueid.' = '.toSQL($rideid)
			.' AND '.$ride_t->field_user." = '".$this->user_id ."' LIMIT 1";
		db::query($query);
	}
	function getRides($start,$limit,$kind){
		$ride_t = new table_rideboard();
		$users_t = new table_users();
		$profile_t = new table_profile();
		$fields_array = array();
		$fields_array[] = TABLE_RIDEBOARD.'.*';
		$fields_array[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields_array[] = TABLE_USERS.'.'.$users_t->field_school;
		$fields_array[] = TABLE_PROFILE.'.'.$profile_t->field_picture;
		$query = "SELECT ".array_tostring($fields_array).' FROM '
			.TABLE_RIDEBOARD.','.TABLE_USERS.','.TABLE_PROFILE.' WHERE '
			.TABLE_USERS.'.'.$users_t->field_id.' = '
			.TABLE_RIDEBOARD.'.'.$ride_t->field_user.' AND '
			.TABLE_PROFILE.'.'.$profile_t->field_id.' = '
			.TABLE_USERS.'.'.$users_t->field_id.' AND '
			.TABLE_RIDEBOARD.'.'.$ride_t->field_kind." = '".$kind."' AND "
			.TABLE_RIDEBOARD.'.'.$ride_t->field_school." = '".$this->school_id."'"
			.' ORDER BY '.TABLE_RIDEBOARD.'.'.$ride_t->field_date
			." LIMIT $start, $limit";
		$result = db::query($query);
		$rows = array();
		while($row = db::fetch_assoc($result)){
			$rows[] = $row;
		}
		return $rows;
	}
}
?>