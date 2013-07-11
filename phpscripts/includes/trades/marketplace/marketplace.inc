<?php 
require_once("market_control.inc");
$control = new market_control($_GET,$_POST,AUTH_USERID,AUTH_COLLEGEID);
?>
 <table width="100%" cellpadding="0" cellspacing="0"><tr><td style="padding:10px;"> 

 <table width="100%" cellpadding="2" cellspacing="5"> <tr><td class="blkBrd"> 
 <?php require_once("searchH.inc");?> </td></tr> <tr><td> 
 <?php
if ($control->getResponse() == MARKET_PAGE_REDIRECT){
	header("Location: http://".$_SERVER['HTTP_HOST'].$control->newlocation);
	exit;
}elseif ($control->getResponse() == MARKET_PAGE_MAIN){
	require_once("main.inc");
}elseif ($control->getResponse() == MARKET_PAGE_ITEM){?>
<?php
	//get item
	$item = $control->getItem();
	//initialize tables
	$face_var = new face_var();
	$item_t = new table_tradeitems();
	$users_t = new table_users();
	//get fields common to all types
	$excededPurchases = ($item[$users_t->field_purchases] >= 10)?true:false;
	$id = $item[$item_t->field_id];
	$timestamp = funcs_getTimeStamp($item[$item_t->field_date]);
	$userid = $item[$item_t->field_user];
	$collegeid = $item[$users_t->field_school];
	$names = funcs_getNames($item[$users_t->field_name]);
	$feedback = feedbackConvert($item[$users_t->field_feedback]);
	$feedback_perc = (100/(max($feedback[0]+$feedback[1]+$feedback[2],1))*$feedback[0]);
	//get item info
	$iteminfo = $item[$face_var->iteminfo];
	if ($control->type == TYPE_ITEM_TEXTBOOK):
		require_once("item_textbook.inc");
	elseif($control->type == TYPE_ITEM_READER):
		require_once("item_reader.inc");
	elseif($control->type == TYPE_ITEM_HOUSING):
		require_once("item_housing.inc");
	elseif($control->type == TYPE_ITEM_OTHER):
		require_once("item_other.inc");
	endif;?>
<?php
}elseif ($control->getResponse() == MARKET_PAGE_FEEDBACK_1 || $control->getResponse() == MARKET_PAGE_FEEDBACK_2){
	require_once("feedback.inc");
}elseif ($control->getResponse() == MARKET_PAGE_HISTORY){
	require_once("history.inc");
}elseif ($control->getResponse() == MARKET_PAGE_ACTIVITY){
	require_once("activity.inc");
}elseif ($control->getResponse() == MARKET_PAGE_MARKETSEARCH){
	$search = $control->searchMarket();
	require_once($_SERVER['DOCUMENT_ROOT']
	."/phpscripts/includes/search/mrktsrch.inc");
	//print page string
	$rseed = $search->getRandomSeed();
	$url = preg_replace('/(&?page=[^&]*)|(&?r=[^&]*)/i','',$_SERVER['REQUEST_URI']).'&r='.$rseed;
	$PAGE_STRING = printPageNumbers($search->page,
		$search->getTotal(),
		$search->getResultsPerPage(),
		$url,'page');
	echo '<p align="center">Pages: '.$PAGE_STRING.'</p>';
}
?>
	</td></tr>
	</table>
</td></tr></table>