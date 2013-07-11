<?php
function &feedbackForm(){
	foreach ($_POST as $key => $value){
		$_POST[$key] = toHTML($value);
	}
	$contents =& ob_get_contents();
	$contents = funcs_processForm($contents,$_POST);
	return $contents;
}
if ($control->getResponse() == MARKET_PAGE_FEEDBACK_1){
	if (isset($_POST['rate']) && isset($_POST['text']) && is_null($control->error)){
		?>
		<p>Thank you for taking time to leave feedback for this user and thus helping us strengthen Trading Space community. 
		We want to let you know that we greatly appreciate your effort to make our Trading Space a more 
		resourceful and safe place for all of its members. Please post your items for trade often and help 
		us grow even more.<font color="#CC0000">Thank You</font></p><br>
		click <a href="/trades/marketplace?feedback=1&id=<?=$_GET['set']?>">here</a> to see the feedback you've left for this user
		<?php
	}else{
		?>
		<form action="/trades/marketplace?feedback=1&item=<?=$_GET['item']?>&set=<?=$_GET['set']?>" method="post">
		<?php if (!empty($control->error)):?>
		<p style="color:#FF0000"><?=$control->error?></p>
		<?php endif;?>
		<?php ob_start('feedbackForm');?>
		<table cellpadding="2" cellspacing="0">
		<tr><td colspan="2">Please shortly describe your experience with this user while using Trading Space. We ask you to be very reasonable in this desicision since this feedback will remain permanently on the person's record.</td></tr>
		<tr>
			<td>
		<select name="rate">
			<option value="default"></option>
			<option value="1" style="color:#006633">positive</option>
			<option value="0" style="color:#000000">neutral</option>
			<option value="-1" style="color:#CC0000">negative</option>
		</select>
			</td>
			<td>
			<input type="text" name="text" size="80" maxlength="200">
			</td>
		</tr>
		</table>
		<?php ob_end_flush();?>
		<center><input type="submit" value="submit" class="custButton"></center>
		</form>
		<?php
	}
}else{
	$data = null;
	if (!isset($_GET['page']) || !is_numeric($_GET['page']) || $_GET['page']<1){
		$page = 1;
	}else{
		$page = $_GET['page'];
	}
	define('LIMIT',20);
	$result = $control->viewFeedback($data,$page,LIMIT);
	if ($result === false){
		header("Location: http://".$_SERVER['HTTP_HOST'].$control->newlocation);
		exit;
	}
	$users_t = new table_users();
	$my_name = funcs_getNames($data[$users_t->field_name]);
	$my_id = $data[$users_t->field_id];
	$my_feedbacksummary = explode('/',$data[$users_t->field_feedback]);
	$my_membersince = funcs_getTimeStamp($data[$users_t->field_membersince]);
	?>
	<table width="400" cellpadding="2" cellspacing="2">
	<tr><td nowrap bgcolor="#996666" style="color:#FFFFFF">Feedback summary for <b><?=($my_id == AUTH_USERID)?'Me':$my_name[0].' '.$my_name[1]?></b></td></tr>
	<td style="border-width:1px;border-color:#006699;border-style:solid;padding:5px;">
		<table width="100%" cellpadding="0" cellspacing="0"><tr valign="top"><td>
			<table cellpadding="0" cellspacing="2">
			<tr><td><font color="#006633">Positive Feedback: <b><?=$my_feedbacksummary[0]?></b></font></td></tr>
			<tr><td>Percent Positive: <b><?=(100/(max($my_feedbacksummary[0]+$my_feedbacksummary[1]+$my_feedbacksummary[2],1))*$my_feedbacksummary[0])?>%</b></td></tr>
			<tr><td>Total Count: <?=($my_feedbacksummary[0]+$my_feedbacksummary[1]+$my_feedbacksummary[2])?></td></tr>
			<tr><td>Neutral: <b><?=$my_feedbacksummary[1]?></b></td></tr>
			<tr><td><font color="#CC0000">Negative: <b><?=$my_feedbacksummary[2]?></b></font></td></tr>
			</table>
		</td><td>
		<p style="padding:5px;">member since: <?=date('F jS, Y',$my_membersince)?><br>
		score: <b><?=($my_feedbacksummary[0]-$my_feedbacksummary[2])?></b></p>
		</td></tr></table>
	</td>
	</table>
	<?php
	$feedback_t = new table_tradefeedback();
	if ($row = db::fetch_assoc($result)){
		?>
		<h1 style="margin-bottom:0px;">Comments:</h1>
		<table width="100%" cellpadding="3" cellspacing="0" class="blkBrd" style="border-top:0px;">
		<tr bgcolor="#EDEAE9"><td align="center">Item #:</td><td align="center">Date:</td>
		<td align="center">User:</td><td align="left" width="300">Feedback:</td></tr>
		<?php
		$rowOdd = true;
		$fdbk_images = array(1 => 'feedback_pos.gif',0 => 'feedback_neut.gif',-1 => 'feedback_neg.gif');
		do{
			$itemid = $row[$feedback_t->field_itemid];
			$user_name = funcs_getNames($row[$users_t->field_name]);
			$user_id = $row[$feedback_t->field_user];
			$user_feedbksmry = explode('/',$row[$users_t->field_feedback]);
			$score = $row[$feedback_t->field_score];
			$text = $row[$feedback_t->field_comment];
			$date = funcs_getTimeStamp($row[$feedback_t->field_date]);
			echo '<tr valign="top" '.(($rowOdd)?'bgColor="#ffffff">':'bgColor="#E3EAEA">');
			echo '<td align="center">'.$itemid.'</td>';
			echo '<td nowrap>&nbsp;'.date('F j, Y',$date).'</td>';
			echo '<td nowrap>&nbsp;<a class="ilnk" href="/trades/marketplace?feedback=1&id='.$user_id.'">'.$user_name[0].'</a> ('.($user_feedbksmry[0]-$user_feedbksmry[2]).')</td>';
			echo '<td valign="top"><table><tr valign="top"><td><img src="/images/'.$fdbk_images[$score].'"></td><td>&nbsp;'.toHTML(funcs_cutLength($text,null,30)).'</td></tr></table></td>';
			echo '</tr>';
			$rowOdd = !$rowOdd;
		}while($row = db::fetch_assoc($result));
		echo '</table>';
		$total = $my_feedbacksummary[0]+$my_feedbacksummary[1]+$my_feedbacksummary[2];
		$pageString = printPageNumbers($page,$total,LIMIT,$_SERVER['REQUEST_URI'],'page');
		echo '<p align="center">Pages: '.$pageString.'</p>';
	}else{
		echo '<p align="center">'.$my_name[0].' '.$my_name[1].' doesn\'t have any feedback on his record</p>';
	}
	?>
	<?php
}
?>