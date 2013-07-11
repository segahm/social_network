<?php
$face_other = new face_other();
?>
<table width="100%" cellpadding="5" cellspacing="0">
<tr><td>
	<p><b><font style="font-family:verdana,arial,helvetica,sans-serif;">
		<?=toHTML(funcs_cutLength($iteminfo[$face_other->title],200,30))?></font></b><br>
		date posted: <?=date('F jS, Y',$timestamp)?></p>
</td></tr>
<tr valign="top">
<td>
		<p style="margin-bottom:5px;">Seller:  <a class="ilnk" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($names[0].' '
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
</td>
</tr>
<tr><td><div style="float: left;margin-bottom:10px;margin-top:0px;margin-left:0px;margin-right:10px;">
	<?php
	if (isset($iteminfo[$face_other->image])){
		$image_file = ITEM_IMAGES_FOLDER.$iteminfo[$face_other->image];
		$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$image_file);
		$width = $array[0];
		$height = $array[1];
		$resizeByWidth = true;
		funcs_imageResize($width,$height,100,$resizeByWidth);
		echo '<a href="'.$image_file.'"><img src="'.$image_file.'" width="'.$width.'" height="'.$height.'" border="1" style="border-color:#000000"></a>';
	}
	?></div>
	<font style="font-size:14px;">Comments: </font><?=toHTML(funcs_cutLength($iteminfo[$face_other->more],500,30))?></td></tr>
<tr><td>
<hr>
<p>If you wish to contact the above person regarding this item/posting, we recomment that you use our official system below, 
for it will provide you with a chance to use our feedback system and a few other tools.</p> 
<center><form method="post" action="/trades/marketplace?buy=<?=$id?>&user=<?=$userid?>">
<table><tr valign="top"><td align="center"><input type="submit" value="propose to buy" class="custButton"><br>
<?=($excededPurchases === true)?'<font class="error">you have exceeded the maximum number of purchases today</font>':'(The posting will be removed)'?>
</td><td> or <a href="/message?outbox=1&id=<?=$userid?>">ask a question here</a></td></tr></table>
</form></center>
</td></tr>
</table>