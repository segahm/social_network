<?php 
//calendar constants for url parameters
define('USED_DOMAIN_NAME','www.mypartyhub.com');
define("CALENDAR_YEAR","caly");
define("CALENDAR_MONTH","calm");
define("CALENDAR_DATE","cald");
define("CONFIRM_ID","cid");
define("CONFIRM_WHAT","val");
define("CONFIRM_NEWEMAIL","1");
define("CONFIRM_EMAIL","2");
define("CONFIRM_PREFERREDEMAIL","3");
define("DEFAULT_PROFILE_PICTURE","/images/defaultprf.gif");
define("PROFILE_PICTURE_PATH","/img/profile/");
define("ITEM_PICTURE_PATH","/img/items/");
define("MAIL_MESSAGES","/phpscripts/includes/messages/");
define("SEARCH_TYPE_PEOPLE",1);
define("SEARCH_TYPE_MARKET",2);
define("SEARCH_TYPE_EVENT",3);
define("SEARCH_TYPE_PARTY",4);
define("SEARCH_TYPE_PHOTO",5);
define("SEARCH_TYPE_RIDE",6);
define("SEARCH_TYPE_MAX",6);

$CONSTANT_PROFILE_STATUS = array(1 => "undergraduate, full-time",
	"undergraduate, part-time","graduate student","alumni");
$CONSTANT_PROFILE_SEX = array(1 => "male",2 => "female");
$CONSTANT_PROFILE_INTERESTEDINSEX = array(1 => "guys",2 => "girls");
define("JAVA_PATH","/usr/sbin/java");
define('PHP_PATH','/usr/bin/php');
define('JAVA_CLASSPATH','-classpath "'.$_SERVER['DOCUMENT_ROOT'].'/Web-inf/classes/"');
	
//at what hour does cron run
define('CRON_RUN_HOUR',3);

define('DEFAULT_IMAGE_WIDTH',76);
define('DEFAULT_IMAGE_HEIGHT',90);
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/tables.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/utils/funcs.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/utils/sqlFunc.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/utils/parser.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/utils/date.inc");
?>