<?php
define('MAX_ABOUTME_TEXT_LENGTH',200);
?>
<script language="javascript" type="text/javascript">
<!--
function shfabme(){
	var div = document.getElementById('shflabme');
	var div2 = document.getElementById('shshrtlabme');
	if (div.style.display == 'none'){
		div.style.display = 'block';
		div2.style.display = 'none';
	}else{
		div.style.display = 'none';
		div2.style.display = 'block';
	}
}
//-->
</script>
<!-- aboutme --><!--
<table cellpadding="0" cellspacing="0" width="100%">
  <tr>
    <td align="left" valign="top" class="prfTbCrnlt"><img src="/images/c_lt.gif"></td>
    <td class="prfedge" width="100%">&nbsp;</td>
    <td align="right" valign="top" class="prfTbCrnrt"><img src="/images/c_rt.gif"></td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides"><span class="prfTbHdr">About Me</span> </td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides" style="padding:5px;">-->
      <!-- body -->
	  <span class="prfTbHdr">About Me</span> 
      <?php
      	$replace = array('/  /','/\n/');
		$replace_with = array(' &nbsp;',' <br>');
		$text = preg_replace($replace,$replace_with,toHTML($data[$profile_table->field_aboutme]));
    	if (strlen($text) > MAX_ABOUTME_TEXT_LENGTH){
			$short_text = substr($text,0,MAX_ABOUTME_TEXT_LENGTH).' <a href="javascript:shfabme()">...</a>';
		}else{
			$short_text = $text;
		} 
	?>
	 <div id="shshrtlabme">
	 <p class="txt"><?=$short_text?></p>
     </div>
	 <div id="shflabme" style="display:none">
	 <p class="txt"><?=$text?> <a href="javascript:shfabme()">-</a></p>
     </div>
	  <!-- body -->
   <!-- </td>
  </tr>
  <tr>
    <td align="left" valign="bottom" class="prfTbCrnlb"><img src="/images/c_lb.gif"></td>
    <td class="prfedgebt" width="100%">&nbsp;</td>
    <td align="right" valign="bottom" class="prfTbCrnrb"><img src="/images/c_rb.gif"></td>
  </tr>
</table>-->
<!-- aboutme -->