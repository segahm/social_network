<?php
define('MAX_BLOGDISPLAY_SIZE',500);
$fields = $control->response_codes['fields'];
drawMain_1("Blog Home");
?>
<style type="text/css">
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
		document.crt_frm.perm_friends2.checked = true;
		document.crt_frm.perm_friends1.checked = false;
	}
	document.crt_frm.selfriends.value = str;
	document.crt_frm.submit();
}
function uncheck(id){
	getObjectByID(id).checked = false;
}
var deleteDialog = null;
function dispDelete(){
	if (deleteDialog != null){
		return;
	}
	deleteDialog = document.createElement('DIV');
	deleteDialog.style.backgroundColor  = "#CCCCCC";
	deleteDialog.style.borderStyle = 'solid';
	deleteDialog.style.borderWidth = '1px';
	deleteDialog.style.borderColor = '#000000';
	deleteDialog.style.position = 'absolute';
	deleteDialog.style.left = window.screen.width/2-100;
	deleteDialog.style.top = window.screen.height/2-100;
	deleteDialog = document.body.appendChild(deleteDialog);
	var text = '<table><tr><td style="padding:10px;"><table cellpading="5" cellspacing="5">'
		+'<tr><td align="center"><p style="font-size:12px;">Are you sure you wish to delete this blog?</p></td></tr>'
		+'<tr><td align="center"><input type="button" onClick="document.location = \'/blogger?delblg=<?=$control->response_codes['blgid']?>\';" class="custButton" value="confirm">&nbsp;<input type="button" onClick="closeDelete()" class="custButton" value="cancel"></td></tr></table></td></tr></table>';
	setInnerHTML(deleteDialog,text);
}
function closeDelete(){
	if (deleteDialog != null){
		try{
			document.body.removeChild(deleteDialog);
		}catch(ignore){
		}
		deleteDialog = null;
	}
}
function openExamples(){
	window.open("/html/blogExamples.html","_blank","directories=no,status=no,toolbar=no,menubar=no,location=no,width=550,height=450,scrollbars=yes");
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
$stngs_t = new table_blogsettings();
?>
<table width="100%" cellpadding="0" cellspacing="0">
  <tr><td style="padding:10px;">
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
	<tr><td colspan="3" class="sides" style="padding-top:3px;padding-bottom:3px;">
	&nbsp;<span class="roundHdr">New Blog</span></td></tr>
	<tr><td colspan="3" class="sides" style="padding:5px;">
	<!--main body -->
	<form style="margin:0px;" name="crt_frm" action="/blogger?crtstngs=1&frm=1&id=<?=$control->response_codes['blgid']?>" method="post">
	<?php ob_start('funcs_fillForm');?>
	<input type="hidden" name="is_not_new">
	<input type="hidden" name="selfriends">
	<table width="100%" cellpadding="3" cellspacing="3">
		<tr><td>
		<hr color="#999966">
		 <p>Please enter the title for this blog and select the appropriate permissions.</p>
			<table cellpadding="0" cellspacing="0"><tr>
				<td><font color="#669966"><b>Blog Title:</b></font>&nbsp;&nbsp;&nbsp;</td>
				<td><input type="text" name="title" maxlength="40" size="40"></td>
			</tr></table>
		</td></tr>
		<tr><td><font color="#669966"><b>Permissions</b></font><br>
		<p>Specify who would be granted access to this blog</p>
		</td></tr>
		<tr><td>
			<table cellpadding="0" cellspacing="5"><tr>
				<td><input type="radio" name="perm" value="0"></td>
				<td><img width="20" height="20" src="/images/secure.gif"></td>
				<td>Private</td>
			</tr><tr>
				<td>&nbsp;</td>
				<td><img src="/images/corner_arrow.gif"></td>
				<td><font style="font-size:11px;">Only I have access to this blog and (optionaly) the following people)</font></td>
			</tr></table>
		</td></tr>
		<tr><td style="padding-left:30px;">
			<table cellpadding="0" cellspacing="3">
			<tr><td nowrap colspan="2"><input type="checkbox" id="perm_school1" name="perm_school1" onClick="uncheck('perm_school2');" value="<?=Blog::PERM_SCHOOL?>"> People in my school - OR - <input type="checkbox" id="perm_school2" name="perm_school2" onClick="uncheck('perm_school1');uncheck('perm_friends1');uncheck('perm_friends2');" value="<?=Blog::PERM_ALLSCHOOLS?>"> All schools</td></tr>
			<tr valign="top"><td>
			<table>
				<tr valign="top"><td><img src="/images/corner_arrow.gif"></td>
					<td>
							<!-- friends settings begin-->
					<table>
						<tr>
							<td><input type="checkbox" id="perm_friends1" name="perm_friends1" onClick="uncheck('perm_school2');uncheck('perm_friends2');" value="<?=Blog::PERM_ALLFRIENDS?>"></td>
							<td>All of my friends, OR</td>
						</tr>
						<tr>
							<td><input type="checkbox" id="perm_friends2" name="perm_friends2" onClick="uncheck('perm_school2');uncheck('perm_friends1');" value="<?=Blog::PERM_SPECIFIED?>"></td>
							<td>The following friends &/or groups <span style="font-size:10px;">(you can also use your friends' list)</span></td>
						</tr>
						<tr valign="top">
							<td><img src="/images/corner_arrow.gif"></td>
							<td>
								<table width="100%" cellpadding="0" cellspacing="5">
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
								</table>
							</td>
						</tr>
					</table>
					<!-- friends settings ends-->
					</td>
				</tr>
			</table>
			</td></tr>
			</table>
		</td></tr>
		<tr><td>
			<table cellpadding="0" cellspacing="3">
			<tr>
				<td><input type="radio" name="perm" onClick="uncheck('perm_school1');uncheck('perm_school2');uncheck('perm_friends1');uncheck('perm_friends2');" value="<?=Blog::PERM_PUBLIC?>"></td>
				<td><img width="20" height="20" src="/images/group.gif"></td>
				<td>&nbsp;Public</td>
			</tr>
			<tr valign="middle"><td>&nbsp;</td><td><img src="/images/corner_arrow.gif">&nbsp;</td>
			<td><font style="font-size:11px;">People from outside can access my blog</font></td></tr>
			</table>
		</td></tr>
		<tr><td><hr color="#999966"></td></tr>
		<tr><td><font color="#669966"><b>Editing Preferences</b></font> <font style="font-size:11px;"> - who can post comments to this blog</font></td></tr>
		<tr><td>
		<table>
			<tr valign="top">
				<td><img width="20" height="20" src="/images/corner_arrow.gif"></td>
				<td>
					<table>
						<tr><td style="padding-left:10px;">
							<input type="checkbox" name="perm_edit1" value="<?=Blog::PERM_EDIT_FRIENDS?>"> - <font style="font-size:12px;">my freinds</font>
						</td></tr>
						<tr><td style="padding-left:10px;">
							<input type="checkbox" name="perm_edit2" value="<?=Blog::PERM_EDIT_MYSCHOOL?>"> - <font style="font-size:12px;">people in my school</font>
						</td></tr>
						<tr><td style="padding-left:10px;">
							<input type="checkbox" name="perm_edit3" value="<?=Blog::PERM_EDIT_ALLSCHOOLS ?>"> - <font style="font-size:12px;">anyone using partyHub</font>
						</td></tr>
					</table>
				</td>
			</tr>
		</table>
		</td></tr>
		<tr><td><hr color="#999966"></td></tr>
		<tr><td><font color="#669966"><b>Custom Blog Page</b></font> <font style="font-size:11px;"> - customize the way your blog page looks <b>(ADVANCED ONLY)</b></font></td></tr>
		<tr><td><p style="font-size:11px;">partyHub allows you to customize CSS properties (style sheets) for your blog page. Simply take a look at our native style sheets and modify them as you feel fit. Then cut & paste them in here. (DO NOT INCLUDE any tags - i.e. no style tags)</p></td></tr>
		<tr><td><textarea name="style" rows="8" cols="50"></textarea><br><span style="font-size:11px;">feel free to use our source code and our <a href="javascript:openExamples();">examples</a> to understand how this works</span></td></tr>
		<tr><td>&nbsp;</td></tr>
		<tr><td><table width="100%"><tr><td bgcolor="#999966" align="right">
		<input class="albm_btn" type="button" value="cancel" onClick="document.location = '/blogger';">&nbsp;
		<?php if(isset($fields['is_not_new']) && $fields['is_not_new'] == 1):?>
		<input class="albm_btn" type="button" style="background-color:#333333" value="delete" onClick="dispDelete()">&nbsp;
		<?php endif;?>
		<input class="albm_btn" style="background-color:#336699;" type="button" value="continue" onClick="submitForm();">
		</td></tr></table>
		</td></tr>
	</table>
	<?php ob_end_flush();?>
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
  </td></tr>
</table>
<?php
drawMain_3(true,true);
function printArrays(){
	$quickAddIn = new quickAddIn(AUTH_USERID);
	return printMatchArrays($quickAddIn->getFriends(),$quickAddIn->getGroups());
}
?>