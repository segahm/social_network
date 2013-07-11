<?php
$result = $search->getResult();
//now buffer data for the use by printRides.inc
$rides = array();
while ($row = db::fetch_assoc($result)){
	$rides[] = $row;
}
?>
<table width="100%">
<tr><td style="padding:10px;">
<table cellpadding="0" cellspacing="0" width="100%">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
		<table width="100%" cellpadding="0" cellspacing="0"><tr><td align="left" style="padding:5px;">
		<span class="roundHdr">Find My Ride!</span>
		</td>
		<td align="right">
		<font color="#336699">Showing Results <?=$RESULTS_FROM.' ~ '.$RESULTS_TO.' out of '.$RESULTS_OUT?>&nbsp;</font>
		</td></tr></table>
	</td></tr>
	<tr><td colspan="3" class="roundsides">
	<?php
	if (isset($_GET['kind']) && $_GET['kind']==1){
		echo '<a href="'.preg_replace('/[&]?((kind=)|(page))[^&]*/','',$_SERVER['REQUEST_URI']).'&kind=2">available rides</a>&nbsp;&nbsp;&nbsp;';
		echo 'looking for a ride';
	}else{
		echo 'available rides&nbsp;&nbsp;&nbsp;';
		echo '<a href="'.preg_replace('/[&]?((kind=)|(page))[^&]*/','',$_SERVER['REQUEST_URI']).'&kind=1">looking for a ride</a>';
	}
	if (!empty($rides)){
		$PAGE_STRING = printNextPrev($search->page,$RESULTS_OUT,$RESULT_PER_PAGE,$_SERVER['REQUEST_URI'],'page');
		require_once($_SERVER['DOCUMENT_ROOT']
		."/phpscripts/includes/rideboard/printRides.inc");
		echo $PAGE_STRING;
	}else{
		echo '<p style="padding-top:10px;">we were unable to find anything matching your criteria</p>';
	}
	?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</td></tr>
</table>