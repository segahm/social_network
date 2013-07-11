<? 
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/pagedata/profile_forms.php");
auth_authCheck();
$parser = new parser($_POST);
$error = null;
$default = true; //default page
$fields = null; //values to be stored in this array for default requests
//process data
$profile_table = new table_profile();
$courses_table = new table_courses();
$departments_table = new table_departments();
$mycourses_table = new table_mycourses();
$form = new myprofile(AUTH_USERID);
if (isset($_GET['submit'])){
	$error = $form->updateContactFields($_POST);
	//if failed to update database
	if (is_null($error)){
		$default = false;
	}else{
		//get values inputed by user during the submission
		foreach ($_POST as $key => $value){
			$fields[$key] = toHTML($value);
		}
	}
}else{
	$fields =& $form->getContactFields();
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
		<th class="edit_off">
			<a class="edit_links" href="/editp/general">My Profile</a>
		</th>
		<th class="edit_on">
			My Contact Information
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
		<td class="edit_main"colspan="5" align="center">
			<?php if ($default):?>
		<table width="100%"><tr valign="top"><td>
		<?php
		if (!empty($error)){
			echo '<p align="left"><font class="error"><b>Error: </b>'.$error.'</font></p>';
		}
		?>
		<?php ob_start('funcs_fillForm');?>
		<form action="/editp/contact?submit=1" method="post">
		<table width="100%" cellspacing="0" cellpadding="5" align="left">
			<tr><td align="right">Phone:</td>
			<td>
			<input size="15" type="text" size=20 
				name="<?=$profile_table->field_phone?>" maxlength="<?=$profile_table->field_phone_length?>">
			</td></tr>
			<tr><td align="right">Cellphone:</td>
			<td><input size="15" type="text" 
				name="<?=$profile_table->field_cellphone?>" maxlength="<?=$profile_table->field_cellphone_length?>">
			</td></tr>
			<tr><td align="right">Mailbox:</td>
			<td><input size="15" type="text" 
				name="<?=$profile_table->field_mailbox?>" maxlength="<?=$profile_table->field_mailbox_length?>">
			</td></tr>
			<tr><td align="right" width="110" nowrap>AIM screen name:</td>
			<td><input type="text" name="<?=$profile_table->field_AIM?>" maxlength="<?=$profile_table->field_AIM_length?>">
			</td></tr>
			<tr><td align="right">Address:</td>
			<td><input size="30" type="text" name="<?=$profile_table->field_address?>" maxlength="<?=$profile_table->field_address_length?>">
			</td></tr>
			<tr><td align="right">City:</td>
			<td><input size="20" type="text" name="<?=$profile_table->field_city?>" maxlength="<?=$profile_table->field_city_length?>">
			</td></tr>
			<tr><td align="right">State:</td>
			<td>
			<input size="2" type="text" name="<?=$profile_table->field_state?>" maxlength="<?=$profile_table->field_state_length?>">&nbsp;&nbsp;&nbsp;
			Zip:
			<input size="5" type="text" name="<?=$profile_table->field_zip?>" maxlength="<?=$profile_table->field_zip_length?>">
			</td></tr>
			<tr><td colspan="2" align="center">
			<input type="submit" class="submitButton" 
			style="background-color:#339966;" value="submit">
			</td></tr>
		</table>
		</form>
		<?php ob_end_flush();?>
		 </td>
	 	<td style="border-left-width:1px;border-left-style:solid;
	 	border-left-color:#000000;padding-left:10px">
	 	<p>Please note: this information will only be used for your profile and will not be distributed by us in any way.</p>
		</td></tr></table>
		 <?php
		 else:
			echo '<center>Your profile has been updated successfully!</center><br><a href="/editp/contact">go back</a>';
		 endif;
		 ?>
	 	</td>
	 </tr>
</table>
</td></tr>
</table>
<!-- main panels ends -->
<?php
drawMain_3(false);
?>