<?php
$result = $control->getMessages();
if ($result!=null){
$values = $control->storage_array;
$topicid = $values['tpcid'];
$topic = $values['topic'];
?>
<table width="100%" cellpadding="5" cellspacing="5"><tr><td>
<table width="100%" cellpadding="5" cellspacing="0">
	<tr><td colspan="2">
	<table width="100%" cellpadding="2" cellspacing="2">
	<tr valign="top">
	<td>
	<table cellpadding="2" cellspacing="2">
	<tr><td bgcolor="#006633" style="color:#CCCCCC;">Invite Friends</td></tr><tr>
	<td style="border-width:1px;border-color:#006633;border-style:solid;">
	To invite some of your friends to this discussion simply select their names from the list on the left and then click invite.<br>
	<center><input type="button" class="custButton" value="invite" onClick="javascript:scroll_friends_submit('/discussion?msgs=1&tpcid=<?=$topicid?>&invite=1');">
	<?=(isset($values['invited']))?'<br><font color="#336699">friends were invited</font>':''?>
	</center>
	</td></tr>
	</table>
	</td>
	<td align="right" nowrap>
	<input type="button" onClick="javascript:document.location = '/discussion';" value="back to topics" style="font-size:11px;border-width:1px;border-color:#000000;background-color:#ffffff;color:#000000;margin:3px;"><br>
	<input type="button" <?=((isset($_GET['mycol']))?'onClick="javascript:document.location = \'/discussion?msgs=1&tpcid='.$topicid.'\';" value="browse all"':'onClick="javascript:document.location = \'/discussion?msgs=1&tpcid='.$topicid.'&mycol=1\';" value="my college only"')?> style="font-size:11px;border-width:1px;border-color:#000000;background-color:#ffffff;color:#000000;margin:3px;"><br>
	<input type="button" onClick="javascript:document.location = '/discussion?newmsg=1&tpcid=<?=$topicid?>';" value="post a message" style="font-size:11px;border-width:1px;border-color:#000000;background-color:#ffffff;color:#000000;margin:3px;">

	</td></tr>
	</table>
	</td></tr>
	<tr><td colspan="2">
	<p align="center" style="margin:5px;">"<?=toHTML(funcs_cutLength($topic,null,50))?>"</p>
	</td></tr>
	<tr><td colspan="2"><table width="100%" cellpadding="5" cellspacing="0">
<?php
$response_array = $control->storage_array;
$pagestring = printPageNumbers($response_array['page'],$response_array['totalcount'],
	$response_array['limit'],$_SERVER['REQUEST_URI'],'page');
if ($response_array['totalcount'] == 0){
	//no messages
	echo '<tr><td colspan="2" style="padding:10px;border-color:#336699;border-style:solid;border-width:1px;border-bottom-width:0px;"><p align="center">Hmmmm... it appears that we can\'t find anything. How lame is that? - Very</p></td></tr>';
}else{
	$users_t = new table_users();
	$profile_t = new table_profile();
	$topicmsg_t = new table_topicmessages();
	while($row = db::fetch_assoc($result)){
		$picture = funcs_getPictureURL($row[$profile_t->field_picture]);
		//resize image to the target
		$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture);
		$width = $array[0];
		$height = $array[1];
		$resizeByWidth = true;
		funcs_imageResize($width,$height,100,$resizeByWidth);
		$name = funcs_getNames($row[$users_t->field_name]);
		$school = $row[$users_t->field_school];
		$userid = funcs_getNames($row[$topicmsg_t->field_userid]);
		$messageid = $row[$topicmsg_t->field_messageid];
		$message = $row[$topicmsg_t->field_message];
		$timestamp = funcs_getTimeStamp($row[$topicmsg_t->field_time]);
		?>
			<tr><td colspan="2" style="border-color:#336699;border-style:solid;border-width:1px;border-bottom-width:0px;">
			<table width="100%" cellpadding="0" cellspacing="0"><tr><td>
				<font style="font-size:12px;">date: <?=date('g\:i a, jS F, Y',$timestamp)?> in reply to <?=toHTML(funcs_cutLength($topic,20))?></font>
				</td></tr></table>
			</td></tr>
			<tr valign="top">
			<td class="tbltL" style="border-color:#336699" align="center" nowrap>
			<a class="ilnk" title="search for <?=$name[0].' '.$name[1]?>" href="/search?col=<?=$school?>&type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($name[0].' '.$name[1])?>">
			<img src="<?=$picture?>" width="<?=$width?>" height="<?=$height?>" border="0"><br>
			<?=$name[0].' '.$name[1]?></a>
			</td>
			<td class="tbltR" style="border-color:#336699;" width="100%">
			<div style="word-wrap:break-word;display:inline;overflowX:hidden;overflow:hidden;width:500">
			<?=$message?>
			</div>
			</td>
			</tr>
		<?php
	}
}
?>
<tr><td colspan="2" style="border-width:1px;border-style:solid;border-color:#336699;"><p align="center">Page[s]: <?=$pagestring?></p></td></tr>
</table></td></tr>
</table>
</td></tr></table>
<?php }else{?>
<p align="center" style="padding:5px;margin-bottom:0px;">We couldn't find anything for this topic. Why not be the first one to express interest in it?</p>
<?php
	$control->fields['header'] = $_GET['tpc'];
	require_once($_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/discussion/newtopic.inc');
}?>