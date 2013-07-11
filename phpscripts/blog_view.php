<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/DBconnect.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/auth.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/blog/blog_access.inc");
db::connect();
//first check wherever this blog is public and wherever we need to authorize
$control = new BlogAccess();
if (isset($control->response_codes[BlogAccess::REDIRECT])){
	header("Location: ".$control->response_codes[BlogAccess::REDIRECT]);
	db::close();
	exit;
}
$strngs_t = new table_blogsettings();
$blog_t = new table_blog();
$comments_t = new table_blogcomments();
$college_t = new table_colleges();
$profile_t = new table_profile();
$users_t = new table_users();
$SETTINGS = $control->response_codes[BlogAccess::SETTINGS];
$owners_name_array = funcs_getNames($SETTINGS[$users_t->field_name]);
//see if I'm the owner
if ($control->response_codes[BlogAccess::IS_ACCESS] == Blog::PERM_PRIVATE){
	$IS_OWNER = true;
}else{
	$IS_OWNER = false;
}
//process picture
$picture = funcs_getPictureURL($SETTINGS[$profile_t->field_picture]);
//resize image to the target
$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture);
$width = $array[0];
$height = $array[1];
$resizeByWidth = true;
funcs_imageResize($width,$height,120,$resizeByWidth);
$img_string = '<img border="0" src="'.$picture.'" width="'.$width.'" height="'.$height.'">';
if (isset($control->response_codes['fields'])){
	$fields = $control->response_codes['fields'];
}else{
	$fields = array();
}
//generate id for the comment
$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
$genid = substr($genid, 0,$comments_t->field_commentid_length);
$fields['postid'] = $genid;
?>
<html>
<head>
<link rel="Stylesheet" type="text/css" href="/styles/styles.css">
<link rel="Stylesheet" type="text/css" href="/styles/blog.css">
<?php 
//display styles if not empty and either we are not logged in, or safe view is off, or 
//we are the owner
if (!defined('AUTH_PERMISSIONS') 
			|| (!(AUTH_PERMISSIONS & Permissions::PERM_SAFEVIEW)
			 || $IS_OWNER)
	&& !is_null($SETTINGS[$strngs_t->field_custom])):?>
<style type="text/css">
<?=$SETTINGS[$strngs_t->field_custom]?>
</style>
<?php endif;?>
<title>partyHub - Blog - <?=toHTML($SETTINGS[$strngs_t->field_header])?></title>
</head>
<table width="850" cellpadding="5" cellspacing="0" align="center">
<tr><td colspan="2"><img src="/images/logo2.gif"></td></tr>
<tr><td align="center">
<div style="background-color:#336699;width:100%;border-width:1px;border-color:#000000;border-style:solid" align="left">
<table cellpadding="0" cellspacing="0" width="100%"><tr><td>
<form method="get" action="/search">
<input type="hidden" name="r" value="<?=rand(1,100)?>">	
<table cellpadding="2" cellspacing="0">
<tr><td><span style="color:#FFFFFF;font-weight:bold;">People Search:</span></td>
<td><input name="q" type="text" size="30" maxlength="255"></td>
<td><input type="submit" class="blgBtn" value="search"></td></tr>
</table>
<input type="hidden" name="type" value="<?=SEARCH_TYPE_PEOPLE?>">
</form></td>
<td align="right"><a href="/blogger" style="color:#FEFEFE;font-weight:bold;text-decoration:none">home</a><span style="color:#FEFEFE;font-weight:bold;"> | </span><a href="/findblog?rndm=1" style="color:#FEFEFE;font-weight:bold;text-decoration:none">?? random blog ??</a></td></tr>
</table>
</div>
</td></tr>
<tr><td align="center">
<table cellpadding="0" cellspacing="0"><tr><td>
<span class="blog_title"><?=toHTML('"'.$SETTINGS[$strngs_t->field_header].'"')?></span></td></tr>
<tr><td align="right"><span class="date_stl">-updated: <?=date('jS F Y',funcs_getTimeStamp($SETTINGS[$strngs_t->field_date]))?></span></td></tr>
</table>
</td></tr>
<tr><td valign="top">
<table width="100%" cellpadding="5" cellspacing="5">
<tr valign="top"><td>
	<?php
	$result = $control->getPosts();
	if ($row = db::fetch_assoc($result)){
		$flag = false;
		do{
			?>
			<table width="500" cellpadding="0" cellspacing="0"><tr><td>
			<div class="post_div">
			<a name="pst_<?=$row[$blog_t->field_id]?>"></a>
			<table><tr><td><span class="post_title"><?=toHTML($row[$blog_t->field_title])?></span>
			<?php if ($IS_OWNER):?>
			- <a class="edt_link" href="/blogger?crtblg=<?=$control->blogid?>&edt=<?=$row[$blog_t->field_id]?>">edit</a>
			<?php endif;?>&nbsp;
			</td></tr>
			<tr><td align="right"><span class="date_stl">posted at <?=date('g\:ia jS F Y',funcs_getTimeStamp($row[$blog_t->field_timedate]))?></span></td><td>&nbsp;</td></tr>
			<tr><td colspan="2">&nbsp;</td></tr>
			</table>
			<?=$row[$blog_t->field_text]?>
			</div>
			</td></tr></table>
			<?php
			if ($flag){
				echo '<hr class="divider">';
			}
			$flag = true;
		}while($row = db::fetch_assoc($result));
	}else{
		?>
		<?=$owners_name_array[0]?> doesn't currently have any posts
		<?php
	}
	?>
	<p/>
<a name="cmnts"></a>
<p class="other_header">Comments:</p><br>
	<?php
	$result = $control->getComments();
	if ($row = db::fetch_assoc($result)){
		$flag = false;
		do{
			?>
			<div class="comment_div">
			<span class="date_stl"><?=date('g\:ia jS F Y',funcs_getTimeStamp($row[$comments_t->field_time]))?>:</span>
			<p>
			<?php 
				$replace = array('/  /','/\n/');
				$replace_with = array(' &nbsp;','<br/>');
			?>
			<?=preg_replace($replace,$replace_with,toHTML($row[$comments_t->field_text]))?>
			<?php if ($IS_OWNER):?>
			- <a class="edt_link" href="/blogger?delcmnt=<?=$row[$comments_t->field_commentid]?>&blgid=<?=$control->blogid?>">delete</a>
			<?php endif;?></p>
			</div>
			<?php
		}while($row = db::fetch_assoc($result));
	}
	?>
	<form action="/findblog?id=<?=$control->blogid?>&cmnt=1<?=(($IS_OWNER)?'&isme=1':'')?>#cmnts" method="POST">
	<?php ob_start('funcs_fillForm');?>
	<input type="hidden" name="postid">
	<p style="font-weight:bold">post your own:</p>
	<?php if(!empty($control->error)):?>
	<font class="error"><b>Error:</b> <?=$control->error?></font><br>
	<?php endif;?>
	<textarea name="text" cols="30" rows="5"></textarea><br>
	<p align="left"><input type="submit" class="blgBtn" value="post comment"></p>
	<?php ob_end_flush();?>
	</form>
</td>
<td align="right">
	<table cellpadding="0" cellspacing="0" width="180">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
	<span class="other_header" style="font-size:100%">&nbsp;About <?=$owners_name_array[0]?></span>
	</td></tr>
	<tr><td colspan="3" class="roundsides" align="center">
	<hr style="color:#663333">
	<?=$img_string?>
	</td></tr>
	<tr><td colspan="3" class="roundsides" style="padding-left:5px;">
	<p style="font-family:Geneva, Arial, Helvetica, sans-serif">full name: 
	<a class="ilnk" href="/search?col=<?=$SETTINGS[$users_t->field_school]?>&q=<?=urlencode($owners_name_array[0].' '.$owners_name_array[1])?>&type=<?=SEARCH_TYPE_PEOPLE?>"><?=$owners_name_array[0].' '.$owners_name_array[1]?></a><br>
	school: <a class="ilnk" href="/search?col=<?=$SETTINGS[$users_t->field_school]?>&type=<?=SEARCH_TYPE_PEOPLE?>"><?=toHTML($SETTINGS[$college_t->field_fullname])?></a>
	</p>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
	</table>
	<br>
	<table cellpadding="0" cellspacing="0" width="180">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
	<span class="other_header">&nbsp;<?=$owners_name_array[0]?>'s other blogs</span>
	<hr style="color:#999966">
	<?php
	$result = $control->getOtherBlogsByTheUser();
	if ($row = db::fetch_assoc($result)){
		echo '<table cellpadding="3" cellspacing="0">';
		do{
			echo '<tr><td><a href="/findblog?id='.$row[$strngs_t->field_id].(($IS_OWNER)?'&isme=1':'').'" class="other_title">'.toHTML(funcs_cutLength($row[$strngs_t->field_header],20)).'</a></td></tr>';
		}while($row = db::fetch_assoc($result));
		echo '</table>';
	}else{
		echo 'no blogs were found';
	}
	?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
	</table>
	<br>
	<table cellpadding="0" cellspacing="0" width="180">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
	<span class="other_header">&nbsp;Friends' blogs</span>
	<hr style="color:#CC9966">
	<?php
	$result = $control->getByUsersFriends();
	if ($row = db::fetch_assoc($result)){
		echo '<table cellpadding="3" cellspacing="0">';
		do{
			echo '<tr><td><a href="/findblog?id='.$row[$strngs_t->field_id].'" class="other_title">'.toHTML(funcs_cutLength($row[$strngs_t->field_header],20)).'</a></td></tr>';
		}while($row = db::fetch_assoc($result));
		echo '</table>';
	}else{
		echo 'no blogs were found';
	}
	?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
	</table>
</td>
</tr>
</table></td></tr>
<tr><td>
<?php include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>
</td></tr>
</table>
<?php
db::close();
?>
