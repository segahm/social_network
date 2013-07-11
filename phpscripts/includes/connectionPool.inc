<?php
class connectionPool{
	var $url;
	var $username;
	var $password;
	var $database;
	var $init_con;
	var $max_con;
	var $wait_if_busy;
	var $connectionPending = false;
	//arrays used for storing connections
	var $available_con, $busy_con;
	function connectionPool($database, $url, $username, 
							$password, $initialConnections,
							$maxConnections,$waitIfBusy = true){
		$this->url = $url;
		$this->username = $username;
		$this->password = $password;
		$this->database = $database;
		$this->wait_if_busy = $waitIfBusy;
		$this->max_con = $maxConnections;
		if ($initialConnections > $maxConnections){
			$initialConnections = $maxConnections;
		}
		$this->init_con = $initialConnections;
		//creating initial # of connections
		$this->available_con = array();
		$this->busy_con = array();
		for ($i=0;$i<$initialConnections; $i++) {
			$this->available_con[] =& $this->makeNewConnection();
		}
	}
	function getConnection(){
		$link = array_pop($this->available_con);
		if (!is_null($link)){
			$genid = str_shuffle('123456789abcdefghijklmnop');
			$genid = substr($genid,0,10);
			$this->busy_con[$genid] = $link;
		}else{
			if (count($this->busy_con)<$this->max_con){
				//maximum limit is not reached, hence create a new connection
				$link =& $this->makeNewConnection();
				$genid = str_shuffle('123456789abcdefghijklmnop');
				$genid = substr($genid,0,10);
				$this->busy_con[$genid] =& $link;
			}else{
				//wait awhile and then try again
				usleep(100);
				$this->getConnection();
			}
		}
		return array($genid, $link);
	}
	function closeConnection($genid){
		if (!isset($this->busy_con[$genid])){
			return false;
		}
		//free busy connection
		$this->available_con[] = $this->busy_con[$genid];
		unset($this->busy_con[$genid]);
		return true;
	} 
	function &makeNewConnection(){
		$mysqli_obj = mysqli_connect("localhost","root", "fignamoya")
			or die('Error connecting to the database: '.$mysqli_obj->connect_error ());
		$mysqli_obj->select_db($this->database)
			or die("Database Failure! Please Try Again Later<P>".$mysqli_obj->error());
		return $mysqli_obj;
	}
	function serialize(){
		foreach($this->available_con as $key => $link){
			$this->available_con[$key] = serialize($link);
		}
		foreach($this->busy_con as $key => $link){
			$this->busy_con[$key] = serialize($link);
		}
		//$this->available_con = serialize($this->available_con);
		//$this->busy_con = serialize($this->busy_con);
	}
	function unserialize(){
		//$this->available_con = unserialize($this->available_con);
		//$this->busy_con = unserialize($this->busy_con);
		foreach($this->available_con as $key => $link){
			$this->available_con[$key] = unserialize($link);
		}
		foreach($this->busy_con as $key => $link){
			$this->busy_con[$key] = unserialize($link);
		}
	}
}
?>