<?php require_once("market_control.inc");?>
<table width="100%" cellpadding="0" cellspacing="0">
<tr valign="top">
<td>
	<table width="150" cellpadding="2" cellspacing="2">
	<tr bgcolor="#339966">
		<td style="color:#FFFFFF;font-weight:bold" align="center">Featured Books</td>
	</tr>
	<tr valign="top">
		<td align="center" valign="top" style="border-width:1px;border-color:#339966;border-style:solid;padding:10px;padding-top:5px;">
					<?php
						$books = $control->getFeaturedBooks(3);
						if (!empty($books)){
							echo '<table width="150"  cellpadding="2" cellspacing="5">';
							$items_t = new table_tradeitems();
							$face_var = new face_var();
							$face_isbn = new face_isbn();
							$face_textbook = new face_textbook();
							foreach($books as $book){
								$isbninfo = $book[$face_var->isbninfo];
								$bookinfo = $book[$face_var->iteminfo];
								$id = $book[$items_t->field_id];
								$type = $book[$items_t->field_type];
								$date = $book[$items_t->field_date];
								$user = $book[$items_t->field_user];
								$title = (isset($isbninfo[$face_isbn->title])) ? $isbninfo[$face_isbn->title] : $bookinfo[$face_textbook->title];
								$author = (isset($isbninfo[$face_isbn->author])) ? $isbninfo[$face_isbn->author] : $bookinfo[$face_textbook->author];
								$image = (isset($isbninfo[$face_isbn->mimage])) ? $isbninfo[$face_isbn->mimage] : '/images/noimage.gif';
								?>
								<tr><td align="center">
								<a href="/trades/marketplace?item=<?=$id?>&type=<?=$type?>">
								<img alt="<?=toHTML($title)?>" src="<?=$image?>" border="1" style="border-color:#000000">
								</a><br>
								<a href="/trades/marketplace?item=<?=$id?>&type=<?=$type?>" 
								style="color:#333333;text-decoration:none;font-size:10px;font-weight:bold;" 
								title="<?=toHTML($title)?>"><?=toHTML(funcs_cutLength($title,60,20))?></a><br>
								by <?=toHTML(funcs_cutLength($author,30,15))?><br>
								<font style="font-size:12px;">date: <?=date('F jS',funcs_getTimeStamp($date))?></font>
								</td></tr>
								<?php
							}
							echo '</table>';
						}else{
							echo 'no featured books available at yuor college yet<br><a href="/trades/post">post some now!</a>';
						}
					?>
		</td>
	</tr>
	</table>
</td>
<td>
	<table width="100%" cellpadding="5" cellspacing="5">
	<tr><td>
		<table width="100%" cellpadding="2" cellspacing="5">
			<tr valign="top">
						<td align="center">
						<a href="/trades/marketplace?categ=<?=TYPE_ITEM_TEXTBOOK?>">Textbooks</a><br>
						used & new textbooks for competitive prices set by college students just like you
						</td>
						<td align="center">
						<a href="/trades/marketplace?categ=<?=TYPE_ITEM_READER?>">Course Readers</a><br>
						stop buying those brand new course readers every quarter and buy a used one here
						</td>
						<td align="center">
						<a href="/trades/marketplace?categ=<?=TYPE_ITEM_HOUSING?>">For Rent</a><br>
						find a place to crash without the usual hassle
						</td>
			</tr>
			<tr valign="top">
				<td align="center">
						<a href="/trades/marketplace?categ=<?=TYPE_ITEM_OTHER?>">Uncategorized</a><br>
						other items/postings that are about to become their own category but are not quite there yet
				</td>
				<td align="center" colspan="2">
						More to come...
						</td>
			</tr>
		</table>
	</td></tr>
	<tr><td>
		<table width="350" cellpadding="2" cellspacing="2">
			<tr height="20" bgcolor="#660033">
				<td style="color:#FFFFFF">Marketplace Items Preview</td>
			</tr>
			<tr valign="top">
				<td align="left" valign="top" style="border-width:1px;border-color:#660033;border-style:solid;padding:10px;padding-top:0px;">
					<?php
						$items_t = new table_tradeitems();
						$face_var = new face_var();
						$items = $control->getFeaturedItems(10);
						if (!empty($items)){
							$count = 0;
							foreach($items as $item){
								$count++;
								$id = $item[$items_t->field_id];
								$type = $item[$items_t->field_type];
								$iteminfo = $item[$face_var->iteminfo];
								$title = $iteminfo[$face_var->common_title];
								echo $count.') <a href="/trades/marketplace?item='.$id.'&type='.$type.'">'.toHTML(funcs_cutLength($title,60,20)).'</a><br>';
							}
						}else{
							echo '<center>no marketplace items available at your college yet<br><a href="/trades/post">post some now!</a></center>';
						}
					?>
				</td>
			</tr>
		</table>
	</td></tr>
	</table>
</td>
</tr>
</table>