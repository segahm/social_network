<?php
$users_t = new table_users();
$profile_t = new table_profile();
$message_t = new table_messages();
$mail = $controller->checkMail();
$picture_file = funcs_getPictureURL($mail[$profile_t->field_picture]);
$size = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture_file);
$width = $size[0];
$height = $size[1];
funcs_imageResize($width, $height, 60, false);
$name = funcs_getNames($mail[$users_t->field_name]);
$messageid = $mail[$message_t->field_messageid];
$school = $mail[$users_t->field_school];
?>
<table width="100%" cellpadding="2" cellspacing="0" 
	style="border-width:1px;border-color:#000000;border-style:solid;">
<tr valign="middle">
	<td style="padding-left:5px;">
	<a href="/search?col=<?=$school?>&type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($name[0].' '.$name[1])?>">
	<img border="0" src="<?=$picture_file?>" width="<?=$width?>" height="<?=$height?>">
	</a>
	</td>
	<td>
	<table width="100%" cellpadding="0" cellspacing="0">
	<tr><td>
	You have a new message from  
	<a href="/search?col=<?=$school?>&type=<?=SEARCH_TYPE_PEOPLE?>&q=
	<?=urlencode($name[0].' '.$name[1])?>" class="ilnk">
	<?=$name[0].' '.$name[1]?></a>, 
	click <a href="/message?view=<?=$messageid?>&read=1">here</a> 
	to read it now.
	</td></tr>
	<tr><td align="center"><a href="/message?view=<?=$messageid?>&read=1">
	<img border="0" src="/images/mailbox.gif" alt="you've got mail"></a></td></tr>
	</table>
	</td>
</tr>
</table>