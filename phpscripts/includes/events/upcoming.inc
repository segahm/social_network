<?php
$eventsByDate = $event->findUpcoming();
$events_t = new table_events();
$users_t = new table_users();
?>
<table cellpadding="0" cellspacing="0" width="100%">
<tr><td colspan="3" align="center"><font style="color:#003366;font-size:14px;">
[Today]</font></td></tr>
<tr valign="<?=(!isset($eventsByDate[0])) ? 'middle' : 'top'?>">
<td>
<img src="/images/cal/m<?=date('n',time())?>.gif" border="0">
</td>
<td>
<img src="/images/cal/<?=date('j',time())?>.gif" border="0"></td>
<td align="left" width="80%">
<?php
if (!isset($eventsByDate[0])){
	echo 'No events today, (<a href="/events?newevent=1">Add one now!</a>)';
}else{
	echo '<table>';
	foreach($eventsByDate[0] as $aevent){
		printEvent($aevent);
	}
	echo '</table>';
	unset($eventsByDate[0]);
}
echo '</td></tr>';
foreach($eventsByDate as $offset => $events):?>
<tr><td colspan="3" style="border-bottom-width:1px;
border-bottom-style:solid;border-bottom-color:#000000;">&nbsp;</td></tr>
<tr><td colspan="3">&nbsp;</td></tr>
<?php
$timstamp = funcs_getTimeStamp($events[0][$events_t->field_date]);
?>
<tr valign="top">
<td colspan="3" align="left"><font style="color:#003366;font-size:14px;">
[<?=date('l',$timstamp)?>, <?=date('F jS',$timstamp)?>]</font><br><br>
<?php 
echo '<table>';
foreach($events as $aevent){
	printEvent($aevent);
}
echo '</table>';
?>
</td></tr>
<?php endforeach;?>
</table>
<?php 
function printEvent($event){
	global $events_t, $users_t;
	$names = funcs_getNames($event[$users_t->field_name]);
	?>
	<tr valign="top"><td><a style="text-decoration:none;" 
		href="/events?remove=<?=$event[$events_t->field_id]?>">
		<img border="0" 
		alt="<?=($event[$events_t->field_postedby] == AUTH_USERID) ? 'delete this event' : 'remove this event from my event\'s list'?>" 
		src="/images/delete.gif"></a>&nbsp;
	<?php if ($event[$events_t->field_postedby] == AUTH_USERID):?>
		<a style="text-decoration:none;" 
			href="/events?newevent=1&edit=<?=$event[$events_t->field_id]?>">
			<img border="0" alt="edit this event" src="/images/edit.gif"></a>&nbsp;
	<?php endif;?>
	</td>
	<td colspan="2" style="word-break:break-all;">
	<a style="padding-left:10px;font-size:14px;color:#006699" 
		href="/events?event=<?=$event[$events_t->field_id]?>">
			<?=toHTML(funcs_cutLength($event[$events_t->field_title],40))?></a></td></tr>
	<tr><td>&nbsp;</td>
	<td>
	<img border="0" src="/images/corner_arrow.gif"></td><td>
	type: <?=$event[$events_t->field_type]?>, posted by <a 
	title="search for <?=$names[0].' '.$names[1]?>'s" 
	href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($names[0].' '.$names[1])?>&col=<?=$event[$users_t->field_school]?>"><?=$names[0]?></a>
	</td></tr>
	
<?php }?>