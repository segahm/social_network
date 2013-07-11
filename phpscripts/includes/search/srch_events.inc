<?php
$SHOW_RIGHT = true;
$SHOWCALENDAR = false;
$result = $search->getResult();
$PAGE_STRING = printNextPrev($search->page,$RESULTS_OUT,$RESULT_PER_PAGE,$_SERVER['REQUEST_URI'],'page');
$events_t = new table_events();
$users_t = new table_users();
?>
<table width="100%">
<tr><td style="padding:10px;">
<table cellpadding="0" cellspacing="0" width="100%">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
		<table width="100%" cellpadding="0" cellspacing="0"><tr><td align="left" style="padding:5px;">
		<span class="roundHdr">Event Search</span>
		</td>
		<td align="right">
		<font color="#336699">Showing Results <?=$RESULTS_FROM.' ~ '.$RESULTS_TO.' out of '.$RESULTS_OUT?>&nbsp;</font>
		</td></tr></table>
	</td></tr>
	<tr><td colspan="3" class="roundsides">
	<?php
	if ($event = db::fetch_assoc($result)){
		echo '<table width="100%">';
		do{
			displayEvent($event,$events_t,$users_t);
		}while($event = db::fetch_assoc($result));
		echo '</table>';
	}else{
		echo '<p style="padding-top:10px;">we were unable to find anything matching your criteria</p>';
	}
	?>
	<?=$PAGE_STRING?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</td></tr>
</table>
<?php
function displayEvent($event,$events_t,$users_t){
	$name = funcs_getNames($event[$users_t->field_name]);
	echo '<tr><td><a href="/events?event='.$event[$events_t->field_id].'">'.toHTML($event[$events_t->field_title]).'</a></td></tr>';
	echo '<tr><td style="word-break:break-all;"><img src="/images/corner_arrow.gif">';
	if (empty($event[$events_t->field_text])){
		echo 'type: <font color="#006699">'.$event[$events_t->field_type].'</font>, ';
	}else{
		echo '<span style="color:#000000">'.toHTML(funcs_cutLength($event[$events_t->field_text],200)).'</span><br>';
	}
	echo 'posted by <a class="ilnk" href="/search?type='.SEARCH_TYPE_PEOPLE.'&q='.urlencode($name[0].' '.$name[1]).'">'.$name[0].' '.$name[1].'</a>';
	echo '</td></tr>';
}
?>