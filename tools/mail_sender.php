<?php
define('MAX_MAILS',20);
//set server parameter
$_SERVER['DOCUMENT_ROOT'] = substr(__FILE__,0,strlen(__FILE__)-strlen('/tools/mail_sender.php'));
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/DBconnect.inc");
db::connect();
$mail_t = new table_mail();
$query = 'SELECT * FROM '.TABLE_MAIL.' LIMIT '.MAX_MAILS;
$result = db::query($query);
$ids = array();
while ($row = db::fetch_assoc($result)){
	$ids[] = $row[$mail_t->field_id];
	$content = explode('<<delim>>',$row[$mail_t->field_message]);
	$content_type = (($content[0] === 'true')?'text/html':'text/plain');
	$to = $content[1];
	$subject = $content[2];
	$body = $content[3];
	$header = "From: info@myPartyHub.com\r\nReply-To: no_reply@myPartyHub.com\r\n"; 
	$header .= "MIME-Version: 1.0\n"; 
	$header .= "Content-Type: $content_type; charset=\"ISO-8859-1\"\r\n"; 
	$header .= "Content-Transfer-Encoding: 8bit\r\n"; 
	mail($to,$subject,$body,$header) or log_writeLog('failed sending a message to: '.$to);
}
//delete all mails that were sent
if (!empty($ids)){
	$query = 'DELETE LOW_PRIORITY FROM '.TABLE_MAIL.' WHERE '.$mail_t->field_id.' IN '
		.' ('.toSQL($ids).') LIMIT '.MAX_MAILS;
	db::query($query);
}
db::close();
?>