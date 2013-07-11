<?php
define('MAX_BLOGDISPLAY_SIZE',150);
$blog_t = new table_blog();
$stngs_t = new table_blogsettings();
?><!--
<table width="100%" cellpadding="0" cellspacing="0">
	<tr><td align="left" valign="top" class="prfTbCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="prfedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="prfTbCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="prfsides" style="padding-top:3px;padding-bottom:3px;">
	&nbsp;<span class="prfTbHdr">Recently in Blogs:</span> <?=toHTML(funcs_cutLength($blog[$stngs_t->field_header],25))?>
	</td></tr>
	<tr><td colspan="3" class="prfsides" style="padding:5px;padding-left:15px;">-->
	<span class="prfTbHdr">Recently in Blogs:</span> <?=toHTML(funcs_cutLength($blog[$stngs_t->field_header],25))?>
	<?php
	//get recent posts
	$text = strip_tags($blog[$blog_t->field_text]);
	$timestamp = $blog[$blog_t->field_timedate];
	?>
	<p class="txt"><?=funcs_cutLength($text,MAX_BLOGDISPLAY_SIZE,null)?><br>
	<a href="/findblog?id=<?=$blog[$blog_t->field_settingsid]?>#pst_<?=$blog[$blog_t->field_id]?>" class="ilnk">more...</a></p>
	<!--</td></tr>
	<tr><td align="left" valign="bottom" class="prfTbCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="prfedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="prfTbCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>-->