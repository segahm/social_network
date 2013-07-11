<?php
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/rideboard/rideboard.inc");
auth_authCheck();
//which form to display
if (!isset($_GET['form']) || ($_GET['form']!=1 && $_GET['form']!=2)){
	$_GET['form'] = 2;
}
if (!isset($_GET['type']) || ($_GET['type']!=1 && $_GET['type']!=2)){
	$_GET['type'] = 2;
}
if (!isset($_GET['limit']) || !is_numeric($_GET['limit']) || $_GET['limit']<2
	|| $_GET['limit']>20){
	$_GET['limit'] = 20;
}
//set month and day dates for default
$fields = array();
$fields['month'] = date('n');
$fields['day'] = date('j');
$rideboard = new ride(AUTH_USERID,AUTH_COLLEGEID);
//was form submited?
if (isset($_GET['submit'])){
	if ($rideboard->postForm($_GET['form'],$_POST)){
		$message = 'successfully posted!';
	}else{
		$error = 'please complete all the fields';
		$fields = $_POST;
	}
}
if (isset($_GET['remove'])){
	$rideboard->remove($_GET['remove']);
}
if (!isset($_GET['start']) || !is_numeric($_GET['start'])
	|| $_GET['start']<0){
	$_GET['start'] = 0;
}
drawMain_1("Ride Board");
?>
<script language="javascript">
<!--
function shRide(which){
	var div1 = document.getElementById('ride1');
	var div2 = document.getElementById('ride2');
	if (which == 1){
		div2.style.display = "none";
		div1.style.display = "inline";
	}else{
		div1.style.display = "none";
		div2.style.display = "inline";
	}
}
//-->
</script>
<?php
if ($_GET['form'] == 1){
	drawMain_2('shRide(1);');
}else{
	drawMain_2('shRide(2);');
}
?>
<table cellpadding="0" cellspacing="0" width="100%">
<tr><td align="center" valign="top" style="padding:10px;">
<div id="ride1" style="display:none;">
<table width="100%" cellpadding="0" border="0" 
	cellspacing="0">
<tr bgcolor="#336699">
	<td style="padding-left:10px;">
		<table cellpadding="3" cellspacing="0">
		<tr>
			<td style="color:#ffffff">Post to: </td>
			<td align="right" nowrap>
			<a href="javascript:shRide(1);" style="font-size:12px;color:#ffffff;
				font-weight:bold;text-decoration:none;">Need A Ride!</a>
			</td>
			<td><font color="#FFFFFF">|</font></td>
			<td align="left" nowrap>
			<a href="javascript:shRide(2);" style="font-size:12px;color:#ffffff;
				font-weight:bold;">Give A Ride!</a>
			</td>
		</tr>
		</table>
	</td>
	<td>&nbsp;</td>
</tr>
<tr>
	<td colspan="2" style="padding:8px" align="left" valign="top">
	<div id="inride1" style="display:block">
	<h1 style="color:#009933">Looking for a ride</h1>
	<?php if ($_GET['form'] == 1 && isset($message)):?>
	<font style="color:#003399"><b>Message: </b><?=$message?></font>
	<?php elseif($_GET['form'] == 1 && isset($error)):?>
	<font class="error"><b>Error: </b><?=$error?></font>
	<?php endif;?>
	<?php if($_GET['form'] == 1){
		ob_start('processForm');
			}?>
	<form action="/rides?form=1&submit=1" method="post">
	<table cellpadding="2">
	<tr>
		<td>To:</td>
		<td>date:</td></tr>
	<tr valign="top">
		<td align="left"><input type="text" name="to" value="" size="40" maxlength="100"><br>
		<font class="dimmedTxt"> ex: st# street name, city state</font>
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
		<select name="year">
		<option value="<?=date('Y',time())?>"><?=date('Y',time())?></option>
		<option value="<?=date('Y',time())?>"><?=(date('Y',time())+1)?></option>
		</select>
		</td>
	</tr>
	<tr><td colspan="2">From:</td></tr>
	<tr valign="top">
	<td><input type="text" name="from" value="" size="40" maxlength="100"></td>
	<td>
	approx. time:
	<input type="text" name="time" value="" size="3" maxlength="7">
	</td>
	</tr>
	<tr><td colspan="2">how many people:
	<input type="text" name="number" value="" size="2" maxlength="2">
	</td></tr>
	</table>
	<center><input type="submit" class="custButton" value="post"></center>
	</form>
	<?php if($_GET['form'] == 1){
		ob_end_flush();
	}?>
	</div>
	</td>
</tr>
</table>
</div>
<div id="ride2" style="display:none;">
<table width="100%" cellpadding="0" border="0" 
	cellspacing="0">
<tr bgcolor="#336699">
	<td style="padding-left:10px;">
		<table cellpadding="3" cellspacing="0">
		<tr>
			<td style="color:#ffffff">Post to: </td>
			<td align="right" nowrap>
			<a href="javascript:shRide(1);" style="font-size:12px;color:#ffffff;
				font-weight:bold;">Need A Ride!</a>
			</td>
			<td><font color="#FFFFFF">|</font></td>
			<td align="left" nowrap>
			<a href="javascript:shRide(2);" style="font-size:12px;color:#ffffff;
				font-weight:bold;text-decoration:none;">Give A Ride!</a>
			</td>
		</tr>
		</table>
	</td>
	<td align="right">&nbsp;</td>
</tr>
<tr>
	<td colspan="2" style="padding:8px" align="left" valign="top">
	<div id="inride2" style="display:block">
	<h1 style="color:#009933">Can give a ride</h1>
	<?php if ($_GET['form'] == 2 && isset($message)):?>
	<font style="color:#003399"><b>Message: </b><?=$message?></font>
	<?php elseif($_GET['form'] == 2 && isset($error)):?>
	<font class="error"><b>Error: </b><?=$error?></font>
	<?php endif;?>
	<?php if($_GET['form'] ==2){
		ob_start('processForm');
	}?>
	<form action="/rides?form=2&submit=1" method="post">
	<table cellpadding="2">
	<tr>
		<td>To:</td>
		<td>date:</td></tr>
	<tr valign="top">
		<td align="left"><input type="text" name="to" value="" size="40" maxlength="100"><br>
		<font class="dimmedTxt"> ex: st# street name, city state</font>
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
		<select name="year">
		<option value="<?=date('Y',time())?>"><?=date('Y',time())?></option>
		<option value="<?=date('Y',time())?>"><?=(date('Y',time())+1)?></option>
		</select>
		</td>
	</tr>
	<tr><td colspan="2">From:</td></tr>
	<tr valign="top">
	<td><input type="text" name="from" value="" size="40" maxlength="100"></td>
	<td>
	approx. time:
	<input type="text" name="time" value="" size="3" maxlength="7">
	</td>
	</tr>
	<tr><td colspan="2">seats available:
	<input type="text" name="number" value="" size="2" maxlength="2">
	</td></tr>
	</table>
	<center><input type="submit" class="custButton" value="post"></center>
	</form>
	<?php if($_GET['form'] ==2){
		ob_end_flush();
	}?>
	</div>
	</td>
</tr>
</table>
</div>
<p align="left">
view: <a <?=($_GET['type']==1)?'':'class="ilnk"'?> href="/rides?type=2">[available rides]</a>&nbsp;
<a <?=($_GET['type']==2)?'':'class="ilnk"'?> href="/rides?type=1">[in need of a ride]</a></p>
<?php
$rides = $rideboard->getRides($_GET['start'],$_GET['limit'],$_GET['type']);
$howMany = count($rides);
if ($howMany !=0 ){
	$bottomPageLinks = '';
	if ($_GET['start'] >= ($_GET['limit']-1) || $howMany == $_GET['limit']){
		$bottomPageLinks .= '<table width="100%"><tr>';
		//if there is a previous page
		if ($_GET['start'] >= ($_GET['limit']-1)){
			$bottomPageLinks .= '<td align="left">'
				.'<a href="'.preg_replace('/[&]?start=[^&]+/i','',$_SERVER['REQUEST_URI'])
				.'&start='.($_GET['start']-$_GET['limit']+1).'">&#60;&#60;Prev</a></td>';
		}
		/*if this page is full then we can print the link to the next page
		 *where the start will be the last element of this page*/
		if ($howMany == $_GET['limit']){
			$bottomPageLinks .= '<td align="right">'
				.'<a href="'.preg_replace('/[&]?start=[^&]+/i','',$_SERVER['REQUEST_URI'])
				.'&start='.($_GET['start']+$_GET['limit']-1).'">Next&#62;&#62;</a></td>';
		}
		$bottomPageLinks .= '</tr></table>';
	}
	require_once($_SERVER['DOCUMENT_ROOT']
		."/phpscripts/includes/rideboard/printRides.inc");
}else{
	echo '<center>no results were found</center>';
}
?>
<?=(isset($bottomPageLinks)) ? $bottomPageLinks : ''?>
</td></tr></table>	
<?php
drawMain_3(true);
function &processForm(){
	global $fields;
	$contents =& ob_get_contents();
	if (empty($fields)){
		return $contents;
	}
	foreach ($fields as $key => $value){
		$fields[$key] = toHTML($value);
	}
	$contents = funcs_processForm($contents,$fields);
	return $contents;
}
?>