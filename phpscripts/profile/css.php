<?
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/utils/collegeinfo.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/pagedata/profile_forms.php");
auth_authCheck();
$parser = new parser($_POST);
$error = null;
$default = true; //default page
$fields = null; //values to be stored in this array for default requests
//process data
$profile_table = new table_profile();
$form = new myprofile(AUTH_USERID);
if (isset($_GET['submit'])){
	$error = $form->updateStyles($_POST);
	//if failed to update database
	if (is_null($error)){
		$default = false;
	}else{
		//get values inputed by user during the submission
		$fields = $_POST;
	}
}else{
	$fields = $form->getStyles();
}
drawMain_1("Edit Profile");
?>
<link rel="Stylesheet" type="text/css" href="/styles/edtprf.css">
<script language="javascript" type="text/javascript">
<!--
function openExamples(){
	window.open("/html/blogExamples.html","_blank","directories=no,status=no,toolbar=no,menubar=no,location=no,width=550,height=450,scrollbars=yes");
}
//-->
</script><?php
drawMain_2();
?>
<!-- main panels begins -->
<table cellpadding="0" cellspacing="0" width="100%">
<tr><td align="center" valign="top" style="padding-top:10px">
<table cellpadding="2" cellspacing="0" width="90%">
	<tr valign="bottom">
		<th class="edit_off">
			<a class="edit_links" href="/editp/general">My Profile</a>
		</th>
		<th class="edit_off">
			<a class="edit_links" href="/editp/contact">My Contact Information</a>
		</th>
		<th class="edit_off">
			<a class="edit_links" href="/editp/courses">My Courses</a>
		</th>
		<th class="edit_off">
			<a class="edit_links" href="/editp/picure">My Picture</a>
		</th>
		<th class="edit_on">
			CSS
		</th>
	</tr>
	<tr>
		<td class="edit_main" colspan="5" align="center">
<?php
if ($default){
	defaultPage();
}else{
	updatedPage();
}
?>
	 	</td>
	 </tr>
</table>
</td></tr>
</table>
<!-- main panels ends -->
<?php
drawMain_3(false);
function defaultPage(){
if (!empty($error)){
	echo '<p align="left"><font class="error"><b>error: </b>'.$error.'</font></p>';
}
?>
<form method="post" action="/editp/css?submit=1">
<?php ob_start('funcs_fillForm');?>
<table width="100%">
<tr><td><span class="edtHdr">Custom Profile Page</span> <font style="font-size:11px;"> - customize the way your profile page looks <b>(ADVANCED ONLY)</b></font></td></tr>
<tr><td><p style="font-size:11px;">partyHub allows you to customize CSS properties (style sheets) for your profile page. Simply take a look at our native style sheets and modify them as you feel fit. Then cut & paste them in here. (DO NOT INCLUDE any tags - i.e. no style tags)</p></td></tr>
<tr><td><table><tr valign="top"><td>
<textarea name="prf_style" rows="8" cols="50"></textarea><br><span style="font-size:11px;">we suggest taking a look at our blog <a href="javascript:openExamples();">examples</a> to understand how this works</span>
</td><td>
<p style="font-size:11px;">Note: these style properties will apply to how everyone (who has "safe view" turned off) sees your profile page. In other words, proceed with care...</p>
</td></tr></table>
</td></tr>
</table>
<hr color="#999966">
<table width="100%">
<tr><td><span class="edtHdr">Custom Browse</span> <font style="font-size:11px;"> - customize your own view <b>(applies to all pages)</b></font></td></tr>
<tr><td><table><tr valign="top"><td>
<textarea name="main_style" rows="8" cols="50"></textarea>
</td><td>
<p style="font-size:11px;">Note: these style properties will affect how you see our entire site. In other words, proceed with even greater care...</p>
</td></tr></table>
</td></tr>
</table>
<?php ob_end_flush();?>
<center><input type="submit" value="submit" class="custButton"></center>
</form>
<?php }

function updatedPage(){
echo '<center>Your profile has been updated successfully!</center><br><a href="/editp/css">go back</a>';
}?>