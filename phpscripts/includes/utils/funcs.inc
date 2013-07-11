<?php
function funcs_fillForm(){
	$contents = ob_get_contents();
	if (empty($GLOBALS['fields'])){
		return $contents;
	}
	//if send failed, then insert post data back
	foreach ($GLOBALS['fields'] as $key => $value){
		$GLOBALS['fields'][$key] = toHTML($value);
	}
	$contents = funcs_processForm($contents,$GLOBALS['fields']);
	return $contents;
}
/**physically resizes image either jpeg/gif to the same location or 
 *optionally to a different destinarion;
 *if it is jpeg file then an optinal quality parameter is used*/
function funcs_resizeImageFile($file,$newWidth,$newHeight,$oldWidth,$oldHeight,$destination = null,$quality = 75){
	//create a new pallete for both source and destination
	$dest = imagecreatetruecolor($newWidth, $newHeight);
	if (preg_match('/\.gif$/',$file)){
		$source = imagecreatefromgif($file);
	}else{
		//assume jpeg since we only allow jpeg and gif
		$source = imagecreatefromjpeg($file);
	}
	//copy the source pallete onto destination pallete
	imagecopyresampled($dest,$source,0,0,0,0, $newWidth, $newHeight, $oldWidth,$oldHeight);
	//save file
	if (preg_match('/\.gif$/',$file)){
		imagegif($dest,((is_null($destination))?$file:$destination));
	}else{
		imagejpeg($dest,((is_null($destination))?$file:$destination),$quality);
	}
}
function funcs_arrangeColumns($tds,$numb_col_per_row,$row = '<tr>'){
	$offset = 0;
	$result = '';
	while($offset < count($tds)){
		$result .= $row;
		$temparray = array_slice($tds,$offset,$numb_col_per_row);
		$result .= implode('',$temparray);
		$times = $numb_col_per_row - count($temparray);
		$i = 0;
		while ($i < $times){
			$result .= '<td></td>';
			$i++;
		}
		$result .= '</tr>';
		$offset += $numb_col_per_row;
	}
	return $result;
}
/**takes sql date and returns a timestamp represented by this date
 *this is simply a shortcur to the class date*/
function funcs_getTimeStamp($date){
	$date = new Mydate($date);
	return $date->getStamp();
}
/**if string is longer than length it will cut it and add ... at the end*/
function funcs_cutLength($string, $length, $word_length = null,$act_length = null){
	$string = (!is_null($string))?trim($string):'';
	//do not check the size of the string if we already know it
	if (is_null($act_length))$act_length=strlen($string);
	if (!is_null($word_length) && !empty($string)){
		//in the future use this instead of calling this whole function
		$string = wordwrap($string,$word_length,"\n",1);
	}
	if (!is_null($length) && $act_length>$length && $length>4){
		$string = substr($string,0,$length-3).'...';
	}
	return $string;
}
/**takes html text as an input and sets values according to the names
 *of the fields, i.e.:
 *for radio and checkboxes it sets checked if 
 *fields[name] == value and for text it sets value = fields[name],
 *for select it sets it to the option that has te same value*/
function funcs_processForm($text,$fields){
	//do not scan the text if the fields are empty
	if (empty($fields)){
		return $text;
	}
	$pos = 0;
	$element = '<';
	//iterate through each element
	while (($pos = strpos($text,$element,$pos)) !== false){
		if (preg_match('/^<input$/i',substr($text,$pos,6))){
			$tag_type = 1;
			$element2 = '>';
		}elseif (preg_match('/^<select$/i',substr($text,$pos,7))){
			$tag_type = 2;
			$element2 = '</select>';
		}elseif (preg_match('/^<textarea$/i',substr($text,$pos,9))){
			$tag_type = 3;
			$element2 = '</textarea>';
		}else{
			$pos += 1;
			continue;
		}
		//keep grabing pieces with bigger size while not full element object
		$size = 50;
		do{
			$grabed_string =& substr($text,$pos,$size);
			$size += 10;
		}while(($secPos = strpos($grabed_string,$element2)) === false);
		$grabed_string = substr($grabed_string,0,$secPos+strlen($element2));
		if ($tag_type == 1){
			//get element's properties
			$type = preg_replace('/^.*type=[\'"]?([^"\'> ]+).*$/is','\\1',$grabed_string);
			$name = preg_replace('/^.*name=[\'"]?([^"\'> ]+).*$/is','\\1',$grabed_string);
			if (preg_match('/^.*value=[\'"]?([^"\'> ]+).*$/i',$grabed_string)){
				$value = preg_replace('/^.*value=[\'"]?([^"\'> ]+).*$/is','\\1',$grabed_string);
			}else{
				$value = null;
			}
			if (($type == 'checkbox' || $type == 'radio')
					&& isset($fields[$name]) && $fields[$name] == $value ){
				//need to select this value
				$replacement = ' checked>';
				$new_string = str_replace('>',$replacement,$grabed_string);
				$text =& substr_replace($text,$new_string,$pos,strlen($grabed_string));
			}elseif(($type == 'text' || $type == 'hidden') && isset($fields[$name])){
				$replacement = ' value="'.$fields[$name].'">';
				$new_string = preg_replace('/value=[^ >]+/','',$grabed_string);
				$new_string = str_replace('>',$replacement,$new_string);
				$text =& substr_replace($text,$new_string,$pos,strlen($grabed_string));
			}
		}elseif($tag_type == 2){
			//select tag
			$names = array();
			preg_match('/name=[\'"]?([^"\'\> ]+)/i',$grabed_string,$names);
			$name = preg_replace('/^.*=[\'"]?(.+)$/i','\\1',$names[0]);
			if (isset($fields[$name])){
				if ($fields[$name] !== ''){
					$new_string = preg_replace('/(<option[^>]+value=[\'"]?'.$fields[$name].'\b[\'"]?[^\/>]*)([\/]?)>/i','\\1 selected\\2>',$grabed_string);
				}else{
					$new_string = preg_replace('/(<option[^>]+value=[\'"][\'"][^\/>]*)([\/]?)>/i','\\1 selected\\2>',$grabed_string);
				}
				$text = substr_replace($text,$new_string,$pos,strlen($grabed_string));
			}
		}elseif($tag_type == 3){
			//textarea tag
			$names = array();
			preg_match('/name=[\'"]?([^"\'\> ]+)/i',$grabed_string,$names);
			$name = preg_replace('/^.*=[\'"]?(.+)$/i','\\1',$names[0]);
			if (isset($fields[$name])){
				$pos1st = strpos($grabed_string,'>');
				$replacement = $fields[$name];
				$length = strlen($grabed_string)-($pos1st+1);
				$new_string = substr_replace($grabed_string,
					$replacement.'</textarea>',$pos1st+1,
					$length);
				$text =& substr_replace($text,$new_string,$pos,strlen($grabed_string));
			}
		}
		$pos += $secPos;
	}
	return $text;
}
function toHTML($variable){
	if (!isset($variable) || is_null($variable)){
		return '';
	}else{
		return htmlspecialchars(trim($variable));
	}
}
/**converts a string into a valid sql string*/
function tosql_string($string,$addslashes = false,$quotes){
	if (empty($string) && (!isset($string) || $string!='0')){
		$string = "NULL";
	}else{
		$string = trim(addslashes($string));
		if ($quotes){
			if ($addslashes){
				$string = "\\'".$string."\\'";
			}else{
				$string = "'".$string."'";
			}
		}
	}
	return $string;
}
/**converts an array to strings separating values with commas*/
function array_tostring($array){
	$string = '';
	foreach($array as $value){
		if ($string != ''){
			$string .= ',';
		}
		$string .= $value;
	}
	return $string;
}
function sqlFields($mixed){
	if (!is_array($mixed)){
		return '`'.$mixed.'`';
	}
	$string = '';
	foreach($mixed as $value){
		if ($string != ''){
			$string .= ',';
		}
		$string .= '`'.$value.'`';
	}
	return $string;
}
/**converts both arrays and single values into sql*/
function toSQL($mixed,$addslashes = false,$quotes = true){
	if (is_null($mixed)){
		return 'NULL';
	}
	if (!is_array($mixed)){
		return tosql_string($mixed,$addslashes,$quotes);
	}
	$string = '';
	foreach($mixed as $value){
		if ($string != ''){
			$string .= ',';
		}
		$string .= tosql_string($value,$addslashes,$quotes);
	}
	return $string;
}
/**returns: boolean
  *checks email format
  */
function funcs_isValidEmail($email){
	if (!isset($email) || (empty($email) && $email!=0) 
			|| !preg_match('/^[a-z0-9_\-\.]+@[a-z0-9\-]+\.[a-z0-9\-\.]+$/i',trim($email))){
		return false;
	}else{
		return true;
	}
}
function funcs_isValidPass($pass){
	if (!isset($pass) || empty($pass) 
			|| !eregi('^[a-zA-Z0-9]+$',trim($pass))
			|| strlen(trim($pass))>10 || strlen(trim($pass))<4){
		return false;
	}else{
		return true;
	}
}
function log_writeLog($message){
	$filename = $_SERVER['DOCUMENT_ROOT'] . "/logs/logs.txt";
	$out = date("F j, Y, g:i a") . ":\n";
	$out .= $message . "\n";
	$write = 'a';
	if (file_exists($filename) && filesize($filename)>(2*1024*1024)){
		$write = 'w';
	}
	@ $fp = fopen($filename,$write);
	if ($fp){
		fwrite($fp,$out,strlen($out));
		fclose($fp);
	}
	//writing output
}
function funcs_imageResize(&$width, &$height, $target, $bywidth = null) { 
	//do not resize if already smaller than the target
	if ($width<=$target && $height<=$target){
		return;
	}
	//takes the larger size of the width and height and applies the  
	//formula accordingly...this is so this script will work  
	//dynamically with any size image 
	if (is_null($bywidth)){
		if ($width > $height) { 
			$percentage = ($target / $width); 
		} else { 
			$percentage = ($target / $height); 
		}
	}else{
		if ($bywidth){
			$percentage = ($target / $width);
			//if height would increase as a result
			if ($height < round($height * $percentage)){
				return;
			} 
		}else{
			$percentage = ($target / $height); 
			//if width would increase as a result
			if ($width < round($width * $percentage)){
				return;
			}
		}
	} 
	//gets the new value and applies the percentage, then rounds the value 
	$width = round($width * $percentage); 
	$height = round($height * $percentage); 
}
function isValidImage($image_file,$max_size){
	if ($image_file['size']>$max_size){
		return 'the file is too big';
	}
	if (empty($image_file['type']) || !preg_match('/jpg|jpeg|gif/i',$image_file['type'])){
		return 'the image must be either jpeg or gif';
	}
	if (!empty($image_file['error'])){
		return 'we were unable to process this file';
	}
	if (!preg_match('/^[^\.]+\.(jpg|jpeg|gif)$/i',$image_file['name'])){
		return 'the image must be either jpeg or gif';
	}
	return true;
}
/**takes a name - the way it is stored in the database and
 *converts it to an array(first name,last name)*/
function funcs_getNames($name,$bothnames = true){
	$last = ucwords(preg_replace('/^(.+), .+/','\\1',$name));
	$first = ucwords(preg_replace('/^.+, (.+)/','\\1',$name));
	if ($bothnames){
		return array($first,$last);
	}else{
		return $first;
	}
}
function funcs_getPictureURL($sqlvalue){
	if (!is_null($sqlvalue)){
		return PROFILE_PICTURE_PATH.$sqlvalue;
	}else{
		return DEFAULT_PROFILE_PICTURE;
	}
}
function printPageNumbers($current_page,$total,$results_per_page,$url,$page_var){
	if ($total == 0){
		return '0';
	}
	//append ? if string query is currently not present
	if (!preg_match('/\?/',$url)){
		$url .= '?';
	}
	$results_per_page = min($total,$results_per_page);
	$string = '';
	if ($current_page>1){
		$string .= '<a href="'.preg_replace('/\&?'.$page_var.'=[^&]+/','',$url)
					.'&'.$page_var.'='.($current_page-1).'"><img border="0" src="/images/left_arrow.gif"></a>';
	}
	$offset = 3;	//how many pages to show
	for ($page=max(1,$current_page-$offset);$page<=min($current_page+$offset,ceil($total/$results_per_page));$page++){
		if ($page == $current_page){
			$string .= '&nbsp;<font color="#000000">'.$page.'</font>';
		}else{
			$string .= '&nbsp;<a href="'
					.preg_replace('/(\&)?'.$page_var.'=[^\&]+/','',$url)
					.'&'.$page_var.'='.$page
					.'" style="text-decoration:underline;color:#003399;">'
					.$page.'</a>';
		}
	}
	if ($current_page<($total/$results_per_page)){
		$string .= '&nbsp;<a href="'.preg_replace('/\&?'.$page_var.'=[^&]+/','',$url)
					.'&'.$page_var.'='.($current_page+1).'"><img border="0" src="/images/right_arrow.gif"></a>';
	}
	return $string;
}
function &array_difKeys($array1,$array2){
	foreach($array1 as $key => $value){
		unset($array2[$key]);
		unset($array1[$key]);
	}
	return array_merge($array1,$array2);
}
function printMatchArrays($array1, $array2 = null,$array3 = null){
	$array_all = array($array1,$array2,$array3);
	$array_string = array();
	$count = 0;
	foreach($array_all as $array){
		if (!is_null($array)){
			$array_string[$count] = 'new Array("';
			if (!empty($array)){
				$array_string[$count] .= implode('","',$array);
			}
			$array_string[$count] .= '")';
			$count++;
		}
	}
	if (empty($array_string)){
		return 'var match_array = null;';
	}else{
		$string = 'var match_array = new Array(';
		$string .= implode(',',$array_string);
		$string .= ');';
		return $string;
	}
}
?>