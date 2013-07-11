<?php
class collegeinfo{
	public $id;
	public $fullname;
	public $emailformat;
	public $shortname;
	public $majors;
	public $houses;
	public $more;
	function collegeinfo($collegeid){
		$this->id = $collegeid;
		$college_table = new table_colleges();
		$query = "SELECT * FROM ".TABLE_COLLEGES
			." WHERE ".$college_table->field_id." = '".$collegeid."' LIMIT 1";
		$result = db::query($query);
		if ($array = db::fetch_array($result)){
			$this->id = $array[$college_table->field_id];
			$this->fullname = $array[$college_table->field_fullname];
			$this->emailformat = $array[$college_table->field_emailformat];
			$this->shortname = $array[$college_table->field_shortname];
			$this->majors = $array[$college_table->field_majors];
			$this->houses = $array[$college_table->field_houses];
			$this->more = $array[$college_table->field_more];
		}
	}
	public function getId(){
		return $this->id;
	}
	public function getFullname(){
		return $this->fullname;
	}
	public function getEmailformat(){
		return $this->emailformat;
	}
	public function getShortname(){
		return $this->shortname;
	}
	public function getHouseList(){
		if (empty($this->houses) || trim($this->houses) === ''){
			return array();
		}
		$list = split(":",$this->houses);
		$newlist = array();
		foreach ($list as $value){
			$temp = split(';',$value);
			$newlist[$temp[0]] = $temp[1];
		}
		return $newlist;
	}
	public function getMajorList(){
		return self::funcs_getAListOf($this->majors);
	}
	/**converts a string (from database) of majors and indexes to array with
		indexes as keys*/
	public static function funcs_getAListOf($string){
		if (empty($string))return array();
		$list = split(":",$string);
		$newlist = array();
		foreach ($list as $value){
			$temp = split(';',$value);
			$newlist[$temp[0]] = $temp[1];
		}
		return $newlist;
	}
}
?>