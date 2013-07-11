
<table width="100%" cellpadding="0" cellspacing="0">
<tr><td>
<table width="100%" cellpadding="0" cellspacing="0">
	<tr height="20">
		<td width="70" class="title" nowrap>Items Sold</td>
		<td background="/images/brightbar.gif" nowrap style="background-repeat:no-repeat;" width=20></td>
		<td class="itert" width="90%">&nbsp;</td>
	</tr>
	<tr>
		<td colspan=3 class="iterb" align="center" style="padding:5px">
		<p align="left">Please note: the following is not the actual record of all items sold, but only a summary of transactions that have not yet been completed <font class="dimmedTxt">(i.e. feedback wasn't left)</font></p>
		<?php
		$purchases = $control->getSold();
		if ($row = db::fetch_assoc($purchases)){
			$history_t = new table_tradehistory();
			$user_t = new table_users();
			$rowOdd = true;
			?>
			<script language=javascript>
			<!--
			function descr(title,id){
				var newWindow = window.open("","_blank","top = 200,left = "+(document.body.clientWidth-150)+",height = 100,width = 250,resizable = no,status = no,scrollbars = no,titlebar = no,toolbar = no,menubar = no,location = no",false);
				newWindow.document.write("<html><body>Title: \""+title+"\"<br>Item #: "+id+"</body></html>");
				newWindow.document.close();
			}
			//-->
			</script>
			<table width="100%" cellpadding="2" cellspacing="0" class="blkBrd">
			<tr bgcolor="#EDEAE9"><td align="center">Title</td><td align="center">Feedback</td>
				<td align="center">Buyer</td><td align="center">Date</td></tr><?php
			do{
				$itemid = $row[$history_t->field_id];
				$name_array = funcs_getNames($row[$user_t->field_name]);
				$userid = $row[$history_t->field_buyer];
				$coder = new itemCoder($row[$history_t->field_content],$row[$history_t->field_type]);
				$contents = $coder->getResult();
				$face_var = new face_var();
				$title = $contents[$face_var->common_title];
				//date purchased
				$timestamp = funcs_getTimeStamp($row[$history_t->field_date]);
				echo '<tr valign="top" '.(($rowOdd)?'bgColor="#ffffff">':'bgColor="#E3EAEA">');
				echo '<td width="320"><a href="javascript:descr(\''.addslashes(toHTML(toHTML($title))).'\',\''.$itemid.'\');">'.toHTML(funcs_cutLength($title,200,30)).'</a></td><td align="center">';
				if (is_null($row['userleftfeedback'])){
					echo '<input type="button" onClick="javascript:document.location = \'/trades/marketplace?feedback=1&item='.$itemid.'&set='.$userid.'\';" value="rate me" style="font-size:11px;border-width:1px;border-color:#000000;background-color:#ffffff;color:#000000">';
				}
				echo '</td><td align="center"><a class="ilnk" title="search for '.$name_array[0].' '.$name_array[1].'" href="/search?type='.SEARCH_TYPE_PEOPLE.'&q='.urlencode($name_array[0].' '.$name_array[1]).'">'.$name_array[0].'</a></td>';
				echo '<td align="center">'.date('F jS',$timestamp).'</td>';
				echo '</tr>';
				$rowOdd = !$rowOdd;
			}while($row = db::fetch_assoc($purchases));
			echo '</table>';
		}else{
			echo '<center>You do not have any recent transactions!</cemter>';
		}
		?>
		</td>
	</tr>
</table>
</td></tr>
<tr><td>&nbsp;</td></tr>
<tr><td>
<table width="100%" cellpadding="0" cellspacing="0">
	<tr height="20">
		<td width="60" class="title" nowrap>My Items</td>
		<td background="/images/brightbar.gif" nowrap style="background-repeat:no-repeat;" width=20></td>
		<td class="itert" width="90%">&nbsp;</td>
	</tr>
	<tr>
		<td colspan=3 class="iterb" align="left" style="padding:10px">
		<?php
		$posted = $control->getPosted();
		if ($row = db::fetch_assoc($posted)){
			$item_t = new table_tradeitems();
			$rowOdd = true;
			?>
			<script language="javascript">
			<!--
			function delItem(itemid){
				if (window.confirm('Are you sure you want to remove this item from our trading space')){
					window.location = "/trades/marketplace?activity=1&del="+itemid;
				}
			}
			//-->
			</script>
			<table width="100%" cellpadding="2" cellspacing="0" class="blkBrd">
			<tr bgcolor="#EDEAE9"><td align="left" style="padding-left:10px;">Title</td><td align="center">Date</td><td align="center">Action</td></tr><?php
			do{
				$itemid = $row[$item_t->field_id];
				$type = $row[$item_t ->field_type];
				$coder = new itemCoder($row[$item_t ->field_content],$type);
				$contents = $coder->getResult();
				$face_var = new face_var();
				$title = $contents[$face_var->common_title];
				//date purchased
				$timestamp = funcs_getTimeStamp($row[$item_t ->field_date]);
				echo '<tr valign="top" '.(($rowOdd)?'bgColor="#FFFFFF">':'bgColor="#E3EAEA">');
				echo '<td  width="400" align="left" style="padding-left:5px;"><a href="/trades/marketplace?item='.$itemid.'&type='.$row[$item_t ->field_type].'">'.toHTML(funcs_cutLength($title,200,40)).'</a></td>';
				echo '<td align="center">'.date('F jS',$timestamp).'</td>';
				echo '<td align="center">'
					.(($type == TYPE_ITEM_TEXTBOOK || $type == TYPE_ITEM_READER)?'<a href="/trades/post?edit='.$itemid.'&item=i'.(($type == TYPE_ITEM_TEXTBOOK)?'1':'2').'"><img alt="edit this item" src="/images/edit2.gif" border="0"></a>&nbsp;':'')
					.'<a href="javascript:delItem(\''.$itemid.'\');"><img alt="remove this item" border="0" src="/images/delete.gif"></a></td>';
				echo '</tr>';
				$rowOdd = !$rowOdd;
			}while($row = db::fetch_assoc($posted));
			echo '</table>';
		}else{
			echo '<center>You currently have no items posted!</center>';
		}
		?>
		</td>
	</tr>
</table>
</td></tr>
<tr><td>&nbsp;</td></tr>
<tr><td>
<table width="100%" cellpadding="0" cellspacing="0">
	<tr height="20">
		<td width="90" class="title" nowrap>My Purchases</td>
		<td background="/images/brightbar.gif" nowrap style="background-repeat:no-repeat;" width=20></td>
		<td class="itert" width="90%">&nbsp;</td>
	</tr>
	<tr>
		<td colspan=3 class="iterb" align="center" style="padding:5px">
		<p align="left">Please note: the following is not the actual record of all purchases, but only a summary of transactions that have not yet been completed <font class="dimmedTxt">(i.e. feedback wasn't left)</font></p>
		<?php
		$purchases = $control->getPurchases();
		if ($row = db::fetch_assoc($purchases)){
			$history_t = new table_tradehistory();
			$user_t = new table_users();
			$rowOdd = true;
			?>
			<script language=javascript>
			<!--
			function descr(title,id){
				var newWindow = window.open("","_blank","top = 200,left = "+(document.body.clientWidth-150)+",height = 100,width = 250,resizable = no,status = no,scrollbars = no,titlebar = no,toolbar = no,menubar = no,location = no",false);
				newWindow.document.write("<html><body>Title: \""+title+"\"<br>Item #:"+id+"</body></html>");
				newWindow.document.close();
			}
			//-->
			</script>
			<table width="100%" cellpadding="2" cellspacing="0" class="blkBrd">
			<tr bgcolor="#EDEAE9"><td align="center">Title</td><td align="center">Feedback</td>
				<td align="center">Seller</td><td align="center">Date</td></tr><?php
			do{
				$itemid = $row[$history_t->field_id];
				$name_array = funcs_getNames($row[$user_t->field_name]);
				$userid = $row[$history_t->field_seller];
				$coder = new itemCoder($row[$history_t->field_content],$row[$history_t->field_type]);
				$contents = $coder->getResult();
				$face_var = new face_var();
				$title = $contents[$face_var->common_title];
				//date purchased
				$timestamp = funcs_getTimeStamp($row[$history_t->field_date]);
				echo '<tr valign="top" '.(($rowOdd)?'bgColor="#ffffff">':'bgColor="#E3EAEA">');
				echo '<td width="320"><a href="javascript:descr(\''.addslashes(toHTML(toHTML($title))).'\',\''.$itemid.'\');">'.toHTML(funcs_cutLength($title,200,30)).'</a></td><td align="center">';
				if (is_null($row['userleftfeedback'])){
					echo '<input type="button" onClick="javascript:document.location = \'/trades/marketplace?feedback=1&item='.$itemid.'&set='.$userid.'\';" value="rate me" style="font-size:11px;border-width:1px;border-color:#000000;background-color:#ffffff;color:#000000">';
				}
				echo '</td><td align="center"><a class="ilnk" title="search for '.$name_array[0].' '.$name_array[1].'" href="/search?type='.SEARCH_TYPE_PEOPLE.'&q='.urlencode($name_array[0].' '.$name_array[1]).'">'.$name_array[0].'</a></td>';
				echo '<td align="center">'.date('F jS',$timestamp).'</td>';
				echo '</tr>';
				$rowOdd = !$rowOdd;
			}while($row = db::fetch_assoc($purchases));
			echo '</table>';
		}else{
			echo '<center>You have no current purchases!</center>';
		}
		?>
		</td>
	</tr>
</table>
</td></tr>
</table>