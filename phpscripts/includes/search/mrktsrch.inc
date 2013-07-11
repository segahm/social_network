<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/trades/trade_interface.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/trades/itemCoder.inc");
$result = $search->getResult();
$items_t = new table_tradeitems();
$isbn_t = new table_isbns();
$users_t = new table_users();
$face_isbn = new face_isbn();
$face_var = new face_var();
if(!is_null($result) && $itemrow = db::fetch_assoc($result)){
	?><table cellpadding="3" cellspacing="5">
	<?php
	do{
		printItem($itemrow);
	}while($itemrow = db::fetch_assoc($result));
	?></table><?php
}else{
	?>
	<p><img src="/images/alert.gif"> we were unable to find a match for "<?=toHTML($search->getQuery())?>"</p>
	<p>It is possible that what you are looking for is not currently listed for your college. 
	Nevertheless, we advise you to consider the following: 
	<ul>
	<li>try browing through all listings</li>
	<li>consider making your search criteria broader</li>
	</ul>
	Still can't find it and think there is a great demand for it?<br>
	Consider being the first to list/post your item/listing on our trading space and perhaps even making a little 
	profit. By doing so, you will not just have a chance to make a little extra money but you will also help our 
	community grow.
	</p>
	<?php
}
function printItem($item){
	global $items_t, $face_var, $isbn_t, $users_t,$face_isbn;
	$coder = new itemCoder($item[$items_t->field_content],$item[$items_t->field_type]);
	$content = $coder->getResult();
	$name = funcs_getNames($item[$users_t->field_name]);
	$feedback = explode('/',$item[$users_t->field_feedback]);
	$isbn_array = array();
	if (!is_null($item[$isbn_t->field_text])){
		$fields = explode('>:<',$item[$isbn_t->field_text]);
		foreach($fields as $field){
			$key = preg_replace('/([a-z]+)=>.+/i','\\1',$field);
			$value = preg_replace('/[a-z]+=>(.+)/i','\\1',$field);
			$isbn_array[$key] = $value;
		}
		$isbn_array[$face_isbn->title] = $item[$isbn_t->field_title];
		$isbn_array[$face_isbn->author] = $item[$isbn_t->field_author];
		$isbn_array[$face_isbn->isbn] = $item[$isbn_t->field_isbn];
		if (isset($isbn_array[$face_isbn->simage])){
			$picture = $isbn_array[$face_isbn->simage];
		}else if (isset($isbn_array[$face_isbn->mimage])){
			$picture = $isbn_array[$face_isbn->mimage];
		}else if (isset($isbn_array[$face_isbn->limage])){
			$picture = $isbn_array[$face_isbn->limage];
		}
	}
	?>
	<tr valign="top"><td align="center">
	<?php if (isset($content[$face_var->common_image])){
		if (!is_array($content[$face_var->common_image])){
			$picture = ITEM_IMAGES_FOLDER.$content[$face_var->common_image];
		}else{
			$picture = ITEM_IMAGES_FOLDER.$content[$face_var->common_image][array_rand($content[$face_var->common_image])];
		}
		$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture);
		$width = $array[0];
		$height = $array[1];
		$resizeByWidth = true;
		funcs_imageResize($width,$height,80,$resizeByWidth);
	}elseif($item[$items_t->field_type] == TYPE_ITEM_TEXTBOOK
		&& isset($picture)){
		//if picture's dimensions are small, then no need to resize 
		//hence do not get dimensions for it will slow things down
		if (is_null($isbn_array[$face_isbn->simage])){
			$array = getimagesize($picture);
			$width = $array[0];
			$height = $array[1];
			$resizeByWidth = true;
			funcs_imageResize($width,$height,120,$resizeByWidth);
		}
	}?>
	<a href="/trades/marketplace?item=<?=$item[$items_t->field_id]?>&type=<?=$item[$items_t->field_type]?>">
	<?php if (isset($picture)):?>
	<img src="<?=$picture?>" <?=(isset($width)?'width="'.$width.'"':'')?> <?=(isset($height)?'height="'.$height.'"':'')?> 
	border="1" style="border-color:#000000">
	</a>
	<?php else:?>
	<img src="/images/noimage.gif" border="0"
	<?php endif;?>
	</a>
	</td>
	<td align="left"><a href="/trades/marketplace?item=<?=$item[$items_t->field_id]?>&type=<?=$item[$items_t->field_type]?>" 
	style="font-size:14px;"><?=toHTML(funcs_cutLength($content[$face_var->common_title],100,30))?></a><br>
	<font style="font-size:14px;"><b>$<?=$content[$face_var->common_price]?></b> - <a class="ilnk" 
		href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($name[0].' '.$name[1])?>">
		<?=$name[0].' '.$name[1]?></a> (<?=($feedback[0] + $feedback[1] + $feedback[2])?>)</font><br>
	<?php if (isset($content[$face_var->common_comments])):?>
	<?=toHTML(funcs_cutLength($content[$face_var->common_comments],150,30))?>
	<?php elseif(isset($content[$face_var->common_more])):?>
	<?=toHTML(funcs_cutLength($content[$face_var->common_more],150,30))?>
	<?php endif;?>
	</td></tr>
	<?php
}
?>