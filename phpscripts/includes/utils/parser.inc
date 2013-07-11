<?php
class parser{
	public $form;
	function parser(&$array){
		$this->form = $array;
	}
	function getString($element, $default = null){
		if (isset($this->form[$element]) && !is_null($element) && trim($this->form[$element]) != ''){
			return trim($this->form[$element]);
		}else{
			return $default;
		}
	}
	function getDate($element, $default = null){
		if (isset($this->form[$element]) 
			&& !empty($this->form[$element]) && !ereg('^[0\-]+$',$this->form[$element])){
			return trim($this->form[$element]);
		}else{
			return $default;
		}
	}
	function getHTML($element, $default = ''){
		if (isset($this->form[$element]) && trim($this->form[$element]) != ''){
			return toHTML(trim($this->form[$element]));
		}else{
			return $default;
		}
	}
	function getRadio($element, $value){
		if (isset($this->form[$element]) && $this->form[$element] == $value){
			return 'checked';
		}else{
			return '';
		}
	}
	function getSelect($element, $value){
		if (isset($this->form[$element]) && $this->form[$element] == $value){
			return 'selected';
		}else{
			return '';
		}
	}
	function getSql($element, $default = null){
		return toSQL($this->getString($element,$default));
	}
	function getInt($element, $default = null){
		if (isset($this->form[$element])
			&& is_numeric(trim($this->form[$element]))){
			return (int)trim($this->form[$element]);
		}else{
			return $default;
		}
	}
	function getBoolean($element, $default = null){
		if (isset($this->form[$element])
			&& is_bool(trim($this->form[$element]))){
			return (bool)trim($this->form[$element]);
		}else{
			return $default;
		}
	}
}
?>