<!-- contact info -->
<!--
<table cellpadding="0" cellspacing="0">
  <tr>
    <td align="left" valign="top" class="prfTbCrnlt"><img src="/images/c_lt.gif"></td>
    <td class="prfedge" width="100%">&nbsp;</td>
    <td align="right" valign="top" class="prfTbCrnrt"><img src="/images/c_rt.gif"></td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides" align="left"><span class="prfTbHdr">Contact Info</span> </td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides" align="center" style="padding-top:5px;">-->
      <!-- body -->
	  <?php
	  	$eml_array = str_split($data[$users_table->field_email]);
	  	$eml_string = '';
		foreach ($eml_array as $char){
			$eml_string .= 'str += "'.$char.'";';
		}
	  ?>
      <table cellpadding="2" cellspacing="0" width="100%">
        <?php if ($profile_access->show_email):?>
      	<tr>
          <td><font class="inListTxt">Email: </font>
            <script language="javascript">
			var str = '';
			<?=$eml_string?>
			document.write('<a class="inLinks" href="mailto:'+str+'">'+str+'</a>');
			</script>
           </td>
        </tr>
        <?php if(!is_null($data[$users_table->field_preferedemail])):?>
        <tr>
          <td><font class="inListTxt">Alternative email: </font><a class="inLinks" href="mailto:<?=$data[$users_table->field_preferedemail]?>">
            <?=$data[$users_table->field_preferedemail]?>
            </a> </td>
        </tr>
        <?php endif;?>
        <?php endif;?>
        <tr>
          <td><font class="inListTxt">Phone number: </font>
            <?=toHTML($parser->getString($profile_table->field_phone,''))?>
          </td>
        </tr>
        <tr>
          <td><font class="inListTxt">Cellphone: </font>
            <?=toHTML($parser->getString($profile_table->field_cellphone,''))?>
          </td>
        </tr>
        <tr>
          <td><font class="inListTxt">Mailbox: </font>
            <?=toHTML($parser->getString($profile_table->field_mailbox,''))?>
          </td>
        </tr>
        <tr>
          <td><?php if(!is_null($parser->getString($profile_table->field_address)) 
			|| !is_null($parser->getString($profile_table->field_city))
			|| !is_null($parser->getString($profile_table->field_state))):?>
            <font class="inListTxt">Address: </font>
            <?php endif;?>
            <?php if(!is_null($parser->getString($profile_table->field_address))):?>
            <?=toHTML($parser->getString($profile_table->field_address))?>
            ,&nbsp;
            <?php endif;?>
            <?php if(!is_null($parser->getString($profile_table->field_city))):?>
            <a href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&city=<?=urlencode($parser->getString($profile_table->field_city))?>" 
			 class="inLinksCat"><?=toHTML($parser->getString($profile_table->field_city))?></a>,&nbsp;
            <?php endif;?>
            <?php if(!is_null($parser->getString($profile_table->field_state))):?>
            <a href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&state=<?=urlencode($parser->getString($profile_table->field_state))?>" 
			 class="inLinksCat"><?=toHTML($parser->getString($profile_table->field_state))?></a>,&nbsp;
            <?php endif;?>
            <?php if(!is_null($parser->getString($profile_table->field_zip))):?>
            <a href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&zip=<?=urlencode($parser->getString($profile_table->field_zip))?>" 
			class="inLinksCat"><?=toHTML($parser->getString($profile_table->field_zip))?></a>
            <?php endif;?>
          </td>
        </tr>
        <tr>
          <td><font class="inListTxt">Aim screen name: </font><a class="inLinks" href="aim:goim?screenname=<?=urlencode($parser->getString($profile_table->field_AIM,''))?>">
            <?=toHTML($parser->getString($profile_table->field_AIM,''))?>
            </a> </td>
        </tr>
      </table>
      <!-- body -->
 <!--   </td>
  </tr>
  <tr>
    <td align="left" valign="bottom" class="prfTbCrnlb"><img src="/images/c_lb.gif"></td>
    <td class="prfedgebt" width="100%">&nbsp;</td>
    <td align="right" valign="bottom" class="prfTbCrnrb"><img src="/images/c_rb.gif"></td>
  </tr>
</table>-->
<!-- contact info -->
