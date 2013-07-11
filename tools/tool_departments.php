<?php
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/DBconnect.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/tables.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT'] 
	. "/phpscripts/includes/utils/funcs.inc");
require_once('tool_config.inc');
db::connect();
class ToolsDepartments extends ToolConfig{
	function __construct(){
		if (isset($_POST['colleges']) 
				&& $_POST['colleges'] != -1 
				&& isset($_POST['update'])){
			//add new department
			if (!empty($_POST['newlist'])){
				$this->newDepartment();
			}
			if (!empty($_POST['edit']) && !empty($_POST['departments']) 
					&& $_POST['departments'] != -1 && !isset($_POST['removei'])){
				$this->updateDepartment();
			}
			if (isset($_POST['removei']) && !empty($_POST['departments']) 
					&& $_POST['departments'] != -1){
				$this->deleteDepartment();
			}
		}
	}
	private function deleteDepartment(){
		$department_table = new table_departments();
		$query = "DELETE FROM ".TABLE_DEPARTMENTS." WHERE ".$department_table->field_id
			.' = '.$_POST['departments'].' LIMIT 1';;
		db::query($query); 
	}
	private function updateDepartment(){
		$department_table = new table_departments();
		$query = "UPDATE ".TABLE_DEPARTMENTS." SET "
			.$department_table->field_description.' = '
				.toSQL(preg_replace('/[ ]+/',' ',strtolower(trim($_POST['departments']))))
			." WHERE ".$department_table->field_id."='".$_POST['departments']."' LIMIT 1";
		db::query($query); 
	}
	private function newDepartment(){
		$mask = (!empty($_POST['mask']))?$_POST['mask']:'/:/';
		$array = preg_split($mask,$_POST['newlist']);
		foreach ($array as $key => $value){
			if (trim($value) === ''){
				unset($array[$key]);
				continue;
			}
			$array[$key] =  toSQL($this->formatValue(trim($value)));
		}
		if (empty($array)){
			return;
		}
		asort($array);
		$department_table = new table_departments();
		$query = "INSERT INTO ".TABLE_DEPARTMENTS." ("
					.$department_table->field_description.","
					.$department_table->field_collegeid.") values ";
		$query .= "(".implode(",".$_POST['colleges']."),(",$array)
			.",".$_POST['colleges'].")";
		db::query($query); 
	}
	public function getDepartments($id){
		$dep_t = new table_departments();
		$query = "SELECT * FROM ".TABLE_DEPARTMENTS." WHERE "
			.$dep_t->field_collegeid."='".$id."'";
		return db::query($query); 
	}
}
$control = new ToolsDepartments();
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
$col_t = new table_colleges();
$dep_t = new table_departments();
?>
<html>
<body>
<form name="main" action="/tools/tool_departments.php" method="post">
<table cellpadding="4">
<tr valign="top">
	<td>
	<select name="colleges" onChange="document.main.submit();">
	<option value="-1"/>
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
	</select>
	</td>
	<td>
	New Departs (use ':' to separate names):<br>
	<table><tr valign="top"><td><textarea name="newlist" rows="10" cols="30"></textarea></td>
	<td>regex mask: <input type="text" name="mask"></td></tr></table>
	Existing:<Br>
	<select name="departments" size="1">
	<option value="-1"/>
<?php
	$edit_value = '';
if ($selectedC != -1){
	$result = $control->getDepartments($selectedC);
	while ($row = db::fetch_assoc($result)){
		echo '<option ';
		if ($row[$dep_t->field_id] == $selectedD){
			echo 'selected ';
			$edit_value = $row[$dep_t->field_description];
		}
		echo 'value="'.$row[$dep_t->field_id].'">'.$row[$dep_t->field_description].'</option>';
	}
}
?>
	</select><br>
	<input type="submit" value="submit">
	</td>
	<td>
	edit: <input type="text" name="edit" value="<?=$edit_value?>"><br>
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