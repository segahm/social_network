<?php
	require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/pagedata/quickAddIn.inc");
	require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/connections/connections_forms.php");
	//process forms if needed
	$con_form = new manage_groups(AUTH_USERID,$_POST,$_GET);
	$error = $con_form->getError();
	$all_groups = $control->getAllGroups();
	//combine the two arrays so that empty groups would be present
	$groups = array_merge($all_groups,$con_form->getGroups());
?>
<style>
.edtColBrd {
	border-top-style:solid;border-top-width:1px;border-top-color:#336699;
	background-color:#e5ecf9;
}
</style>
<script language="javascript" src="/html/utils.js"></script>
<script language="javascript">
<!--
<?=printArrays()?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/match.js");?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/binarySearch.js");?>
//-->
</script>
<table width="100%" cellpadding="0" cellspacing="0" width="100%">
	<tr><td colspan="3">
		<table width="100%" cellpadding="0" cellspacing="0"><tr><td align="left" style="padding:5px;">
		<span class="roundHdr">My Groups</span>
		</td>
		<td align="right">
		<a href="/myconnections">back to my connections</a>&nbsp;
		</td></tr></table>
	</td></tr>
	<tr><td colspan="3" style="padding:10px;">
	<?php if (!empty($error)):?>
	<font class="error">error: <?=$error?></font>
	<?php endif;?>
	<h1>Add New Groups</h1>
	<p>Please enter the title you wish to use for the new group. 
	<font class="dimmedTxt">Note that this feature is designed to only assist 
	you in managing permission rights for your friends. By putting 
	different users in different groups you can decide which group gains access to 
	which pictures and events (i.e. this group will not be seen publicly).</font></p>
	<form action="/myconnections?editgroups=1&newgroup=1" method="post">
	<table>
	<tr valign="top">
	<td align="right">Group Title: </td>
	<td align="left">
	<input type="text" name="title" value="" size="20" maxlength="20"> <font class="dimmedTxt">20 max.</font><br>
	<font class="dimmedTxt">ex: my classmates</font>
	</td>
	<td align="left"><input type="submit" value="create new" class="custButton" style="font-size:9pt;"></td>
	</tr>
	</table>
	</form>
	<h1>Manage Friends</h1>
	<p>Enter school email address[es] or full name[s] (first last) of people 
	you wish to <b>add</b> to your friend groups <font class="dimmedTxt">(separated by commas).</font></p>
	<form name="mfriends" action="/myconnections?editgroups=1&mfriends=1" method="post">
	<input type="text" name="usersadd" onFocus="match_focus(0,this,'1shMtch')" onBlur="match_blur()"
	size="70" maxlength="255">&nbsp;
	<select name="groupadd">
	<option value="0">Select a Group</option>
	<?php foreach($all_groups as $group_id => $group_title):?>
	<option value="<?=$group_id?>"><?=toHTML($group_title)?></option>
	<?php endforeach;?>
	</select><br>
	<div id="1shMtch" style="display:none;position:absolute;"></div>
	<font class="dimmedTxt">ex: John Doe, ..., ... someone@ucsc.edu,...</font><br><br>
	or<br>
	<p>Enter school email address[es] or full name[s] (first last) of people 
	you wish to <b>remove</b> from your friend groups <font class="dimmedTxt">(separated by commas).</font></p>
	<input type="text" name="usersremove" onFocus="match_focus(0,this,'2shMtch')" onBlur="match_blur()"
	size="70" maxlength="255">&nbsp;
	<select name="groupremove">
	<option value="0">Select a Group</option>
	<?php foreach($all_groups as $group_id => $group_title):?>
	<option value="<?=$group_id?>"><?=toHTML($group_title)?></option>
	<?php endforeach;?>
	</select><br>
	<div id="2shMtch" style="display:none;position:absolute;"></div>
	<br>
	</form>
	<input type="submit" onClick="javascript:document.mfriends.submit();" value="update" class="custButton" style="font-size:9pt;margin-left:100px">

	<?php if (!empty($groups)):?>
	<script language="javascript">
	<!--
		function editGroup(groupid){
			div = document.getElementById(groupid+"_t");
  			if (div.style.display == "none") {
  				div.style.display = "block";
  			}else{
  				div.style.display = "none";
  			}
		}
	//-->
	</script>
	<table width="100%">
	<tr valign="top">
	<td width="250">
	<table width="100%">
	<?php
	$count = count($groups);
	$size1 = ceil($count/2);
	$size2 = (int)$count/2;
	$bool_first = true; 
	$count = 0;
	foreach($groups as $group_id => $group){
		$count++;
		if ($bool_first && $count>$size1){
			$bool_first = false;
			echo '</table></td><td></td><td width="250"><table width="100%">';
		}
		echo '<tr><td valign="top">';
		//if this is a group with users
		if (is_array($group)){
			$group_title = $group['title'];
			$group_users = $group['users'];
		}else{
			$group_title = $group;
			$group_users = array();
		}
		//now print group column
		?>
		<table width="100%" cellpadding="3" cellspacing="0">
		<tr>
			<td align="left" class="edtColBrd">
			<a style="color:#996666;" style="font-weight:bold;" href="/myconnections?group=<?=$group_id?>"><?=toHTML($group_title)?></a>
			</td>
			<td class="edtColBrd" align="right"><a href="javascript:editGroup('<?=$group_id?>')">edit</a></td>
		</tr>
		<tr bgcolor="#e5ecf9"><td colspan="2">
		<div id="<?=$group_id?>_t" style="display:none">
		<form action="/myconnections?editgroups=1&edtgrp=1" method="post">
		<input type="hidden" name="id" value="<?=$group_id?>">
		<table width="100%">
		<tr><td align="left"><input type="text" name="title" value="<?=toHTML($group_title)?>" size="20" maxlength="20"> <font class="dimmedTxt">20 max.</font></td></tr>
		<tr><td align="center"><input type="submit" value="save changes" class="custButton" style="font-size:9pt;"></td></tr>
		</table>
		</form>
		&nbsp;<a href="/myconnections?editgroups=1&removegroup=<?=$group_id?>">remove this group</a>
		</div>
		</td></tr>
		<?php foreach($group_users as $user_id => $user_name):
		$name = funcs_getNames($user_name);
		?>
		<tr><td><a class="ilnk" href="/profile?id=<?=$user_id?>"><?=$name[0].' '.$name[1]?></a></td><td align="right"><a href="/myconnections?editgroups=1&delufrmgrp=<?=$group_id?>&user=<?=$user_id?>" style="text-decoration:none;">[remove]</a></td></tr>
		<?php endforeach;?>
		</table>
		<?php
		//finish column
		echo '</td></tr>';
	}
	?>
	</table>
	</td>
	<?php if($size2 == 0):?>
	<td></td>
	<?php endif;?>
	</tr>
	</table>
	<?php endif;?>
	</td></tr>
</table>
<?php
function printArrays(){
	$quickAddIn = new quickAddIn(AUTH_USERID);
	return printMatchArrays($quickAddIn->getFriends());
}
?>