<script language="javascript">
var flag = false;
function almostSubmit(){
	var f = document.post_f;
	if (flag && f.department.value != "-1" && f.coursenumb.value != ""){
		f.action = "/trades/post?item=<?=POST_READERS?>&step=course";
		f.submit();
	}
}
function chgFlag(){
	flag = true;
}
</script>
<form name="post_f" action="/trades/post?item=<?=POST_READERS?>&step=2" method="post">
<table width="100%" cellpadding="5" cellspacing="5">
<tr><td class="blkBrd">
<p>Please complete some additional info about this reader</p>
<?php if (!is_null($error = $control->getError())):?>
<font class="error"><b>Error: </b><?=$error?></font>
<?php endif;?>
<table cellpadding="3" cellspacing="3">
<tr><td colspan="2">
<h1>Course Information</h1>
<font class="dimmedTxt">Please fill out information about the course this reader was (or can be) used for</font>
</td></tr>
<tr>
	<td align="right">Department: </td>
	<td>
	<?php ob_start('processForm');?>
	<input type="hidden" name="id">
	<select name="department" onChange="javascript:chgFlag();">
	<option value="-1">select a department</option>
	<?php
	$dep_t = new table_departments();
	$result = sqlFunc_getDepartments(AUTH_COLLEGEID);
	while ($row = db::fetch_assoc($result)){
		echo '<option value="'.$row[$dep_t->field_id].'">'
			.ucwords(funcs_cutLength($row[$dep_t->field_description],35))
			.'</option>';
	}
	?>
	</select>
	</td>
</tr>
<tr><td colspan="2">
<table cellpadding="3" cellpadding="0">
	<tr>
		<td>course #: <input type="text" size="3" name="coursenumb" onChange="javascript:chgFlag();"></td>
		<td>course: <input onFocus="javascript:almostSubmit();" type="text" size="40" name="course"></td>
	</tr>
</table>
</td></tr>
<tr><td colspan="2">
<h1>Additional info</h1>
</td></tr>
<tr valign="top"><td>Comments: <br><font class="dimmedTxt" style="font-size:10px;"><i>optional</i></font></td><td><textarea name="comments" cols="40" rows="5"></textarea></td></tr>
<tr><td colspan="2" align="center"><input type="submit" value="post" class="custButton"></td></tr>
</table>
<h1>Basic Info</h1>
<table cellpadding="3" cellspacing="0">
<tr><td>Title:</td>
<td><input type="text" name="title" size="45"></td></tr>
<tr><td>Year Published:</td>
<td>
<select name="year">
<option value="-1"></option>
<?php
$currentYear = date('Y',mktime());
for ($i=-4;$i<=0;$i++){
	echo '<option value="'.($currentYear+$i).'">'.($currentYear+$i).'</option>';
}
?>
</select>
<font class="dimmedTxt">(year used)</font></td></tr>
<tr><td>Professor[s]:</td>
<td><input type="text" name="teacher" size="30"></td></tr>
<tr><td>Semester/Quarter:</td>
<td>
<select name="quarter">
<option value="-1"></option>
<option value="fall">Fall</option>
<option value="winter">Winter</option>
<option value="spring">Spring</option>
<option value="summer">Summer</option>
</select>
</td></tr>
<tr><td colspan="2">
<table cellpadding="3" cellspacing="0">
<tr>
	<td>Price: $<input type="text" name="price" size="3" maxlength="3">.00</td>
	<td width="50">&nbsp;</td>
	<td>Condition: <select name="condition">
				<option value="-1"></option>
				<option value="new">New</option>
				<option value="like new">Like New</option>
				<option value="good">Good</option>
				<option value="acceptable">Acceptable</option>
				</select>&nbsp;&nbsp;<a target="_blank" 
				href="javascript:window.open('/html/book_cond.html','_self');">condition guideline</a>
	</td>
</tr>
</table>
<?php ob_end_flush();?>
</td></tr>
</table>
</form>