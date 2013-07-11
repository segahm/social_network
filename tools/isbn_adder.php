<?php
define('MAX_ISBNS_LIMIT',100);
//determine the document root dynamically
$_SERVER['DOCUMENT_ROOT'] = substr(__FILE__,0,strlen(__FILE__)-strlen('/tools/isbn_adder.php'));
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/DBconnect.inc");
log_writeLog('running isbn adder');
db::connect();
//first query database for new isbns
$tisbn_t = new table_tempisbn();
$query = 'SELECT '.$tisbn_t->field_isbn.' FROM '.TABLE_TEMPISBN.' LIMIT '.MAX_ISBNS_LIMIT;
$result = db::query($query);
//combine all isbns gotten into an array
$allisbns = array();
$new_isbns = array();
while ($row = db::fetch_row($result)){
	$allisbns[] = $row[0];
	$new_isbns[$row[0]] = null;
}
if (empty($allisbns)){
	exit;
}
//now check which isbns already exist and unset them from the new_isbns array
$isbn_t = new table_isbns();
$query = 'SELECT '.$isbn_t->field_isbn.' FROM '.TABLE_ISBNS.' WHERE '
	.$isbn_t->field_isbn.' IN ('.toSQL($allisbns).') LIMIT '.MAX_ISBNS_LIMIT;
$result = db::query($query);
while ($row = db::fetch_row($result)){
	unset($new_isbns[$row[0]]);
}
//now for each isbn left issue a request
foreach ($new_isbns as $key => $value){
	issueRequest($key);
	sleep(1);	//make sure that we wait at least one second (although this is 
	//probably not needed since the whole process is slower than this)
}
//now delete all isbns we got at the beginning
$query = 'DELETE QUICK FROM '.TABLE_TEMPISBN.' WHERE '.$tisbn_t->field_isbn.' IN ('
	.toSQL($allisbns).') LIMIT '.MAX_ISBNS_LIMIT;
db::query($query);
$query = 'OPTIMIZE NO_WRITE_TO_BINLOG TABLE '.TABLE_TEMPISBN;
db::query($query);
function issueRequest($isbn){
	$args = array();
	$path = JAVA_PATH.' '.JAVA_CLASSPATH.' sitename.getisbn '.$isbn;
	$result = exec($path,$args);
	//glue all lines just in case output wasn't send in the same line
	$string = implode(' ',$args);
	//now check that it is valid output
	if (!preg_match('/^beg.*end$/',$string)){
		return;
	}
	//remove ^beg.*end$ from the string
	$string = substr($string,3,strlen($string)-6);
	//do not add if it only contains one element (i.e. isbn)
	$elements = explode('>:<',$string);
	//now filter empty values
	array_filter($elements,create_function(
		'$a',
		'return trim($a) != "";'
	));
			//it needs to contain at least title, isbn, author
	if (count($elements)<3){
		exit();
	}
	//get certain fields out of the array that should be isnerted into db separately
	$isbn = null;
	$title = null;
	$author = null;
	foreach ($elements as $key => $value){
		if (!is_null($isbn) && !is_null($title) && !is_null($author)){
			break;
		}
		$matches = array();
		if (preg_match('/isbn=>/',$value,$matches)){
			$isbn = substr($value,6);
			unset($elements[$key]);
		}
		if (preg_match('/title=>/',$value,$matches)){
			$title = substr($value,7);
			unset($elements[$key]);
		}
		if (preg_match('/author=>/',$value,$matches)){
			$author = substr($value,8);
			unset($elements[$key]);
		}
	}
	if (is_null($isbn) || is_null($title) || is_null($author)){
		return;
	}
	$isbn_t = new table_isbns();
	$fields = array();
	$fields[$isbn_t->field_isbn] = $isbn;
	$fields[$isbn_t->field_title] = $title;
	$fields[$isbn_t->field_author] = $author;
	if (!empty($elements)){
		$fields[$isbn_t->field_text] = implode('>:<',$elements);
	}
	$query = 'INSERT DELAYED INTO '.TABLE_ISBNS.' ('.sqlFields(array_keys($fields))
		.') values ('.toSQL($fields).')';
	db::query($query);
}
db::close();
log_writeLog('finished with isbn adder');
?>