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
	. "/phpscripts/includes/utils/collegeinfo.inc");
require_once('tool_config.inc');
db::connect();
class ToolsAddCollege extends ToolConfig{
	function __construct(){
		if (!isset($_GET['new']) || empty($_GET['new'])){
			$_GET['new'] = -1;
		}
		if ($_GET['new']==2){
			$this->newCollege();
		}
		if ($_GET['new']==1){
			//unless previous is not a real college id, update previous college
			if ($_POST['colleges'] != -1 && isset($_POST['update'])){
				$this->updateCollege();
			}
		}
	}
	private function updateCollege(){
		$table = new table_colleges();
		//begin sorting majors
		$mask = (!empty($_POST['mask_major']))?$_POST['mask_major']:'/:/';
		$array = preg_split($mask,$_POST['majors']);
		foreach ($array as $key => $value){
			if (trim($value) === ''){
				unset($array[$key]);
				continue;
			}
			$array[$key] = trim($value);
		}
		asort($array);
		$i=1;
		foreach ($array as $key=>$value){
			$array[$key] = $i.';'.$value;
			$i++;
		}
		$_POST['majors'] = $this->formatValue(implode(':',$array));
		//sort houses
		$mask = (!empty($_POST['mask_houses']))?$_POST['mask_houses']:'/:/';
		$array = preg_split($mask,$_POST['houses']);
		foreach ($array as $key => $value){
			if (trim($value) === ''){
				unset($array[$key]);
				continue;
			}
			$array[$key] = trim($value);
		}
		asort($array);
		$i=1;
		foreach ($array as $key=>$value){
			$array[$key] = $i.';'.$value;
			$i++;
		}
		$_POST['houses'] = 	$this->formatValue(implode(':',$array));
		$query = "UPDATE ".TABLE_COLLEGES." SET "
			.$table->field_fullname."='".trim($_POST['full'])."', "
			.$table->field_emailformat."='".trim($_POST['emailformat'])."', "
			.$table->field_shortname."='".trim($_POST['short'])."', "
			.$table->field_majors."=".toSQL($_POST['majors']).", "
			.$table->field_houses."=".toSQL($_POST['houses']).", "
			.$table->field_more."=".toSQL(trim($_POST['more']))." WHERE "
			.$table->field_id."='".trim($_POST['colleges'])."' LIMIT 1";
		db::query($query); 
	}
	private function newCollege(){
		if (empty($_POST['college'])){
			return;
		}
		$table = new table_colleges();
		$query = 'SELECT 1 FROM '.TABLE_COLLEGES.' WHERE '.$table->field_fullname." = '".$_POST['college']
		."' LIMIT 1";
		$result = db::query($query);
		if (db::fetch_row($result)){
			return;
		}
		$query = "INSERT INTO ".TABLE_COLLEGES." (".$table->field_id.", "
			.$table->field_fullname.", ".$table->field_emailformat." , "
			.strtolower($table->field_shortname)." , ".$table->field_majors." , "
			.$table->field_houses." , ".$table->field_more.") VALUES ('',"
			."'".$_POST['college']."', '', '', '', '', '');";
		db::query($query); 
	}
	public function getCollegeData(){
		if (!isset($_POST['colleges']) || $_POST['colleges'] == -1){
			return null;
		}
		$collegeinfo = new collegeinfo($_POST['colleges']);
		return $collegeinfo;
	}
}
$control = new ToolsAddCollege();
if (isset($_POST['colleges'])){
	$selected = $_POST['colleges'];
}else{
	$selected = -1;
}
$table = new table_colleges();
?>
<html>
<body>
<form method="post" action="/tools/tool_college.php?new=2">
	<input type="text" name="college">
	<input type="submit" value="add college">
</form>
<form name="changeCollege" method="post" action="/tools/tool_college.php?new=1">
<table cellpadding="5">
<tr valign="top">
	<td>
	<select name="colleges" onChange="document.changeCollege.submit();">
	<option value="-1"/>
	<?php
	$result = $control->getCollegeNames(); 
	while ($row = db::fetch_row($result)){
		echo '<option ';
		if ($row[0] == $selected){
			echo 'selected';
		} 
		echo ' value="'.$row[0].'">'.$row[1].'</option>';
	}
	?>
	</select>
	</td>
	<td>
	<?php
	$collegeinfo = $control->getCollegeData();
	if (!is_null($collegeinfo)):
		$majorList = $collegeinfo->getMajorList();
		$houseList = $collegeinfo->getHouseList();
	?>
	EDIT:<Br>
	update (true/false): <input type="checkbox" name="update" value="1"><br>
	full name: <input type="text" name="full" value="<?=toHTML($collegeinfo->getFullname())?>"><br>
	short name: <input type="text" name="short" value="<?=toHTML($collegeinfo->getShortname())?>"><br>
	email format: @<input type="text" name="emailformat" value="<?=toHTML($collegeinfo->getEmailformat())?>"><br>
	majors (separate majors with ':'):<br> 
	<table><tr valign="top"><td><textarea name="majors" rows="5" 
	cols="40"><?=toHTML(implode(':',$majorList))?></textarea></td>
	<td>majors regex mask: <input type="text" name="mask_major"></td>
	</tr>
	</table>
	<br>
	houses (separate with ':'): <br>
	<table><tr valign="top"><td><textarea name="houses" rows="5" 
	cols="40"><?=toHTML(implode(':',$houseList))?></textarea>
	</td><td>
	houses regex mask: <input type="text" name="mask_houses">
	</td></tr></table>
	<br>
	more (separate with ':'): <br>
	<textarea  name="more" rows="5" 
	cols="40"><?=toHTML($collegeinfo->more)?></textarea><br>
	<?php endif;?>
	regex mask: <input type="submit" value="submit">
	</td>
</tr>
</table>
</form>
</body>
</html>
<?php
db::close();
?>