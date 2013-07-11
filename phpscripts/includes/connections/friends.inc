<?php
define("RESULT_LIMIT",6);
//get friends
$table_users = new table_users();
$groups_t = new table_friendgroups();
$parser = new parser($_GET);
$RANDOM_SEED = $parser->getInt('r',rand(1,100));
$PAGE = $parser->getInt('page',1);
if (!($PAGE>=1)){
	$PAGE = 1;
}
$TOTAL_PEOPLE = 0;
$resultFriends = getFriends(AUTH_USERID,$PAGE,RESULT_LIMIT,$RANDOM_SEED,$parser->getString('group',null),$TOTAL_PEOPLE);
//get all groups owner by the owner (viewer)
$all_groups = $control->getAllGroups();
//if group id was specified, find out the title
if (isset($_GET['group']) && isset($all_groups[$_GET['group']])){
	$CURRENT_GROUP_TITLE = $all_groups[$_GET['group']]; 
}
//clean the url string
$url = parse_url('http://'.$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI']);
$urlstring = $url['path'].'?'.((isset($url['query'])) ? (preg_replace('/(&?addtogroup=[^&]*)|(&?delufrmgrp=[^&]*)|(&?user=[^&]*)|([&]?r=[^&]*)/i','',$url['query'])) : '');
$urlstring .= '&r='.$RANDOM_SEED;
?>
<table width="100%" cellpadding="0" cellspacing="0" width="100%">
	<tr><td colspan="3">
		<table width="100%" cellpadding="0" cellspacing="0"><tr><td align="left" style="padding:5px;">
		<span class="roundHdr">Friends</span>
		</td>
		<td align="right">
		<a href="/myconnections?editgroups=1">edit groups</a>&nbsp;
		</td></tr></table>
	</td></tr>
	<tr><td colspan="3" style="padding:10px;">
	<?php if (isset($CURRENT_GROUP_TITLE)):?>
	<p align="center">Viewing "<?=toHTML($CURRENT_GROUP_TITLE)?>" group</p>
	<?php endif;?>
<?php if (!is_null($resultFriends) && ($person = db::fetch_assoc($resultFriends))):?>
<script language="javascript">
<!--
function showgroups(id,s1,s2){
	var div1 = getObjectByID(id+s1);
	var div2 = getObjectByID(id+s2);
  	div1.style.display = "block";
	div2.style.display = "none";
}
function delfriend(name,id){
	if (window.confirm('Are you sure you want to remove '+name+' from your list of friends?')){
		window.location = '/myconnections?removefriend='+id;
	}
}
//-->
</script>
<?php
		/*iterate through each person keeping track of the total count*/
		do{
			$person_groups = array(0 => $person);
			$count = 1;
			/*get all entries that are about the same user 
			 *but with different groups that this user belongs to*/
			while(($person_groups[$count] = db::fetch_assoc($resultFriends))
					&& $person_groups[$count][$table_users->field_id] 
						== 
					$person[$table_users->field_id]){
				$count++;
			}
			//remove last entry since it refers to the next person
			$person = array_pop($person_groups);
			print_friend($person_groups,$all_groups);
		}while($person);
		//now construct page string
		$page_string = printPageNumbers($PAGE,$TOTAL_PEOPLE,RESULT_LIMIT,$urlstring,'page');
		echo "<center>$page_string</center>";
	else:
		//no friends
		if (isset($CURRENT_GROUP_TITLE)){
			echo	"You don't have any friends in: \"".toHTML($CURRENT_GROUP_TITLE)."\"";
		}else{
			echo "Mmmm..., it appears that you don't have any friends.";
		}
	endif;
?>
	</td></tr>
</table>
<?php
function print_friend($groupee,$all_groups){
	global $urlstring;
	$groups = array();
	//initialize tables
	$friends_table = new table_friends();
	$friendgroup_table = new table_friendgroups();
	$friends_in_groups_t = new table_friends_in_groups();
	$users_table = new table_users();
	$profile_table = new table_profile();
	$college_table = new table_colleges();
	//iterate through all the groups that this person belongs too
	foreach($groupee as $group){
		//groups[groups id] = group title
		if (!is_null($group[$friends_in_groups_t->field_groupid])){
			$groups[$group[$friends_in_groups_t->field_groupid]] = $group[$friendgroup_table->field_title];
		}
	}
	//at this point groups stores all group ids and titles
	$person = array_pop($groupee);
	//get user parameters
	$image_file = funcs_getPictureURL($person[$profile_table->field_picture]);
	$id = $person[$users_table->field_id];
	$name = funcs_getNames($person[$users_table->field_name]);
	$college_id = $person[$users_table->field_school];
	$college = $person[$college_table->field_fullname];
	$permissions = $person[$users_table->field_permissions];
	//change image size
	$size = getimagesize($_SERVER['DOCUMENT_ROOT'].$image_file);
	$width = $size[0];
	$height = $size[1];
	//resize image by width
	funcs_imageResize($width, $height, 130, true);
	$profile_control = new profile_control($id,AUTH_USERID,AUTH_COLLEGEID,
		true,$permissions,$college_id);
	$access_friends = $profile_control->show_friends;
?>
<table width="100%" cellspacing="2">
<tr>
	<td width="130" align="left" height="<?=$height?>">
	<a href="/profile?id=<?=$id?>"><img src='<?=$image_file?>' border="0" width="<?=$width?>" height="<?=$height?>"></a>
	</td>
	<td valign="top">
	<table width="100%">
	<tr>
		<td>
		<font color="#006699">groups in: </font>
		<?php if (empty($groups)){
			echo 'none';
		}
		$count = 0;	//keeps track of how long the string is
		foreach($groups as $group_id => $group_title){
			if ($count>99){
				echo '...';
				break;
			}
			echo '<a href="/myconnections?group='.$group_id.'" class="searchCat">'.toHTML(strtolower($group_title)).'</a>, ';
			$count += strlen($group_title);
		}
		?>
			<table><tr>
			<td>
				<a href="javascript:delfriend('<?=$name[0]?>','<?=$id?>')"><img width="30" height="30" alt="remove <?=$name[0]?> from my buddy list" src="/images/remove.gif" border="0"></a>
			</td>
			<td>
				<a href="/message?outbox=1&id=<?=$id?>"><img width="30" height="30" alt="send <?=$name[0]?> a message" src="/images/mail.gif" border="0"></a>
			</td>
			<?php if($access_friends):?>
			<td>
				<a href="/search?type=1&friends=<?=$id?>"><img width="30" height="30" alt="view <?=$name[0]?>'s friends" src="/images/group.gif" border="0"></a>
			</td>
			<?php endif;?>
			<td>
				<a href="/mypictures?usr=<?=$id?>"><img width="30" height="30" alt="view <?=$name[0]?>'s photos" src="/images/album.gif" border="0"></a>
			</td>
			<td>
				<a href="/blogger?usrblgs=<?=$id?>"><img width="30" height="30" alt="view <?=$name[0]?>'s blogs" src="/images/panel/blog.gif" border="0"></a>
			</td>
			</tr></table>
		</td>
		<td valign="top" width="150" nowrap style="padding:0px;">
		
		<div id="<?=$id?>_s1" style="display:block;">
		<table width="140" cellpadding="3" cellspacing="0">
		<tr><td align="left" style="border-style:solid;border-width:1px;border-color:#000000;"><a href="javascript:showgroups('<?=$id?>','_s2','_s1');"><img src="/images/down.gif" border="0">&nbsp;add to group</a></td></tr>
		<tr><td align="left" style="border-style:solid;border-width:1px;border-color:#000000;"><a href="javascript:showgroups('<?=$id?>','_s3','_s1');"><img src="/images/down.gif" border="0">&nbsp;remove from group</a></td></tr>	
		</table>
		</div>
		<div id="<?=$id?>_s2" style="display:none;position:absolute;">
		<table width="140" cellpadding="3" cellspacing="0">
		<tr><td align="left" bgcolor="#FAFAFA" style="border-style:solid;border-width:1px;border-color:#000000;"><a href="javascript:showgroups('<?=$id?>','_s1','_s2');"><img src="/images/down2.gif" border="0">&nbsp;add to group</a></td></tr>
		<?php
		//get a list of groups this user does not yet belong to
		$difArrays =& array_difKeys($groups,$all_groups);
		foreach($difArrays as $group_id => $group_title):?>
		<tr><td align="left" bgcolor="#FAFAFA" style="border-style:solid;border-width:1px;border-color:#000000;"><a style="color:#663333" href="<?=$urlstring.'&addtogroup='.$group_id.'&user='.$id?>"><?=toHTML(funcs_cutLength($group_title,15))?></a></td></tr>
		<?php endforeach;?>
		<tr><td align="left" bgcolor="#FAFAFA" style="border-style:solid;border-width:1px;border-color:#000000;"><a href="javascript:showgroups('<?=$id?>','_s3','_s2');"><img src="/images/down.gif" border="0">&nbsp;remove from group</a></td></tr>
		</table>
		</div>
		<div id="<?=$id?>_s3" style="display:none;position:absolute;">
		<table width="140" cellpadding="3" cellspacing="0" align="left">
		<tr><td align="left" bgcolor="#FAFAFA" style="border-style:solid;border-width:1px;border-color:#000000;"><a href="javascript:showgroups('<?=$id?>','_s2','_s3');"><img src="/images/down.gif" border="0">&nbsp;add to group</a></td></tr>
		<tr><td align="left" bgcolor="#FAFAFA" style="border-style:solid;border-width:1px;border-color:#000000;"><a href="javascript:showgroups('<?=$id?>','_s1','_s3');"><img src="/images/down2.gif" border="0">&nbsp;remove from group</a></td></tr>
		<?php foreach($groups as $group_id => $group_title):?>
		<tr><td align="left" bgcolor="#FAFAFA" style="border-style:solid;border-width:1px;border-color:#000000;"><a style="color:#663333" href="<?=$urlstring.'&delufrmgrp='.$group_id.'&user='.$id?>"><?=toHTML(funcs_cutLength($group_title,15))?></a></td></tr>
		<?php endforeach;?>
		</table>
		</div>
		</td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td colspan="2">
		<table cellpadding="0" cellspacing="0">
		<tr>
			<td style="padding-right:10px" align="left" nowrap><font color="#009966"><b><?=$name[0].' '.$name[1]?></b></font></td>
			<td>College: <a class="searchCat" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&col=<?=$college_id?>"><?=$college?></a></td>
		</tr>
		</table>
	</td>
</tr>
<tr><td colspan="2"><hr color="#000000" size="1px"></td></tr>
</table>
<?php
}
function getFriends($user_id, $page, $limit,$r_seed, $group = null,&$count){
	$start = ($page-1)*$limit;	
	//first get the limit number of friends's ids
	$friends_table = new table_friends();
	$friends_in_groups_t = new table_friends_in_groups();
	$from_tables = array();
	$from_tables[] = TABLE_FRIENDS;
	$where = '';
	if(!is_null($group)){
		//add table for specific group if one was specified
		$from_tables[] = TABLE_FRIENDS_IN_GROUPS;
		$where = ' AND '.TABLE_FRIENDS_IN_GROUPS.'.'.$friends_in_groups_t->field_groupid
					.' = '.toSQL($group).' AND '
					.TABLE_FRIENDS_IN_GROUPS.'.'.$friends_in_groups_t->field_userid
					.' = '.TABLE_FRIENDS.'.'.$friends_table->field_user2;
	}
	$query = 'SELECT SQL_CALC_FOUND_ROWS '.TABLE_FRIENDS.'.'.$friends_table->field_user2
				.' FROM '.implode(',',$from_tables).' WHERE '.TABLE_FRIENDS.'.'.$friends_table->field_user1
				." = '".$user_id."'".$where.' LIMIT '.$start.', '.$limit;
	$result = db::query($query);
	$count = sqlFunc_getCount();
	$friendsList = array();
	if ($row = db::fetch_row($result)){
		do{
			$friendsList[] = $row[0];
		}while($row = db::fetch_row($result));
	}else{
		return null;
	}
	//not get group information, profile, names of the user found
	$users_table = new table_users();
	$profile_table = new table_profile();
	$college_table = new table_colleges();
	$friendgroup_table = new table_friendgroups();
	$get_fields = array();
	$get_fields[] = TABLE_USERS.'.'.$users_table->field_name;
	$get_fields[] = TABLE_USERS.'.'.$users_table->field_permissions;
	$get_fields[] = TABLE_USERS.'.'.$users_table->field_id;
	$get_fields[] = TABLE_USERS.'.'.$users_table->field_school;
	$get_fields[] = TABLE_COLLEGES.'.'.$college_table->field_fullname;
	$get_fields[] = TABLE_PROFILE.'.'.$profile_table->field_picture;
	$get_fields[] = TABLE_FRIENDS_IN_GROUPS.'.'.$friends_in_groups_t->field_groupid;
	$get_fields[] = TABLE_FRIENDGROUPS.'.'.$friendgroup_table->field_title;
	$from_tables = array();
	$from_tables[] = TABLE_USERS;
	$from_tables[] = TABLE_PROFILE;
	$from_tables[] = TABLE_COLLEGES;
	$fields = implode(',',$get_fields);
	$from_tables = implode(',',$from_tables);
	$query = "SELECT $fields FROM ($from_tables  LEFT JOIN ".TABLE_FRIENDS_IN_GROUPS.' ON '
			.TABLE_USERS.'.'.$users_table->field_id.' = '
			.TABLE_FRIENDS_IN_GROUPS.'.'.$friends_in_groups_t->field_userid.' AND '
			.TABLE_FRIENDS_IN_GROUPS.'.'.$friends_in_groups_t->field_ownerid
			." = '$user_id') LEFT JOIN "
			.TABLE_FRIENDGROUPS.' ON '
			.TABLE_FRIENDGROUPS.'.'.$friendgroup_table->field_groupid.' = '
			.TABLE_FRIENDS_IN_GROUPS.'.'.$friends_in_groups_t->field_groupid
			.' WHERE '.TABLE_USERS.'.'.$users_table->field_id
				.' = '.TABLE_PROFILE.'.'.$profile_table->field_id
				.' AND '.TABLE_USERS.'.'.$users_table->field_id.' IN ('.toSQL($friendsList).')'
				.' AND '.TABLE_COLLEGES.'.'.$college_table->field_id
				.' = '.TABLE_USERS.'.'.$users_table->field_school
				.' ORDER BY '.TABLE_USERS.'.'.$users_table->field_name;
	$result = db::query($query);
	return $result;
}
?>