<?php
define('MAX_BLOGDISPLAY_SIZE',500);
drawMain_1("Blog Home");
?>
<style type="text/css">
.blg_stngs_lnk{
	color:#3399CC;
}
.blg_stngs_lnk:hover{
	color:#CC9999;
}
</style>
<?php
drawMain_2();
$blog_t = new table_blog();
$users_t = new table_users();
$stngs_t = new table_blogsettings();
$profile_t = new table_profile();
$userinfo = $control->response_codes['userinfo'];
$username = funcs_getNames($userinfo[$users_t->field_name]);
$user_id = $control->response_codes['userinfo'][$users_t->field_id];
$user_collegeid = $control->response_codes['userinfo'][$users_t->field_school];
//process picture
$picture = funcs_getPictureURL($userinfo[$profile_t->field_picture]);
//resize image to the target
$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture);
$width = $array[0];
$height = $array[1];
$resizeByWidth = true;
funcs_imageResize($width,$height,120,$resizeByWidth);
$img_string = '<img border="0" src="'.$picture.'" width="'.$width.'" height="'.$height.'">';
$blog_id_array = array(); //will gold blog ids for posts
if ($user_id === AUTH_USERID){
	$IS_OWNER = true;
}else{
	$IS_OWNER = false;
}
?>
<table cellpadding="5" cellspacing="5" width="100%">
<tr><td valign="top"><table width="100%"><tr valign="top"><td>
<table width="300" cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding-top:3px;padding-bottom:3px;">
	&nbsp;<span class="roundHdr"><?=$username[0]?>'s Blogs</span>
	</td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;"><hr color="#666633"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;padding-left:15px;">
	<?php
	//print blogs
	$result = $control->getUsersBlogs($user_id,$user_collegeid);
	if ($row = db::fetch_assoc($result)){
		$count = 0;
		do{
			$count++;
			$timestamp = funcs_getTimeStamp($row[$stngs_t->field_date]);
		?>
		<p style="margin:3px;"><a class="blg_stngs_lnk" href="/findblog?id=<?=$row[$stngs_t->field_id].(($IS_OWNER)?'&isme=1':'')?>">
			<?=toHTML(funcs_cutLength($row[$stngs_t->field_header],20))?></a> - <span style="font-size:10px">updated: <?=date('jS F Y',$timestamp)?></span></p>
		<?php
			$blog_id_array[] = $row[$stngs_t->field_id];
		}while($row = db::fetch_assoc($result));
		//print page links if exceeded limit
		if (!isset($_GET['blgstart']) || !is_numeric($_GET['blgstart'])
				|| $_GET['blgstart'] < 0){
			$start = 0;
		}else{
			$start = $_GET['blgstart'];
		}
		echo '<table width="100%"><tr>';
		$url = parse_url($_SERVER['REQUEST_URI']);
		$link = $url['path'].'?'.preg_replace('/[&]?blgstart=[^&]*/i','',$url['query'])
			.'&blgstart=';
		if ($start > 0){
			$start1 = max(0,$start-USER_BLOG_DISPLAY_LIMIT);
			echo '<td align="left"><a href="'.$link.$start1.'">&lt;&lt;prev</a></td>';
		}
		if ($count == USER_BLOG_DISPLAY_LIMIT){
			$start2 = $start+USER_BLOG_DISPLAY_LIMIT-1;
			echo '<td align="right"><a href="'.$link.$start2.'">next&gt;&gt;</a></td>';
		}
		echo '</tr></table>';
	}else{
		echo 'no blogs were found';
	}
	?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table></td>
	<td>
	<table cellpadding="0" cellspacing="0" width="180">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
	&nbsp;About <?=$username[0]?>
	</td></tr>
	<tr><td colspan="3" class="roundsides" align="center">
	<hr style="color:#663333">
	<?=$img_string?>
	</td></tr>
	<tr><td colspan="3" class="roundsides" style="padding-left:5px;">
	<p align="center" style="font-family:Geneva, Arial, Helvetica, sans-serif">
	<a class="ilnk" href="/search?col=<?=$userinfo[$users_t->field_school]?>&q=<?=urlencode($username[0].' '.$username[1])?>&type=<?=SEARCH_TYPE_PEOPLE?>"><?=$username[0].' '.$username[1]?></a></p>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
	</table>
	</td></tr></table>
	</td>
</tr>
<tr><td>
<table width="100%" cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding-top:3px;padding-bottom:3px;">
	&nbsp;<span class="roundHdr">Some of <?=$username[0]?>'s posts...</span>
	</td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;"><hr color="#666633"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;padding-left:15px;">
	<?php
	//get recent posts
	$result = $control->getUsersRecent($blog_id_array,$user_id);
	$replace_match = array('/  /','/[\n]/');
	$replace_with = array(' &nbsp;',' <br>');
	if (!is_null($result) && $row = db::fetch_assoc($result)){
		do{
			$text = strip_tags($row[$blog_t->field_text]);
			$timestamp = $row[$blog_t->field_timedate];
			?>
			<p style="font-size:14px;font-weight:bold;"><?=toHTML($row[$blog_t->field_title])?></p>
			<p style="margin-top:5px;"><?=preg_replace($replace_match,$replace_with,funcs_cutLength($text,MAX_BLOGDISPLAY_SIZE,null))?></p>
			<a href="/findblog?id=<?=$row[$blog_t->field_settingsid]?>#pst_<?=$row[$blog_t->field_id]?>" class="ilnk">more...</a></p>
			<?php
		}while($row = db::fetch_assoc($result));
	}else{
		echo '<p>we were unable to find any posts.</p>';
	}
	?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</td></tr>
</table>
<?php
drawMain_3(true,true);
?>