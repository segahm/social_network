<? 
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/pagedata/profile_forms.php");
auth_authCheck();
$parser = new parser($_POST);
//process data
$form = new myprofile(AUTH_USERID);
drawMain_1("Edit Profile");
?>
<script language="javascript">
var firstTime = true;
function setFocus(){
	<?php if (isset($_POST['row']) && $_POST['row']>=1):?>
	document.cfrm.description<?=$_POST['row']?>.focus();
	<?php endif;?>
	
}
function checksubmit(element,row){
	//submit only if something was changed
	if (firstTime==false){
		var el = null;
		if(document.all){
			el = document.all(element);
		}else if(document.getElementById){
			el = document.getElementById(element);
		}
		if (el.value != ""){
			//which row was updated
			document.cfrm.row.value = row;
			//set actual submit to false
			document.cfrm.actsubmit.value = "0";
			document.cfrm.submit();
		}
	}
}
function changeFirstTime(){
	firstTime = false;
}
</script>
<link rel="Stylesheet" type="text/css" href="/styles/edtprf.css">
<style>
.edit_rodd{
	background-color:#FFFFFF;
}
.edit_reven{
	background-color:#CCCCCC;
}
</style>
<?php
drawMain_2("setFocus();");
?>
<!-- main panels begins -->
<table cellpadding="0" cellspacing="0" width="100%">
  <tr>
    <td align="center" valign="top" style="padding-top:10px"><?php
//process form if final submit
if (isset($_POST['actsubmit']) && $_POST['actsubmit'] == 1){
	//iterate through all course rows
	$i = 1;
	while (!empty($_POST['number'.$i])){
		//check that all fields for this row are entered
		if (!empty($_POST['description'.$i]) && !empty($_POST['departments'.$i])){
			//now we can add this to my courses replacing the old record (same course number)
			$form->addMyCourse($_POST['number'.$i],$_POST['description'.$i],$_POST['departments'.$i]);
		}
		$i++;
	}
}if (isset($_GET['coursedelete'])){
	//delte course
	$form->deleteMyCourse($_GET['coursedelete']);
}elseif($parser->getInt('row',0)!=0 
	&& $parser->getInt('departments'.$_POST['row'],0)!=0){
	//find course description
	$row =& sqlFunc_getCourseByNumb(
			AUTH_COLLEGEID,
			$_POST['departments'.$_POST['row']],
			$_POST['number'.$_POST['row']]
		);
	$courses_table = new table_courses();
	$_POST['description'.$_POST['row']] = $row[$courses_table->field_description];
}
?>
      <table cellpadding="2" cellspacing="0" width="90%">
        <tr valign="bottom">
          <th class="edit_off"> <a class="edit_links" href="/editp/general">My Profile</a> </th>
          <th class="edit_off"> <a class="edit_links" href="/editp/contact">My Contact Information</a> </th>
          <th class="edit_on"> My Courses </th>
          <th class="edit_off"> <a class="edit_links" href="/editp/picure">My Picture</a> </th>
          <th class="edit_off"> <a class="edit_links" href="/editp/css">CSS</a> </th>
        </tr>
        <tr>
          <td class="edit_main" colspan="5" align="left"><table width="500" cellpadding="0" cellspacing="1">
              <tr>
                <td colspan="3" style="background-color:#996666;color:#FFFFFF;padding-left:10px;">My Courses</td>
              </tr>
              <?php function printCourse($id,$course,$number,$fieldOfstudy,$row){?>
              <tr class="<?=(($row%2)==0)?'edit_reven':'edit_rodd'?>">
                <td style="padding-left:10px;"><a href="/discussion?msgs=1&tpc=<?=urlencode($fieldOfstudy)?>" style="color:#003366;">
                  <?=$course.' ['.ucwords($fieldOfstudy).']'?>
                  </a> </td>
                <td align="center" width="80"><a href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&course=<?=urlencode($course)?>" 
	style="color:#003399">people</a></td>
                <td align="center"><a href="/editp/courses?coursedelete=<?=$id?>"> <img src="/images/delete.gif" border="0"> </a> </td>
              </tr>
              <?php }
$TOTAL_COURSE_COUNT = 0; //keeps track of courses
$result = $form->getCourses();
$rowCount=1;
$mycourses_t = new table_mycourses();
$dep_t = new table_departments();
while ($row = db::fetch_array($result)){
	$id = $row[$mycourses_t->field_id];
	$course = $row[$mycourses_t->field_course];
	$nid = $row[$mycourses_t->field_number];
	$department = $row[$dep_t->field_description];
	printCourse($id,$course,$nid,$department,$rowCount);
	$rowCount++;
	$TOTAL_COURSE_COUNT++;
}
?>
            </table>
            <?php
		$result = sqlFunc_getDepartments(AUTH_COLLEGEID);
		?>
            <form name="cfrm" method="post" action="/editp/courses">
              <input type="hidden" name="row" value="0">
              <input type="hidden" name="actsubmit" value="1">
              <table cellspacing="5" cellpadding="0" width="100%">
                <tr>
                  <td>Department:</td>
                  <td>Course #:</td>
                  <td>Course Title:</td>
                </tr>
                <?php
		//buffer departments list
		$rows = array();
		while ($row = db::fetch_array($result)){ 
			$row[$dep_t->field_description] = ucwords(
				funcs_cutLength($row[$dep_t->field_description],25));
			$rows[] = $row;
		}
		$i = 1;
		while (!empty($_POST['number'.$i]) && $TOTAL_COURSE_COUNT<7):
		?>
                <tr>
                  <td><select name="departments<?=$i?>">
                      <?php
			//iterate through rows where value is an array
			foreach ($rows as $row){
				echo '<option ';
				if ($row[$dep_t->field_id] == $_POST['departments'.$i]){
					echo 'selected ';
				}
				echo 'value="'.$row[$dep_t->field_id].'">'
					.$row[$dep_t->field_description]
					.'</option>';
			}
			?>
                    </select>
                  </td>
                  <td><input id="number<?=$i?>" value="<?=$_POST['number'.$i]?>" type="text" name="number<?=$i?>" size="3" onChange="changeFirstTime();">
                  </td>
                  <td><input value="<?=toHTML($_POST['description'.$i])?>" type="text" name="description<?=$i?>" size="50" onFocus="checksubmit('number<?=$i?>',<?=$i?>);">
                  </td>
                </tr>
                <?php
		$i++;
		$TOTAL_COURSE_COUNT++;
		endwhile;
		//now do one more line
		?>
                <?php if($TOTAL_COURSE_COUNT <6):?>
                <tr>
                  <td><select name="departments<?php echo $i;?>">
                      <?php
			//iterate through rows where value is an array
			foreach ($rows as $row){
				echo '<option value="'.$row[$dep_t->field_id].'">'
					.$row[$dep_t->field_description]
					.'</option>';
			}
			?>
                    </select>
                  </td>
                  <td><input id="number<?=$i?>" value="" type="text" name="number<?=$i?>" size="3" onChange="changeFirstTime();">
                  </td>
                  <td><input value="" type="text" name="description<?=$i?>" size="50" onFocus="checksubmit('number<?=$i?>',<?=$i?>);">
                  </td>
                </tr>
                <?php else:?>
                <tr>
                  <td><select disabled>
                      <option value="-1"/>
                    </select>
                  </td>
                  <td><input disabled type="text" size="3">
                  </td>
                  <td><input disabled type="text" size="50">
                  </td>
                </tr>
                <?php endif;?>
              </table>
              <center>
                <input type="submit" class="submitButton" 
			style="background-color:#996666;" value="save">
              </center>
            </form></td>
        </tr>
      </table></td>
  </tr>
</table>
<!-- main panels ends -->
<?php
drawMain_3(false);
?>
