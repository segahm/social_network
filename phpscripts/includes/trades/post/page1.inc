<script language="javascript">
function display(element){
	div = document.getElementById("<?=POST_TEXTBOOKS?>");
	div.style.display = "none";
	div = document.getElementById("<?=POST_READERS?>");
	div.style.display = "none";
	div = document.getElementById("<?=POST_HOUSING?>");
	div.style.display = "none";
	div = document.getElementById("<?=POST_OTHER?>");
	div.style.display = "none";
	div = document.getElementById(element);
  	div.style.display = "block";
}
</script>
<table width="100%" cellpadding="5" cellspacing="5">
<tr><td class="blkBrd">
<p>Choose the category you wish to post to: 
<a href="javascript:display('<?=POST_TEXTBOOKS?>');">textbooks</a>, <a href="javascript:display('<?=POST_READERS?>');">readers</a>, 
<a href="javascript:display('<?=POST_HOUSING?>');">room for rent</a>, <a href="javascript:display('<?=POST_OTHER?>');">other</a>
</p>
<font class="dimmedTxt">Note: this section is restricted only to the items and services that involve money and trade. 
Trading space is not a discussion board and nor is it a place for spam related messages, so we ask you to keep it clean and consistent with its meaning.</font>
</td></tr>
<tr><td class="blkBrd">
<div id="<?=POST_TEXTBOOKS?>" style="display:none;">
<table width="100%" cellpadding="5" cellspacing="0"><tr valign="top">
<td width="250" style="border-right-color:#CCCCCC;border-right-style:solid;border-right-width:1px;">
<font color="#993300">What is an ISBN?</font>
<p style="margin-bottom:5px;">The ISBN is a 
10-digit (in some cases a 13-digit) 
identification system that allows booksellers and 
libraries to easily differentiate between books and 
other media when ordering. The ISBN refers to the 
specific edition (e.g. trade paperback, hardcover) and 
is usually located on the back cover of the book and on 
the copyright page. </p>
<table cellpadding="0" cellspacing="0"><tr valign="top"><td><i>example:</i></td>
<td><img src="/images/barcode.gif"></td></tr></table>
</td>
<td>
<h1>Textbook Sale (step 1)</h1>
<?php if (!is_null($error = $control->getError()) && isset($_GET['item']) && $_GET['item'] == POST_TEXTBOOKS):?>
<font class="error"><b>Error: </b><?=$error?></font>
<?php endif;?>
<p>Please enter one of the following:</p>
<?php $DIV_PAGE = POST_TEXTBOOKS;?>
<?php ob_start('processForm');?>
<form name="post_form1" action="/trades/post?item=<?=POST_TEXTBOOKS?>&step=1" method="post">
<table>
<tr><td><input id="isbn_r" type="radio" name="isisbn" value="1"></td><td><label for="isbn_r">ISBN #</label></td></tr>
<tr><td>&nbsp;</td><td><input type="text" name="isbn" value="" size="13" maxlength="13" onFocus="document.post_form1.isbn_r.checked = true"><font class="dimmedTxt"> ex: 7684975238</font></td></tr>
<tr><td><input id="title_r" type="radio" name="isisbn" value="0"></td><td><label for="title_r">Title</label></td></tr>
<tr><td>&nbsp;</td><td><input type="text" name="title" value="" size="40" onFocus="document.post_form1.title_r.checked = true"></td></tr>
</table>
<p align="center"><input type="submit" value="next step" class="custButton"></p>
</form>
<?php ob_end_flush();?>
</td>
</tr></table>
</div>
<div id="<?=POST_READERS?>" style="display:none;">
<h1>Course Reader (step 1)</h1>
<?php if (!is_null($error = $control->getError()) && isset($_GET['item']) && $_GET['item'] == POST_READERS):?>
<font class="error"><b>Error: </b><?=$error?></font>
<?php endif;?>
<p>Please complete the following fields</p>
<?php $DIV_PAGE = POST_READERS;?>
<?php ob_start('processForm');?>
<form name="post_form2" action="/trades/post?item=<?=POST_READERS?>&step=1" method="post">
<table cellpadding="3" cellspacing="0">
<tr><td>Title:</td>
<td><input type="text" name="title" size="45"></td></tr>
<tr><td>Year Published:</td>
<td>
<select name="year">
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
<td><input type="text" name="teacher" size="30"></td></tr>
<tr><td>Semester/Quarter:</td>
<td>
<select name="quarter">
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
	<td>Price: $<input type="text" name="price" size="3" maxlength="3">.00</td>
	<td width="50">&nbsp;</td>
	<td>Condition: <select name="condition">
				<option value="-1"></option>
				<option value="new">New</option>
				<option value="like new">Like New</option>
				<option value="good">Good</option>
				<option value="acceptable">Acceptable</option>
				</select>&nbsp;&nbsp;<a target="_blank" 
				href="javascript:window.open('/html/book_cond.html','_self');">condition guideline</a>
	</td>
</tr>
</table>
</td></tr>
</table>
<p align="center"><input type="submit" value="next step" class="custButton"></p>
</form>
<?php ob_end_flush();?>
</div>
<div id="<?=POST_HOUSING?>" style="display:none;">
<h1>A Place for Rent</h1>
<p>Please complete the following fields. <font class="dimmedTxt">Note: the use of this section is restricted 
to those who are currently renting a room/house/apartment and need more people to split the cost.</font></p>
<?php if (!is_null($error = $control->getError()) && isset($_GET['item']) && $_GET['item'] == POST_HOUSING):?>
<font class="error"><b>Error: </b><?=$error?></font>
<?php endif;?>
<form method="post" enctype="multipart/form-data" action="/trades/post?item=<?=POST_HOUSING?>">
<input type="hidden" name="MAX_FILE_SIZE" value="<?=MAX_IMAGE_SIZE?>"/>
<?php $DIV_PAGE = POST_HOUSING;?>
<?php ob_start('processForm');?>
<?php
//generate id to be passed with the form
$items_t = new table_tradeitems();
$id = str_shuffle('123456789');
$id = substr($id, 0, $items_t->field_id_length);
?>
<input type="hidden" name="id" value="<?=$id?>">
<table width="100%" cellpadding="3" cellspacing="0">
<tr><td>
<input type="checkbox" name="agree" value="1">&nbsp;I certify that I have the right to distribute these 
images and they are not obscene
</td></tr>
<tr><td>
1) <input type="file" name="userfile1" value="" style="border-width:1px;border-style:solid;border-color:#666666;">&nbsp;&nbsp;&nbsp;
3) <input type="file" name="userfile3" value="" style="border-width:1px;border-style:solid;border-color:#666666;">
</td></tr>
<tr><td>
2) <input type="file" name="userfile2" value="" style="border-width:1px;border-style:solid;border-color:#666666;">&nbsp;&nbsp;&nbsp
4) <input type="file" name="userfile4" value="" style="border-width:1px;border-style:solid;border-color:#666666;">
<br><font style="font-size:10px;" class="dimmedTxt">optional</font>
</td></tr>
<tr>
	<td>
	each file must be either gif or jpeg/jpg and shouldn't exceed 500kb
	</td>
</tr>
</table>
<table cellpadding="3" cellspacing="0">
<tr><td>Header:&nbsp;&nbsp;<input type="text" name="title" size="40"></td></tr>
<tr><td><table cellpadding="3" cellspacing="0">
<tr valign="top"><td>Location:</td><td><input type="text" name="location" size="50"><br>
<font class="dimmedTxt">ex: st# street name, city state</font></td></tr>
</table></td></tr>
<tr><td nowrap>Monthly Price:&nbsp;$<input type="text" name="price" size="3" maxlength="4">.00&nbsp;&nbsp;&nbsp;
utilities included? <input type="checkbox" name="utils" value="1">&nbsp;&nbsp;
looking for: 
<select name="sex">
<option value="-1"></option>
<option value="male">Male</option>
<option value="female">Female</option>
<option value="any">whoever</option>
</select>
</td></tr>
<tr><td>living arrangements:&nbsp;&nbsp;<input type="text" name="arrangement" size="20"> <font class="dimmedTxt">ex: living room floor, couch in the garage...</font>
</td></tr>
<tr><td>
<table cellpadding="3"><tr valign="top"><td>
More info:</td><td><textarea name="more" cols="40" rows="5"></textarea>
</td></tr></table>
</td></tr>
</table>
<center><input class="custButton" type="submit" value="post"></center>
<?php ob_end_flush();?>
</form>
</div>
<div id="<?=POST_OTHER?>" style="display:none;">
<h1>Other</h1>
<p>Please complete the following fields as much as possible.</p>
<?php if (!is_null($error = $control->getError()) && isset($_GET['item']) && $_GET['item'] == POST_OTHER):?>
<font class="error"><b>Error: </b><?=$error?></font>
<?php endif;?>
<form method="post" enctype="multipart/form-data" action="/trades/post?item=<?=POST_OTHER?>">
<input type="hidden" name="MAX_FILE_SIZE" value="<?=MAX_IMAGE_SIZE?>"/>
<?php $DIV_PAGE = POST_OTHER;?>
<?php ob_start('processForm');?>
<?php
//generate id to be passed with the form
$items_t = new table_tradeitems();
$id = str_shuffle('123456789');
$id = substr($id, 0, $items_t->field_id_length);
?>
<input type="hidden" name="id" value="<?=$id?>">
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
</div>
</td></tr>
</table>
<script language="javascript">
<?php
	//load display if item has already been set
	if (isset($_GET['item'])){
		echo 'display("'.$_GET['item'].'");';
	}
?>
</script>