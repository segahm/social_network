<table width="100%" cellpadding="5" cellspacing="5">
<tr><td class="blkBrd">
<h1>A brief message</h1>
<p>Please review the already posted textbook info and click on edit (at the bottom of the page) if you find something to be incorrect.</p>
<p>For your convenience we have come up with a brief description of how this works. First and foremost, 
your textbook is now available to other partyHub members at your college. If someone decides to purchase your book, 
you will be notified immediately by our own message system & an actual email where we will further explain the procedure and what is 
expected of you.</p>
<?php ob_start('processForm');?>
<form method="post" action="/trades/post?edit=1&item=<?=POST_TEXTBOOKS?>">
<input type="hidden" name="id">
<table cellpadding="3" cellspacing="3">
<tr><td colspan="2">
<h1>Review</h1>
</td></tr>
<tr><td align="right">ISBN:</td><td><input type="text" name="isbn" size="13" disabled></td></tr>
<tr><td align="right">Title:</td><td><input type="text" name="title" size="40" disabled></td></tr>
<tr><td align="right">Author:</td><td><input type="text" name="author" size="40" disabled></td></tr>
<tr><td colspan="2">Course: <input  type="text" size="40" name="course" disabled></td></tr>
<tr><td align="right">Professor[s]: </td><td><input type="text" name="teacher" size="30" disabled></td></tr>
<tr><td colspan="2">
	<table cellspacing="3" cellspacing="0">
	<tr>
		<td>Price: </td><td>$<input type="text" name="price" size="3" maxlength="3" disabled>.00</td>
		<td width="50"></td>
		<td>Condition: </td>
		<td>
			<select name="condition" disabled>
			<option value="new">New</option>
			<option value="like new">Like New</option>
			<option value="good">Good</option>
			<option value="acceptable">Acceptable</option>
			</select>
		</td>
	</tr>
	</table>
</td></tr>
<tr valign="top"><td>Comments:</td><td><textarea name="comments" cols="40" rows="5" disabled></textarea></td></tr>
<tr><td colspan="2" align="center"><input type="submit" value="Edit Posting" class="custButton">&nbsp;
<a href="/trades/post">I'm done here, let's post another</a>
</td></tr>
</table>
</form>
</td></tr>
</table>
<?php ob_end_flush();?>