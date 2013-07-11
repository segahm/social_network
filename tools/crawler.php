<?php
define('MAX_USERS',10);
//set server parameter
$_SERVER['DOCUMENT_ROOT'] = substr(__FILE__,0,strlen(__FILE__)-strlen('/tools/crawler.php'));
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/DBconnect.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/tools/browser.inc");
db::connect();
$fb_t = new table_facebook();
$query = 'SELECT '.$fb_t->field_userid.','.$fb_t->field_fb_user.','.$fb_t->field_fb_pass.' FROM '
	.TABLE_FACEBOOK.' WHERE '.$fb_t->field_isread." = 0 LIMIT ".MAX_USERS;
$result = db::query($query);
$found_users = array();
while ($row = db::fetch_row($result)){
	$found_users[$row[0]] = array($row[1],$row[2]);
}
//if nothing to be done exit
if (empty($found_users)){
	db::close();
	exit;
}
/*$query = 'UPDATE LOW_PRIORITY '.TABLE_FACEBOOK.' SET '.$fb_t->field_isread.' = 1 WHERE '.$fb_t->field_userid
	.' IN ('.toSQL(array_keys($found_users)).') LIMIT '.MAX_USERS;
db::query($query);*/
//will prevent repeating pages from been viewed more than once
$seen_ids = array();
//will collect all found people
$friends = array();
foreach($found_users as $inviter => $fsb){
	$current_user = $inviter;
	$browser = new Browser('www.facebook.com',false);
	facebookUser($fsb[0],$fsb[1]);
}
//we won't be needing this anymore
$seen_ids = null;
//if no friends were found
if (empty($friends)){
	db::close();
	exit;
}
$fields = array();
$inv_t = new table_invite_emails();

$fields[] = $inv_t->field_name;
$fields[] = $inv_t->field_email;
$fields[] = $inv_t->field_id;
$fields[] = $inv_t->field_time;
$fields[] = $inv_t->field_userid;

$friends_string = array();
foreach($friends as $person){
	$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
	$genid = substr($genid, 0, $inv_t->field_id_length);
	$friends_string[] = "(".toSQL($person[0]).','.toSQL($person[1]).",'".$genid."',NOW(),'".$person[2]."')";
}
//unique index will make sure that emails do not repeat
$query = 'INSERT DELAYED IGNORE INTO '.TABLE_INVITE_EMAILS.' ('.sqlFields($fields).') VALUES '
	.implode(',',$friends_string);
db::query($query);
function facebookUser($usr,$pass){
	global $browser,$seen_ids;
	$vl = array();
	$vl['noerror'] = 1;
	$vl['email'] = $usr;
	$vl['pass'] = $pass;
	//first perform an authentication
	$output = $browser->issueRequest('/login.php',$vl);
	//scan output to find my profile id
	$matches = array();
	$found = stripos($output,'/profile.php?id=');
	if ($found === false){
		//abort procedure since we couldn't find our profile id
		log_writeLog('aborting facebook email:'.$vl['email']);
		return;
	}
	//set a constant for easier reading
	$prf_length = strlen('/profile.php?id=');
	//extract id from str
	$pos = $found+$prf_length;
	$id = substr($output,$pos,strpos($output,'"',$pos)-$pos);
	//save my profile id so that we won't mistaken it for other people's ids
	$seen_ids[$id] = null;
	$output = $browser->issueRequest('/friends.php?link_id=0&category_id=2');
	$pos = 0;
	while (($pos = stripos($output,'/profile.php?id=',$pos)) !== false){
		//grab a a chunk from left and right
		$str = substr($output,$pos-40,($pos+100)-($pos-40));
		$matches = array();
		if (preg_match('~(http://[^/]+/profile\.php\?id=([^ ">]+))["]?>([a-z ]+)</a>~is',$str,$matches) == 0){
			//invalid link, skip
			$pos += $prf_length-1;
			continue;
		}
		$link = $matches[1];
		$id = $matches[2];
		$name = $matches[3];
		$pos+=$prf_length-1;
		//if already checked this id
		if (isset($seen_ids[$id])){
			echo "already seen:$id\n";
			continue;
		}
		$seen_ids[$id] = null;
		echo "extracting $name profile at $link\n";
		crawlFriend($link,$name);
		return;
	}
}
function crawlFriend($url,$name){
	global $browser,$current_user,$friends;
	$out = $browser->issueRequest($url,null);
	$pos = stripos($out,'emailgen.php');
	if ($pos === false)return;
	$str = substr($out,$pos-50,($pos+100)-($pos-50));
	$matches = array();
	if (preg_match('/=([^ =>"]*emailgen.php[^ ">]+)/i',$str,$matches) == 0)return;
	$img_url = $browser->buildURL($matches[1]);
	$email = extractEmail($img_url);
	if (!is_null($email)){
		echo "found email: $email\n";
		$friends[] = array($name,$email,$current_user);
	}
}
function extractEmail($img_url){
	if (($file = savePngFile($img_url)) === false){
		log_writeLog('failed to save an image from '.$img_url);
		return;
	}
	//chmod on the file so that another prg can work with it no matter what
	chmod($file,0777);
	$path = '/usr/local/bin/gocr '.$file;
	exec($path,$args);
	$email = implode("\n",$args);
	//remove jpeg file
	//unlink($file);
	//we now need to convert output to normal email
	$email = str_replace(' ','',$email);
	$pos = strrpos($email,',');
	if ($pos === false)return null;
	$domain_type = substr($email,$pos+1);
	//find where @ sign is and for each check wherever domain name is valid
	$email = substr($email,0,$pos);
	$length = strlen($email);
	$pos = 0;
	while ($pos < $length && ($pos = strpos($email,'e',$pos)) !== false){
		$domain = substr($email,$pos+1).'.'.$domain_type;
		if (isValidDomain($domain)){
			return substr($email,0,$pos).'@'.$domain;
		}
		++$pos;
	}
	return null;
}
function savePngFile($url){
	//$domain = preg_replace('#[^/]+//([^/]+).*#','\\1',$url);
	$fp = fopen($url,'br');
	if (!$fp) {
		echo "failed retrieving image from $url\n";
		return false;
	}
	$str = '';
	while (!feof($fp)){
		$str .= fread($fp,1024);
	}
	//close tcp/ip stream
	fclose($fp);
	//create a new png file
	$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
	$genid = substr($genid, 0, 5);
	$png_out = $_SERVER['DOCUMENT_ROOT'].'/tmp/'.$genid.'.png';
	file_put_contents($png_out,$str);
	chmod($png_out,0777);
	/*$im = imagecreatefrompng($png_out);
	if (!$im){
		echo '!imagecreatefrompng';
		return false;
	}
	//create a name for the file
	$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
	$genid = substr($genid, 0, 5);
	$file = $_SERVER['DOCUMENT_ROOT'].'/tmp/'.$genid.'.jpg';
	if (!imagejpeg($im,$file,100)){
		echo '!imagejpeg';
		return false;
	}
	return $file;*/
	return false;
}
function isValidDomain($domain){
	$service_port = getservbyname('www', 'tcp');
	$address = gethostbyname($domain);
	$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
	if ($socket < 0){
		return false;
	}
	$result = @socket_connect($socket, $address, $service_port);
	if ($result < 0) {
   		return false;
	}
	socket_close($socket);
	return true;
}
$browser->close();
db::close();
?>