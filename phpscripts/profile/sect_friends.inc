<!-- friends -->
<?php
	$totalResults = 0;
	$friends = $profile_access->getFriends($totalResults,SHOW_MAX_FRIENDS);
?>
    <?php if (!empty($friends)):?>
      <table cellpadding="2" width="100%" align="left">
	 <span class="prfTbHdr" style="font-size:11px;">
      <?php if ($GLOBALS['IS_ME']){echo 'My Buddies...';}else{echo funcs_getNames($data[$users_table->field_name],false)."'s Buddy List...";}?>
      </span>
	  <?php
	$numb_friends = 0;	//will be used to determine wherever friends were displayed
	$list = array();
	foreach ($friends as $array){
		$numb_friends++;
		$picture_url = funcs_getPictureURL($array[$profile_table->field_picture]);
		if (!is_null($array[$profile_table->field_picture])){
			$picture_size = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture_url);
			$width = $picture_size[0];
			$height = $picture_size[1];
			funcs_imageResize($width,$height,60,true);
		}
		$name = $array[$users_table->field_name];
		$perm = $array[$users_table->field_permissions];
		$id = $array[$users_table->field_id];
		//create a profile control for this friend of the person's profile
		$profile_control = new profile_control($id,AUTH_USERID,
			AUTH_COLLEGEID,!empty($array['buddy']),$perm,
			$array[$users_table->field_school]);
		$allow_to_see_profile =$profile_control->canAccess();
		$td = '<td align="center" width="100">';
		if ($allow_to_see_profile){
			$td .= '<a href="/profile?id='.$id.'">';
		}
		if (!is_null($array[$profile_table->field_picture])){
			$td .= '<img border="0" src="'.$picture_url.'" width="'.$width.'" height="'.$height.'">';
			if ($allow_to_see_profile){
				$td .= '</a>';
			}
			$td .= '<br><span class="ilnk" style="font-size:10px;">'.funcs_getNames($name,false).'</span></td>';
		}else{
			$td .= '<br><span class="ilnk" style="font-size:10px;">'.funcs_getNames($name,false).'</span></td>';
			if ($allow_to_see_profile){
				$td .= '</a>';
			}
		}
		$list[] = $td;
	}
	echo funcs_arrangeColumns($list,MAX_FIRENDS_PER_LINE,'<tr>');
	//if there are more friends than shown, show more
	if ($totalResults>$numb_friends){
		echo '<tr><td colspan="'.MAX_FIRENDS_PER_LINE.'" align="right"><a href="/search?type='.SEARCH_TYPE_PEOPLE.'&friends='.$profile_access->owner_id.'" class="ilnk">more...</a></td></tr>';
	}
	?>
      </table>
      <?php else:?>
      no friends
      <?php endif;?>
