<?php
class ToolConfig{
	function __construct(){
	}
	public function getCollegeNames(){
		$col_t = new table_colleges();
		$query = 'SELECT '.$col_t->field_id.','.$col_t->field_fullname.' FROM '.TABLE_COLLEGES;
		return db::query($query); 
	}
	protected function formatValue($str){
		return html_entity_decode(preg_replace('/[\s\n\r]+/',' ',strtolower($str)));
	}
}
?>