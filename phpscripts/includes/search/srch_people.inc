<?php
$result = $search->getResult();
$users_table = new table_users();
$college_table = new table_colleges();
$profile_table = new table_profile();
if(!is_null($result) && $person = db::fetch_assoc($result)){
$PAGE_STRING = printNextPrev($search->page,$RESULTS_OUT,$RESULT_PER_PAGE,$_SERVER['REQUEST_URI'],'page');
?>
<script language="javascript">
<!--
function delfriend(name,id){
	if (window.confirm('Are you sure you want to remove '+name+' from your list of friends?')){
		window.location = '/myconnections?removefriend='+id;
	}
}
//-->
</script>
<table width="100%">
<tr><td style="padding:10px;">
<table cellpadding="0" cellspacing="0" width="100%">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
		<table width="100%" cellpadding="0" cellspacing="0"><tr><td align="left">
		<span class="roundHdr">
		<?php 
			if (isset($_GET['friends'])){
				echo 'Searching Friends';
			}elseif(isset($_GET['intrs'])){
				echo 'Searching by Interest';
			}elseif(isset($_GET['col']) && $_GET['col'] !== AUTH_COLLEGEID){
				echo 'People Search by School';
			}else{
				echo 'People Search';
			}
		?>
		</span>
		</td>
		<td align="right">
		<font color="#336699">Showing Results <?=$RESULTS_FROM.' ~ '.$RESULTS_TO.' out of '.$RESULTS_OUT?>&nbsp;</font>
		</td></tr></table>
	</td></tr>
	<tr><td colspan="3" class="roundsides">
	<?php
	do{
		display_person($person[$users_table->field_id],
						$person[$users_table->field_name],
						$person[$users_table->field_permissions],
						!empty($person['buddy']),
						$person[$users_table->field_school],
						funcs_getPictureURL($person[$profile_table->field_picture]),
						$person[$college_table->field_fullname]);
	}while($person = db::fetch_assoc($result));
	echo $PAGE_STRING;
	?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</td></tr>
</table>
<?php
}else{
	display_notFound();
}
function display_person($id,$name,$permissions,$is_friend,$college_id,$picture_file,$college){
	$size = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture_file);
	$width = $size[0];
	$height = $size[1];
	funcs_imageResize($width, $height, 130, true);
	$name = funcs_getNames($name);
	$profile_control = new profile_control($id,AUTH_USERID,AUTH_COLLEGEID,
		$is_friend,$permissions,$college_id);
	$access = $profile_control->canAccess();
	$access_friends = $profile_control->show_friends;
	?>
<table width="100%">
<tr><td colspan="2"><hr color="#000000" size="1px"></td></tr>
<tr height="<?=$height?>">
	<td width="110" align="center">
	<?php if($access):?>
	<a href="/profile?id=<?=$id?>"><img src='<?=$picture_file?>' border="0" width="<?=$width?>" height="<?=$height?>"></a></td>
	<?php else:?>
	<img src='<?=$picture_file?>' border="0" width="<?=$width?>" height="<?=$height?>"></td>
	<?php endif;?>
	<td style="padding:10px" valign="middle">
		<table height="100%">
		<tr><td style="word-break:break-all;">
		Name: <font color="#009966"><?=$name[0].' '.$name[1]?></font>
		</td></tr>
		<tr><td>
		College: <a class="searchCat" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&col=<?=$college_id?>"><?=$college?></a>
		</td></tr>
		<tr valign="top"><td>
		<table><tr>
		<?php if($is_friend):?>
			<td>
				<a href="javascript:delfriend('<?=$name[0]?>','<?=$id?>')"><img width="30" height="30" alt="remove <?=$name[0]?> from my buddy list" src="/images/remove.gif" border="0"></a>
			</td>
		<?php elseif($id !== AUTH_USERID):?>
			<td>
		<a href="/myconnections?addfriend=<?=$id?>"><img alt="add a new friend" src="/images/addfriend.gif" border="0"></a>
			</td>
		<?php endif;?>
		<?php if($access || $profile_control->is_sameCollege() || $profile_control->is_friends()):?>
			<td>
				<a href="/message?outbox=1&id=<?=$id?>"><img width="30" height="30" alt="send <?=$name[0]?> a message" src="/images/mail.gif" border="0"></a>
			</td>
		<?php endif;?>
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
		</td></tr>
		</table>
	</td>
</tr>
</table>
	<?php
}
function display_notFound(){
global $fields;
$fields  = array();
$parser = new parser($_GET);
$col_t = new table_colleges();
$col_result = sqlFunc_getColleges($col_t->field_id.','.$col_t->field_fullname);
?>
<table width="100%">
<tr><td style="padding:20px" align="center">
<table cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
	<?php
	$fields = $_GET;
	?>		
	<?php ob_start('funcs_fillForm');?>	
	<form action="/search" method="get">
			<input type="hidden" name="type" value="1">
			<table width="100%">
			<tr valign="middle">
				<td>Search:</td>
				<td nowrap>
					<script language="javascript">
					<!--
					function displayAdv(){
						div = document.getElementById("advPsearch");
  						if (div.style.display == "none") {
  							div.style.display = "block";
  						}else{
  							div.style.display = "none";
  						}
					}
					//-->
					</script>
				 <input type="text" name="q" size="40" maxlength="255">
				 <input type="submit" value="search" class="advSearchButton">
				<a href="javascript:displayAdv();">advanced search</a>
				</td>
			</tr>
			<tr>
				<td colspan="2">
				<div id="advPsearch">
				<table width="100%">
				<tr valign="top">
					<td>
						<input type="radio" name="<?=WHATSEARCH?>" value="<?=WHATSEARCH_EMAIL?>"> - email<br>
						<input type="radio" name="<?=WHATSEARCH?>" value="<?=WHATSEARCH_FNAME?>"> - first name<br>
						<input type="radio" name="<?=WHATSEARCH?>" value="0"> - all
					</td>
					<td align="left" nowrap>
						<input type="radio" name="<?=WHATSEARCH?>" value="<?=WHATSEARCH_LNAME?>"> - last name<br>
						<input type="radio" name="<?=WHATSEARCH?>" value="<?=WHATSEARCH_FULLNAME?>"> - full name<br>
						limit: <input type="text" size="2" maxlength="2" name="limit" value="10">
					</td>
					<td valign="top" style="border-left-style:solid;border-left-color:#336699;border-left-width:1px;">
						<table width="100%">
							<tr><td nowrap>city: 
							<input type="text" name="city" size="20">
							</td></tr>
							<tr><td nowrap>high school: 
							<input type="text" name="hs" size="20">
							</td></tr>
							<tr><td>state: 
							<input type="text" name="state" size="2" maxlength="2">
							</td></tr>
						</table>
					</td>
				</tr>
				</table><br>
					College:
					<select name="col">
			<?php
			if (!isset($fields['col'])){
				$fields['col'] = AUTH_COLLEGEID;
			}
			while($college = db::fetch_assoc($col_result)){
				echo '<option value="'.$college[$col_t->field_id].'">'.$college[$col_t->field_fullname].'</option>';
			}
			?>
			</select>
					</div>
				</td>
			</tr>
			</table>
			</form>
			<?php ob_end_flush();?>
		</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
<br><img src="/images/alert.gif" border="0">
<font class="error">we were unable to find anyone based on your search</font>
</td></tr></table>
<?php
}
?>