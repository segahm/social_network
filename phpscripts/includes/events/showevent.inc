<?php
$eventData = $event->getEvent($_GET['event']);
if (!is_null($eventData)):
	require_once($_SERVER['DOCUMENT_ROOT']
		."/phpscripts/includes/pagedata/profile_control.php");
	$events_t = new table_events();
	$users_t = new table_users();
	$profile_t = new table_profile();
	//get parameters
	$college = $eventData['college_name'];
	$college_id = $eventData[$users_t->field_school];
	$date = $eventData[$events_t->field_date];
	$timeStamp = funcs_getTimeStamp($date);
	$names = funcs_getNames($eventData[$users_t->field_name]);
?>
<table width="100%" cellpadding="0" cellspacing="0">
<tr><td align="center" style="word-break:break-all;"><table><tr valign="top">
<td><font style="color:#006699"><?=toHTML($eventData[$events_t->field_title])?> </font></td>
<td nowrap>posted by 
<a style="color:#008000" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($names[0].' '.$names[1])?>&col=<?=$college_id?>"><?=$names[0].' '.$names[1]?></a>
</td>
</tr></table>
</td></tr>
<tr>
<td>
	<table cellpadding="0" cellspacing="0">
	<tr valign="top">
	<td><img src="/images/cal/m<?=date('n',$timeStamp)?>.gif" border="0"></td>
	<td><img src="/images/cal/<?=date('j',$timeStamp)?>.gif" border="0"></td>
	<td style="padding-top:5px"><img src="/images/corner_arrow.gif" border="0"></td>
	<td style="padding-top:10px;padding-left:5px">
		<table cellpadding="3">
			<tr><td>college: <a href="/search?type=1&col=<?=$college_id?>" class="searchCat"><?=$college?></a></td></tr>
			<tr><td>event type: <font color="#000066"><?=$eventData[$events_t->field_type]?></font></td></tr>
			<?php if ($eventData[$events_t->field_type] == 'party'):?>
			<tr><td>location: <a href="/search?type=1&locat=<?=urlencode($eventData[$events_t->field_location])?>" class="searchCat"><?=toHTML($eventData[$events_t->field_location])?></a></td></tr>
			<tr><td>time: <font color="#003366"><?=toHTML($eventData[$events_t->field_time])?></font></td></tr>
			<tr><td>hosted by: <a href="/search?type=1&q=<?=urlencode($eventData[$events_t->field_host])?>" class="searchCat"><?=toHTML($eventData[$events_t->field_host])?></a></td></tr>
			<?php endif;?>
		</table>
	</td>
	</tr>
	</table>
</td>
</tr>
<?php if (!empty($eventData[$events_t->field_text])):?>
<tr><td>
<table cellspacing="5">
<tr valign="top"><td>description:</td><td style="color:#003366;">
<?php
$replace = array('/  /','/\n/');
$replace_with = array(' &nbsp;',' <br>');
?>
<?=preg_replace($replace,$replace_with,funcs_cutLength(toHTML($eventData[$events_t->field_text]),null,50))?>
</td></tr>
</table>
</td></tr>
<?php endif;?>
</table>
<?php
endif;
?>