<?php
//set server parameter
$_SERVER['DOCUMENT_ROOT'] = substr(__FILE__,0,strlen(__FILE__)-strlen('/tools/session_cron.php'));
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/DBconnect.inc");
db::connect();
$t_session = new table_session();
$query = 'DELETE LOW_PRIORITY FROM '.TABLE_SESSION.' WHERE DATE_ADD('.$t_session->field_time.',INTERVAL 30 MINUTE) < NOW()';
db::query($query);
db::close();
?>