<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/trades/trade_interface.inc");
/**encode/decodes item into its appropriate format*/
class itemCoder{
	public $variable;
	public $result;
	function itemCoder($variable,$type){
		$this->variable = $variable;
		if (is_array($variable)){
			$this->encode($type);
		}else{
			$this->decode($type);
		}
	}
	function encode($type){
		$var = $this->variable;
		foreach($var as $key => $value){
			if (trim($value) == ''){
				$var[$key] = 'NULL';
			}else{
				$var[$key] = preg_replace('/:/','>:<',$value);
			}
		}
		if ($type == TYPE_ITEM_TEXTBOOK){
			$face_textbook = new face_textbook();
			$string = $var[$face_textbook->isbn].' : '.$var[$face_textbook->title].' : '
				.$var[$face_textbook->author].' : '.$var[$face_textbook->course].' : '
				.$var[$face_textbook->teacher].' : '.$var[$face_textbook->price].' : '
				.$var[$face_textbook->condition].' : '.$var[$face_textbook->comments];
			$this->result = $string;
		}
		if ($type == TYPE_ITEM_READER){
			$face_reader = new face_reader();
			$string = $var[$face_reader->title].' : '.$var[$face_reader->year].' : '.$var[$face_reader->course]
				.' : '.$var[$face_reader->teacher].' : '.$var[$face_reader->price].' : '
				.$var[$face_reader->condition].' : '.$var[$face_reader->comments].' : '
				.$var[$face_reader->quarter];
			$this->result = $string;
		}
		if ($type == TYPE_ITEM_HOUSING){
			$face_housing = new face_housing();
			$string = $var[$face_housing->title].' : '.$var[$face_housing->location].' : '
				.$var[$face_housing->price].' : '.$var[$face_housing->utils].' : '.$var[$face_housing->sex].' : '
				.$var[$face_housing->arrangement].' : '.$var[$face_housing->more];
			$images = array();
			foreach($var as $key => $value){
				if (preg_match('/^image/',$key)){
					$images[] = $value;
				}
			}
			if (!empty($images)){
				$string .= ' : '.implode(' : ',$images);
			}
			$this->result = $string;
		}
		if ($type == TYPE_ITEM_OTHER){
			$face_other = new face_other();
			$string = $var[$face_other->title].' : '.$var[$face_other->price].' : '.$var[$face_other->more];
			if (isset($var[$face_other->image])){
				$string .= ' : '.$var[$face_other->image];
			}
			$this->result = $string;
		}
	}
	function decode($type){
		$var = $this->variable;
		$array = preg_split('/[\s]:[\s]/',$var);
		foreach ($array as $key => $value){
			if (trim($value) == 'NULL'){
				$array[$key] = null;
			}else{	
				$array[$key] = preg_replace('/>:</',':',trim($value));
			}
		}
		if ($type == TYPE_ITEM_TEXTBOOK){
			$face_textbook = new face_textbook();
			$this->result = array();
			$this->result[$face_textbook->isbn] = $array[0];
			$this->result[$face_textbook->title] = $array[1];
			$this->result[$face_textbook->author] = $array[2];
			$this->result[$face_textbook->course] = $array[3];
			$this->result[$face_textbook->teacher] = $array[4];
			$this->result[$face_textbook->price] = $array[5];
			$this->result[$face_textbook->condition] = $array[6];
			$this->result[$face_textbook->comments] = $array[7];
		}
		if ($type == TYPE_ITEM_READER){
			$face_reader = new face_reader();
			$this->result = array();
			$this->result[$face_reader->title] = $array[0];
			$this->result[$face_reader->year] = $array[1];
			$this->result[$face_reader->course] = $array[2];
			$this->result[$face_reader->teacher] = $array[3];
			$this->result[$face_reader->price] = $array[4];
			$this->result[$face_reader->condition] = $array[5];
			$this->result[$face_reader->comments] = $array[6];
			$this->result[$face_reader->quarter] = $array[7];
		}
		if ($type == TYPE_ITEM_HOUSING){
			$face_housing = new face_housing();
			$this->result = array();
			$this->result[$face_housing->title] = $array[0];
			$this->result[$face_housing->location] = $array[1];
			$this->result[$face_housing->price] = $array[2];
			$this->result[$face_housing->utils] = $array[3];
			$this->result[$face_housing->sex] = $array[4];
			$this->result[$face_housing->arrangement] = $array[5];
			$this->result[$face_housing->more] = $array[6];
			//if there are images
			if (count($array)>7){
				$this->result[$face_housing->image] = array();
				for ($i=7;$i<count($array);$i++){
					$this->result[$face_housing->image][] = $array[$i];
				}
			}
		}
		if ($type == TYPE_ITEM_OTHER){
			$face_other = new face_other();
			$this->result = array();
			$this->result[$face_other->title] = $array[0];
			$this->result[$face_other->price] = $array[1];
			$this->result[$face_other->more] = $array[2];
			//if there is an image
			if (count($array)>3){
				$this->result[$face_other->image] = $array[3];
			}
		}
	}
	function getResult(){
		return $this->result;
	}
}
?>