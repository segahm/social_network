<?php
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/DBconnect.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/tables.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/utils/funcs.inc");
db::connect();
$college_table = new table_colleges();
$fields_table = new table_studyfields();
if (isset($_POST['colleges']) && !empty($_POST['colleges'])){
	$selectedC = $_POST['colleges'];
}else{
	$selectedC = -1;
}
if (isset($_POST['fields']) && !empty($_POST['fields'])){
	$selectedField = $_POST['fields'];
}else{
	$selectedField = -1;
}
//if college is specified and update is set then proceed with changes if any
if ($selectedC!=-1 && isset($_POST['update'])){
	//if new list of fields, then insert them
	if (isset($_POST['newlist']) && !empty($_POST['newlist'])){
		$array = split(":",trim($_POST['newlist']));
		foreach ($array as $value){
			$query = "INSERT INTO ".TABLE_STUDYFIELDS." ("
				.$fields_table->field_title.","
				.$fields_table->field_collegeid.") values ('"
				.trim($value)."','".$selectedC."')";
			db::query($query); 
		}
	}
	//if fieldOfStudy is selected and edit option is requested then update
	if (!empty($_POST['edit']) && $selectedField != -1 && !isset($_POST['removei'])){
		$query = "UPDATE ".TABLE_STUDYFIELDS." set "
			.$fields_table->field_title."='".trim($_POST['edit'])."'"
			." WHERE ".$fields_table->field_id."='".$selectedField."' LIMIT 1";
		db::query($query); 
	}
	//if field is selected and remove is requested then remove
	if (isset($_POST['removei']) && $selectedField != -1){
		$query = "DELETE FROM ".TABLE_STUDYFIELDS." WHERE ".$fields_table->field_id."='".$selectedField."'";
		db::query($query); 
	}
}
?>
<html>
<body>
<form name="main" action="/tools/tool_fieldsofstudy.php" method="post">
<table width="100%">
<tr valign="top">
	<td>
	<select name="colleges" size="5" onChange="document.main.submit();">
	<?php
	$query = "SELECT * FROM ".TABLE_COLLEGES;
	$result = db::query($query); 
	while ($row = db::fetch_array($result)){
		echo '<option ';
		if ($row[$college_table->field_id]==$selectedC){
		echo 'selected ';
		} 
		echo 'value="'.$row[$college_table->field_id].'">'.$row[$college_table->field_fullname].'</option>';
	}
	?>
	</select>
	</td>
	<td>
	New Fields (use ':' to separate names):<br>
	<textarea name="newlist" rows="10" cols="30"></textarea><br>
	Existing:<Br>
	<select name="fields" size=10>
<?php
if ($selectedC != -1){
$query = "SELECT * FROM ".TABLE_STUDYFIELDS." WHERE ".$fields_table->field_collegeid."='".$selectedC."'";
$result = db::query($query); 
	while ($row = db::fetch_array($result)){
		echo '<option ';
		if ($row[$fields_table->field_id] == $selectedField){
			echo 'selected ';
		}
		echo 'value="'.$row[$fields_table->field_id].'">'.$row[$fields_table->field_title].'</option>';
	}
}
?>
	</select><br>
	<input type="submit" value="submit">
	</td>
	<td>
	edit: <input type="text" name="edit" value=""><br>
	remove: <input type="checkbox" name="removei" value="1"><br>
	update: <input type="checkbox" name="update" value="1">
	</td>
</tr>
</table>
</form>
</body>
</html>
<?php
db::close();
?>