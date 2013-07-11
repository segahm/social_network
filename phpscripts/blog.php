<?php
	require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/main.php");
	auth_authCheck();
	require_once($_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/blog/blog_control.inc');
$control = new Blog(AUTH_USERID,AUTH_COLLEGEID,$_GET,$_POST);
if (isset($control->response_codes[Blog::REDIRECT])){
	header("Location: ".$control->response_codes[Blog::REDIRECT]);
}elseif(!is_null($control->page)){
	require_once($_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/blog/'
		.$control->page);
}
?>