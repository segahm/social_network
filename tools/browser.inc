<?php
class Browser{
	private $host;
	private $last_address;
	private $service_port;
	private $address;
	private $is_open = false;
	private $storage = array();
	function __construct($new_host,$debug = true){
		$this->debug = $debug;
		$this->host = $new_host;
		$this->service_port = getservbyname('www', 'tcp');
		$this->address = gethostbyname($new_host);
		$this->openConnection();
	}
	private function openConnection(){
		$this->out("creating socket\n");
		$this->socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if ($this->socket < 0){
			die("socket_create() failed: reason: ".socket_strerror($this->socket));
		}
		$this->out("connecting to: $this->address on port: $this->service_port\n");
		$result = socket_connect($this->socket, $this->address, $this->service_port);
		if ($result < 0) {
   			die("socket_connect() failed: $result: " . socket_strerror($result)."\n");
		}else{
			$this->out("O.K.\n");
		}
		$this->is_open = true;
	}
	private function out($msg){
		if ($this->debug){
			echo $msg;
		}
	}
	public function buildURL($url){
		if (empty($url))return null;
		$url = str_replace(array("\n","\r"),'',$url);
		if (substr($url,0,1) === '/'){
			$url = 'http://'.$this->host.$url;
		}elseif(substr($url,0,7) !== 'http://'){
			//if invalid request
			if (empty($this->last_address) || is_null($pos = strrpos($this->last_address,'/')))return null;
			$url = substr($this->last_address,0,$pos).'/'.$url;
		}else{
			//if http specified then update host
			$this->host = preg_replace('#^http://([^/]+)[/]?.*$#','\\1',$url);
		}
		return $url;
	}
	public function issueRequest($url,$post = null){
		if (!$this->is_open){
			$this->openConnection();
		}
		$url = $this->buildURL($url);
		$this->last_address = $url;
		$this->out("Requested URL: $url\r\n");
		$this->out("-----------------------------\r\n");
		if (empty($url))return '';
		$header = ((is_null($post))?"GET ":"POST ").$url." HTTP/1.1\r\n"
			. "Host: ".$this->host."\r\n"
			."User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows XP)\r\n"
			."Connection: Close\r\n"
			.$this->getCookies();
		if (!is_null($post)){
			$tmp = array();
			foreach ($post as $key => $value){
				$tmp[] = $key.'='.$value;
			}
			$content = implode('&',$tmp);
			$header .= "Content-Type: application/x-www-form-urlencoded\r\n"
				."Content-Length: ".strlen($content)."\r\n\r\n"
				.$content;

		}else{	
			$header .= "\r\n";
		}
		socket_write($this->socket, $header, strlen($header));
		$output = '';
		while ($out = socket_read($this->socket,2048)){
			$output .= $out;
		}
		$this->close();
		$pos = 0;
		$prev = 0;
		$pos = strpos($output,"\r\n\r\n");
		if ($pos !== false){
			$str = substr($output,0,$pos);
			$output = substr($output,$pos+2);
			$headers = null;
			$this->setHeaders($str,$headers);
			if (!empty($headers['location'])){
				$output = $this->issueRequest($headers['location'],null);
			}
		}
		return $output;
	}
	private function setHeaders($string,&$headers){
		$headers = array();
		$temp = explode("\r\n",$string);
		$this->out("_____HEADERS_____\n");
		foreach($temp as $string){
			$this->out("$string\n");
			$name = '';
			$value = '';
			$this->getHeader($string,$name,$value);
			if (strtolower($name) == 'location'){
				$headers['location'] = $value;
			}
			//check for cookies
			if (strtolower($name) == 'set-cookie'){
				$this->setCookie($value);
			}
		}
	}
	private function getHeader($str,&$hdr_name,&$hdr_value){
		$pos = strpos($str,':');
		$hdr_name = substr($str,0,$pos);
		$hdr_value = trim(substr($str,$pos+1));
	}
	private function getCookies(){
		if (empty($this->storage['cookie'])){
			return '';
		}
		$str = 'Cookie:';
		foreach($this->storage['cookie'] as $key => $cookie){
			$str .= ' '.$key.'='.$cookie.';';
		}
		$str .= "\r\n";
		return $str;
	}
	private function setCookie($value){
		if (!isset($this->storage['cookie'])){
			$this->storage['cookie'] = array();
		}
		$value = preg_replace('/(?:path|expires|domain|secure)[ ]*=[ ]*[^ \n\r]+/i','',$value);
		$name = preg_replace('/[ ]*([^ =]+)[ ]*=.+/i','\\1',$value);
		$value = preg_replace('/[ ]*[^ =]+[ ]*=[ ]*([^ ;\n\r]+).*/i','\\1',$value);
		$this->storage['cookie'][$name] = $value;
	}
	public function close(){
		if ($this->is_open){
			socket_close($this->socket);
			$this->is_open = false;
		}
	}
}
?>