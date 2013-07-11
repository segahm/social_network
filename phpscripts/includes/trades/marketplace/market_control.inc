<?php
require_once($_SERVER['DOCUMENT_ROOT']
	."/phpscripts/includes/search/searchEngine.php");
define("MARKET_PAGE_REDIRECT",1);
define("MARKET_PAGE_MAIN",2);
define("MARKET_PAGE_ITEM",3);
define("MARKET_PAGE_FEEDBACK_1",4);
define("MARKET_PAGE_FEEDBACK_2",5);
define("MARKET_PAGE_HISTORY",6);
define("MARKET_PAGE_ACTIVITY",7);
define("MARKET_PAGE_MARKETSEARCH",8);
class market_control{
	public $response;
	public $get;
	public $post;
	public $userid;
	public $collegeid;
	public $item;
	public $type;
	public $newlocation;	//for redirect response
	public $error;
	public $resultSet;
	public $data;
	const MAXPURCHASES = 9;
	function market_control($get,$post,$userid,$collegeid){
		//set default redirect path
		$this->newlocation = '/trades/marketplace';
		$this->get = $get;
		$this->post = $post;
		$this->userid = $userid;
		$this->collegeid = $collegeid;
		$items_t = new table_tradeitems();
		if (isset($get['type']) && is_numeric($get['type'])){
			$this->type = $get['type'];
			if (isset($get['item']) && strlen($get['item']) == $items_t->field_id_length
				&& !is_null($this->getItem())){
				$this->setResponse(MARKET_PAGE_ITEM);
				return;
			}
		}elseif(isset($get['categ'])){
			$this->get['type'] = SEARCH_TYPE_MARKET;
			$this->setResponse(MARKET_PAGE_MARKETSEARCH);
			return;
		}
		$item = null;
		if (isset($get['buy']) && isset($get['user']) 
			&& $this->buy($get['buy'],$item)){
			$items_t = new table_tradeitems();
			$this->setResponse(MARKET_PAGE_REDIRECT);
			$this->newlocation = '/message?outbox=1&id='.$get['user'].'&item='.$get['buy'].'&type='.$item[$items_t->field_type];
			return;
		}
		if (isset($get['feedback'])){
			if (isset($get['set']) && isset($get['item'])){
				$response = $this->setFeedback($get['set'],$get['item']);
				if ($response === true){
					$this->setResponse(MARKET_PAGE_FEEDBACK_1);
					return;
				}elseif($response !== false){
					$this->error = $response;
					$this->setResponse(MARKET_PAGE_FEEDBACK_1);
					return;
				}
			}
			if (isset($get['id'])){
				$this->setResponse(MARKET_PAGE_FEEDBACK_2);
				return;			
			}
		}
		if (isset($get['history'])){
			$this->setResponse(MARKET_PAGE_HISTORY);
			return;
		}
		if (isset($get['activity'])){
			$this->setResponse(MARKET_PAGE_ACTIVITY);
			if (isset($get['del'])){
				$this->delItem($get['del']);
			}
			return;
		}
		//default value
		$this->setResponse(MARKET_PAGE_MAIN);
	}
	function searchMarket(){
		$search = new search($this->userid,$this->collegeid,$this->get);
		return $search;
	}
	function delItem($itemid){
		$items_t = new table_tradeitems();
		//first get an item in order to delete the images
		$query = 'SELECT '.$items_t->field_content.','.$items_t->field_type.' FROM '.TABLE_TRADEITEMS
			.' WHERE '.$items_t->field_id.' = '.toSQL($itemid).' AND '
			.$items_t->field_user." = '".$this->userid."' LIMIT 1";
		$result = db::query($query);
		//if nothing was found ingore request
		if (!($row = db::fetch_row($result))){
			return;
		}
		$coder = new itemCoder($row[0],$row[1]);
		$face_var = new face_var();
		$result_array = $coder->getResult();
		if (isset($result_array[$face_var->common_image])){
			if (is_array($result_array[$face_var->common_image])){
				foreach($result_array[$face_var->common_image] as $image){
					$picture_file = $_SERVER['DOCUMENT_ROOT'].ITEM_PICTURE_PATH.$image;
					if (file_exists($picture_file)){
						//delete old file from disk
						unlink($picture_file);
					}
				}
			}else{
				$picture_file = $_SERVER['DOCUMENT_ROOT'].ITEM_PICTURE_PATH.$result_array[$face_var->common_image];
				if (file_exists($picture_file)){
					//delete old file from disk
					unlink($picture_file);
				}
			}
		}
		$query = 'DELETE FROM '.TABLE_TRADEITEMS.' WHERE '.$items_t->field_id.' = '.toSQL($itemid).' AND '
			.$items_t->field_user." = '".$this->userid."' LIMIT 1";
		db::query($query);
	}
	function getHistory($page,$limit){
		$start = ($page-1)*$limit;
		$history_t = new table_tradehistory();
		$users_t = new table_users();
		$query = 'SELECT SQL_CALC_FOUND_ROWS '.TABLE_TRADEHISTORY.'.*,'.TABLE_USERS.'.'.$users_t->field_name.' FROM '
			.TABLE_TRADEHISTORY.','.TABLE_USERS.' WHERE ('.TABLE_TRADEHISTORY.'.'.$history_t->field_seller
			." = '".$this->userid."' AND ".TABLE_TRADEHISTORY.'.'.$history_t->field_buyer.' = '
			.TABLE_USERS.'.'.$users_t->field_id.') OR ('.TABLE_TRADEHISTORY.'.'.$history_t->field_buyer
			." = '".$this->userid."' AND ".TABLE_TRADEHISTORY.'.'.$history_t->field_seller.' = '
			.TABLE_USERS.'.'.$users_t->field_id.') ORDER BY '.TABLE_TRADEHISTORY.'.'.$history_t->field_date
			." DESC LIMIT $start, $limit";
		$result = db::query($query);
		return $result;
	}
	function getSold(){
		$history_t = new table_tradehistory();
		$bids_t = new table_tradebids();
		$users_t = new table_users();
		$feedback_t = new table_tradefeedback();
		$query = 'SELECT '.TABLE_TRADEHISTORY.'.*,'.TABLE_USERS.'.'.$users_t->field_name.','
			.TABLE_TRADEFEEDBACK.'.'.$feedback_t->field_itemid.' userleftfeedback FROM '
			.TABLE_TRADEHISTORY.','.TABLE_USERS.','.TABLE_TRADEBIDS.' LEFT JOIN '.TABLE_TRADEFEEDBACK.' ON '
			.TABLE_TRADEFEEDBACK.'.'.$feedback_t->field_itemid.' = '.TABLE_TRADEBIDS.'.'.$bids_t->field_itemid
			.' AND '.TABLE_TRADEFEEDBACK.'.'.$feedback_t->field_user." = '".$this->userid."' WHERE "
			.TABLE_TRADEBIDS.'.'.$bids_t->field_seller." = '".$this->userid."' AND "
			.TABLE_TRADEBIDS.'.'.$bids_t->field_buyer.' = '.TABLE_USERS.'.'.$users_t->field_id.' AND '
			.TABLE_TRADEHISTORY.'.'.$history_t->field_id.' = '.TABLE_TRADEBIDS.'.'.$bids_t->field_itemid
			.' ORDER BY '.TABLE_TRADEHISTORY.'.'.$bids_t->field_date.' DESC';
		$result = db::query($query);
		return $result;
	}
	function getPurchases(){
		$history_t = new table_tradehistory();
		$feedback_t = new table_tradefeedback();
		$bids_t = new table_tradebids();
		$users_t = new table_users();
		$query = 'SELECT '.TABLE_TRADEHISTORY.'.*,'.TABLE_USERS.'.'.$users_t->field_name.','
			.TABLE_TRADEFEEDBACK.'.'.$feedback_t->field_itemid.' userleftfeedback FROM '
			.TABLE_TRADEHISTORY.','.TABLE_USERS.','.TABLE_TRADEBIDS.' LEFT JOIN '.TABLE_TRADEFEEDBACK.' ON '
			.TABLE_TRADEFEEDBACK.'.'.$feedback_t->field_itemid.' = '.TABLE_TRADEBIDS.'.'.$bids_t->field_itemid
			.' AND '.TABLE_TRADEFEEDBACK.'.'.$feedback_t->field_user." = '".$this->userid."' WHERE "
			.TABLE_TRADEBIDS.'.'.$bids_t->field_buyer." = '".$this->userid."' AND "
			.TABLE_TRADEBIDS.'.'.$bids_t->field_seller.' = '.TABLE_USERS.'.'.$users_t->field_id.' AND '
			.TABLE_TRADEHISTORY.'.'.$history_t->field_id.' = '.TABLE_TRADEBIDS.'.'.$bids_t->field_itemid
			.' ORDER BY '.TABLE_TRADEHISTORY.'.'.$bids_t->field_date.' DESC';
		$result = db::query($query);
		return $result;
	}
	function getPosted(){
		$items_t = new table_tradeitems();
		$query = 'SELECT * FROM '.TABLE_TRADEITEMS.' WHERE '.$items_t->field_user." = '".$this->userid."'";
		$result = db::query($query);
		return $result;
	}
	function viewFeedback(&$data,$page,$limit){
		$user = $this->get['id'];
		$users_t = new table_users();
		$feedback_t = new table_tradefeedback;
		if (strlen($user) != $users_t->field_id_length){
			return false;
		}
		//get user viewed and make sure he is in the same college
		$query = 'SELECT '.$users_t->field_name.','.$users_t->field_feedback.','.$users_t->field_id
			.','.$users_t->field_membersince.' FROM '.TABLE_USERS.' WHERE '
			.$users_t->field_id.' = '.toSQL($user).' AND '
			.$users_t->field_school." = '".$this->collegeid."' LIMIT 1";
		$result = db::query($query);
		if (!($row = db::fetch_assoc($result))){
			//user wasn't found
			return false;
		}
		$data = $row;
		$start = ($page-1)*$limit;
		$this->data = $row;
		$fields = array();
		$fields[] = TABLE_TRADEFEEDBACK.'.*';
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_USERS.'.'.$users_t->field_feedback;
		$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_TRADEFEEDBACK.','.TABLE_USERS.' WHERE '
			.TABLE_USERS.'.'.$users_t->field_id.' = '.TABLE_TRADEFEEDBACK.'.'.$feedback_t->field_user.' AND '
			.TABLE_TRADEFEEDBACK.'.'.$feedback_t->field_owner." = '".$user."' LIMIT $start, $limit";
		$result= db::query($query);
		return $result;
	}
	function setFeedback($user,$itemid){
		$users_t = new table_users();
		$feedback_t = new table_tradefeedback;
		if (strlen($user) != $users_t->field_id_length){
			return false;
		}
		//user has yet to submit any data
		if (!isset($this->post['rate']) && !isset($this->post['text'])){
			return true;
		}
		if (!isset($this->post['rate']) || !is_numeric($this->post['rate'])
			|| $this->post['rate']<(-1) || $this->post['rate']>1){
			return 'please rate your experience with this person';
		}
		$score = $this->post['rate'];
		if (!isset($this->post['text']) || empty($this->post['text'])){
			return 'please describe your experience dealing with this person';
		}
		//make sure the user is allowed to leave feedback for this item and user
		$bids_t = new table_tradebids();
		$query = '(SELECT '.TABLE_USERS.'.'.$users_t->field_feedback.' as field FROM '
			.TABLE_TRADEBIDS.','.TABLE_USERS
			.' WHERE '.TABLE_TRADEBIDS.'.'.$bids_t->field_itemid.' = '.toSQL($itemid).' AND (('
			.TABLE_TRADEBIDS.'.'.$bids_t->field_buyer." = '".$this->userid."' AND "
			.TABLE_TRADEBIDS.'.'.$bids_t->field_seller.' = '.toSQL($user).') OR ('
			.TABLE_TRADEBIDS.'.'.$bids_t->field_buyer.' = '.toSQL($user).' AND '
			.TABLE_TRADEBIDS.'.'.$bids_t->field_seller." = '".$this->userid."')) AND "
			.TABLE_USERS.'.'.$users_t->field_id." = '".$user."'"
			.') UNION (SELECT "2" as field FROM '.TABLE_TRADEFEEDBACK.' WHERE '
			.$feedback_t->field_itemid.' = '.toSQL($itemid).' AND '.$feedback_t->field_user
			." = '".$this->userid."' AND ".$feedback_t->field_owner.' = '.toSQL($user)
			.') UNION (SELECT "3" as field FROM '.TABLE_TRADEFEEDBACK.' WHERE '
			.$feedback_t->field_itemid.' = '.toSQL($itemid).' AND '.$feedback_t->field_user
			.' = '.toSQL($user).' AND '.$feedback_t->field_owner." = '".$this->userid."') LIMIT 3";
		$result = db::query($query);
		$feedback = null;
		$deleteBids = false;
		if ($row = db::fetch_row($result)){
			do{
				if ($row[0] == 2){
					//user already left feedback for this user regarding this item
					return false;
				}elseif($row[0] == 3){
					//the other user already left feedback for this user, so we can delete bids at the end
					$deleteBids = true;
				}else{
					$currentfeedback = $row[0];
				}
			}while($row = db::fetch_row($result));
		}else{
			//there is no transaction between the 2 users or user doesn't exists
			return false;
		}
		$array = explode('/',$currentfeedback);
		switch($score){
			case 1:
				$array[0]++;
				break;
			case 0:
				$array[1]++;
				break;
			case -1:
				$array[2]++;
				break;
		}
		//update score for the user in user's tables
		$query = 'UPDATE '.TABLE_USERS.' SET '.$users_t->field_feedback." = '".implode('/',$array)."'"
			.' WHERE '.$users_t->field_id." = '".$user."' LIMIT 1";
		db::query($query);
		$text = $this->post['text'];
		$fields = array();
		$fields[$feedback_t->field_owner] = $user;
		$fields[$feedback_t->field_user] = $this->userid;
		$fields[$feedback_t->field_itemid] = $itemid;
		$fields[$feedback_t->field_comment] = $text;
		$fields[$feedback_t->field_score] = $score;
		$query = 'INSERT INTO '.TABLE_TRADEFEEDBACK.' ('.sqlFields(array_keys($fields)).',`'
			.$feedback_t->field_date.'`) values ('.toSQL($fields).',CURDATE())';
		db::query($query);
		if ($deleteBids){
			$query = 'DELETE FROM '.TABLE_TRADEBIDS.' WHERE '.$bids_t->field_itemid.' = '
				.toSQL($itemid).' LIMIT 1';
			db::query($query);
		}
		return true;
	}
	function buy($itemid,&$ITEM){
		$items_t = new table_tradeitems();
		if (strlen($itemid) != $items_t->field_id_length){
			return false;
		}
		$users_t = new table_users();
		/*check if buyer has exceeded his maximum purchases and while at it check that the seller and items
		 *exist and that seller is in the same college*/
		$fields = array();
		$fields[] = TABLE_USERS.'.'.$users_t->field_purchases;
		$fields[] = TABLE_USERS.'.'.$users_t->field_name.' as buyername';
		$fields[] = TABLE_TRADEITEMS.'.*';
		$fields[] = 'u2.'.$users_t->field_email;
		$fields[] = 'u2.'.$users_t->field_preferedemail;
		$fields[] = 'u2.'.$users_t->field_name;
		$fields[] = 'u2.'.$users_t->field_id.' as sellerid';
		$query = 'SELECT '.implode(',',$fields).' FROM '.TABLE_USERS.','.TABLE_TRADEITEMS.','.TABLE_USERS
			.' as u2 WHERE '.TABLE_USERS.'.'.$users_t->field_id
			." = '".$this->userid."' AND ".TABLE_TRADEITEMS.'.'.$items_t->field_id.' = '.toSQL($itemid)
			.' AND '.TABLE_TRADEITEMS.'.'.$items_t->field_user.' = u2.'.$users_t->field_id.' AND '
			.'u2.'.$users_t->field_school." = '".$this->collegeid."' LIMIT 1";
		$result = db::query($query);
		//if conditions weren't satisified
		if (!($data = db::fetch_assoc($result))){
			return false;
		}
		//if buying from one's self
		if ($data['sellerid'] == $this->userid){
			return false;
		}
		$purchases = $data[$users_t->field_purchases];
		/*if user already exceeded maximum purchases today then prevent access*/
		if ($purchases >= self::MAXPURCHASES){
			return false;
		}
		//save email addresses to be used to send an email
		$toname = funcs_getNames($data[$users_t->field_name],false);
		$array = funcs_getNames($data['buyername']);
		$buyername = $array[0].' '.$array[1];
		$emails = array();
		$emails[] = $data[$users_t->field_email];
		if (!is_null($data[$users_t->field_preferedemail])){
			$emails[] = $data[$users_t->field_preferedemail];
		}
		$toemail = $data[$users_t->field_name];
		unset($data['buyername']);
		unset($data[$users_t->field_name]);
		unset($data[$users_t->field_email]);
		unset($data[$users_t->field_preferedemail]);
		$ITEM = $data;
		//update purchases count
		$query = 'UPDATE '.TABLE_USERS.' SET '.$users_t->field_purchases
			." = '".($purchases+1)."' LIMIT 1";
		db::query($query);
		//now delete an item
		$query = 'DELETE FROM '.TABLE_TRADEITEMS.' WHERE '
			.$items_t->field_id.' = '.toSQL($itemid).' LIMIT 1';
		db::query($query);
		//now get rid of images
		$coder = new itemCoder($data[$items_t->field_content],$data[$items_t->field_type]);
		$face_var = new face_var();
		$result_array = $coder->getResult();
		if (isset($result_array[$face_var->common_image])){
			if (is_array($result_array[$face_var->common_image])){
				foreach($result_array[$face_var->common_image] as $key => $image){
					$picture_file = $_SERVER['DOCUMENT_ROOT'].ITEM_PICTURE_PATH.$image;
					if (file_exists($picture_file)){
					//delete old file from disk
						unlink($picture_file);
					}
				}
			}else{
				$picture_file = $_SERVER['DOCUMENT_ROOT']
					.ITEM_PICTURE_PATH.$result_array[$face_var->common_image];
					if (file_exists($picture_file)){
					//delete old file from disk
						unlink($picture_file);
					}
			}
		}
		unset($result_array[$face_var->common_image]);
		$coder = new itemCoder($result_array,$data[$items_t->field_type]);
		$data[$items_t->field_content] = $coder->getResult();
		//insert data into history table
		$hist_t = new table_tradehistory();
		$fields = array();
		$fields[$hist_t->field_id] = $data[$items_t->field_id];
		$fields[$hist_t->field_buyer] = $this->userid;
		$fields[$hist_t->field_seller] = $data[$items_t->field_user];
		$fields[$hist_t->field_content] = $data[$items_t->field_content];
		$fields[$hist_t->field_type] = $data[$items_t->field_type];
		$query = 'INSERT INTO '.TABLE_TRADEHISTORY.' ('.sqlFields(array_keys($fields))
			.',`'.$hist_t->field_date.'`) values ('.toSQL($fields).',CURDATE())';
		db::query($query);
		//insert an entry into bits table
		$bids_t = new table_tradebids();
		$fields = array();
		$fields[$bids_t->field_buyer] = $this->userid;
		$fields[$bids_t->field_seller] = $data[$items_t->field_user];
		$fields[$bids_t->field_itemid] = $data[$items_t->field_id];
		$query = 'INSERT INTO '.TABLE_TRADEBIDS.' ('.sqlFields(array_keys($fields))
			.',`'.$bids_t->field_date.'`) values ('.toSQL($fields).',CURDATE())';
		db::query($query);
		
		//send a mail to the seller
		$filename = $_SERVER['DOCUMENT_ROOT'].MAIL_MESSAGES.'itemupdate.txt';
		$message = file_get_contents($filename);
		$message = str_replace('<<name>>',$toname,$message);
		$message = str_replace('<<buyer>>',$buyername,$message);
		$coder = new itemCoder($data[$items_t->field_content],$data[$items_t->field_type]);
		$result_array = $coder->getResult();
		$face_var = new face_var();
		$title = '"'.$result_array[$face_var->common_title].'"';
		$message = str_replace('<<item>>',$title,$message);
		$subject = 'an update about your trade space listing from partyHub';
		myMail(false, $emails, $subject, $message);
		return true;
	}
	function getItem(){
		if (!is_null($this->item)){
			return $this->item;
		}
		$items_t = new table_tradeitems();
		$users_t = new table_users();
		$fields = array();
		$fields[] = TABLE_TRADEITEMS.'.*';
		$fields[] = 'u2.'.$users_t->field_purchases;
		$fields[] = TABLE_USERS.'.'.$users_t->field_name;
		$fields[] = TABLE_USERS.'.'.$users_t->field_feedback;
		$fields[] = TABLE_USERS.'.'.$users_t->field_school;
		if ($this->type == TYPE_ITEM_TEXTBOOK){
			$isbn_t = new table_isbns();
			$face_isbn = new face_isbn();
			$fields[] = TABLE_ISBNS.'.*';
			$query = 'SELECT '.array_tostring($fields).' FROM '.TABLE_TRADEITEMS.','.TABLE_USERS.','
				.TABLE_USERS.' as u2'
				.' LEFT JOIN '.TABLE_ISBNS.' ON '
				.'SUBSTRING_INDEX('.TABLE_TRADEITEMS.'.'.$items_t->field_content.", ' :', 1)"
				.' = '.TABLE_ISBNS.'.'.$isbn_t->field_isbn
				.' WHERE '.TABLE_TRADEITEMS.'.'.$items_t->field_type.' = '.TYPE_ITEM_TEXTBOOK;
		}elseif($this->type == TYPE_ITEM_READER){
			$query = 'SELECT '.array_tostring($fields).' FROM '.TABLE_TRADEITEMS.','.TABLE_USERS
				.','.TABLE_USERS.' as u2'
				.' WHERE '.TABLE_TRADEITEMS.'.'.$items_t->field_type.' = '.TYPE_ITEM_READER;
		}elseif($this->type == TYPE_ITEM_HOUSING){
			$query = 'SELECT '.array_tostring($fields).' FROM '.TABLE_TRADEITEMS.','.TABLE_USERS
				.','.TABLE_USERS.' as u2'
				.' WHERE '.TABLE_TRADEITEMS.'.'.$items_t->field_type.' = '.TYPE_ITEM_HOUSING;
		}elseif($this->type == TYPE_ITEM_OTHER){
			$query = 'SELECT '.array_tostring($fields).' FROM '.TABLE_TRADEITEMS.','.TABLE_USERS
				.','.TABLE_USERS.' as u2'
				.' WHERE '.TABLE_TRADEITEMS.'.'.$items_t->field_type.' = '.TYPE_ITEM_OTHER;
		}else{
			return null;
		}
		$query .= ' AND u2.'.$users_t->field_id." = '".$this->userid."'"
			.' AND '.TABLE_TRADEITEMS.'.'.$items_t->field_id.' = '.toSQL($this->get['item'])
			.' AND '.TABLE_TRADEITEMS.'.'.$items_t->field_user.' = '
			.TABLE_USERS.'.'.$users_t->field_id.' AND '
			.TABLE_TRADEITEMS.'.'.$items_t->field_school." = '".$this->collegeid."' LIMIT 1";
		$result  = db::query($query);
		$face_var = new face_var();
		if ($row = db::fetch_assoc($result)){
			if ($row[$items_t->field_type] == TYPE_ITEM_TEXTBOOK){
				$isbninfo = $this->isbnDecoder($row[$isbn_t->field_text]);
				if (!is_null($row[$isbn_t->field_isbn])){
					$isbninfo[$face_isbn->isbn] = $row[$isbn_t->field_isbn];
				}
				if (!is_null($row[$isbn_t->field_title])){
					$isbninfo[$face_isbn->title] = $row[$isbn_t->field_title];
				}
				if (!is_null($row[$isbn_t->field_author])){
					$isbninfo[$face_isbn->author] = $row[$isbn_t->field_author];
				}
				unset($row[$isbn_t->field_isbn]);
				unset($row[$isbn_t->field_title]);
				unset($row[$isbn_t->field_author]);
				unset($row[$isbn_t->field_text]);
				$row[$face_var->isbninfo] = $isbninfo;
			}
			$itemcoder = new itemCoder($row[$items_t->field_content],$row[$items_t->field_type]);
			$item = $itemcoder->getResult();
			unset($row[$items_t->field_content]);
			$row[$face_var->iteminfo] = $item;
			$this->item = $row;
			return $row;
		}else{
			return null;
		}
	}
	function setResponse($code){
		if (is_null($this->response)){
			$this->response = $code;
		}
	}
	function getResponse(){
		return $this->response;
	}
	function getFeaturedBooks($limit){
		$items_t = new table_tradeitems();
		$isbn_t = new table_isbns();
		$fields = array();
		$fields[] = TABLE_TRADEITEMS.'.*';
		$fields[] = TABLE_ISBNS.'.*';
		$query = 'SELECT '.array_tostring($fields).' FROM '.TABLE_TRADEITEMS.' LEFT JOIN '.TABLE_ISBNS.' ON '
			.'SUBSTRING_INDEX('.TABLE_TRADEITEMS.'.'.$items_t->field_content.", ' :', 1)"
			.' = '.TABLE_ISBNS.'.'.$isbn_t->field_isbn
			.' WHERE '.TABLE_TRADEITEMS.'.'.$items_t->field_type.' = '.TYPE_ITEM_TEXTBOOK.' AND '
			.TABLE_TRADEITEMS.'.'.$items_t->field_school." = '".$this->collegeid."'"
			.' ORDER BY RAND() LIMIT '.$limit;
		$result = db::query($query);
		$results = array();
		$face_var = new face_var();
		$face_isbn = new face_isbn();
		while ($row = db::fetch_assoc($result)){
			$isbninfo = $this->isbnDecoder($row[$isbn_t->field_text]);
			if (!is_null($row[$isbn_t->field_isbn])){
				$isbninfo[$face_isbn->isbn] = $row[$isbn_t->field_isbn];
			}
			if (!is_null($row[$isbn_t->field_title])){
				$isbninfo[$face_isbn->title] = $row[$isbn_t->field_title];
			}
			if (!is_null($row[$isbn_t->field_author])){
				$isbninfo[$face_isbn->author] = $row[$isbn_t->field_author];
			}
			$itemcoder = new itemCoder($row[$items_t->field_content],TYPE_ITEM_TEXTBOOK);
			$book = $itemcoder->getResult();
			
			unset($row[$isbn_t->field_isbn]);
			unset($row[$isbn_t->field_title]);
			unset($row[$isbn_t->field_author]);
			unset($row[$isbn_t->field_text]);
			unset($row[$items_t->field_content]);
			//append arrays to the returning value
			$row[$face_var->iteminfo] = $book;
			$row[$face_var->isbninfo] = $isbninfo;
			$results[] = $row;
		}
		return $results;
	}
	function getFeaturedItems($limit){
		$items_t = new table_tradeitems();
		$query = 'SELECT * FROM '.TABLE_TRADEITEMS.' WHERE '
			.TABLE_TRADEITEMS.'.'.$items_t->field_school." = '".$this->collegeid
			."' ORDER BY RAND() LIMIT $limit";
		$result = db::query($query);
		$list = array();
		$face_var = new face_var();
		while($row = db::fetch_assoc($result)){
			$itemcoder = new itemCoder($row[$items_t->field_content],$row[$items_t->field_type]);
			unset($row[$items_t->field_content]);
			$row[$face_var->iteminfo] = $itemcoder->getResult();
			$list[] = $row;
		}
		return $list;
	}
	function isbnDecoder($string){
		if (empty($string)){
			return array();
		}
		$array = explode('>:<',$string);
		if (empty($array)){
			return array();
		}
		$newArray = array();
		foreach($array as $element){
			$pos = strpos($element,'=>');
			$newArray[substr($element,0,$pos)] = substr($element,$pos+2);
		}
		return $newArray;
	}
}
?>