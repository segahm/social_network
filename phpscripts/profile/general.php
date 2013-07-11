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
	$error = $form->updateGeneralFields($_POST);
	//if failed to update database
	if (is_null($error)){
		$default = false;
	}else{
		//get values inputed by user during the submission
		$fields = $_POST;
	}
}else{
	$fields = $form->getGeneralFields();
}
drawMain_1("Edit Profile");
?>
<link rel="Stylesheet" type="text/css" href="/styles/edtprf.css">
<?php
drawMain_2();
?>
<!-- main panels begins -->
<table cellpadding="0" cellspacing="0" width="100%">
<tr><td align="center" valign="top" style="padding-top:10px">
<table cellpadding="2" cellspacing="0" width="90%">
	<tr valign="bottom">
		<th class="edit_on">
			My Profile
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
		<th class="edit_off">
			<a class="edit_links" href="/editp/css">CSS</a>
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
/**creates a general profile form with all the fields entered
  *with previously entered data*/
function defaultPage(){
	global $CONSTANT_PROFILE_STATUS, $fields, $profile_table, $error,
	$CONSTANT_PROFILE_SEX, $CONSTANT_PROFILE_INTERESTEDINSEX;
	$collegeinfoclass = new collegeinfo(AUTH_COLLEGEID);
if (!empty($error)){
	echo '<p align="left"><font class="error"><b>Error: </b>'.$error.'</font></p>';
}
?>
<form method="post" action="/editp/general?submit=1" style="margin:0px;padding:0px;">
<?php ob_start('funcs_fillForm');?>
<table cellpadding="0" cellspacing="5" align="left">
	<tr><td colspan="2"><h1><span class="edtHdr">School</span></h1></td></tr>
	<tr>
		<td align="right" width="120" nowrap >Current Status:</td>
		<td align="left">
			<select name="<?=$profile_table->field_status?>">
			<option value=""/>
			<?php foreach ($CONSTANT_PROFILE_STATUS as $key => $value):?>
				<option value="<?=$key?>"><?=$value?></option>
			<?php endforeach;?>
			</select>
		</td>
	</tr>
	<tr>
		<td align="right" width="120">Year:</td>
		<td align="left">
		<?php $currentYear = date('Y',mktime());?>
		<select name="<?=$profile_table->field_year?>">
		<option value=""/>
		<?php for ($i=0;$i<5;$i++):?>
			<option value="<?=($currentYear+$i)?>"><?=($currentYear+$i)?></option>
		<?php endfor;?>
		</select>
		</td>
	</tr>	
	<tr>
		<td align="right" width="120">Major: </td>
		<td align="left">
		<?php
		$majorList = $collegeinfoclass->getMajorList();
		?>
		<select name="<?=$profile_table->field_major?>">
		<option value=""/>
		<?php foreach ($majorList as $key => $value):?>
			<option value="<?=$key?>"><?=ucwords($value)?></option>
		<?php endforeach;?>
		</select>
		</td>
	</tr>
	<tr>
		<td align="right" width="120" nowrap>Second Major: </td>
		<td align="left">
		<select name="<?=$profile_table->field_majorsecond?>">
		<option value=""/>
		<?php foreach ($majorList as $key => $value):?>
			<option value="<?=$key?>"><?=ucwords($value)?></option>
		<?php endforeach;?>
		</select>
		</td>
	</tr>
	<tr>
		<td align="right" width="120">House: </td>
		<td align="left">
		<?php
		$houseList = $collegeinfoclass->getHouseList();
		?>
		<select name="<?=$profile_table->field_house?>">
		<option value=""/>
		<?php foreach ($houseList as $key => $value):?>
			<option value="<?=$key?>"><?=ucwords($value)?></option>
		<?php endforeach;?>
		</select>
		</td>
	</tr>
	<tr>
		<td align="right">High School:</td>
		<td><input type="text" name="<?=$profile_table->field_hs?>" size="30" maxlength="<?=$profile_table->field_hs_length?>"></td>
	</tr>
	<tr>
		<td align="right" width="120" nowrap>Building, Room #:</td>
		<td align="left">
			<input type="text" name="<?=$profile_table->field_room?>" size="3">
		</td>
	</tr>
	<tr><td colspan="2"><h1><span class="edtHdr">General</span></h1></td></tr>
	<tr valign="top">
		<td align="right">About me:</td>
		<td>
		<textarea name="<?=$profile_table->field_aboutme?>" cols="40" rows="6"></textarea>
		</td>
	</tr>
	<tr valign="top">
		<td align="right">My Interests:</td>
		<td>
		<textarea name="<?=$profile_table->field_interests?>" cols="40" rows="6"></textarea><br>
		<font style="font-size:10px">separate by commas</font>
		</td>
	</tr>
	<tr>
		<td align="right">Sex:</td>
		<td>
		<select name="<?=$profile_table->field_sex?>">
		<option value=""/>
		<option value="1">male</option>
		<option value="2">female</option>
		</select>
		</td>
	</tr>
	<tr>
		<td colspan="2" align="center">
		<input type="submit" class="submitButton" style="background-color:#6699CC;" value="submit">
		</td>
	</tr>
</table>
<?php ob_end_flush();?>
</form>
<?php }

function updatedPage(){
echo '<center>Your profile has been updated successfully!</center><br><a href="/editp/general">go back</a>';
}?>