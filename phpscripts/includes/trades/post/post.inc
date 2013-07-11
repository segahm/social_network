<?php require_once("post_control.inc");?>
<table width="100%" cellpadding="0" cellspacing="10">
<tr><td>
<!-- beginning padding table -->
<?php
$control = new post_control($_GET,$_POST,$_FILES,AUTH_USERID,AUTH_COLLEGEID);
if ($control->getPage() == TRADES_POST_REDIRECT){
	header("Location: http://".$_SERVER['HTTP_HOST']."/home");
	exit;
}elseif ($control->getPage() == TRADES_POST_PAGE1){
	require_once("page1.inc");
}elseif($control->getPage() == TRADES_POST_PAGE2){
	require_once("page2_".$_GET['item'].".inc");
}elseif($control->getPage() == TRADES_POST_PAGE3){
	require_once("page3_".$_GET['item'].".inc");
}
?>
<!-- padding table ends -->
</td></tr>
</table>
<?php
function &processForm(){
	global $control,$DIV_PAGE;
	$contents =& ob_get_contents();
	//do not check other irrelavent div layers on page 1
	if ($control->getPage() == TRADES_POST_PAGE1 && (!isset($_GET['item']) || $_GET['item'] != $DIV_PAGE)){
		return $contents;
	}
	$fields = $control->getFields();
	if (empty($fields)){
		return $contents;
	}
	foreach ($fields as $key => $value){
		$fields[$key] = toHTML($value);
	}
	$contents = funcs_processForm($contents,$fields);
	return $contents;
}
?>