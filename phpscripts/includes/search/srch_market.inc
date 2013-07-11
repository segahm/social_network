<?php
$PAGE_STRING = printNextPrev($search->page,$RESULTS_OUT,$RESULT_PER_PAGE,$_SERVER['REQUEST_URI'],'page');
?>
<table width="100%">
<tr><td style="padding:10px;">
<table cellpadding="0" cellspacing="0" width="100%">
	<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
		<table width="100%" cellpadding="0" cellspacing="0"><tr><td align="left" style="padding:5px;">
		<span class="roundHdr">Market Search</span>
		</td>
		<td align="right">
		<font color="#336699">Showing Results <?=$RESULTS_FROM.' ~ '.$RESULTS_TO.' out of '.$RESULTS_OUT?>&nbsp;</font>
		</td></tr></table>
	</td></tr>
	<tr><td colspan="3" class="roundsides">
	<?php require_once($_SERVER['DOCUMENT_ROOT']
	."/phpscripts/includes/search/mrktsrch.inc");?>
	<?=$PAGE_STRING?>
	</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</td></tr>
</table>