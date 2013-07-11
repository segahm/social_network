<?php
$result = $controller->getMessages();
?>
<form name="mail_form" action="/message" method="post">
<table width="100%" cellpadding="1" cellspacing="0" 
	style="border-width:1px;border-style:solid;border-color:#000000;border-bottom-width:0px;">
<tr>
	<td align="left" style="border-bottom-width:1px;border-bottom-style:solid;border-bottom-color:#000000" 
		bgcolor="#EDEAE9" colspan="3">
		<table width="100%" cellspacing="0" cellpadding="0">
		<tr>
			<td align="left">
			Select:&nbsp;<a href="javascript:select('1')">[read]</a>&nbsp;
			<a href="javascript:select('2')">[unread]</a>&nbsp;
			<a href="javascript:select('3')">[all]</a>&nbsp;
			<a href="javascript:select('4')">[none]</a>
			</td>
			<td align="right">action:&nbsp;
				<select name="action" onChange="javascript:document.mail_form.submit();">
					<option value="-1">select action</option>
					<option value="markread">mark as read</option>
					<option value="markunread">mark as unread</option>
					<option value="delm">delete</option>
				</select>
			</td>
		</tr>
		</table>
	</td>
</tr>
<?php 
$readArray = array();
$unreadArray = array();
$i=1;
if ($row = db::fetch_assoc($result)){
	$message_t = new table_messages();
	$user_t = new table_users();
do{
	$timestamp = funcs_getTimeStamp($row[$message_t->field_date]);
	if (mktime(0,0,0,(int)date('n',$timestamp),(int)date('j',$timestamp),
		(int)date('Y',$timestamp)) == mktime(0,0,0)){
		//if today display as time
		$date = date('g\:i a',$timestamp);
	}else{
		//display as date
		$date = date('M jS',$timestamp);
	}
	$from_name = funcs_getNames($row[$user_t->field_name]);
	$schoolid = $row[$user_t->field_school];
	$subject = $row[$message_t->field_subject];
	$id = $row[$message_t->field_messageid];
	if ($row[$message_t->field_isread] == 1){
		$class_style = 'mail_read';
		$readArray[] = "selected$i";
	}else{
		$class_style = 'mail_unread';
		$unreadArray[] = "selected$i";
	}
?>
		<tr>
			<td class="<?=$class_style?>" align="left" nowrap>
			<input type="checkbox" id="selected<?=$i?>" name="selected<?=$i?>" value="<?=$id?>">
			&nbsp;&nbsp;
			<a style="text-decoration:none;" title="<?=toHTML($subject)?>" 
			href="/message?view=<?=$id?>"><?=toHTML(funcs_cutLength($subject,25))?></a></td>
			<td align="right" class="<?=$class_style?>" nowra>
			<a title="<?=$from_name[0].' '.$from_name[1]?>" 
			href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($from_name[0].' '.$from_name[1])?>&col=<?=$schoolid?>" 
			style="color:#008000;text-decoration:none">
			<?=funcs_cutLength(($from_name[0]),18)?></a></td>
			<td align="right" class="<?=$class_style?>"><?=$date?></td>
		</tr>
	<?php 
		$i++;
	}while($row = db::fetch_assoc($result));
}else{
	echo '<tr><td colspan="3" align="center" style="border-bottom-width:1px;
		border-bottom-style:solid;border-bottom-color:#000000">you currently have no messages</td></tr>';
}?>
<input type="hidden" name="totalmessages" value="<?=$i-1?>">
</form>
</table>
<script language="javascript">
<!--
	<?=toArray('read',$readArray)?>
	<?=toArray('unread',$unreadArray)?>
	function select(which){
		if (which == 1){
			if (read != null){
				for(var i=0;i<read.length;i++){
					var elem = document.getElementById(read[i])
					elem.checked = true;
				}
			}
			if (unread != null){
				for(var i=0;i<unread.length;i++){
					var elem = document.getElementById(unread[i])
					elem.checked = false;
				}
			}
		}else if(which == 2){
			if (unread != null){
				for(var i=0;i<unread.length;i++){
					var elem = document.getElementById(unread[i])
					elem.checked = true;
				}
			}
			if (read != null){
				for(var i=0;i<read.length;i++){
					var elem = document.getElementById(read[i])
					elem.checked = false;
				}
			}
		}else if(which == 3){
			if (read != null){
				for(var i=0;i<read.length;i++){
					var elem = document.getElementById(read[i])
					elem.checked = true;
				}
			}
			if (unread != null){
				for(var i=0;i<unread.length;i++){
					var elem = document.getElementById(unread[i])
					elem.checked = true;
				}
			}
		}else{
			if (read != null){
				for(var i=0;i<read.length;i++){
					var elem = document.getElementById(read[i])
					elem.checked = false;
				}
			}
			if (unread != null){
				for(var i=0;i<unread.length;i++){
					var elem = document.getElementById(unread[i])
					elem.checked = false;
				}
			}
		}
	}
//-->
</script>
<?php
function toArray($array_name,$array){
	$string = '';
	if (!empty($array)){
		$string .= 'var '.$array_name.' = new Array(';
		$flag = true;
		foreach($array as $value){
			if (!$flag){
				$string .= ',';
			}
			$flag = false;
			$string .= '"'.$value.'"';
		}
		$string .= ');';
	}else{
		$string .= 'var '.$array_name.' = null;';
	}
	return $string;
}
?>