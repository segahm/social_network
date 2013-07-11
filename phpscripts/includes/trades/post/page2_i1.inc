<script language="javascript">
var flag = false;
function almostSubmit(){
	var f = document.post_f;
	if (flag && f.department.value != "-1" && f.coursenumb.value != ""){
		f.action = "/trades/post?item=<?=POST_TEXTBOOKS?>&step=course";
		f.submit();
	}
}
function chgFlag(){
	flag = true;
}
</script>
<?php ob_start('processForm');?>
<form name="post_f" action="/trades/post?item=<?=POST_TEXTBOOKS?>&step=2" method="post">
<input type="hidden" name="id" value="">
<table width="100%" cellpadding="5" cellspacing="5">
<tr><td class="blkBrd">
<p>Please complete the following information about your textbook</p>
<?php if (!is_null($error = $control->getError())):?>
<font class="error"><b>Error: </b><?=$error?></font>
<?php endif;?>
<table cellpadding="3" cellspacing="3">
<tr><td colspan="2"><h1>Textbook</h1>
<font class="dimmedTxt">Please complete the official info about this textbook</font>
</td></tr>
<tr><td align="right">ISBN:</td><td><input type="text" name="isbn" size="13"> <font class="dimmedTxt" style="font-size:10px;"><i>optional</i></font></td></tr>
<tr><td align="right">Title:</td><td><input type="text" name="title" size="50"></td></tr>
<tr><td align="right">Author:</td><td><input type="text" name="author" size="50"></td></tr>
<tr><td colspan="2">
<h1>Course Information</h1>
<font class="dimmedTxt">Please fill out information about the course this textbook was (or can be) used for</font>
</td></tr>
<tr>
	<td align="right">Department: </td>
	<td>
	<select name="department" onChange="javascript:chgFlag();">
	<option value="-1"/>
	<?php
	$fields = $control->getFields();
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
<tr><td align="right">Professor[s]: </td><td><input type="text" name="teacher" size="30"></td></tr>
<tr><td colspan="2"><h1>General</h1></td></tr>
<tr><td colspan="2">
	<table cellspacing="3" cellspacing="0">
	<tr>
		<td>Price: </td><td>$<input type="text" name="price" size="3" maxlength="3">.00</td>
		<td width="50"></td>
		<td>Condition: </td>
			<td>
				<select name="condition">
				<option value="-1"/>
				<option value="new">New</option>
				<option value="like new">Like New</option>
				<option value="good">Good</option>
				<option value="acceptable">Acceptable</option>
				</select>&nbsp;&nbsp;<a target="_blank" 
				href="javascript:window.open('/html/book_cond.html','_self');">condition guideline</a>
			</td>
	</tr>
	</table>
</td></tr>
<tr valign="top"><td>Comments: <br><font class="dimmedTxt" style="font-size:10px;"><i>optional</i></font></td><td><textarea name="comments" cols="40" rows="5"></textarea></td></tr>
<tr><td colspan="2" align="center"><input type="submit" value="post" class="custButton"></td></tr>
</table>
</td></tr>
</table>
</form>
<?php ob_end_flush();?>