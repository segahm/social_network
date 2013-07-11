<?php
$user_name = funcs_getNames(AUTH_USER_NAME);
$albm_t = new table_albums();
$fields = $control->response_codes['fields'];
drawMain_1("My Photos");
?>
<style>
.sides{
	border-left-width:1px;border-left-style:solid;border-left-color:#C5C5C5;
		border-right-width:1px;border-right-style:solid;border-right-color:#C5C5C5;
}
.albm_btn{
	font-family:tahoma,arial,sans-serif;font-size: 10pt;
	font-weight: normal;font-Style: normal;font-variant: normal;
	border-width:1px;
	color: #ffffff;background-color:#999999;
}
</style>
<script language="javascript" src="/html/utils.js"></script>
<script language="javascript">
<!--
<?=printArrays()?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/match.js");?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/binarySearch.js");?>
function submitForm(){
	var str = scroll_toString();
	if (str != ''){
		document.crtalbm_frm.perm_friends2.checked = true;
	}
	document.crtalbm_frm.selfriends.value = str;
	document.crtalbm_frm.submit();
}
function uncheck(elem){
	eval('document.crtalbm_frm.'+elem+'.checked = false');
}
//-->
</script>
<?php
//create an array of user ids for scroll panel if submition failed
if (!empty($_POST['selfriends'])){
	$ids = quickAddIn::getScrollingIds($_POST['selfriends']);
}else{
	$ids = null;
}
drawMain_2(null,true,$ids);
?>
<table width="100%" cellpadding="0" cellspacing="0">
	<tr><td style="padding:10px;">
	<h1 style="color:#006633">Create Album</h1>
	<!-- main table goes here -->
	<?php if(!empty($control->error)):?>
	<table width="500" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
		<td class="roundedge" width="100%">&nbsp;</td>
		<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
	</tr>
	<tr><td colspan="3" class="sides" style="padding-left:5px;"><font class="error"><b>Error: </b><?=$control->error?></font></td></tr>
	<tr>
		<td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
		<td class="roundedgebt" width="100%">&nbsp;</td>
		<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
	</tr>
	</table>
	<br>
	<?php endif;?>
	<table width="500" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
		<td class="roundedge" width="100%">&nbsp;</td>
		<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
	</tr>
	<tr><td colspan="3" class="sides" style="padding:5px;">
	<!--main body -->
	<form style="margin:0px;" name="crtalbm_frm" action="/mypictures?newalbm=1&pf=1&albm=<?=$control->response_codes['albmid']?>" method="post">
	<?php ob_start('funcs_fillForm');?>
	<input type="hidden" name="selfriends">
	<table width="100%" cellpadding="3" cellspacing="3">
		<tr><td bgcolor="#996666" style="font-size:2px;">&nbsp;</td></tr>
		<tr><td> <p>Please enter the name for this album and choose a sharing settings, then click "continue" to begin uploading photos.</p>
			<table cellpadding="0" cellspacing="0"><tr>
				<td><font color="#669966"><b>Album Name:</b></font>&nbsp;&nbsp;&nbsp;</td>
				<td><input type="text" name="name" maxlength="40" size="40"></td>
			</tr></table>
		</td></tr>
		<tr><td><font color="#669966"><b>Sharing Setting</b></font><br/>
		<p>Select a sharing setting that best fits the purpose of this album in terms of who can access 
		it within our site</p>
		</td></tr>
		<tr><td>
			<table cellpadding="0" cellspacing="5"><tr>
				<td><input type="radio" name="perm" value="0"></td>
				<td><img width="20" height="20" src="/images/secure.gif"></td>
				<td>Private</td>
			</tr><tr>
				<td>&nbsp;</td>
				<td><img src="/images/corner_arrow.gif"></td>
				<td><font style="font-size:11px;">Only I have access to these pictures and (optionaly) 
				the following people</font></td>
			</tr></table>
		</td></tr>
		<tr><td style="padding-left:30px;">
			<table cellpadding="0" cellspacing="3">
			<tr><td><input type="checkbox" name="perm_school" value="<?=Picts::PERM_SCHOOL?>"></td>
			<td>People in my school</td>
			<tr><td><input type="checkbox" name="perm_friends1" onclick="uncheck('perm_friends2')" value="<?=Picts::PERM_ALLFRIENDS?>"></td>
			<td>All of my friends</td></tr>
			<tr><td><input type="checkbox" name="perm_friends2" onclick="uncheck('perm_friends1')" value="<?=Picts::PERM_SPECIFIED?>"></td>
			<td>The following friends &/or groups <span style="font-size:10px;">(you can also use friends's list)</span></td></tr>
			<tr valign="top"><td><img src="/images/corner_arrow.gif"></td>
			<td><table width="100%" cellpadding="0" cellspacing="5">
				<tr valign="top">
					<td width="50%"><font class="event_add_color">friends:</font></td>
					<td width="50%"><font class="event_add_color">groups:</font></td>
				</tr>
				<tr valign="top">
					<td width="50%"><input type="text" name="users" size="30" onFocus="match_focus(0,this,'1shMtch')" onBlur="match_blur()"></td>
					<td width="50%"><input type="text" name="groups" size="30" onFocus="match_focus(1,this,'2shMtch')" onBlur="match_blur()"></td>
				</tr>
				<tr valign="top">
					<td nowrap><div id="1shMtch" style="display:none;position:absolute;"></div>
					<font class="dimmedTxt">ex: John Doe, mike@ucsc.edu, ...</font></td>
					<td><div id="2shMtch" style="display:none;position:absolute;"></div>
					<font class="dimmedTxt">ex: Project Buddies, ...</font></td>
				</tr>
			</table></td></tr>
			</table>
		</td></tr>
		<tr><td>
			<table cellpadding="0" cellspacing="3">
			<tr>
				<td><input type="radio" name="perm" value="<?=Picts::PERM_PUBLIC?>"></td>
				<td><img width="20" height="20" src="/images/group.gif"></td>
				<td>&nbsp;Public</td>
			</tr>
			<tr valign="middle"><td>&nbsp;</td><td><img src="/images/corner_arrow.gif">&nbsp;</td>
			<td><font style="font-size:11px;">People from all colleges will have access to your photos</font></td></tr>
			</table>
		</td></tr>
		<tr><td>&nbsp;</td></tr>
		<tr><td><table width="100%"><tr><td bgcolor="#996666" align="right">
		<input class="albm_btn" type="button" value="cancel" onClick="document.location = '/mypictures';">&nbsp;
		<input class="albm_btn" style="background-color:#CC9966" type="button" value="continue" onClick="submitForm();">
		</td></tr></table>
		</td></tr>
	</table>
	</form>
	<!-- main body ends -->
	</td></tr>
	<tr>
		<td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
		<td class="roundedgebt" width="100%">&nbsp;</td>
		<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
	</tr>
	</table>
	<!-- main table ends -->
	</td></tr>
</table>
<?php
drawMain_3(true);
function printArrays(){
	$quickAddIn = new quickAddIn(AUTH_USERID);
	return printMatchArrays($quickAddIn->getFriends(),$quickAddIn->getGroups());
}
?>