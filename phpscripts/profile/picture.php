<? 
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/main.php");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/pagedata/profile_forms.php");
auth_authCheck();
$error = null;
$form = new myprofile(AUTH_USERID);
define('MAX_IMAGE_SIZE',522937);
drawMain_1("Edit Profile");
//if file was submited
function fileUpload(){
	global $form;
	if (isset($_FILES['userfile']['tmp_name'])){
		//begin error check
		if(!isset($_POST['agree'])){
			return "you must agree to our terms of use in order to continue";
		}elseif($_FILES['userfile']['size']>MAX_IMAGE_SIZE){
			return "the file is too big";
		}elseif(!empty($_FILES['userfile']['type']) 
			&& !ereg('(jpeg|gif)',$_FILES['userfile']['type'])){
			return "the image must be in jpeg format";
		}
		$new_file_name = genPictName(
			preg_replace('/^[^\.]+.([^\.]+)$/','\\1',$_FILES['userfile']['name']));
		if (is_null($new_file_name)){
			return "please upload a valid image type";
		}
		$uploadfile = $_SERVER['DOCUMENT_ROOT'].PROFILE_PICTURE_PATH.$new_file_name;
		$success = move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadfile);
		if ($success){
			if (!chmod($uploadfile,0755))log_writeLog('failed changing permissions for img file: '.$uploadfile);
			//resize image to the default image size
			$array = getimagesize($uploadfile);
			$width = $array[0];
			$height = $array[1];
			$resizeByWidth = true;
			$newWidth = $width;
			$newHeight = $height;
			funcs_imageResize($newWidth,$newHeight,200,$resizeByWidth);
			funcs_resizeImageFile($uploadfile,$newWidth, $newHeight, $width,$height);
			$form->setPicture($new_file_name);
			return null;
		}else{
			return "we were unable to upload this image (try a smaller one)";
		}
	}else{
		return null;
	}
}
$error = fileUpload();
?>
<link rel="Stylesheet" type="text/css" href="/styles/edtprf.css">
<?php
drawMain_2();
?>
<!-- main panels begins -->
<table cellpadding="0" cellspacing="0" width="100%">
<tr><td align="center" valign="top" style="padding-top:10px">
<table cellpadding="2" cellspacing="0" width="90%">
	<tr valign="bottom">
		<th class="edit_off">
			<a class="edit_links" href="/editp/general">My Profile</a>
		</th>
		<th class="edit_off">
			<a class="edit_links" href="/editp/contact">My Contact Information</a>
		</th>
		<th class="edit_off">
			<a class="edit_links" href="/editp/courses">My Courses</a>
		</th>
		<th class="edit_on">
			My Picture
		</th>
		<th class="edit_off">
			<a class="edit_links" href="/editp/css">CSS</a>
		</th>
	</tr>
	<tr>
		<td class="edit_main" colspan="5" align="left">
			<table width="100%" border="0" cellpadding="0" cellspacing="0">
			<tr valign="top">
				<td align="center">
				<table width="100%" cellpadding="5" cellspacing="0">
				<tr valign="top">
				<td>
				<!-- image begins -->
				<table cellpadding="0" cellspacing="0">
					<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
					<td class="roundedge" width="100%">&nbsp;</td>
					<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
					<tr><td colspan="3" class="roundsides" align="left">
					<span class="roundHdr">My Picture...</span>
					</td></tr>
					<tr><td colspan="3" class="roundsides" align="center">
					<hr color="#666633">
					<?php
						$picture_file = $form->getPicture();
						//resize image to the target
						$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture_file);
						$width = $array[0];
						$height = $array[1];
						funcs_imageResize($width,$height,200);
						echo '<img src="'.$picture_file.'" width="'.$width.'" height="'.$height.'" border=0>';
					?>
					</td></tr>
					<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
					<td class="roundedgebt" width="100%">&nbsp;</td>
					<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
				</table>
				<!-- image ends -->
				</td>
				<td align="left">
					<?php if (!empty($error)){
						echo '<font class="error"><b>error: </b>'.$error.'</font>';
					}?>
					<form name="picform" method="post" enctype="multipart/form-data" action="/editp/picure">
					<p>
					<input type="checkbox" name="agree" value="1"> <font style="font-size:10px;">I certify 
						that I have the right to distribute this image and it 
						is not obscene</font></p>
					<input type="hidden" name="MAX_FILE_SIZE" value="<?php echo MAX_IMAGE_SIZE?>"/>
					<input type="file" name="userfile" value="" style="border-width:1px;border-style:solid;border-color:#666666;"><br>
					<p>file must be either jpeg or jpg and shouldn't exceed 500kb (try using "<a href="/mypictures">my photo album</a>" for bigger pictures)</p>
					<center><input class="submitButton" style="background-color:#999999;" type="submit" value="upload"></center>
					</form>
				</td>
				</tr>
				</table>
				</td>
			</tr>
			</table>
	 	</td>
	 </tr>
</table>
</td></tr>
</table>
<!-- main panels ends -->
<?php
drawMain_3(false);
function genPictName($type,$id_length = 10){
	$type = strtolower($type);
	if(empty($type)){
		return null;
	}elseif ($type == "jpeg" || $type == "jpg"){
		$type = ".jpg";
	}elseif($type == "gif"){
		$type = ".gif";
	}else{
		return null;
	}
	$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
	$genid = substr($genid, 0, $id_length);
	return $genid.$type;
}
?>