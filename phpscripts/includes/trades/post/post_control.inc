<?php
define("POST_TEXTBOOKS",'i1');
define("POST_READERS",'i2');
define("POST_HOUSING",'i3');
define("POST_OTHER",'i4');
define('TRADES_POST_PAGE1',0);
define('TRADES_POST_PAGE2',1);
define('TRADES_POST_PAGE3',2);
define('TRADES_POST_REDIRECT',3);
define('MAX_IMAGE_SIZE',522937);	//max image size for all sections
class post_control{
	public $get;
	public $post;
	public $files;
	public $userid;
	public $collegeid;
	public $error;
	public $page;
	public $fields;
	function post_control($get_form,$post_form,$files,$user_id,$college_id){
		$this->get = $get_form;
		$this->post = $post_form;
		$this->files = $files;
		$this->userid = $user_id;
		$this->collegeid = $college_id;
		if (!isset($this->get['item'])){
			$this->post_page = TRADES_POST_PAGE1;
		}elseif ($this->get['item'] == POST_TEXTBOOKS){
			$this->textbooks();
		}elseif($this->get['item'] == POST_READERS){
			$this->readers();
		}elseif($this->get['item'] == POST_HOUSING){
			$this->housing();
		}elseif($this->get['item'] == POST_OTHER){
			$this->other();
		}else{
			$this->post_page = TRADES_POST_REDIRECT;
		}
	}
	function getPage(){
		return $this->page;
	}
	function getError(){
		return $this->error;
	}
	function setError($error){
		if (is_null($this->error)){
			$this->error = $error;
		}
	}
	function other(){
		$items_t = new table_tradeitems();
		if (isset($this->get['edit'])){
			//if editing, then item id can be passed either in get or post
			$id = $this->get['edit'];
			if (strlen($id) != $items_t->field_id_length){
				if (!isset($this->post['id']) || strlen($this->post['id']) != $items_t->field_id_length){
					$this->page = TRADES_POST_REDIRECT;
					return;
				}
				$id = $this->post['id'];
			}
			$query = 'SELECT * FROM '.TABLE_TRADEITEMS.' WHERE '.$items_t->field_id.' = '.toSQL($id).' AND '
				.$items_t->field_user." = '".$this->userid."' AND "
				.$items_t->field_type." = '".TYPE_ITEM_OTHER."' LIMIT 1";
			$result = db::query($query); 
			if ($row = db::fetch_assoc($result)){
				//valid request
				$this->page = TRADES_POST_PAGE2;
				//fill the form with values
				$itemCoder = new itemCoder($row[$items_t->field_content],TYPE_ITEM_OTHER);
				$this->fields = $itemCoder->getResult();
				$this->fields['id'] = $id;
				return;
			}else{
				//if we couldn't find item id under this user then this request is invalid
				$this->page = TRADES_POST_REDIRECT;
				return;
			}
		}else{
			if (empty($this->post['id']) || strlen($this->post['id']) != $items_t->field_id_length){
				$this->page = TRADES_POST_REDIRECT;
				return;
			}
			$fields = array();
			if (!empty($this->files['userfile']['tmp_name'])){
				$file = $this->files['userfile'];
				//check image file for errors
				$response = isValidImage($file,MAX_IMAGE_SIZE);
				if ($response !== true){
					$this->page = TRADES_POST_PAGE1;
					$this->setError($response);
					$this->fields = $this->post;
					return;
				}
				$type = preg_replace('/^[^\.]+.([^\.]+)$/','\\1',$file['name']);
				$type = strtolower($type);
				if ($type == "jpeg" || $type == "jpg"){
					$type = ".jpg";
				}elseif($type == "gif"){
					$type = ".gif";
				}
				$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
				$genid = substr($genid, 0, 5);
				$fields['image'] = $genid.$type;
			}
			
			if (!empty($fields['image']) && !isset($this->post['agree'])){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('you must agree to our terms of use in order to continue');
				$this->fields = $this->post;
				return;
			}
			unset($this->post['agree']);
			$parser = new parser($this->post);
			$fields['title'] = $parser->getString('title',false);
			if ($fields['title'] === false){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please enter the header/title for this posting');
				$this->fields = $this->post;
				return;
			}
			$fields['price'] = $parser->getInt('price',false);
			if ($fields['price'] === false){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please specify the price');
				$this->fields = $this->post;
				return;
			}
			$fields['more'] = $parser->getString('more','');
			//now save image
			if (isset($fields['image'])){
				$uploadfile = $_SERVER['DOCUMENT_ROOT'].ITEM_PICTURE_PATH.$fields['image'];
				$issuccess = move_uploaded_file($this->files['userfile']['tmp_name'], $uploadfile);
				//if we were able to upload the file, then we can add a reference to the database
				if (!$issuccess){
					unset($fields['image']);
				}else{
					if (!chmod($uploadfile,0777))log_writeLog('chmod failed on:'.$uploadfile);
				}
			}
			$coder = new itemCoder($fields,TYPE_ITEM_OTHER);
			$content = $coder->getResult();
			//insert into items table
			$sqlfields = array();
			$sqlfields[$items_t->field_id] = $this->post['id'];
			$sqlfields[$items_t->field_user] = $this->userid;
			$sqlfields[$items_t->field_content] = $content;
			$sqlfields[$items_t->field_type] = TYPE_ITEM_OTHER;
			$this->insertItem($sqlfields);
			//now remove old image if one existed and we are editing
			if (!empty($this->post['image'])){
				$picture_file = $_SERVER['DOCUMENT_ROOT'].ITEM_PICTURE_PATH.$this->post['image'];
				if (file_exists($picture_file)){
					//delete old file from disk
					unlink($picture_file);
				}
			}
			$this->page = TRADES_POST_PAGE3;
		}
	}
	function housing(){
		$items_t = new table_tradeitems();
		$face_housing = new face_housing();
			$fields = array();
			if (empty($this->post['id']) || strlen($this->post['id']) != $items_t->field_id_length){
				$this->page = TRADES_POST_REDIRECT;
				return;
			}
			$fields = array();
			$images = array();
			for ($i=1;$i<=4;$i++){
				if (!empty($this->files['userfile'.$i]['tmp_name'])){
					$file = $this->files['userfile'.$i];
					//begin error check
					$response = isValidImage($file,MAX_IMAGE_SIZE);
					if ($response !== true){
						$this->page = TRADES_POST_PAGE1;
						$this->setError($response);
						$this->fields = $this->post;
						return;
					}
					$type = preg_replace('/^[^\.]+.([^\.]+)$/','\\1',$file['name']);
					$type = strtolower($type);
					if ($type == "jpeg" || $type == "jpg"){
						$type = ".jpg";
					}elseif($type == "gif"){
						$type = ".gif";
					}
					$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
					$genid = substr($genid, 0, 5);
					$images[] = $genid.$type;
				}
			}
			if (!empty($images) && !isset($this->post['agree'])){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('you must agree to our terms of use in order to continue');
				$this->fields = $this->post;
				return;
			}
			unset($this->post['agree']);
			$parser = new parser($this->post);
			$fields[$face_housing->title] = $parser->getString('title',false);
			if ($fields[$face_housing->title] === false){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please enter the header/title for this posting');
				$this->fields = $this->post;
				return;
			}
			$fields[$face_housing->location] = $parser->getString('location',false);
			if ($fields[$face_housing->location] === false){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please specify the address/location');
				$this->fields = $this->post;
				return;
			}
			$fields[$face_housing->price] = $parser->getInt('price',false);
			if ($fields[$face_housing->price] === false){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please specify the price per month');
				$this->fields = $this->post;
				return;
			}
			$fields[$face_housing->sex] = $parser->getString('sex','-1');
			if ($fields[$face_housing->sex] == '-1'){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please specify wherever you are looking for male/femaile/either');
				$this->fields = $this->post;
				return;
			}
			$fields[$face_housing->arrangement] = $parser->getString('arrangement',false);
			if ($fields[$face_housing->arrangement] === false){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please specify the living arrangements');
				$this->fields = $this->post;
				return;
			}
			if (isset($this->post['utils'])){
				$fields[$face_housing->utils] = 1;
			}else{
				$fields[$face_housing->utils] = 0;
			}
			$fields[$face_housing->more] = $parser->getString('more','');
			//if still here, then everything is fine and we can begin saving files
			$count = 1;
			foreach ($images as $image){
				$uploadfile = $_SERVER['DOCUMENT_ROOT'].ITEM_PICTURE_PATH.$image;
				$issuccess = move_uploaded_file($this->files['userfile'.$count]['tmp_name'], $uploadfile);
				//if we were able to upload the file, then we can add a reference to the database
				if ($issuccess){
					$fields['image'.$count] = $image;
				}else{
					if (!chmod($uploadfile,0777))log_writeLog('chmod failed on:'.$uploadfile);
				}
				$count++;
			}
			//post data
			$coder = new itemCoder($fields,TYPE_ITEM_HOUSING);
			$content = $coder->getResult();
			//insert into items table
			$sqlfields = array();
			$sqlfields[$items_t->field_id] = $this->post['id'];
			$sqlfields[$items_t->field_user] = $this->userid;
			$sqlfields[$items_t->field_content] = $content;
			$sqlfields[$items_t->field_type] = TYPE_ITEM_HOUSING;
			$this->insertItem($sqlfields);
			$this->page = TRADES_POST_PAGE3;
	}
	function readers(){
		$face_reader = new face_reader();
		if (isset($this->get['step']) && $this->get['step'] == '1'){
			//generate id for the form to be tracked by
			$items_t = new table_tradeitems();
			$id = str_shuffle('123456789');
			$id = substr($id, 0, $items_t->field_id_length);
			$this->fields = array();
			$this->fields['id'] = $id;
		}
		if (!isset($this->get['edit'])){
			//do a general check of all the fields
			$parser = new parser($this->post);
			$this->fields[$face_reader->title] = $parser->getString('title',false);
			if ($this->fields[$face_reader->title] === false){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please enter the title');
				$this->fields = $this->post;
				return;
			}
			$this->fields[$face_reader->year] = $parser->getInt('year','-1');
			if ($this->fields[$face_reader->year] == '-1'){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please select the year this reader was published/used');
				$this->fields = $this->post;
				return;
			}
			$this->fields[$face_reader->teacher] = $parser->getString('teacher',false);
			if ($this->fields[$face_reader->teacher] === false){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please enter the names of professors who use this reader');
				$this->fields = $this->post;
				return;
			}
			$this->fields[$face_reader->quarter] = $parser->getString('quarter','-1');
			if ($this->fields[$face_reader->quarter] == '-1'){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please choose the quarter you used this reader');
				$this->fields = $this->post;
				return;
			}
			$this->fields[$face_reader->condition] = $parser->getString('condition','-1');
			if ($this->fields[$face_reader->condition] == '-1'){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please specify the appropriate conditon of this reader');
				$this->fields = $this->post;
				return;
			}
			$this->fields[$face_reader->price] = $parser->getInt('price',false);
			if ($this->fields[$face_reader->price] === false){
				$this->page = TRADES_POST_PAGE1;
				$this->setError('please specify the price you wish to sell this reader for');
				$this->fields = $this->post;
				return;
			}
		}
		if (isset($this->get['step']) && $this->get['step'] == '1'){
			$this->page = TRADES_POST_PAGE2;
			return;
		}elseif(isset($this->get['step']) && $this->get['step'] == 'course'){
			//first of all return all submited fields
			$this->fields = $this->post;
			//now append course field if one found
			if (isset($this->post['department']) && isset($this->post['coursenumb'])
				&& $this->post['department'] != -1){
				$found = sqlFunc_getCourseByNumb($this->collegeid,$this->post['department'],$this->post['coursenumb']);
				if (!is_null($found)){
					$table = new table_courses();
					$this->fields['course'] = $found[$table->field_description];
				}
			}
			$this->page = TRADES_POST_PAGE2;
			return;
		}elseif(isset($this->get['step']) && $this->get['step'] == 2){
			$this->fields[$face_reader->course] = $parser->getString('course',false);
			if ($this->fields[$face_reader->course] === false){
				$this->setError('please enter the title of the course this book was used for');
				$this->page = TRADES_POST_PAGE2;
				$this->fields = $this->post;
				return;
			}
			$this->fields[$face_reader->comments] = $parser->getString('comments','');
			$coder = new itemCoder($this->fields,TYPE_ITEM_READER);
			$content = $coder->getResult();
			$items_t = new table_tradeitems();
			//get the hidden id from the form
			$id = $parser->getString('id',false);
			if ($id === false){
				$this->page = TRADES_POST_REDIRECT;
				return;
			}
			//insert into items table
			$sqlfields = array();
			$sqlfields[$items_t->field_id] = $id;
			$sqlfields[$items_t->field_user] = $this->userid;
			$sqlfields[$items_t->field_content] = $content;
			$sqlfields[$items_t->field_type] = TYPE_ITEM_READER;
			$this->insertItem($sqlfields);
			$this->page = TRADES_POST_PAGE3;
			$this->fields = $this->post;
		}elseif(isset($this->get['edit'])){
			//if editing, then item id can be passed either in get or post
			$items_t = new table_tradeitems();
			$id = $this->get['edit'];
			if (strlen($id) != $items_t->field_id_length){
				if (!isset($this->post['id']) || strlen($this->post['id']) != $items_t->field_id_length){
					$this->page = TRADES_POST_REDIRECT;
					return;
				}
				$id = $this->post['id'];
			}
			$query = 'SELECT * FROM '.TABLE_TRADEITEMS.' WHERE '.$items_t->field_id.' = '.toSQL($id).' AND '
				.$items_t->field_user." = '".$this->userid."' AND "
				.$items_t->field_type." = '".TYPE_ITEM_READER."' LIMIT 1";
			$result = db::query($query); 
			if ($row = db::fetch_assoc($result)){
				//valid request
				$this->page = TRADES_POST_PAGE2;
				//fill the form with values
				$itemCoder = new itemCoder($row[$items_t->field_content],TYPE_ITEM_READER);
				$this->fields = $itemCoder->getResult();
				$this->fields['id'] = $id;
				return;
			}else{
				//if we couldn't find item id under this user then this request is invalid
				$this->page = TRADES_POST_REDIRECT;
				return;
			}
		}
	}
	function textbooks(){
		if (isset($this->get['step']) && $this->get['step'] == '1'){
			/*generate a new item id to be submitted with the form every time, so that if the user submit the form
			 *multiple times, we always insert it into one location*/
			$items_t = new table_tradeitems();
			$id = str_shuffle('123456789');
			$id = substr($id, 0, $items_t->field_id_length);
			$this->fields  = array();
			$this->fields['id'] = $id;
			if (!isset($this->post['isisbn'])){
				$this->page = TRADES_POST_REDIRECT;
				return;
			}elseif ($this->post['isisbn'] == 1){
				//if isbn field was chosen in preference of the title, check that it is not empty
				if (empty($this->post['isbn'])){
					$this->page = TRADES_POST_PAGE1;
					$this->setError('please enter the correct isbn number');
					$this->fields = $this->post;
					return;
				}
				$isbn = $this->post['isbn'];
				$isbn = preg_replace('/[^a-z0-9]/i','',$isbn);
				if (empty($isbn) || (strlen($isbn) != 10 && strlen($isbn) != 13)){
					$this->page = TRADES_POST_PAGE1;
					$this->setError("please enter the correct isbn number");
					$this->fields = $this->post;
					return;
				}
				//else perform a search based on isbn
				$isbn_t = new table_isbns();
				$fields = array();
				$fields[] = $isbn_t->field_title;
				$fields[] = $isbn_t->field_author;
				
				$query = 'SELECT '.array_tostring($fields).' FROM '.TABLE_ISBNS.' WHERE '
					.$isbn_t->field_isbn.' = '.toSQL($isbn).' LIMIT 1';
				$result = db::query($query);
				$this->fields['isbn'] = $isbn;
				if ($row = db::fetch_row($result)){
					$this->fields['title'] = $row[0];
					$this->fields['author'] = $row[1];
				}
			}else{
				if (empty($this->post['title'])){
					$this->page = TRADES_POST_PAGE1;
					$this->setError('please enter the title of this book');
					$this->fields = $this->post;
					return;
				}
				$title = trim($this->post['title']);
				if (empty($this->post['title'])){
					$this->page = TRADES_POST_PAGE1;
					$this->setError('please enter the title of this book');
					$this->fields = $this->post;
					return;
				}
				$title = trim($this->post['title']);
				//perform a search based on the title
				$isbn_t = new table_isbns();
				$fields = array();
				$fields[] = $isbn_t->field_title;
				$fields[] = $isbn_t->field_isbn;
				$fields[] = $isbn_t->field_author;
				
				$query = 'SELECT '.sqlFields($fields).' FROM '.TABLE_ISBNS.' WHERE '
					.$isbn_t->field_title." LIKE '%".toSQL($title,false,false)."%'  LIMIT 1";
				$result = db::query($query);
				$this->fields['title'] = $title;
				if ($row = db::fetch_row($result)){
					$this->fields['title'] = $row[0];
					$this->fields['isbn'] = $row[1];
					$this->fields['author'] = $row[2];
				}
			}
			//if still here set appropriate page
			$this->page = TRADES_POST_PAGE2;
			return;
		}elseif(isset($this->get['step']) && $this->get['step'] == 'course'){
			$this->fields = $this->post;
			if (isset($this->post['department']) && isset($this->post['coursenumb'])
				&& $this->post['department'] != -1){
				$found = sqlFunc_getCourseByNumb($this->collegeid,$this->post['department'],$this->post['coursenumb']);
				if (!is_null($found)){
					$table = new table_courses();
					$this->fields['course'] = $found[$table->field_description];
				}
			}
			$this->page = TRADES_POST_PAGE2;
			return;
		}elseif(isset($this->get['step']) && $this->get['step'] == 2){
			$parser = new parser($this->post);
			$face_textbook = new face_textbook();
			$fields = array();
			$fields[$face_textbook->isbn] = $parser->getString('isbn','');
			//if isbn number was actually entered
			if ($fields[$face_textbook->isbn] != ''){
				$fields[$face_textbook->isbn] = preg_replace('/[^a-z0-9]/i','',$fields[$face_textbook->isbn]);
				if (strlen($fields[$face_textbook->isbn]) != 10 && strlen($fields[$face_textbook->isbn]) != 13){
					$this->setError('please enter the correct isbn number');
					$this->page = TRADES_POST_PAGE2;
					$this->fields = $this->post;
					return;
				}
			}
			$fields[$face_textbook->title] = $parser->getString('title',false);
			if ($fields[$face_textbook->title] === false){
				$this->setError('please enter the title of this book');
				$this->page = TRADES_POST_PAGE2;
				$this->fields = $this->post;
				return;
			}
			$fields[$face_textbook->author] = $parser->getString('author',false);
			if ($fields[$face_textbook->author] === false){
				$this->setError('please enter the author of this book');
				$this->page = TRADES_POST_PAGE2;
				$this->fields = $this->post;
				return;
			}
			$fields[$face_textbook->course] = $parser->getString('course',false);
			if ($fields[$face_textbook->course] === false){
				$this->setError('please enter the title of the course this book was used for');
				$this->page = TRADES_POST_PAGE2;
				$this->fields = $this->post;
				return;
			}
			$fields[$face_textbook->teacher] = $parser->getString('teacher',false);
			if ($fields[$face_textbook->teacher] === false){
				$this->setError('please enter the name[s] of professor[s] who taught this course');
				$this->page = TRADES_POST_PAGE2;
				$this->fields = $this->post;
				return;
			}
			$fields[$face_textbook->price] = $parser->getInt('price',false);
			if ($fields[$face_textbook->price] === false){
				$this->setError('please enter the price you wish to sell this book for');
				$this->page = TRADES_POST_PAGE2;
				$this->fields = $this->post;
				return;
			}
			$fields[$face_textbook->condition] = $parser->getString('condition','-1');
			if ($fields[$face_textbook->condition] == '-1'){
				$this->setError('please choose the condition of your used textbook');
				$this->page = TRADES_POST_PAGE2;
				$this->fields = $this->post;
				return;
			}
			$fields[$face_textbook->comments] = $parser->getString('comments','');
			$coder = new itemCoder($fields,TYPE_ITEM_TEXTBOOK);
			$content = $coder->getResult();
			$items_t = new table_tradeitems();
			//get the hidden id from the form
			$id = $parser->getString('id',false);
			if ($id === false){
				$this->page = TRADES_POST_REDIRECT;
				return;
			}
			//insert into items table
			$sqlfields = array();
			$sqlfields[$items_t->field_id] = $id;
			$sqlfields[$items_t->field_user] = $this->userid;
			$sqlfields[$items_t->field_content] = $content;
			$sqlfields[$items_t->field_type] = TYPE_ITEM_TEXTBOOK;
			$this->insertItem($sqlfields);
			$this->isbn_store($fields['isbn']);
			$this->page = TRADES_POST_PAGE3;
			$this->fields = $this->post;
		}elseif(isset($this->get['edit'])){
			//if editing, then item id can be passed either in get or post
			$items_t = new table_tradeitems();
			$id = $this->get['edit'];
			if (strlen($id) != $items_t->field_id_length){
				if (!isset($this->post['id']) || strlen($this->post['id']) != $items_t->field_id_length){
					$this->page = TRADES_POST_REDIRECT;
					return;
				}
				$id = $this->post['id'];
			}
			$query = 'SELECT * FROM '.TABLE_TRADEITEMS.' WHERE '.$items_t->field_id.' = '.toSQL($id).' AND '
				.$items_t->field_user." = '".$this->userid."' AND "
				.$items_t->field_type." = '".TYPE_ITEM_TEXTBOOK."' LIMIT 1";
			$result = db::query($query); 
			if ($row = db::fetch_assoc($result)){
				//valid request
				$this->page = TRADES_POST_PAGE2;
				//fill the form with values
				$itemCoder = new itemCoder($row[$items_t->field_content],TYPE_ITEM_TEXTBOOK);
				$this->fields = $itemCoder->getResult();
				$this->fields['id'] = $id;
				return;
			}else{
				//if we couldn't find item id under this user then this request is invalid
				$this->page = TRADES_POST_REDIRECT;
				return;
			}
		}
	}
	function insertItem($sqlfields){
		$items_t = new table_tradeitems();
		$query = 'REPLACE DELAYED INTO '.TABLE_TRADEITEMS
				.' ('.sqlFields(array_keys($sqlfields)).',`'.$items_t->field_date.'`,school) '
				.' values ('.toSQL($sqlfields).",CURDATE(),'$this->collegeid')";
		db::query($query);
	}
	/**checks if isbn has been previously stored in isbns table and if not then it will make
	 *a request to java application for isbn info; afterwards it will check that isbn is not the 
	 *only thing in the response and will insert everything in isbns*/
	function isbn_store($isbn){
		if (empty($isbn) || (strlen($isbn) != 10 && strlen($isbn) != 13)){
			return;
		}
		//insert isbn for future amazon data retrieval
		$isbn_t = new table_tempisbn();
		$query = 'INSERT DELAYED IGNORE INTO '.TABLE_TEMPISBN.' ('.$isbn_t->field_isbn.') '
			.' VALUES ('.toSQL($isbn).')';
		db::query($query);
	}
	function getFields(){
		return $this->fields;
	}
}
?>