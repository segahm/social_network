<?php
//set server parameter
$_SERVER['DOCUMENT_ROOT'] = substr(__FILE__,0,strlen(__FILE__)-strlen('/tools/day_cron.php'));
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/DBconnect.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/picts/picts_control.inc");
log_writeLog('running daily cron');
//how many individual entries are needed to insert a new course
define('MANUAL_COURSE_SCORE',2);
$picts_t = new table_pictures();
$ids_t = new table_ids();
$events_t = new table_events();
$delEvents_t = new table_deletedevents();
$table_rideboard = new table_rideboard();
$table_tempemails = new table_tempemails();
$table_tempusers = new table_tempusers();
$table_tempfriends = new table_tempfriends();
$albm_t = new table_albums();
$blog_t = new table_blog();
$blogstngs_t = new table_blogsettings();
$courses_t = new table_courses();
$mycourses_t = new table_mycourses();
$users_t = new table_users();
db::connect();
/*delete blog posts not connected to blog settings*/
{
	$query = 'DELETE QUICK FROM '.TABLE_BLOG.' USING '.TABLE_BLOG.' LEFT JOIN '.TABLE_BLOGSETTINGS
		.' ON '.TABLE_BLOG.'.'.$blog_t->field_settingsid.' = '
		.TABLE_BLOGSETTINGS.'.'.$blogstngs_t->field_id.' WHERE '
		.TABLE_BLOGSETTINGS.'.'.$blogstngs_t->field_id.' IS NULL';
	db::query($query);
}
/*delete all old events*/
{
	$query = 'DELETE QUICK FROM '.TABLE_DELETEDEVENTS.' USING '.TABLE_EVENTS.','.TABLE_DELETEDEVENTS
		.' WHERE '.TABLE_EVENTS.'.'.$events_t->field_date.' < CURDATE() AND '.TABLE_EVENTS.'.'.$events_t->field_id.' = '
		.TABLE_DELETEDEVENTS.'.'.$delEvents_t->field_eventid;
	db::query($query);	
	$query = 'DELETE QUICK FROM '.TABLE_IDS.' USING '.TABLE_EVENTS.','.TABLE_IDS
		.' WHERE '.TABLE_EVENTS.'.'.$events_t->field_date.' < CURDATE() AND '
		.TABLE_IDS.'.'.$ids_t->field_ownerid.' = '.TABLE_EVENTS.'.'.$events_t->field_id;
	db::query($query);
	$query = 'DELETE QUICK FROM '.TABLE_EVENTS.' WHERE '.TABLE_EVENTS.'.'.$events_t->field_date.' < CURDATE()';
	db::query($query);
}
//delete all old rides
$query = 'DELETE QUICK FROM '.TABLE_RIDEBOARD.' WHERE '.TABLE_RIDEBOARD.'.'.$table_rideboard->field_date.' < CURDATE()';
db::query($query);
//delete temporary friends
$query = 'DELETE QUICK FROM '.TABLE_TEMPFRIENDS.' WHERE DATE_ADD('.$table_tempfriends->field_date.',INTERVAL 3 DAY) < CURDATE()';
db::query($query);
//delete temporary emails
$query = 'DELETE QUICK FROM '.TABLE_TEMPEMAILS.' WHERE DATE_ADD('.$table_tempemails->field_date.',INTERVAL 3 DAY) < CURDATE()';
db::query($query);
//delete temporary users (unregisted)
$query = 'DELETE QUICK FROM '.TABLE_TEMPUSERS.' WHERE DATE_ADD('.$table_tempusers->field_date.',INTERVAL 3 DAY) < CURDATE()';
db::query($query);
//now update purchase count for all users (set it to 0)
$query = 'UPDATE '.TABLE_USERS.' SET '.$users_t->field_purchases.' = 0';
db::query($query);
//now delete everything regarding old unused pictures
{
	//get all picture ids that have no corresponding album
	$query = 'SELECT '.TABLE_PICTURES.'.'.$picts_t->field_pictureid.','
		.TABLE_PICTURES.'.'.$picts_t->field_albumid
		.' FROM '.TABLE_PICTURES.' LEFT JOIN '.TABLE_ALBUMS.' ON '
		.TABLE_PICTURES.'.'.$picts_t->field_albumid.' = '
		.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' WHERE '
		.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' IS NULL';
	$result = db::query($query);
	$count = 0;
	$id_list = array();	//album id list
	while ($row = db::fetch_row($result)){
		$count++;
		unlink($_SERVER['DOCUMENT_ROOT'].Picts::getPictureUrl($row[0]));
		$thmb_file = $_SERVER['DOCUMENT_ROOT'].Picts::getSmallUrl($row[0]);
		if (file_exists($thmb_file)){
			unlink($thmb_file);
		}
		$id_list[] = $row[1];
	}
	//now delete pictures
	if ($count != 0){
		$query = 'DELETE QUICK FROM '.TABLE_PICTURES.' WHERE '
			.$picts_t->field_albumid.' IN ('.toSQL($id_list)
			.') LIMIT '.$count;
		db::query($query);
	}
	//now delete all entries in ids table
	//WARNING!!! - for this to work album_id length needs to be unique in regards
	//to all other possible ownerid fields
	$query = 'DELETE QUICK FROM '.TABLE_IDS.' USING '.TABLE_IDS.' LEFT JOIN '
		.TABLE_ALBUMS.' ON '.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' = '
		.TABLE_IDS.'.'.$ids_t->field_ownerid.' WHERE '
		.'CHAR_LENGTH('.TABLE_IDS.'.'.$ids_t->field_ownerid.') = '
		.$albm_t->field_albumid_length.' AND '
		.TABLE_ALBUMS.'.'.$albm_t->field_albumid.' IS NULL';
	db::query($query);
}
//resize all pictures that were uploaded yesterday
$query = 'SELECT '.$picts_t->field_pictureid.' FROM '.TABLE_PICTURES.' WHERE '
	.'DATE_SUB(CURDATE(),INTERVAL 1 DAY) <= '.$picts_t->field_datetime;
$result = db::query($query);
while ($row = db::fetch_row($result)){
	$file = $_SERVER['DOCUMENT_ROOT'].Picts::getPictureUrl($row[0]);
	//resize image to the maximum image size
	//get its original size
	$array = getimagesize($file);
	$width = $array[0];
	$height = $array[1];
	$resizeByWidth = true;
	$newWidth = $width;
	$newHeight = $height;
	//find new dimensions
	funcs_imageResize($newWidth,$newHeight,Picts::MAX_WIDTH_IMAGE,$resizeByWidth);
	//set permissions in case we messed them up
	funcs_resizeImageFile($file,$newWidth, $newHeight, $width,$height,null,100);
	if (!chmod($file,0777))log_writeLog('failed chmod on:'.$file);
	//create small image
	$new_file = $_SERVER['DOCUMENT_ROOT'].Picts::getSmallUrl($row[0]);
	$width = $newWidth;
	$height = $newHeight;
	//find new dimensions
	$resizeByWidth = true;
	funcs_imageResize($newWidth,$newHeight,Picts::THUMB_WIDTH,$resizeByWidth);
	$resizeByWidth = false;
	funcs_imageResize($newWidth,$newHeight,Picts::THUMB_HEIGHT,$resizeByWidth);
	funcs_resizeImageFile($file,$newWidth, $newHeight, $width,$height,$new_file);
	if (!chmod($new_file,0777))log_writeLog('failed chmod on:'.$new_file);
}
//add new courses based on entries entered by users
{
	//select all manually entered courses that do not have a corresponding entry in courses table
	$query = 'SELECT count(*) as rating,'.TABLE_MYCOURSES.'.*,'.TABLE_USERS.'.'.$users_t->field_school
		.' FROM '.TABLE_MYCOURSES.','.TABLE_USERS.' LEFT JOIN '.TABLE_COURSES
		.' ON '.TABLE_MYCOURSES.'.'.$mycourses_t->field_number.' = '
			.TABLE_COURSES.'.'.$courses_t->field_number.' AND '
			.TABLE_USERS.'.'.$users_t->field_school.' = '.TABLE_COURSES.'.'.$courses_t->field_collegeid.' AND '
			.TABLE_MYCOURSES.'.'.$mycourses_t->field_department.' = '
			.TABLE_COURSES.'.'.$courses_t->field_fieldid.' WHERE '.TABLE_MYCOURSES.'.'.$mycourses_t->field_myid
			.' = '.TABLE_USERS.'.'.$users_t->field_id.' AND '.TABLE_COURSES.'.'.$courses_t->field_number
			.' IS NULL GROUP BY '.TABLE_USERS.'.'.$users_t->field_school.','
				.TABLE_MYCOURSES.'.'.$mycourses_t->field_department.','	
				.TABLE_MYCOURSES.'.'.$mycourses_t->field_number;
	$result = db::query($query);
	$courses = array();
	while ($row = db::fetch_assoc($result)){
		//only add if rating is high enough
		if ($row['rating'] < MANUAL_COURSE_SCORE){
			continue;
		}
		unset($row['rating']);
		$courses[] = $row;
	}
	//now add newly acquired courses
	if (!empty($courses)){
		$fields = array();
		$fields[] = $courses_t->field_collegeid;
		$fields[] = $courses_t->field_description;
		$fields[] = $courses_t->field_fieldid;
		$fields[] = $courses_t->field_number;
		$query = 'INSERT DELAYED INTO '.TABLE_COURSES.' ('.sqlFields($fields).') VALUES ';
		$flag = false;
		foreach($courses as $course){
			if ($flag){
				$query .= ',';
			}
			$flag = true;
			$fields = array();
			$fields[] = $course[$users_t->field_school];
			$fields[] = $course[$mycourses_t->field_course];
			$fields[] = $course[$mycourses_t->field_department];
			$fields[] = $course[$mycourses_t->field_number];
			$query .= '('.toSQL($fields).')';
		}
		db::query($query);
	}
}
//perform optimization on tables
$query = 'OPTIMIZE NO_WRITE_TO_BINLOG TABLE '.TABLE_EVENTS.','.TABLE_DELETEDEVENTS.','
	.TABLE_RIDEBOARD.','.TABLE_TEMPFRIENDS.','
	.TABLE_TEMPEMAILS.','.TABLE_TEMPUSERS.','.TABLE_IDS.','.TABLE_PICTURES.','.TABLE_BLOG;
db::query($query);
db::close();
log_writeLog('finished running daily cron');
?>