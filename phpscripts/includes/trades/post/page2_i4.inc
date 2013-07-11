<table width="100%" cellpadding="3" cellspacing="0"><tr>
<td class="blkBrd">
<h1>Other</h1>
<p>Please complete the following fields as much as possible.</p>
<?php if (!is_null($error = $control->getError()) && isset($_GET['item']) && $_GET['item'] == POST_OTHER):?>
<font class="error"><b>Error: </b><?=$error?></font>
<?php endif;?>
<form method="post" enctype="multipart/form-data" action="/trades/post?item=<?=POST_OTHER?>">
<input type="hidden" name="MAX_FILE_SIZE" value="<?=MAX_IMAGE_SIZE?>"/>
<?php $DIV_PAGE = POST_OTHER;?>
<?php ob_start('processForm');?>
<input type="hidden" name="image">
<input type="hidden" name="id">
<table width="100%" cellpadding="3" cellspacing="0">
<tr><td>
<input type="checkbox" name="agree" value="1">&nbsp;I certify that I have the right to distribute this 
image and it is not obscene
</td></tr>
<tr><td>
<input type="file" name="userfile" value="" style="border-width:1px;border-style:solid;border-color:#666666;"><font style="font-size:10px;" class="dimmedTxt"> optional</font>
</td></tr>
<tr><td>file must be either gif or jpeg/jpg and shouldn't exceed 500kb</td></tr>
</table>
<table cellpadding="3" cellspacing="0">
<tr><td>Header:&nbsp;&nbsp;<input type="text" name="title" size="40"></td></tr>
<tr><td>Price:&nbsp;$<input type="text" name="price" size="3" maxlength="4">.00</td></tr>
<tr><td>
<table cellpadding="3"><tr valign="top"><td>
More info:</td><td><textarea name="more" cols="40" rows="5"></textarea>
</td></tr></table>
</td></tr>
</table>
<center><input class="custButton" type="submit" value="post"></center>
<?php ob_end_flush();?>
</form>
</td>
</tr></table>