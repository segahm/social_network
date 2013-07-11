<table width="100%" cellpadding="0" cellspacing="0">
	<tr height="20">
		<td width="70" class="title" nowrap>My History</td>
		<td background="/images/brightbar.gif" nowrap style="background-repeat:no-repeat;" width=20></td>
		<td class="itert" width="90%">&nbsp;</td>
	</tr>
	<tr>
		<td colspan=3 class="iterb" align="center" style="padding:5px">
		<?php
		if (!isset($_GET['page']) || !is_numeric($_GET['page']) || $_GET['page']<1){
			$page = 1;
		}else{
			$page = $_GET['page'];
		}
		define('LIMIT',5);
		$result = $control->getHistory($page,LIMIT);
		$total = sqlFunc_getCount();
		if ($row = db::fetch_assoc($result)){
			$history_t = new table_tradehistory();
			$user_t = new table_users();
			$rowOdd = true;
			?>
			
			<table width="100%" cellpadding="0" cellspacing="5">
			<?php
			do{
				$itemid = $row[$history_t->field_id];
				$name_array = funcs_getNames($row[$user_t->field_name]);
				$buyer = $row[$history_t->field_buyer];
				$coder = new itemCoder($row[$history_t->field_content],$row[$history_t->field_type]);
				$contents = $coder->getResult();
				//date purchased
				if (AUTH_USERID == $buyer){
					$isbuyer = true;
				}else{
					$isbuyer = false;
				}
				$timestamp = funcs_getTimeStamp($row[$history_t->field_date]);
				echo '<tr><td class="blkBrd">';
				echo '<table width="100%">';
				switch ($row[$history_t->field_type]){
					case TYPE_ITEM_TEXTBOOK:
						printTextbook($contents,$isbuyer);
						break;
					case TYPE_ITEM_READER:
						printReader($contents,$isbuyer);
						break;
					case TYPE_ITEM_HOUSING:
						printHousing($contents,$isbuyer);
						break;
					case TYPE_ITEM_OTHER:
						printOther($contents,$isbuyer);
						break;
				}
				echo '<tr><td><b>user: </b><a href="/search?type='.SEARCH_TYPE_PEOPLE.'&q='
					.urlencode($name_array[0].' '.$name_array[1]).'" class="ilnk">'
					.$name_array[0].' '.$name_array[1].'</a></td></tr>';
				echo '</table>';
				echo '</td></tr>';
				$rowOdd = !$rowOdd;
			}while($row = db::fetch_assoc($result));
			echo '</table>';
			$pageString = printPageNumbers($page,$total,LIMIT,$_SERVER['REQUEST_URI'],'page');
			echo '<p align="center">Page: '.$pageString.'</p>';
		}else{
			echo '<center>You do not have any recent transactions!</cemter>';
		}
		?>
		</td>
	</tr>
</table>
<?php
function printTextbook($content,$isbuyer){
	$face = new face_textbook();
	echo '<tr><td><b>title: </b>'.toHTML(funcs_cutLength($content[$face->title],null,40)).'</td></tr>';
	if (isset($content[$face->isbn])){
		echo '<tr><td><b>isbn: </b>'.toHTML($content[$face->isbn]).'</td></tr>';
	}
	echo '<tr><td><b>author: </b>'.toHTML(funcs_cutLength($content[$face->author],null,40)).'</td></tr>';
	echo '<tr><td><b>course: </b>'.toHTML(funcs_cutLength($content[$face->course],null,40)).'</td></tr>';
	echo '<tr><td><b>teacher: </b>'.toHTML(funcs_cutLength($content[$face->teacher],null,40)).'</td></tr>';
	echo '<tr><td><b>price: </b>$'.$content[$face->price].'</td></tr>';
	echo '<tr><td><b>condition: </b>'.ucfirst($content[$face->condition]).'</td></tr>';
	echo '<tr><td><b>comments: </b>'.toHTML(funcs_cutLength($content[$face->comments],null,40)).'</td></tr>';
	echo '<tr><td><b>action: </b>'.(($isbuyer)?'buyer':'seller').'</td></tr>';
}
function printReader($content,$isbuyer){
	$face = new face_reader();
	echo '<tr><td><b>title: </b>'.toHTML(funcs_cutLength($content[$face->title],null,40)).'</td></tr>';
	echo '<tr><td><b>year: </b>'.$content[$face->year].'</td></tr>';
	echo '<tr><td><b>quarter: </b>'.$content[$face->quarter].'</td></tr>';
	echo '<tr><td><b>course: </b>'.toHTML(funcs_cutLength($content[$face->course],null,40)).'</td></tr>';
	echo '<tr><td><b>teacher: </b>'.toHTML(funcs_cutLength($content[$face->teacher],null,40)).'</td></tr>';
	echo '<tr><td><b>price: </b>$'.$content[$face->price].'</td></tr>';
	echo '<tr><td><b>condition: </b>'.ucfirst($content[$face->condition]).'</td></tr>';
	echo '<tr><td><b>comments: </b>'.toHTML(funcs_cutLength($content[$face->comments],null,40)).'</td></tr>';
	echo '<tr><td><b>action: </b>'.(($isbuyer)?'buyer':'seller').'</td></tr>';
}
function printHousing($content,$isbuyer){
	$face = new face_housing();
	echo '<tr><td><b>title: </b>'.toHTML(funcs_cutLength($content[$face->title],null,40)).'</td></tr>';
	echo '<tr><td><b>location: </b>'.toHTML(funcs_cutLength($content[$face->location],null,40)).'</td></tr>';
	echo '<tr><td><b>price: </b>$'.$content[$face->price].'</td></tr>';
	echo '<tr><td><b>utilities: </b>'.(($content[$face->utils] == 1)?'Yes':'No').'</td></tr>';
	echo '<tr><td><b>looking for: </b>'.$content[$face->sex].'</td></tr>';
	echo '<tr><td><b>arrangement: </b>'.toHTML(funcs_cutLength($content[$face->arrangement],null,40)).'</td></tr>';
	echo '<tr><td><b>more: </b>'.toHTML(funcs_cutLength($content[$face->more],null,40)).'</td></tr>';
	echo '<tr><td><b>action: </b>'.(($isbuyer)?'buyer':'seller').'</td></tr>';
}
function printOther($content,$isbuyer){
	$face = new face_other();
	echo '<tr><td><b>title: </b>'.toHTML(funcs_cutLength($content[$face->title],null,40)).'</td></tr>';
	echo '<tr><td><b>price: </b>$'.$content[$face->price].'</td></tr>';
	echo '<tr><td><b>more: </b>'.toHTML(funcs_cutLength($content[$face->more],null,40)).'</td></tr>';
	echo '<tr><td><b>action: </b>'.(($isbuyer)?'buyer':'seller').'</td></tr>';
}
?>