<? 
define('IMAGE_WIDTH_SIZE',120);
define('MAX_FIRENDS_PER_LINE',2);
define('SHOW_MAX_FRIENDS',50);
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/pagedata/profile_control.php");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/utils/collegeinfo.inc");
auth_authCheck();
$GLOBALS['IS_ME'] = false;
$users_table = new table_users();
//if id is empty or bad length, then make it my profile
if (!isset($_GET['id']) || strlen($_GET['id'])!=$users_table->field_id_length){
	$_GET['id'] = AUTH_USERID;
}
$profile_access = new profile_control($_GET['id'],AUTH_USERID,AUTH_COLLEGEID);
$data = $profile_access->getData();
//if false, then we can't access this user
if ($data === false){
	header("Location: http://".$_SERVER['HTTP_HOST']."/home");
	exit;
}
$parser = new parser($data);
$profile_table = new table_profile();
$college_table = new table_colleges();
$GLOBALS['IS_ME'] = $profile_access->is_me();
$names = funcs_getNames($data[$users_table->field_name]);
$PITURE_STRING = displayPicture();
$courses = $profile_access->getCourses();
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/blog/blog_control.inc");
$blog = Blog::getProfileBlog(AUTH_USERID,$profile_access->owner_id,AUTH_COLLEGEID,$profile_access->owner_college_id);
drawMain_1("Profile");
?>
<style type="text/css">
.inLinksCat{
	color:#333333
}
.inLinksCat:hover{
	color:#993333
}
.prfName{
	font-family:"Arial", "Helvetica";font-variant:small-caps;font-size:10px;color:#006633;font-weight:bold;padding:0px;margin:0px;
}
.prfCourse{
	color:#000000;text-decoration:none;
}
.prfCourse:hover{
	color:#006633
}
.blogTitle{
font-size:14px;font-weight:bold;
}
.prfBtn{
	text-decoration:none;color:#999999;font-weight:bold;font-size:10px;
}
.prfBtn:hover{
	color:#000000;
}
.prfsides{
	border-left-width:1px;border-left-style:solid;border-left-color:#C5C5C5;
		border-right-width:1px;border-right-style:solid;border-right-color:#C5C5C5;
	padding-left:5px;padding-right:5px;
}
.prfedge{
	border-top-width:1px;border-top-style:solid;border-top-color:#C5C5C5;font-size:2px;
}
.prfedgebt{
	border-bottom-width:1px;border-bottom-style:solid;border-bottom-color:#C5C5C5;font-size:2px;
}
.prfTbHdr{
	color:#006633;text-decoration:underline;font-weight:bold;font-size:12px;
}
.prfTbCrnlt{
}
.prfTbCrnlb{
}
.prfTbCrnrb{
}
.prfTbCrnrt{
}
</style>
<script src="/html/utils.js" type="text/javascript" language="javascript"></script>
<?php
$allow_main_style = true;
if ((!(AUTH_PERMISSIONS & Permissions::PERM_SAFEVIEW) || $GLOBALS['IS_ME'])
	&& !is_null($data[$profile_table->field_style])){
	$allow_main_style = false;
	echo '<style type="text/css">';
	echo $data[$profile_table->field_style];
	echo '</style>';
}
drawMain_2(null,false,null,$allow_main_style);
?>
<table width="100%" cellpadding="0" cellspacing="0"><tr><td style="padding:5px;">
<table width="100%" cellpadding="5">
<?php
$login_string = displayLoginInfo();
if (!empty($login_string)):?>
<tr><td>
<table cellpadding="0" cellspacing="0" width="100%">
	<tr><td align="left" valign="top" class="prfTbCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="prfedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="prfTbCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="prfsides">
	<?=$login_string?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="prfTbCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="prfedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="prfTbCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</td></tr>
<?php endif;?>
<tr valign="top">
	<td><table width="100%" cellpadding="5" cellspacing="0"><tr valign="top"><td nowrap>
	<?php require_once('sect_picture.inc');?>
	</td>
	<td width="100%">
	<?php require_once('sect_contact.inc');?>
	<br>
	<?php require_once('sect_buttons.inc');?>
	</td></tr></table>
	</td>
</tr>
<?php if (!is_null($data[$profile_table->field_aboutme])):?>
<tr><td>
<?php require_once('sect_aboutme.inc');?>
</td></tr>
<?php endif;?>
<?php if (!empty($courses)):?>
<tr><td>
<?php require_once('sect_courses.inc');?>
</td></tr>
<?php endif;?>
<tr valign="top"><td>
	<table width="100%" cellpadding="5" cellspacing="0"><tr valign="top">
	<td width="150" nowrap>
	<?php require_once('sect_friends.inc');?>
	</td>
	<td width="100%">
	<?php require_once('sect_general.inc');?>
	<br>
	<?php if (!is_null($data[$profile_table->field_interests])):?>
	<?php require_once('sect_interests.inc');?>
	<?php endif;?>
	<br>
	<?php if (!is_null($blog)):?>
	<?php require_once('sect_blog.inc');?>
	<?php endif;?>
	</td></tr></table>
	</td>
</tr>
</table>
</td></tr></table>
<?php
drawMain_3(true);
function displayPicture(){
	global $data, $profile_table;
	$is_default = false;
	if (empty($data[$profile_table->field_picture])){
		$picture= DEFAULT_PROFILE_PICTURE;
		$is_default = true;
	}else{
		$picture = funcs_getPictureURL($data[$profile_table->field_picture]);
	}
	//resize image to the target
	$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture);
	$width = $array[0];
	$height = $array[1];
	$resizeByWidth = true;
	funcs_imageResize($width,$height,IMAGE_WIDTH_SIZE,$resizeByWidth);
	return '<img '.(($is_default)?'':'border="1"').' src="'.$picture.'" width="'.$width.'" height="'.$height.'">';

}
function displayLoginInfo(){
	global $parser, $data, $users_table, $profile_table, $profile_access;
	$session_table = new table_session();
	$string = '';
	if ($profile_access->show_login){
		if(!is_null($parser->getString($session_table->field_time))):
			$string .= funcs_getNames($data[$users_table->field_name],false).' is currently signed in';
		elseif(!is_null($parser->getDate($users_table->field_lastseen))):
		$string .= funcs_getNames($data[$users_table->field_name],false).' was last seen on '.date('F j, Y',funcs_getTimeStamp($parser->getString($users_table->field_lastseen)));
		endif;
		$string .= ',&nbsp;&nbsp;&nbsp;&nbsp;';
	}
	if ($profile_access->show_lastupdated && !is_null($parser->getDate($profile_table->field_lastupdated))):
		$string .= 'profile updated on: '.date('F j, Y',funcs_getTimeStamp($parser->getString($profile_table->field_lastupdated)));
	elseif($profile_access->show_lastupdated):
		$string .= 'profile has yet to be completed';
	endif;
	return $string;
}
?>