<table width="100%" cellpadding="5" cellspacing="5"><tr><td align="right">
<input type="button"  onClick="javascript:document.location = '/discussion<?=((!isset($_GET['mycol']))?'?mycol=1':'')?>';" value="<?=((isset($_GET['mycol']))?'browse all':'my college only')?>" style="font-size:11px;border-width:1px;border-color:#000000;background-color:#ffffff;color:#000000;margin:3px;">
<input type="button" onClick="javascript:document.location = '/discussion?newtopic=2'" value="new topic" style="font-size:11px;border-width:1px;border-color:#000000;background-color:#ffffff;color:#000000;margin:3px;">
<table width="100%" cellpadding="3" cellspacing="0">
	<tr><td colspan="4">&nbsp;</td></tr>
	<tr><td class="tbltL" style="border-color:#336699">Date</td>
	<td class="tbltR" style="border-color:#336699" width="80%">Topic</td>
	<td class="tbltR" style="border-color:#336699" width="30">responses</td>
	<td class="tbltR" style="border-color:#336699" width="100" nowrap>started by</td></tr>
<?php
$result = $control->getTopics();
$response_array = $control->storage_array;
$pagestring = printPageNumbers($response_array['page'],$response_array['totalcount'],
	$response_array['limit'],$_SERVER['REQUEST_URI'],'page');
if ($response_array['totalcount'] == 0){
	//no topics
	echo '<tr><td colspan="4" style="padding:10px;border-color:#336699;border-style:solid;border-width:1px;border-bottom-width:0px;"><p align="center">Hmmmm... it appears that we can\'t find anything. How lame is that? - Very</p></td></tr>';
}else{
	$users_t = new table_users();
	$topics_t = new table_topics();
	$profile_t = new table_profile();
	while($row = db::fetch_assoc($result)){
		$picture = funcs_getPictureURL($row[$profile_t->field_picture]);
		//resize image to the target
		$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture);
		$width = $array[0];
		$height = $array[1];
		$resizeByWidth = true;
		funcs_imageResize($width,$height,60,$resizeByWidth);
		$resizeByWidth = false;
		funcs_imageResize($width,$height,50,$resizeByWidth);
		$name = funcs_getNames($row[$users_t->field_name]);
		$school = $row[$users_t->field_school];
		$userid = funcs_getNames($row[$topics_t->field_userid]);
		$topic = toHTML(funcs_cutLength($row[$topics_t->field_topic],null,30));
		$topicid = $row[$topics_t->field_id];
		$timestamp = funcs_getTimeStamp($row[$topics_t->field_date]);
		$totalcount = $row['totalcount'];
		//check that totalcount is indeed what it is and not zero
		if (is_null($row['messages_check'])){
			$totalcount = 0;
		}
		?><tr valign="top"><td class="tbltL" style="border-color:#336699" nowrap><?=date('F jS, Y',$timestamp)?></td>
			<td class="tbltR" style="border-color:#336699">
			<a href="/discussion?msgs=1&tpcid=<?=$topicid?>"><?=$topic?></a>
			</td><td class="tbltR" style="border-color:#336699" align="center">
			<?php /*<a href="/discussion?rmvtpc=<?=$topicid?>"><img src="/images/delete.gif" border="0"></a>*/?>
			<?=$totalcount?></td>
			<td class="tbltR" style="border-color:#336699" align="center">
			<a class="ilnk" title="search for <?=$name[0].' '.$name[1]?>" href="/search?col=<?=$school?>&type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($name[0].' '.$name[1])?>">
			<img src="<?=$picture?>" width="<?=$width?>" height="<?=$height?>" border="0"><br>
			<?=$name[0].' '.$name[1]?></a>
			</td></tr>
		<?php
	}
}
?>
<tr><td colspan="4" style="border-width:1px;border-style:solid;border-color:#336699;"><p align="center">Page[s]: <?=$pagestring?></p></td></tr>
</table>
</td></tr></table>