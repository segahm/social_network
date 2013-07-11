<?php
/*all html encoding is done by the processForm function, so DO NOT
 *USE toHTML on any values to be inserted into the form using fields array*/
$events_table = new table_events();
if (isset($_GET['submitEvent'])){
	$date = null;
	$error = $event->post_form($_POST,$date);
	$message = "your event was posted successfully for ".$date;
}
if (isset($_GET['remove'])){
	if ($event->removeEvent($_GET['remove'])){
		$message = 'your event was removed';
	}
}
$my_events = $event->getMyEventTitles();
if (!empty($error)){
	$fields = $_POST;
	unset($message);
}elseif(isset($_GET['edit'])){
	$fields = $event->getEventData($_GET['edit']);
}else{
	$fields = $event->getEventData();
}
?>
<style>
.event_add_color {color:#000000}
</style>
<script language="javascript" src="/html/utils.js"></script>
<script language="javascript">
<!--
<?=printArrays()?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/match.js");?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/binarySearch.js");?>

function frmSubmit(){
	var str = scroll_toString();
	document.new_event.selfriends.value = str;
	document.new_event.submit();
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
?>
<script language="javascript" type="text/javascript">
	<!--
	function checkIfOther(){
		div1 = document.getElementById("type_other_div");
		div2 = document.getElementById("type_party_div");
		if (document.new_event.type.value == "other"){
  			div1.style.display = "block";
		}else{
			div1.style.display = "none";
		}
		if (document.new_event.type.value == "party"){
  			div2.style.display = "block";
		}else{
			div2.style.display = "none";
		}
	}
	//-->
</script>
<?php
drawMain_2('checkIfOther()',true,$ids);
?>
<table width="500" align="left" cellpadding="5" cellspacing="0"><tr><td align="center">
<table width="100%" cellpadding="0" border="0" cellspacing="0" >
<tr>
	<td bgcolor="#336699" style="padding-left:10px;">
		<table cellpadding="3" cellspacing="0">
		<tr>
			<td align="right" nowrap>
			<a href="/events" style="font-size:12px;color:#ffffff;font-weight:bold;">Upcoming</a>
			</td>
			<td><font color="#FFFFFF">|</font></td>
			<td align="left" nowrap>
			<a href="/events?newevent=1" style="text-decoration:none;font-size:12px;color:#ffffff;font-weight:bold;">Add New Event</a>
			</td>
		</tr>
		</table>
	</td>
	<td bgcolor="#336699" align="right" nowrap>
	<b><font color="#ffffff">Edit: </font></b>
	<?php ob_start('funcs_fillForm');?>
	<select name="event_select" style="font-size:8pt" on onChange="selectedEvent(this)">
	<option value="-1">select my event</option>
	<?php foreach ($my_events as $event_id => $event_title):?>
	<option value="<?=$event_id?>"><?=toHTML(funcs_cutLength($event_title,20))?></option>
	<?php endforeach;?>
	</select>&nbsp;
	</td>
</tr>
<tr>
	<td colspan="2" style="padding:10px">
	<?php if (!empty($error)):?>
	<font color="#FF0000"><?=$error?></font>
	<?php elseif(isset($message)):?>
	<font class="error"><b>Message: </b></font><?=$message?>
	<?php endif;?>
	<form name="new_event" method="post" action="/events?newevent=1&submitEvent=1">
	<input type="hidden" name="selfriends">
	<input type="hidden" name="oldid" value="">
	<table cellpadding="3" cellspacing="0">
	<tr><td>
	<h1>What & When</h1>
		<table><tr>
		<td align="left"><font class="event_add_color">title:</font></td>
		<td align="left"><font class="event_add_color">date:</font></td>
		</tr>
		<tr>
		<td>
		<input type="text" name="<?=$events_table->field_title?>" size="30" maxlength="60">
		</td>
		<td>
		<select name="month">
		<?php 
		for ($month=1;$month<=12;$month++){
			echo '<option value="'.$month.'">'.date('M',mktime(0,0,0,$month)).'</option>';
		}
		?>
		</select>
		<select name="day">
		<?php 
		for ($day=1;$day<=31;$day++){
			echo '<option value="'.$day.'">'.$day.'</option>';
		}
		?>
		</select>
		</td>
		</tr>
		</table>
	</td></tr>
	<tr valign="top">
	<td>
		<table cellspacing="0" cellpadding="0">
		<tr valign="top">
			<td align="left" width="140">
			<font class="event_add_color">type:</font>
				<select name="<?=$events_table->field_type?>" onChange="checkIfOther()">
				<option value="other">other</option>
				<option value="academic">academic</option>
				<option value="athletic">athletic</option>
				<option value="club meeting">club meeting</option>
				<option value="exam">exam</option>
				<option value="party">party</option>
				</select>
			</td>
			<td align="left">
			<div id="type_other_div" style="display:block;">
				<input type="text" name="type_other" value="" size="20" maxlength="20"> <font class="dimmedTxt">ex. reunion</font>
			</div>
			</td>
		</tr>
		</table>
	</td>
	</tr>
	<tr><td>
	<h1>Show to...</h1>
	<!--<font color="#006699">people in my</font>-->
	<input type="radio" name="view" value="1"> - <font class="event_add_color">people in my school</font>
	<input type="radio" name="view" value="4"> - <font class="event_add_color">in my major</font>
	<input type="radio" name="view" value="2"> - <font class="event_add_color">friends only</font><br>
	<input type="radio" name="view" value="5"> - <font class="event_add_color">in my courses</font><br>
	<table width="100%" cellpadding="2" cellspacing="0">
	<tr><td align="left">
	<input type="text" name="courses" size="30" onFocus="match_focus(2,this,'3shMtch')" onBlur="match_blur()">
	</td></tr>
	<tr><td align="left">
	<div id="3shMtch" style="display:none;position:absolute;"></div>
	<font class="dimmedTxt">ex: Introduction to Arts, ...</font>
	</td></tr>
	</table>
	<input type="radio" name="view" value="3"> - <font class="event_add_color">restricted to the following friends & groups</font>
	<font class="dimmedTxt"> (existing partyHub friends/groups)</font>
	</td></tr>
	<tr><td>
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
		<td>
			<div id="1shMtch" style="display:none;position:absolute;"></div>
			<font class="dimmedTxt">ex: John Doe, mike@ucsc.edu, ...</font>
		</td>
		<td>
			<div id="2shMtch" style="display:none;position:absolute;"></div>
			<font class="dimmedTxt">ex: Project Buddies, ...</font>
		</td>
	</tr>
	</table>
	</td></tr>
	<tr>
		<td>
		<h1>More info</h1>
			<div id="type_party_div" style="display:none;">
			<table cellpadding="5">
				<tr><td align="right"><font class="event_add_color">location:</font></td><td><input type="text" name="location" size="40" maxlength="100"></td></tr>
				<tr><td align="right"><font class="event_add_color">hosted by:</font></td><td><input type="text" name="host" size="30"></td></tr>
				<tr><td align="right"><font class="event_add_color">time:</font></td><td><input type="text" name="time" size="5" maxlength="10"><font class="dimmedTxt"> ex: 2pm</font></td></tr>
			</table>
			</div>
		</td>
	</tr>
	<tr valign="top">
		<td>
			<table>
			<tr valign="top">
				<td width="70"><font class="event_add_color">message:</font><br><font class="dimmedTxt" style="font-size:9px">optional</font></td>
				<td><textarea name="description" rows="5" cols="40"></textarea></td>
			</tr>
			</table>
		</td>
	</tr>
	</table>
	</form>
	<center>
	<?php if (isset($_GET['edit'])):?>
	<input type="button" onClick="javascript:window.location = '/events?newevent=1&remove=<?=$_GET['edit']?>'" value="remove" class="custButton" style="font-size:9pt;background-color:#993333">
	<?php endif;?>
	<input type="button" onClick="frmSubmit()" value="post event" class="custButton" style="font-size:9pt"></center>
	<?php ob_end_flush();?>
	</td>
	</tr>
</table>
</td></tr></table>
<?php
/**returns arrays with friend names/emails and group titles*/
function printArrays(){
	$quickAddIn = new quickAddIn(AUTH_USERID);
	return printMatchArrays($quickAddIn->getFriends(),$quickAddIn->getGroups(),$quickAddIn->getCourses());
}
?>
