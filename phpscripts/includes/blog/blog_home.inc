<?php
define('MAX_BLOGDISPLAY_SIZE',400);
drawMain_1("Blog Home");
?>
<style type="text/css">
.bgls_hdr{
font-weight:bold;
	background-color:#999966;color:#FFFFFF;
}
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
?>
<table cellpadding="5" cellspacing="5" width="100%">
<tr><td valign="top">
<?php if (!is_null($control->error)):?>
<span class="error"><b>Error: </b><?=$control->error?></span>
<?php endif;?>
<table width="500" cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding-top:3px;padding-bottom:3px;">
	<table width="100%" cellpadding="0" cellspacing="0"><tr><td align="left">
	&nbsp;<span class="roundHdr">My Blogs</span></td>
	<td align="right"><a href="/blogger?crtstngs=1" style="text-decoration:none;color:#006633;">[Create New]</a>&nbsp;</td>
	</tr></table></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;"><hr color="#666633"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;padding-left:15px;">
	<table width="100%" cellpadding="3" cellspacing="0" style="border-color:#999966;border-width:1px;border-style:solid;">
	<tr><td class="bgls_hdr" style="padding-left:10px;" align="left">Blog Title</td><td class="bgls_hdr">New Post</td>
	<td class="bgls_hdr">Settings</td><td class="bgls_hdr">Posts</td><td class="bgls_hdr">Last Update</td></tr>
<?php
	$result = $control->getMySettingsList();
	if ($row = db::fetch_assoc($result)){
		do{
			$posts_number = $row['blog_posts'];
			//make sure that there is indeed not zero posts
			if (is_null($row['postid'])){
				$posts_number = 0;
			}
			$timestamp = funcs_getTimeStamp($row[$stngs_t->field_date]);
			?>
			<tr>
			<td><a class="blg_stngs_lnk" href="/findblog?id=<?=$row[$stngs_t->field_id]?>&isme=1">
			<?=toHTML(funcs_cutLength($row[$stngs_t->field_header],25))?></a></td>
			<td><a href="/blogger?crtblg=<?=$row[$stngs_t->field_id]?>"><img src="/images/edit2.gif" border="0"></a></td>
			<td><a href="/blogger?crtstngs=1&edt=<?=$row[$stngs_t->field_id]?>"><img src="/images/config.gif" border="0"></a></td>
			<td><?=$posts_number?></td>
			<td><?=date('j M Y',$timestamp)?></td>
			</tr>
			<?php
		}while($row = db::fetch_assoc($result));
	}else{
		echo '<tr><td colspan="5" align="center">you do have any blogs</td></tr>';
	}
?>
	</table>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
<br>
<table width="500" cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding-top:3px;padding-bottom:3px;">&nbsp;<span class="roundHdr">My Recent Posts</span></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;"><hr color="#996666"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;">
<?php
//get 3 of my latest posts
	$result = $control->getMine(3);
	$replace_match = array('/  /','/[\n]/');
	$replace_with = array(' &nbsp;',' <br>');
	if ($row = db::fetch_assoc($result)){
		do{
			$text = strip_tags($row[$blog_t->field_text]);
			$timestamp = $row[$blog_t->field_timedate];
			$text_length = strlen($text);
			echo '<p><span style="font-size:14px;font-weight:bold;">'.toHTML($row[$blog_t->field_title]).'</span><br>';
			echo '<p style="margin-top:5px;">'.preg_replace($replace_match,$replace_with,funcs_cutLength($text,MAX_BLOGDISPLAY_SIZE,null,$text_length)).'<br>';
			echo '<a href="/findblog?id='.$row[$blog_t->field_settingsid].'&isme=1#pst_'.$row[$blog_t->field_id].'" class="ilnk">more...</a></p>';
		}while($row = db::fetch_assoc($result));
	}else{
		echo '<p>You currently do not have any blogs. What! No blogs! ...</p>';
	}
?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
<br>
<table width="500" cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding-top:3px;padding-bottom:3px;">&nbsp;<span class="roundHdr">Recently in Blogs</span></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;"><hr color="#336699"></td></tr>
	<tr><td colspan="3" class="roundsides" style="padding:5px;">
<?php
	//get recent posts
	$result = $control->getRecent(3);
	if ($row = db::fetch_assoc($result)){
		do{
			$text = strip_tags($row[$blog_t->field_text]);
			$timestamp = $row[$blog_t->field_timedate];
			$text_length = strlen($text);
			$user_id = $row['userid'];
			$user_school = $row[$users_t->field_school];
			$user_names = funcs_getNames($row[$users_t->field_name]);
			?>
			<p><span style="font-size:14px;font-weight:bold;"><?=toHTML($row[$blog_t->field_title])?></span> by 
			<a class="ilnk" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&col=<?=$user_school?>&q=<?=urlencode($user_names[0].' '.$user_names[1])?>">
			<?=$user_names[0].' '.$user_names[1]?></a><br>
			<p style="margin-top:5px;"><?=preg_replace($replace_match,$replace_with,funcs_cutLength($text,MAX_BLOGDISPLAY_SIZE,null,$text_length))?><br>
			<a href="/findblog?id=<?=$row[$blog_t->field_settingsid]?>#pst_<?=$row[$blog_t->field_id]?>" class="ilnk">more...</a></p>
			<?php
		}while($row = db::fetch_assoc($result));
	}else{
		echo '<p>We were unable to find any recent blogs.</p>';
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