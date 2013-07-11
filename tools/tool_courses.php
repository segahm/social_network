<?php
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/DBconnect.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/tables.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/utils/funcs.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/utils/sqlFunc.inc");
require_once('tool_config.inc');
db::connect();
class ToolsCourses extends ToolConfig{
	function __construct(){
		if (!empty($_POST['colleges']) && isset($_POST['update'])){
			//update
			if (!empty($_POST['newlist']) && !empty($_POST['departments'])){
				$array = split(":",trim($_POST['newlist']));
				foreach ($array as $value){
					$this->insertnew();
				}
			}
			if (!empty($_POST['edit']) && !empty($_POST['courses']) && !isset($_POST['removei'])){
				$this->edit();	
			}
			if (isset($_POST['removei']) && !empty($_POST['courses'])){
				$this->delete();
			}
			if (isset($_POST['removeall'])){
				$this->removeAll();
			}
		}
	}
	private function removeAll(){
		$c_t = new table_courses();
		$query = 'DELETE FROM '.TABLE_COURSES.' WHERE '.$c_t->field_collegeid." = '".$_POST['colleges']."'";
		db::query($query);
	}
	private function delete(){
		$courses_table = new table_courses();
		$query = "DELETE FROM ".TABLE_COURSES." WHERE ".$courses_table->field_id."='".$_POST['courses']."'"
			.' LIMIT 1'; 
		db::query($query);
	}
	private function insertNew(){
		$courses_table = new table_courses();
		$main_mask = ((!empty($_POST['main_mask']))?$_POST['main_mask']:'/;/');
		$inner_mask = ((!empty($_POST['inner_mask']))?$_POST['inner_mask']:'/:/');
		$array = preg_split($main_mask,$_POST['newlist']);
		$values_string = '';
		foreach ($array as $value){
			$temp = preg_split($inner_mask,$value);
			if (trim($value) === '' || count($temp) != 2 || trim($temp[1]) === ''){
				continue;
			}
			if (!empty($values_string))$values_string.=',';
			$values_string .= "(".toSQL(preg_replace('/[ ]+/',' ',strtolower(trim($temp[1])))).","
				.toSQL(trim($temp[0])).",'".$_POST['colleges']."','".$_POST['departments']."')";
		}
		$array = null;
		if (empty($values_string)){
			return;
		}
		$query = "INSERT INTO ".TABLE_COURSES." ("
			.$courses_table->field_description.","
			.$courses_table->field_number.","
			.$courses_table->field_collegeid.","
			.$courses_table->field_fieldid.') VALUES '.$values_string;
		db::query($query); 
	}
	private function edit(){
		$courses_table = new table_courses();
		$query = "UPDATE ".TABLE_COURSES." SET "
			.$courses_table->field_description."='".trim($_POST['edit'])."'"
			." WHERE ".$courses_table->field_id."='".$_POST['courses']."' LIMIT 1";
		db::query($query); 
	}
	
}
$control = new ToolsCourses();
$college_table = new table_colleges();
$dep_t = new table_departments();
$courses_table = new table_courses();
if (!empty($_POST['colleges'])){
	$selectedC = $_POST['colleges'];
}else{
	$selectedC = -1;
}
if (!empty($_POST['departments'])){
	$selectedD = $_POST['departments'];
}else{
	$selectedD = -1;
}
if (!empty($_POST['courses'])){
	$selectedCou = $_POST['courses'];
}else{
	$selectedCou = -1;
}
?>
<html>
<body>
<form name="main" action="/tools/tool_courses.php" method="post">
<table width="100%">
<tr valign="top">
	<td>
	Colleges:<br>
	<select name="colleges" onChange="document.main.submit();">
	<option value=""/>
	<?php
	$result = $control->getCollegeNames(); 
	while ($row = db::fetch_row($result)){
		echo '<option ';
		if ($row[0] == $selectedC){
			echo 'selected';
		} 
		echo ' value="'.$row[0].'">'.$row[1].'</option>';
	}
	?>
	</select><br>
	Departments:<br>
	<select name="departments"  onChange="document.main.submit();" size="5">
	<option value=""/>
<?php
if ($selectedC != -1){
	$result = sqlFunc_getDepartments($selectedC);
	while ($row = db::fetch_assoc($result)){
		echo '<option ';
		if ($row[$dep_t->field_id] == $selectedD){
			echo 'selected ';
		}
		echo 'value="'.$row[$dep_t->field_id].'">'.$row[$dep_t->field_description].'</option>';
	}
}
?>
	</select><br>
	Courses:<br>
<select name="courses" size="5">
<option value=""/>
<?php
if ($selectedC != -1){
	if ($selectedD!=-1){
		$result = sqlFunc_getCourses($selectedC,$selectedD);
	}else{
		$result = sqlFunc_getCourses($selectedC);
	}
	while ($row = db::fetch_assoc($result)){
		echo '<option ';
		if ($row[$courses_table->field_id] == $selectedCou){
			echo 'selected ';
		}
		echo 'value="'.$row[$courses_table->field_id].'">'.$row[$courses_table->field_description].'</option>';
	}
}
?>
	</select><br><br>
	<input type="submit" value="submit">
	</td>
	<td>
	New Courses: (use the following format: 'course #:course name;')<br>
	<table><tr valign="top"><td><textarea name="newlist" rows="10" cols="30"></textarea></td>
	<td>main mask: <input type="text" name="main_mask"><br>inner mask:<input type="text" name="second_mask"></td></table><br>
	edit: <input type="text" name="edit" value=""><br>
	remove: <input type="checkbox" name="removei" value="1"><br>
	update: <input type="checkbox" name="update" value="1"><br>
	remove all: <input type="checkbox" name="removeall" value="1">
	</td>
</tr>
</table>
</form>
</body>
</html>
<?php
db::close();
?>