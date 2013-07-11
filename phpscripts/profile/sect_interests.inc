<!-- interests --><!--
<table cellpadding="0" cellspacing="0" width="100%">
  <tr>
    <td align="left" valign="top" class="prfTbCrnlt"><img src="/images/c_lt.gif"></td>
    <td class="prfedge" width="100%">&nbsp;</td>
    <td align="right" valign="top" class="prfTbCrnrt"><img src="/images/c_rt.gif"></td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides"><span class="prfTbHdr">My Interests</span> </td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides" style="padding:5px;">-->
	<span class="prfTbHdr">My Interests</span>
      <!-- body -->
	 <p class="txt"><?=preg_replace('/[\s]*([^,]+)[\s]*[,]?/e',"' <a class=\"inLinksCat\" href=\"/search?type=".SEARCH_TYPE_PEOPLE."&intrs='.urlencode('\\1').'\">\\1</a>,'",toHTML($data[$profile_table->field_interests]))?></p>
      <!-- body -->
   <!-- </td>
  </tr>
  <tr>
    <td align="left" valign="bottom" class="prfTbCrnlb"><img src="/images/c_lb.gif"></td>
    <td class="prfedgebt" width="100%">&nbsp;</td>
    <td align="right" valign="bottom" class="prfTbCrnrb"><img src="/images/c_rb.gif"></td>
  </tr>
</table>-->
<!-- interests -->