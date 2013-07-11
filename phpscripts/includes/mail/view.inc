<?php
$message = $controller->getMessage();
$message_t = new table_messages();
$users_t = new table_users();
$from_name = funcs_getNames($message[$users_t->field_name]);
$schoolid = $message[$users_t->field_school];
$timestamp = funcs_getTimeStamp($message[$message_t->field_date]);
$messageid = $message[$message_t->field_messageid];
$personid = $message[$message_t->field_from];
?>
<?php if (!empty($controller->viewMessage)):?>
<p style="color:#993333"><?=$controller->viewMessage?></p>
<?php endif;?>
<table width="100%" cellpadding="0" cellspacing="0">
<!-- old color: EDEAE9-->
<tr><td bgcolor="#EDEAE9" class="blkBrd" style="border-bottom-width:0px;padding:2px;">
	Action: <a style="text-decoration:none" href="/messages?delm=<?=$messageid?>">[delete]</a>
	&nbsp;<a style="text-decoration:none" href="/messages?markread=<?=$messageid?>&view=<?=$messageid?>">[read]</a>
	&nbsp;<a style="text-decoration:none" href="/messages?markunread=<?=$messageid?>&view=<?=$messageid?>">[unread]</a>
</td></tr>
<tr><td>
	<table width="100%" cellpadding="3" cellspacing="0">
	<tr valign="top">
		<td width="50" align="right" class="hdrCol" bgcolor="#EDEAE9">Subject:</td>
		<td class="hdrCol2"><?=funcs_cutLength($message[$message_t->field_subject],null,40)?></td>
	</tr>
	<tr valign="top">
		<td bgcolor="#EDEAE9" width="50" align="right" class="hdrCol">From:</td>
		<td class="hdrCol2">
		<a class="ilnk" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($from_name[0].' '.$from_name[1])?>&col=<?=$schoolid?>">
		<?=$from_name[0].' '.$from_name[1]?></a>&nbsp;
		<a style="text-decoration:none" href="javascript:document.reply.submit();">[reply]</a>
		</td>
	</tr>
	<tr valign="top">
		<td bgcolor="#EDEAE9" width="50" align="right" class="hdrCol">Date:</td>
		<td class="hdrCol2"><?=date('g\:i a, jS F, Y',$timestamp)?></td>
	</tr>
	</table>
</td></tr>
<tr valign="top"><td class="blkBrd" height="100" style="padding:5px;">
<div style="word-wrap:break-word;display:inline;overflowX:hidden;overflow:hidden;width:500">
	<?=$message[$message_t->field_body]?>
</div>
</td></tr>
</table>
<form name="reply" method="post" action="/messages?outbox=1&id=<?=$personid?>&reply=<?=$messageid?>">
<input type="hidden" name="subject" value="<?=toHTML($message[$message_t->field_subject])?>">
<input type="hidden" name="body" value="<?=toHTML($message[$message_t->field_body])?>">
<input type="hidden" name="time" value="<?=$timestamp?>">
</form>