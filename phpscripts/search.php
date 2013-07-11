<? 
require_once($_SERVER['DOCUMENT_ROOT']
	."/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT']
	."/phpscripts/includes/pagedata/profile_control.php");
require_once($_SERVER['DOCUMENT_ROOT']
	."/phpscripts/includes/search/searchEngine.php");
auth_authCheck();
$search = new search(AUTH_USERID,AUTH_COLLEGEID,$_GET);
if ($search->is_invalidRequest()){
	header("Location: http://".$_SERVER['HTTP_HOST']."/home");
	exit;
}
$parser = new parser($_GET);
//calculate search result numbers
$RESULTS_FROM = $search->getStart();
$RESULTS_OUT = $search->getTotal();
$RESULT_PER_PAGE = $search->getResultsPerPage();
$RESULTS_TO = min($RESULTS_OUT,$RESULTS_FROM+$RESULT_PER_PAGE-1);
define('SEARCH_QUERY',$parser->getString('q',''));
define('SEARCH_TYPE',$parser->getString('type',1));
$SHOW_RIGHT = true;
$SHOWCALENDAR = false;
switch($_GET['type']){
	case SEARCH_TYPE_MARKET:
		$title = 'Searching Market';
		$path_to_file = 'srch_market.inc';
	break;
	case SEARCH_TYPE_EVENT:
		$title = 'Searching Events';
		$path_to_file = 'srch_events.inc';
	break;
	case SEARCH_TYPE_PARTY:
		$title = 'Searching Parties';
		$path_to_file = 'srch_events.inc';
	break;
	case SEARCH_TYPE_PHOTO:
		$title = 'Searching My Pics';
		$path_to_file = 'srch_pics.inc';
	break;
	case SEARCH_TYPE_RIDE:
		$title = 'Searching Rides';
		$path_to_file = 'srch_rides.inc';
	break;
	default:
		$type = SEARCH_TYPE_PEOPLE;
		$title = 'Searching People';
		$path_to_file = 'srch_people.inc';
	break;
}
drawMain_1("$title: ".$parser->getHTML('q',''));
if ($_GET['type'] ==  SEARCH_TYPE_PEOPLE):?>
	<style>
	.advSearchButton{
		font-family:tahoma,arial,sans-serif;font-size: 8pt;
		font-weight: normal;font-Style: normal;font-variant: normal;
		color: #ffffff;background-color:#CC9966;
	}
	</style>
<?php
endif;
drawMain_2();
//include the search file depending on the type
include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/search/$path_to_file");
drawMain_3($SHOW_RIGHT,$SHOWCALENDAR);
function printNextPrev($current_page,$total,$results_per_page,$url,$page_var){
	//append ? if string query is currently not present
	if (!preg_match('/\?/',$url)){
		$url .= '?';
	}
	$string = '<table width="100%"><tr>';
	if ($current_page>1){
		$string .= '<td align="left"><a style="text-decoration:none;" href="'.preg_replace('/\&?'.$page_var.'=[^&]+/','',$url)
					.'&'.$page_var.'='.($current_page-1).'">&#60;&#60;Prev</a></td>';
	}
	if ($current_page<($total/$results_per_page)){
		$string .= '<td align="right"><a style="text-decoration:none;" href="'.preg_replace('/\&?'.$page_var.'=[^&]+/','',$url)
					.'&'.$page_var.'='.($current_page+1).'">Next&#62;&#62;</a></td>';
	}
	$string .= '</tr></table>';
	return $string;
}
?>