<table width="100%" cellpadding="3" cellspacing="0">
<tr>
	<td>&nbsp;</td>
	<td><span style="text-decoration:underline;">Date</span></td>
	<td><span style="text-decoration:underline;">Event</span></td>
	<td><span style="text-decoration:underline;">Type</span></td>
	<td><span style="text-decoration:underline;">Posted by</span></td>
</tr>
<?php
foreach($events as $event):
	printEventMain($event);
endforeach;
echo '</table>';
function printEventMain($event){
	global $events_t, $users_t;
	$timeStamp = funcs_getTimeStamp($event[$events_t->field_date]);
	$names = funcs_getNames($event[$users_t->field_name]);
	?>
	<tr valign="top">
		<td><a style="text-decoration:none;" href="/events?remove=<?=$event[$events_t->field_id]?>">
		<img border="0" alt="<?=($event[$events_t->field_postedby] == AUTH_USERID) ? 'delete this event' : 'remove this event from my event\'s list'?>" 
		src="/images/delete.gif"></a>&nbsp;
		<?php if ($event[$events_t->field_postedby] == AUTH_USERID):?>
			<a style="text-decoration:none;" href="/events?newevent=1&edit=<?=$event[$events_t->field_id]?>">
			<img border="0" alt="edit this event" src="/images/edit.gif"></a>&nbsp;
		<?php endif;?></td>
		<td nowrap><?=date('M jS',$timeStamp)?></td>
		<td><a style="padding-left:10px;font-size:12px;color:#006699" 
		href="/events?event=<?=$event[$events_t->field_id]?>">
		<?=toHTML(funcs_cutLength($event[$events_t->field_title],30,20))?></a>
		</td>
		<td><?=$event[$events_t->field_type]?></td>
		<td><a class="ilnk" title="view <?=$names[0].' '.$names[1]?>'s profile" 
		href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($names[0].' '.$names[1])?>&col=<?=$event[$users_t->field_school]?>"><?=$names[0]?></a></td>
	</tr>
	
<?php }
?>