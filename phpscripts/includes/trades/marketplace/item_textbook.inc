<?php
	$face_textbook = new face_textbook();
	$face_isbn = new face_isbn();
	$isbninfo = $item[$face_var->isbninfo];
	$width = null;
	$height = null;
	$border = 1;
	$image = $face_isbn->getMajorImage($isbninfo,$width,$height,$border);
	$imageString = '<img src="'.$image.'" border="'.$border.'" '
		.'alt="'.((isset($isbninfo[$face_isbn->title]))?$isbninfo[$face_isbn->title]:toHTML($iteminfo[$face_textbook->title])).'"'
		.((!is_null($width))?' width="'.$width.'" height="'.$height.'"':'').'>';
	?>
	<table width="100%" cellpadding="5" cellspacing="0">
	<tr><td>
	<p><b><font style="font-family:verdana,arial,helvetica,sans-serif;">
		<?=toHTML(funcs_cutLength($iteminfo[$face_textbook->title],null,30))?></font></b><br>
		by <?=toHTML(funcs_cutLength($iteminfo[$face_textbook->author],null,50))?><br>
		<b>date posted:</b> <?=date('F jS, Y',$timestamp)?></p>
	</td></tr>
	<tr valign="top">
		<td><table cellpadding="5" cellspacing="0"><tr valign="top">
		<td><?=$imageString?>
		<p style="margin-bottom:0px;font-size:14px;font-weight:bold">Price: <font color="#CC3300">$<?=$iteminfo[$face_textbook->price]?></font></p>
		</td>
		<td>
		<p style="margin-bottom:5px;">Seller:  <a class="ilnk" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($names[0].' '
		.$names[1])?>&col=<?=$collegeid?>"><?=$names[0].' '.$names[1]?></a>&nbsp;<a style="text-decoration:none" title="<?=$names[0]?>'s cumulative score" href="/trades/marketplace?feedback=1&id=<?=$userid?>">(<?=($feedback[0]-$feedback[2])?>)</a>&nbsp;-&nbsp;<a href="/trades/marketplace?feedback=1&id=<?=$userid?>">view feedback</a></p>
 		<a title="<?=$names[0]?>'s feedback is <?=$feedback_perc?>%">
		<table cellpadding="0" cellspacing="0"><tr><td>
		<table bgcolor="#009933" height="10" width="<?=(int)((150/100)*$feedback_perc)?>">
		<tr><td></td></tr>
		</table>
		</td><td>
		<table bgcolor="#CCCCCC" height="10" width="<?=(int)(150-((150/100)*$feedback_perc))?>">
		<tr><td></td></tr>
		</table>
		</td></tr></table>
		</a>
		<table width="100%" cellpadding="2" cellspacing="2">
			<tr height="20" bgcolor="#996633">
				<td style="color:#FFFFFF;padding-left:5px;" align="left">Official Information</td>
			</tr>
			<tr valign="top">
				<td align="left">
					<table width="100%" cellpadding="2" cellspacing="0">
					<?php if (isset($iteminfo[$face_textbook->isbn])):?>
					<tr valign="top"><td width="80" class="tbltL">ISBN:</td><td class="tbltR"><?=preg_replace('/([a-z0-9]{1,3})([a-z0-9]{1,3})([a-z0-9]{1,3})([a-z0-9]{1,4})/i','\\1-\\2-\\3-\\4',$iteminfo[$face_textbook->isbn])?></td></tr>
					<?php endif?>
					<?php if (isset($isbninfo[$face_isbn->publisher])):?>
					<tr valign="top"><td width="80" class="tbltL">Publisher:</td><td class="tbltR"> <?=$isbninfo[$face_isbn->publisher]?></td></tr>
					<?php endif?>
					<?php if (isset($isbninfo[$face_isbn->binding])):?>
					<tr valign="top"><td width="80" class="tbltL">Binding:</td><td class="tbltR"> <?=$isbninfo[$face_isbn->binding]?></td></tr>
					<?php endif?>
					<?php if (isset($isbninfo[$face_isbn->listprice])):?>
					<tr valign="top"><td width="80" class="tbltL">Retail Price:</td><td class="tbltR"> <?=$isbninfo[$face_isbn->listprice]?></td></tr>
					<?php endif?>
					<?php if (isset($isbninfo[$face_isbn->pages])):?>
					<tr valign="top"><td width="80" class="tbltL"># of Pages:</td><td class="tbltR"> <?=$isbninfo[$face_isbn->pages]?></td></tr>
					<?php endif?>
					<?php if (isset($isbninfo[$face_isbn->title])):?>
					<tr valign="top"><td width="80" class="tbltL">Official Title:</td><td class="tbltR"><?=$isbninfo[$face_isbn->title]?></td></tr>
					<?php endif?>
					<tr><td colspan="2" style="border-top-width:1px;border-top-style:solid;border-top-color:#000000;">&nbsp;</td></tr>
					</table>
				</td>
			</tr>
		</table>
		</td>
		</tr></table></td>
	</tr>
	<tr><td style="padding:0px;">
	<table cellpadding="0" cellspacing="0">
	<tr valign="top"><td><font style="font-size:14px;">Condition: <font color="#CC3300">
	<?=ucfirst($iteminfo[$face_textbook->condition])?></font></font>&nbsp;&nbsp;&nbsp;</td>
	<td><font style="font-size:14px;">Course, Teacher: </font><?=toHTML(funcs_cutLength($iteminfo[$face_textbook->course],null,30))?>, <?=toHTML(funcs_cutLength($iteminfo[$face_textbook->teacher],null,30))?><br>
	<span class="dimmedTxt" style="font-size:10px;">(was/can be used for)</span></td></tr>
	</table>
	</td></tr>
	<tr><td><font style="font-size:14px;">Comments: </font><?=toHTML(funcs_cutLength($iteminfo[$face_textbook->comments],500,30))?></td></tr>
	<tr><td>
	<hr>
	<p>Please Note: the description of the book is based on the information the seller provided. 
	If you wish to purchase this textbook you may do so by following the steps below where you will have a chance to ask the seller a 
	question and express your buying preferences. It is very important that you use our 
	official process for this purchase (i.e. do not contact the seller directly), for we have means to provide the two parties 
	with an opportunity to leave feedback for one another.</p> 
	<center><form method="post" action="/trades/marketplace?buy=<?=$id?>&user=<?=$userid?>">
	<table><tr valign="top"><td align="center"><input type="submit" value="propose to buy" class="custButton"><br>
	<?=($excededPurchases === true)?'<font class="error">you have exceeded the maximum number of purchases today</font>':'(The posting will be removed)'?>
	</td><td> or <a href="/message?outbox=1&id=<?=$userid?>">ask a question here</a></td></tr></table>
	</form></center>
	</td></tr>
	</table>