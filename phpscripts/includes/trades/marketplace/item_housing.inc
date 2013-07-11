<?php
$face_housing = new face_housing();
$images = array();
if (isset($iteminfo[$face_housing->image])){
	$images = $iteminfo[$face_housing->image];
}
?>
<table width="100%" cellpadding="5" cellspacing="0">
<tr><td>
	<p><b><font style="font-family:verdana,arial,helvetica,sans-serif;">
		<?=toHTML(funcs_cutLength($iteminfo[$face_housing->title],200,30))?></font></b><br>
		date posted: <?=date('F jS, Y',$timestamp)?></p>
</td></tr>
<tr valign="top"><td>
		<p style="margin-bottom:5px;">Posted by <a class="ilnk" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($names[0].' '
		.$names[1])?>&col=<?=$collegeid?>"><?=$names[0].' '.$names[1]?></a>&nbsp;<a style="text-decoration:none" title="<?=$names[0]?>'s cumulative score" href="/trades/marketplace?feedback=1&id=<?=$userid?>">(<?=($feedback[0]-$feedback[2])?>)</a>&nbsp;-&nbsp;<a href="/trades/marketplace?feedback=1&id=<?=$userid?>">view feedback</a></p>
 		<a title="<?=$names[0]?>'s feedback is <?=$feedback_perc?>%">
		<table cellpadding="0" cellspacing="0">
		<tr><td>
		<table bgcolor="#009933" height="10" width="<?=(int)((150/100)*$feedback_perc)?>"><tr><td></td></tr></table>
		</td><td>
		<table bgcolor="#CCCCCC" height="10" width="<?=(int)(150-((150/100)*$feedback_perc))?>"><tr><td></td></tr></table>
		</td></tr>
		</table>
		</a>
		<table cellpadding="2" cellspacing="2">
		<tr bgcolor="#333366"><td colspan="2" style="color:#FFFFFF;padding-left:5px;" align="left">Housing Information</td></tr>
		<tr valign="top"><td class="tbltL">Location:</td><td class="tbltR" width="200">
		<?=toHTML(funcs_cutLength($iteminfo[$face_housing->location],null,30))?><br>
		<a href="http://maps.google.com/maps?oi=map&q=<?=urlencode($iteminfo[$face_housing->location])?>">google map</a>&nbsp;&nbsp;
		</td></tr>
		<tr valign="top"><td class="tbltL">Living Arrangements:</td><td class="tbltR"><?=toHTML(funcs_cutLength($iteminfo[$face_housing->arrangement],null,30))?></td></tr>
		<tr valign="top"><td class="tbltL">Utilities Included:</td><td class="tbltR"><?=($iteminfo[$face_housing->utils] == 1)?'Yes':'No'?></td></tr>
		<tr valign="top"><td class="tbltL">Looking For: <font class="dimmedTxt" style="font-size:10px">(male/female)</font></td><td class="tbltR"><?=ucfirst($iteminfo[$face_housing->sex])?> <font class="dimmedTxt" style="font-size:10px">(please do not contact if you do not match this criteria)</font></td></tr>
		<tr><td colspan="2" style="border-top-width:1px;border-top-style:solid;border-top-color:#000000;">&nbsp;</td></tr>
		</table>
</td></tr>
<tr><td style="padding:0px;">
<p style="margin-bottom:0px;font-size:14px;font-weight:bold">Price: <font color="#CC3300">$<?=$iteminfo[$face_housing->price]?></font></p>
</td></tr>
<tr><td><font style="font-size:14px;">More info: </font><?=toHTML(funcs_cutLength($iteminfo[$face_housing->more],null,30))?></td></tr>
<tr><td align="center">
<?php 
$tds = array();
foreach($images as $image){
	$image_file = ITEM_IMAGES_FOLDER.$image;
	$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$image_file);
	$width = $array[0];
	$height = $array[1];
	$resizeByWidth = true;
	funcs_imageResize($width,$height,220,$resizeByWidth);
	$tds[] = '<td><a href="'.$image_file.'"><img src="'.$image_file.'" width="'.$width.'" height="'.$height.'" border="1" style="border-color:#000000;"></a></td>';
}
if (!empty($tds)){
	echo '<table cellpadding="3" cellspacing="3">';
	echo funcs_arrangeColumns($tds,2);
	echo '</table>';
}
?>
</td></tr>
<tr><td>
<hr>
<p>Please Note: the above information was provided by the person mentioned above and makes him 
or her solely responsible for the content posted. Any questions regarding wherever this information 
is indeed correct should be addressed only to that person. If you are interested about this item you may contact him/her by
following the steps below</p> 
<center><form method="post" action="/trades/marketplace?buy=<?=$id?>&user=<?=$userid?>">
<table><tr valign="top"><td align="center"><input type="submit" value="express my interest" class="custButton"><br>
<?=($excededPurchases === true)?'<font class="error">you have exceeded the maximum number of purchases today</font>':'(The posting will be removed)'?>
</td><td> or <a href="/message?outbox=1&id=<?=$userid?>">ask a question here</a></td></tr></table>
</form></center>
</td></tr>
</table>