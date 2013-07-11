<?php
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/pagedata/mainaccount_forms.php");
auth_authCheck();
$default = true;
$error = null;
$form = new myaccount(AUTH_USERID);
$message = 1; //what message to display
if (isset($_GET['action'])){
	//form was submited, now do a specified action
	if ($_GET['action'] == 1){
		$error = $form->changeEmail($_POST);
	}elseif ($_GET['action'] == 2){
		//returns string if error, null if success
		//false if empty email that was removed (i.e. success)
		$error = $form->changeAltEmail($_POST);
		if ($error===2){
			$error = null;
			$message = 2;
		}
	}elseif ($_GET['action'] == 3){
		$error = $form->changeName($_POST);
	}elseif ($_GET['action'] == 4){
		$error = $form->changePass($_POST);
	}
	if (is_null($error)){
		//update was successful
		$default = false;
	}else{
		//if failed to update database
		//get values inputed by user during the submission
		foreach ($_POST as $key => $value){
			$fields[$key] = $value;
		}
	}
}else{
	//form was not submitted
	$fields =& $form->getFields();
}
drawMain_1("My Account");
?>
<script type="text/javascript" language="javascript">
function doSubmit(value){
	document.userdata.action = document.userdata.action+"?"+value; 
	document.userdata.submit();
}
</script>
<?php
drawMain_2();
?>
<!-- main panels/elements go into this table -->
<table width="100%" cellspacing="0"><tr><td style="padding:5px;">
	<table width="100%" cellpadding="0" cellspacing="0">
		<tr><td colspan="3" style="padding-top:3px;padding-bottom:3px;">
		&nbsp;<span class="roundHdr">My Account</span></td></tr>
		<tr><td colspan="3" style="padding:5px;"><hr color="#666633"></td></tr>
		<tr><td colspan="3" style="padding:5px;padding-left:15px;">
				<?php
				if ($default){
					defaultPage();
				}elseif($_GET['action']==1){
					//school email changed successfully
					updated1();
				}elseif($_GET['action']==2){
					//preferred emailed changed successfully
					updated2();
				}elseif($_GET['action']==3){
					//name changed successfully
					updated3();
				}elseif($_GET['action']==4){
					//password changed successfully
					updated4();
				}
				?>
		</td></tr>
	</table>
</td></tr></table>
<?php
drawMain_3(true);
function defaultPage(){
	global $error, $fields;
	$users_table = new table_users();
	$college_table = new table_colleges();
?>
<form name="userdata" action="/myaccount" method="post">
<table width="95%" cellpadding="0" cellspacing="0">
	<tr>
	<td align="left">
	<?php
		if (!empty($error)){
			echo '<p align="left"><font class="error"><b>error: </b>'.$error.'</font></p>';
		}
	?>
	<h1>Password Verification</h1>
	<p>Current Password:&nbsp;&nbsp;&nbsp;<input type="password" name="password" value="" size="10" maxlength="10"></p>
	</td>
	</tr>
	<tr>
	<td>
		<h1>Request an Email Change</h1>
		<table cellpadding="5" cellspacing="0">
		<tr valign="middle">
			<td align="left" nowrap>Email (school):</td>
			<td align="left">
				<input type="text" name="<?=$users_table->field_email?>" 
				value="<?=ereg_replace('@.+$','',$fields[$users_table->field_email])?>" 
				size="15"><?='@'.$fields[$college_table->field_emailformat]?>
				<input type="hidden" name="<?=$college_table->field_emailformat?>" value="<?=$fields[$college_table->field_emailformat]?>">
			</td>
		</tr>
		</table>
		<p class="dimmedTxt">
			Once you request a change in your email address we will send 
			you a confirmation letter (to a new address). Please confirm this 
			letter as soon as possible - for your request will be automatically 
			deleted within a few days.
		</p>
		<p align="center"><input type="button" onClick="doSubmit('action=1');" value="send request" class="custButton"></p>
	</td>
	</tr>
	<tr>
	<td>
		<h1>Alternate Email:</h1>
		<table cellpadding="5" cellspacing="0">
		<tr valign="top">
			<td align="left" nowrap>Alternate Email:</td>
			<td align="left">
				<input type="text" name="<?=$users_table->field_preferedemail?>" value="<?=$fields[$users_table->field_preferedemail]?>" size="15">
			</td>
			<td>
				<font class="dimmedTxt">
				this will be used as an additional email address for all of our communications with you
				</font>
			</td>
		</tr>
		</table>
		<p align="center"><input type="button" onClick="doSubmit('action=2');" value="send request" class="custButton"></p>
	</td>
	</tr>
	<tr>
	<td>
		<h1>Name Change:</h1>
		<table cellpadding="5" cellspacing="0">
		<tr valign="middle">
			<td align="left">First Name:</td>
			<td align="left">
				<input type="text" name="fname" value="<?=$fields['fname']?>" size="15">
			</td>
			<td align="left">Last Name:</td>
			<td align="left">
				<input type="text" name="lname" value="<?=$fields['lname']?>" size="15">
			</td>
		</tr>
		</table>
		<p align="center"><input type="button" onClick="doSubmit('action=3');" value="submit change" class="custButton"></p>
		
	</td>
	</tr>
	<tr>
	<td>
		<h1>Password Change:</h1>
		<table cellpadding="5" cellspacing="0">
		<tr valign="middle">
			<td align="left">New Password:</td>
			<td align="left">
				<input type="password" name="newpass" value="" size="15" maxlength="10"><font class="dimmedTxt"> 4-10 case insensitive</font>
			</td>
		</tr>
		<tr valign="middle">
			<td align="left">confirm above:</td>
			<td align="left">
				<input type="password" name="newpass2" value="" size="15" maxlength="10">
			</td>
		</tr>
		</table>
		<p align="center"><input type="button" onClick="doSubmit('action=4');" value="submit change" class="custButton"></p>
		
	</td>
	</tr>
</table>
</form>
<?php
}
function updated1(){
?>
<p align="left"><font class="error">Important: </font>in order for us to update 
your primary email address we will first have to veryfy that it is indeed correct. You 
should shortly receive a verification letter from us with a confirmation link attached. 
Please confirm your email address as soon as possible - otherwise your request will 
be disregarded and automatically deleted after a few days. 
<a href="/myaccount">click here to go back</a><p>
<?php
}
function updated2(){
global $message;
	if ($message==1){
?>
<p align="left"><font class="error">Important: </font>in order for us to update 
your alternative (preferred) email address we will first have to veryfy that it is indeed correct. You 
should shortly receive a verification letter from us with a confirmation link attached. 
Please confirm your email address as soon as possible - otherwise your request will be disregarded 
within a few days. Once you confirm this email address, it will be used by us in case we need to contact you. 
<a href="/myaccount">click here to go back</a><p>
<?php
	}else{
?>
<p align="left">Your alternative email was removed successfully! <a href="/myaccount">click here to go back</a><p>
<?php
	}
}
function updated3(){
?>
<p align="left">Your name has been updated successfully! <a href="/myaccount">click here to go back</a><p>
<?php
}
function updated4(){
?>
<p align="left">Your password was successfully changed! In a few minutes 
you should receive a confirmation email from us regarding your new password. 
<a href="/myaccount">you may click here to go back</a><p>
<?php
}
?>