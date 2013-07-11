<?php
	require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/main.php");
	auth_authCheck();
	require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/pagedata/quickAddIn.inc");
	require_once($_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/picts/picts_control.inc');
$control = new Picts(AUTH_USERID,AUTH_COLLEGEID,$_GET,$_POST);
if (isset($control->response_codes[Picts::REDIRECT])){
	header("Location: ".$control->response_codes[Picts::REDIRECT]);
}elseif(!is_null($control->page)){
	require_once($_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/picts/'
		.$control->page);
}
?>