<?php
$ride_t = new table_rideboard();
$users_t = new table_users();
$profile_t = new table_profile();
?>
<table width="100%" cellpadding="0" cellspacing="5">
<?php foreach($rides as $aride):
	$picture_file = funcs_getPictureURL($aride[$profile_t->field_picture]);
	$name = funcs_getNames($aride[$users_t->field_name]);
	$school = $aride[$users_t->field_school];
	$size = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture_file);
	$width = $size[0];
	$height = $size[1];
	funcs_imageResize($width, $height, 80,false);
?>
<tr><td>
	<table cellpadding="0" cellspacing="0" width="100%">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
		<!-- body -->
		<table cellpadding="5" cellspacing="0">
		<tr valign="top">
			<td width="<?=$width?>"><a href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($name[0].' '.$name[1])?>"><img border="0" width="<?=$width?>" height="<?=$height?>" src="<?=$picture_file?>"></a></td>
			<td align="left">
				<table cellpadding="2">
					<tr valign="top"><td width="40">To: </td><td align="left" nowrap><a href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&locat=<?=urlencode($aride[$ride_t->field_to])?>"><?=toHTML(funcs_cutLength($aride[$ride_t->field_to],40))?></a> - <a target="_blank" href="http://maps.google.com/maps?oi=map&q=<?=urlencode($aride[$ride_t->field_to])?>" title="view google's map for <?=toHTML($aride[$ride_t->field_to])?>">(google map)</a></td></tr>
					<tr valign="top"><td width="40">From: </td><td align="left" nowrap><a href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&locat=<?=urlencode($aride[$ride_t->field_from])?>"><?=toHTML(funcs_cutLength($aride[$ride_t->field_from],40))?></a> - <a target="_blank" href="http://maps.google.com/maps?oi=map&q=<?=urlencode($aride[$ride_t->field_from])?>" title="view google's map for <?=toHTML($aride[$ride_t->field_from])?>">(google map)</a></td></tr>
					<tr><td colspan="2"># of <?=($_GET['type'] == 2)?'seats':'people'?>: <?=$aride[$ride_t->field_seats]?>, when: <?=toHTML($aride[$ride_t->field_time])?>, <?=date('F jS, Y',funcs_getTimeStamp($aride[$ride_t->field_date]))?></td></tr>
					<tr><td colspan="2">name: <a class="ilnk" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($name[0].' '.$name[1])?>&col=<?=$school?>"><?=$name[0].' '.$name[1]?></a>&nbsp;&nbsp;&nbsp;
					<a href="/message?outbox=1&id=<?=$aride[$ride_t->field_user]?>&rideid=<?=$aride[$ride_t->field_uniqueid]?>" title="contact <?=$name[0]?>">contact</a>
					<?php if ($aride[$ride_t->field_user] == AUTH_USERID):?>
					&nbsp;&nbsp;&nbsp;<a href="/rides?remove=<?=$aride[$ride_t->field_uniqueid]?>">[delete]</a>
					<?php endif;?>
					</td></tr>
				</table>
			</td>
		</tr>
		</table>
		<!-- body -->
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
	</table>
</td></tr>
<?php endforeach;?>
</table>