<?php
define('HOW_MANY_PIC_PERLINE',4);
require_once($_SERVER['DOCUMENT_ROOT'].'/phpscripts/includes/picts/picts_control.inc');
$result = $search->getResult();
$PAGE_STRING = printNextPrev($search->page,$RESULTS_OUT,$RESULT_PER_PAGE,$_SERVER['REQUEST_URI'],'page');
$pict_t = new table_pictures();
$albm_t = new table_albums();
?>
<table width="100%">
<tr><td style="padding:10px;">
<table cellpadding="0" cellspacing="0" width="100%">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
		<table width="100%" cellpadding="0" cellspacing="0"><tr><td align="left" style="padding:5px;">
		<p class="roundHdr">My Photo Search</p>
		</td>
		<td align="right">
		<font color="#336699">Showing Results <?=$RESULTS_FROM.' ~ '.$RESULTS_TO.' out of '.$RESULTS_OUT?>&nbsp;</font>
		</td></tr></table>
	</td></tr>
	<tr><td colspan="3" class="roundsides">
	<?php 
	if ($row = db::fetch_assoc($result)){
		$pictures = array();
		$count = 0;
		echo '<table width="100" cellpadding="3" cellspacing="3">';
		do{
			$img_file = $row[$pict_t->field_pictureid];
			$image_string = '';
			$timestamp = funcs_getTimeStamp($row[$pict_t->field_datetime]);
			if (!Mydate::isBeforeCron($timestamp)){
				$img_file = Picts::getPictureUrl($img_file);
				//resize image to the target
				$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$img_file);
				$width = $array[0];
				$height = $array[1];
				$resizeByWidth = true;
				funcs_imageResize($width,$height,Picts::THUMB_WIDTH,$resizeByWidth);
				$resizeByWidth = false;
				funcs_imageResize($width,$height,Picts::THUMB_HEIGHT,$resizeByWidth);
				$image_string = '<img src="'.$img_file.'" width="'.$width.'" height="'.$height.'" border="0">';
			}else{
				//do not resize the image since it has been physically resized to
				//the right dimensions;
				//also to limit the number of request to disk using getimagesize
				//do not specify width or height in the <img tag
				$img_file = Picts::getSmallUrl($img_file);
				$image_string = '<img src="'.$img_file.'" border="0">';
			}
			if ($count%HOW_MANY_PIC_PERLINE == 0){
				echo ($count != 0)?'</tr>':'';
				echo '<tr>';
			}
			echo '<td nowrap align="center"><a title="dated: '.date('jS F, Y',$timestamp)
				.'" href="/mypictures?pic='.$row[$pict_t->field_pictureid].'">'
				.$image_string
				.'</a><br><span style="color:#008000;">'.toHTML(funcs_cutLength($row[$pict_t->field_title],15)).'</span></td>';
			$count++;
		}while($row = db::fetch_assoc($result));
		if ($count > 0){
			if ($count%HOW_MANY_PIC_PERLINE == 0){
				echo '</tr>';
			}else{
				for ($i=0;$i<($count%HOW_MANY_PIC_PERLINE);$i++){
					echo '<td>&nbsp;</td>';
				}
				echo '</tr>';
			}	
		}
		echo '</table>';
		echo $PAGE_STRING;
	}else{
		echo '<p>it appears that you don\'t have any photos - <a href="/mypictures">post some now</a></p>';
	}
	?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</td></tr>
</table>