<form name="post_f" action="/trades/post?item=<?=POST_READERS?>&edit=1" method="post">
<?php ob_start('processForm');?>
<input type="hidden" name="id">
<table width="100%" cellpadding="5" cellspacing="5">
<tr><td class="blkBrd">
<h1>A brief message</h1>
<p>Please review the information that you provided about this reader and if you find anything to be incorrect 
you may edit it by clicking on the edit button. Your reader is now posted and is available on the Trading Space 
for everyone to see. If someone wishes to purchase this reader, we will notify you immediately by email 
(and that someone will contact you through our messaging system). In our mail we will further explain what to do 
next and how this works (if you are not yet familiar with the process).</p>
<h1>Review</h1>
<table cellpadding="3" cellspacing="3">
<tr><td colspan="2">
course: <input type="text" size="40" name="course" disabled>
</td></tr>
<tr><td colspan="2">
</td></tr>
<tr valign="top"><td>Comments: <br><font class="dimmedTxt" style="font-size:10px;"><i>optional</i></font></td>
<td><textarea name="comments" cols="40" rows="5" disabled></textarea></td></tr>
</table>
<table cellpadding="3" cellspacing="0">
<tr><td>Title:</td>
<td><input type="text" name="title" size="45" disabled><font class="dimmedTxt"> (leave blank if none)</font></td></tr>
<tr><td>Year Published:</td>
<td>
<select name="year" disabled>
<option value="-1"></option>
<?php
$currentYear = date('Y',mktime());
for ($i=-4;$i<=0;$i++){
	echo '<option value="'.($currentYear+$i).'">'.($currentYear+$i).'</option>';
}
?>
</select>
<font class="dimmedTxt">(year used)</font></td></tr>
<tr><td>Professor[s]:</td>
<td><input type="text" name="teacher" size="30" disabled></td></tr>
<tr><td>Semester/Quarter:</td>
<td>
<select name="quarter" disabled>
<option value="-1"></option>
<option value="fall">Fall</option>
<option value="winter">Winter</option>
<option value="spring">Spring</option>
<option value="summer">Summer</option>
</select>
</td></tr>
<tr><td colspan="2">
<table cellpadding="3" cellspacing="0">
<tr>
	<td>Price: $<input type="text" name="price" size="3" maxlength="3" disabled>.00</td>
	<td width="50">&nbsp;</td>
	<td>Condition: <select name="condition" disabled>
				<option value="-1"></option>
				<option value="new">New</option>
				<option value="like new">Like New</option>
				<option value="good">Good</option>
				<option value="acceptable">Acceptable</option>
				</select>
	</td>
</tr>
</table>
<?php ob_end_flush();?>
<center><input type="submit" value="Edit" class="custButton">&nbsp;&nbsp;<a href="/trades/post">I'm done here, post another</a></center>
</td></tr>
</table>
</form>