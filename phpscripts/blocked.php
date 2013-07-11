<?php
	require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/main.php");
	auth_authCheck();
	drawMain_1("Blocked!");
	drawMain_2();
	$page_default = true;
	//if by which is user is specified
	if (isset($_GET['usr'])){
		$users_t = new table_users();
		$blocked_t = new table_blocklist();
		$query = 'SELECT '.TABLE_USERS.'.'.$users_t->field_name.','.TABLE_BLOCKLIST.'.'.$blocked_t->field_date.
			' FROM '.TABLE_USERS.','.TABLE_BLOCKLIST.' WHERE '
			.TABLE_USERS.'.'.$users_t->field_id.' = '.toSQL($_GET['usr']).' AND '
			.TABLE_BLOCKLIST.'.'.$blocked_t->field_blockerid.' = '.TABLE_USERS.'.'.$users_t->field_id.' AND '
			.TABLE_BLOCKLIST.'.'.$blocked_t->field_blockedid." = '".AUTH_USERID."' LIMIT 1";
		$result = db::query($query);
		if ($row = db::fetch_row($result)){
			$page_default = false;
			$date = date('jS F, Y',funcs_getTimeStamp($row[1]));
			$name = funcs_getNames($row[0]);
		}
	}
echo '<table width="100%"><tr><td style="padding:20px;">';
if ($page_default){
	echo <<<EOT
	<p>Unfortunaly it appears that you have been blocked by another user and thus do not have the privilege to 
	proceed with the previous action. This could have occured if you were:</p>
	<li>sending that user a message</li>
	<li>accessing his/her profile</li>
	<li>responding to the group discussion started by that user</li>
	<li>participating in the transaction that involved that user</li>
	<p>
	We apologize for any inconvenience this may have caused but if you have any concerns please address 
	them to the other party involved.</p>
	thank you
EOT;
}else{
	echo <<<EOT
	<p>Unfortunaly it appears that you have been blocked by $name[0] $name[1] on the $date  and thus do not have the privilege to 
	proceed with the previous action. This could have occured if you were:</p>
	<li>sending $name[0] a message</li>
	<li>accessing $name[0]'s profile</li>
	<li>responding to the group discussion started by $name[0]</li>
	<li>participating in the transaction that involved $name[0]</li>
	<p>
	We apologize for any inconvenience this may have caused but if you have any concerns please address 
	them to the other party involved.</p>
	thank you
EOT;
}
echo '</td></tr></table>';
	drawMain_3(false);
?>