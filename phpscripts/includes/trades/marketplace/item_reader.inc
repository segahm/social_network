<?php
$face_reader = new face_reader();
?>
<table width="100%" cellpadding="5" cellspacing="0">
<tr><td>
	<p><b><font style="font-family:verdana,arial,helvetica,sans-serif;">
		<?=toHTML(funcs_cutLength($iteminfo[$face_reader->title],200,30))?></font></b><br>
		teacher: <?=toHTML(funcs_cutLength($iteminfo[$face_reader->teacher],200,30))?><br>
		<b>date posted:</b> <?=date('F jS, Y',$timestamp)?></p>
</td></tr>
<tr valign="top"><td>
	<table width="100%" cellpadding="5" cellspacing="0">
	<tr valign="top">
		<td width="120">
		<img src="/images/reader.gif">
		<p style="margin-bottom:0px;font-size:14px;font-weight:bold">Price: <font color="#CC3300">$<?=$iteminfo[$face_reader->price]?></font></p>
		</td>
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
		<table cellpadding="2" cellspacing="2">
			<tr><td style="color:#FFFFFF;padding-left:5px;" bgcolor="#660033" colspan="2" align="left">Course Information</td></tr>
			<tr valign="top"><td class="tbltL">Course:</td><td class="tbltR" width="200"><?=toHTML(funcs_cutLength($iteminfo[$face_reader->course],200,30))?></td></tr>
			<tr valign="top"><td class="tbltL">Teacher:</td><td class="tbltR"><?=toHTML(funcs_cutLength($iteminfo[$face_reader->teacher],200,30))?></td></tr>
			<tr valign="top"><td class="tbltL">Quarter (Year):</td><td class="tbltR"><?=ucfirst($iteminfo[$face_reader->quarter]).' ('.$iteminfo[$face_reader->year].')'?></td></tr>
			<tr><td colspan="2" style="border-top-width:1px;border-top-style:solid;border-top-color:#000000;">&nbsp;</td></tr>
		</table>
		</td>
	</tr>
	</table>
</td></tr>
<tr><td style="padding:0px;">
<font style="font-size:14px;">Condition: <font color="#CC3300">
	<?=ucfirst($iteminfo[$face_reader->condition])?></font></font>
</td></tr>
<tr><td><font style="font-size:14px;">Comments: </font><?=toHTML(funcs_cutLength($iteminfo[$face_reader->comments],500,30))?></td></tr>
<tr><td>
<hr>
<p>Please Note: the description of the reader is based on the information the seller provided. If you wish 
to purchase this reader you may do so by following the steps below where you will have a chance to ask the 
seller a question and express your buying preferences. It is very important that you use our official process 
for this purchase (i.e. do not contact the seller directly), for we have means to provide the two parties 
with an opportunity to leave feedback for one another.</p> 
<center><form method="post" action="/trades/marketplace?buy=<?=$id?>&user=<?=$userid?>">
<table><tr valign="top"><td align="center"><input type="submit" value="propose to buy" class="custButton"><br>
<?=($excededPurchases === true)?'<font class="error">you have exceeded the maximum number of purchases today</font>':'(The posting will be removed)'?>
</td><td> or <a href="/message?outbox=1&id=<?=$userid?>">ask a question here</a></td></tr></table>
</form></center>
</td></tr>
</table>