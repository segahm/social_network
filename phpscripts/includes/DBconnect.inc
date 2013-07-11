<?php
define('DB_CONPOOL_FILENAME',$_SERVER['DOCUMENT_ROOT'] . '/db/conPool.obj');
//require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/connectionPool.inc");
class db{
	private static $link;
	public static function connect(){
		//if already connected, then ignore request
		if (is_null(self::$link)){
			$mysqli_obj = mysqli_connect("localhost","root", "alternblosomka1")
				or die('Error connecting to the database: '.mysqli_connect_error());
			$mysqli_obj->select_db("sitename_database")
				or die("Database Failure! Please Try Again Later<P>".$mysqli_obj->error);
			self::$link =& $mysqli_obj;
			return $mysqli_obj;
		}else{
			return self::$link;
		}
	}
	public static function close(){
		if (!is_null(self::$link)){
			$mysqli_obj = self::$link;
			$mysqli_obj->close();
			self::$link = null;
			return true;
		}
		return false;
	}
	public static function query($query){
		$obj = self::$link;
		$result = $obj->query($query) or 
			($error = ("Query failed\n".$query."\n".$obj->error) 
				and die($error) and log_writeLog($error));
		return $result;
	}
	public static function fetch_row($result){
		return mysqli_fetch_row($result);
	}
	public static function fetch_array($result){
		return mysqli_fetch_array($result);
	}
	public static function fetch_assoc($result){
		return mysqli_fetch_assoc($result);
	}
	public static function num_fields($result){
		return mysqli_num_fields($result);
	}
	public static function num_rows($result){
		return mysqli_num_rows($result);
	}
	public static function affected_rows(){
		return mysqli_affected_rows(self::$link);
	}
	public static function prepare($query){
		return mysqli_prepare(self::$link,$query);
	}
	public static function insert_id(){
		return mysqli_insert_id(self::$link);
	}
}/*
//define filename of where connection pool class is stored
define('DB_CONPOOL_FILENAME',$_SERVER['DOCUMENT_ROOT'] . '/db/conPool.obj');
$CONNECTION_POOL = null;
function db::connect(){
	global $CONNECTION_POOL;
	//if already connected in this script return connection
	if (isset($GLOBALS['DB_CONNECTION'])){
		return $GLOBALS['DB_CONNECTION'][1];
	}
	//if previously created connections - use them
	if (file_exists(DB_CONPOOL_FILENAME)){
		//getting class out of the file
		//$file =& file_get_contents(DB_CONPOOL_FILENAME);
		$file = implode("", @file(DB_CONPOOL_FILENAME));
		$CONNECTION_POOL =& unserialize($file);
		$CONNECTION_POOL->unserialize();
	}else{
		//instantiating initial connections
		$CONNECTION_POOL = new connectionPool("partyHub_database",
			"localhost", "root", "fignamoya", 10, 50);
	}
	//get connection for this script
	$GLOBALS['DB_CONNECTION'] =& $CONNECTION_POOL->getConnection();
	return $GLOBALS['DB_CONNECTION'][1];	//return link
}
function db::close(){
	global $CONNECTION_POOL;
	if (!isset($GLOBALS['DB_CONNECTION'])){
		//nothing to be closed
		return false;
	}
	//get key identifier
	$key = $GLOBALS['DB_CONNECTION'][0];
	$CONNECTION_POOL->closeConnection($key);
	//store class back to file
	$file = fopen(DB_CONPOOL_FILENAME,'w');
	$CONNECTION_POOL->serialize();
	$contents = serialize($CONNECTION_POOL);
	if ($file){
		fwrite($file,$contents);
		fclose($file);
		unset($GLOBALS['DB_CONNECTION']);
		return true;
	}else{
		return false;
	}
}
function db::query($query){
	$link = $GLOBALS['DB_CONNECTION'][1];
	$result = db::query($query,$link) or 
		($error = ("Query failed\n".$query."\n".db::error($link)) 
			and die($error) and log_writeLog($error));
	return $result;
}*/
?>