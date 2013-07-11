<?php
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/pagedata/privacy_forms.php");
auth_authCheck();
$defaultPage = true;
$form = new privacy_forms(AUTH_USERID);
if (isset($_GET['submit'])){
	//form was submited, now process it
	$form->submitForm($_POST);
	$defaultPage = false;
}else{
	if (isset($_GET['block'])){
		$form->submitBlock($_POST);
	}
	//main form was not submitted
	$fields =& $form->getFields();
	$blockedList =& $form->getBlockedList();
}
drawMain_1("My Options");
?>
<script type="text/javascript" language="javascript">
<!--
function validate(){
	return true;
}
//-->
</script>
<?php
drawMain_2();
?>
<!-- main panels/elements go into this table -->
<table width="100%" cellspacing="0" cellpadding="10">
	<tr><td align="center">
		<table width="100%" cellpadding="0" cellspacing="0">
		<tr><td colspan="3" style="padding-top:3px;padding-bottom:3px;">
		&nbsp;<span class="roundHdr">My Options</span></td></tr>
		<tr><td colspan="3" style="padding:5px;"><hr color="#666633"></td></tr>
		<tr><td colspan="3" style="padding:5px;padding-left:15px;">
				<?php
					if ($defaultPage){
						drawDefault();
					}else{
						drawUpdated();
					}
				?>
		</td></tr>
		</table>
	</td></tr>
	<tr><td align="center">
		<table cellpadding="0" cellspacing="0">
			<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
			<td class="roundedge" width="100%">&nbsp;</td>
			<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
			<tr><td colspan="3" class="roundsides" style="padding-top:3px;padding-bottom:3px;">
			&nbsp;<span class="roundHdr">Blocked List</span></td></tr>
			<tr><td colspan="3" class="roundsides" style="padding:5px;"><hr color="#666633"></td></tr>
			<tr><td colspan="3" class="roundsides" style="padding:5px;padding-left:15px;">
					<a name="p2"></a>
					<form action="/myprivacy?block=1#p2" method="post">
					<p style="font-size:10px;">Note: this feature is designed to 
					completely prevent any commucations with other user[s]. 
					In other words, once you put someone on this list, any existing 
					connections with this user will be removed and further 
					interaction will be prevented.</p>
					<table cellpadding="0" cellspacing="3">
					<tr><td align="left">Enter school email address[es] of user[s] separated by commas:</td></tr>
					<tr><td><input type="text" name="emails" value="" size="50" maxlength="100"></td></tr>
					</table>
					<?php if(!empty($blockedList)):?>
					<p><b>Blocked List:</b></p>
					<table cellpadding="0" cellspacing="0">
					<?php 
					$i = 1;
					foreach($blockedList as $key => $value){
						$names = funcs_getNames($value);
						?>
						<tr>
							<td align="left">
								<input type="checkbox" checked name="<?=$key?>" value="1">
								<font style="padding-left:10px"><?=$names[0].' '.$names[1]?></font>
								<input type="hidden" name="p<?=$i?>" value="<?=$key?>">
							</td>
						</tr>
						<?php 
						$i++;
					}
					?>
					</table>
					<?php endif;?>
					<center><input type="submit" value="submit" class="custButton"></center>
					</form>
					</td></tr>
			<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
			<td class="roundedgebt" width="100%">&nbsp;</td>
			<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
		</table>
	</td></tr>
</table>
<?php
drawMain_3(false);
function drawDefault(){
global $error, $feilds;
ob_start('my_processForm');
?>
<form name="policyform" action="/myprivacy?submit=1" method="post" onSubmit="return validate();">
<h1>My Profile</h1>
<table cellpadding="2" cellspacing="2">
	<tr valign="top">
		<td align="right"><input type="radio" id="a11" name="perm_prf" value="<?=Permissions::PERM_PROFILE_ACCESS_PUBLIC?>"></td>
		<td align="left">
			<label for="a11">
			everyone at my college can see my profile info (i.e. email, phone ...)
			</label>
		</td>
	</tr>
	<tr valign="top">
		<td align="right"><input type="radio" id="a12" name="perm_prf" value="0"></td>
		<td align="left">
			<label for="a12">
			only my friends are allowed to see my profile info
			</label><br>
		</td>
	</tr>
	<tr>
		<td colspan="2"><hr></td>
	</tr>
	<tr valign="top">
		<td>
			<img src="/images/corner_arrow.gif" border="0">
		</td>
		<td>
			<input type="checkbox" name="perm_pefsh1" id="perm_pefsh1" value="<?=Permissions::PERM_PROFILE_SHOWFRIENDS?>"><label for="perm_pefsh1"> allow these people to also see my friends</label><br>
			<input type="checkbox" name="perm_pefsh2" id="perm_pefsh2" value="<?=Permissions::PERM_PROFILE_SHOWLOGIN?>"><label for="perm_pefsh2"> allow them to also see when I was last logged in</label><br>
			<input type="checkbox" name="perm_pefsh3" id="perm_pefsh3" value="<?=Permissions::PERM_PROFILE_SHOWPROFUPDATED?>"><label for="perm_pefsh3"> show them the date my profile was last updated</label><br>
			<input type="checkbox" name="perm_pefsh4" id="perm_pefsh4" value="<?=Permissions::PERM_PROFILE_SHOWEMAIL?>"><label for="perm_pefsh4"> show my email address</label>
		</td>
	</tr>
</table>
<h1>Event Display</h1>
<font style="font-size:11px;">partyHub automatically searches events you are 
interested in and displays them in your calendar. Please specify wherever 
you would like us to search all events or just the ones posted by your friends.</font>
<table cellpadding="2" cellspacing="2">
	<tr>
		<td align="right"><input type="checkbox" id="c11" name="perm_ev1" value="<?=Permissions::PERM_EVENT_PUBLIC?>"></td>
		<td align="left">
			<label for="c11">
			search all college related events (i.e. posted for my school/major/courses)
			</label>
		</td>
	</tr>
	<tr valign="top">
		<td align="right"><input type="checkbox" id="c12" name="perm_ev2" value="<?=Permissions::PERM_EVENT_PRIVATE?>"></td>
		<td align="left">
			<label for="c12">
			search events posted by my friends (for me)<br>
			<font class="dimmedTxt">note: your own events will be displayed in any case</font>
			</label>
		</td>
	</tr>
</table>
<h1>Safe View</h1>
<font style="font-size:11px;">Since all users on our site can create their own style sheets, 
it is possible that some will create styles/pictures inappropriate or dangerous for general browsing. However, you 
can disable this feature and ignore their customly designed pages.</font>
<table cellpadding="2" cellspacing="2">
	<tr>
		<td align="right"><input type="radio" id="d11" name="perm_sv" value="<?=Permissions::PERM_SAFEVIEW?>"></td>
		<td align="left">
			<label for="d11">
			disable custom styles created by other users (applies to profiles and <i>some</i> blogs)
			</label>
		</td>
	</tr>
	<tr valign="top">
		<td align="right"><input type="radio" id="d12" name="perm_sv" value="0"></td>
		<td align="left">
			<label for="d12">
			allow all customly designed styles
			</label>
		</td>
	</tr>
</table>
<center><input type="submit" value="submit" class="custButton"></center>
</form>
<?php
ob_end_flush();
}
function drawUpdated(){
?>
<p align="left">Your privacy policy has been updated successfully! <a href="/myprivacy">click here to go back</a></p>
<?php
}
/*edits html output for the form before outputing it*/
function &my_processForm(){
	global $fields;
	$contents =& ob_get_contents();
	$contents = funcs_processForm($contents,$fields);
	return $contents;
}
?>