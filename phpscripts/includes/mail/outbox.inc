<?php
$my_name = funcs_getNames(AUTH_USER_NAME);
$TYPE = $controller->getOutboxType();
//see if there is any prefileld data available
$fields = $controller->getFormData();
?>
<?php if ($RESPONSE_CODE == MAIL_RESPONSE_SENDFAILED):?>
	<p class="error"><?=$controller->error?></p>
<?php elseif($RESPONSE_CODE == MAIL_RESPONSE_SENDSUCCESS):?>
	<p style="color:#336699">your message was send successfully</p>
<?php elseif(!empty($controller->error)):?>
	<p style="color:#336699"><?=$controller->error?></p>
<?php endif?>
<script type="text/javascript" src="/html/utils.js" language="javascript"></script>
<script type="text/javascript" src="/html/richtext.js" language="javascript"></script>
<script type="text/javascript" language="javascript">
<!--
<?=printArrays()?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/match.js");?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/binarySearch.js");?>
//-->
</script>
<script type="text/javascript" language="javascript">
<!--
function handlesubmit(){
	if (submitRte('bodytxt','fld_body')){
		document.m_f.submit();
	}
}
//-->
</script>
<?php ob_start('processForm');?>
<form name="m_f" action="<?=preg_replace('/[&]?send=[^&]*/','',$_SERVER['REQUEST_URI'])?>&send=1" method="post" style="margin:0px;">
<input type="hidden" name="mid">
<input type="hidden" id="fld_body" name="body">
<table width="100%" cellpadding="0" cellspacing="0">
<tr><td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
	<td class="roundedge" width="100%">&nbsp;</td>
	<td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td></tr>
	<tr><td colspan="3" class="roundsides">
<table width="100%" cellpadding="3" cellspacing="3">
<?php if ($TYPE == MAIL_OUTBOX_TYPE_ID || $TYPE == MAIL_OUTBOX_TYPE_GROUP):?>
<?php 
	$array = $controller->getName();
	$to_name = $array[0];
	$to_school = $array[1];
?>
<tr>
	<td align="right" width="40">To:</td>
	<td><a style="color:#009933" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&q=<?=urlencode($to_name)?>&col=<?=$to_school?>">
	<?=($TYPE == MAIL_OUTBOX_TYPE_ID && AUTH_USERID == $_GET['id'])? 'Me':$to_name?></a><?=($TYPE == MAIL_OUTBOX_TYPE_GROUP)?' (group)':''?>
	<input type="hidden" name="name" value="<?=$to_name?>">
	<input type="hidden" name="school" value="<?=$to_school?>">
	</td>
</tr>
<?php else:?>
<tr><td colspan="2">
<p style="font-size:11px;">Please enter the names of your partyHub existing 
friends/groups that you would like to send a message to</p>
</td></tr>
<tr><td colspan="2">
	<table width="100%" cellpadding="0" cellspacing="0">
	<tr>
	<td>Friends:</td><td><input type="text" name="users" values="" size="50" onFocus="match_focus(0,this,'1shMtch')" onBlur="match_blur()"></td>
	<td>Groups:</td><td><input type="text" name="groups" values="" size="40" onFocus="match_focus(1,this,'2shMtch')" onBlur="match_blur()"></td>
	</tr>
	<tr><td>&nbsp;</td><td>
	<div id="1shMtch" style="display:none;position:absolute;"></div>
			<font class="dimmedTxt" style="font-size:10px;">ex: John Doe, mike@ucsc.edu, ...</font>
	</td><td>&nbsp;</td><td>
	<div id="2shMtch" style="display:none;position:absolute;"></div>
			<font class="dimmedTxt" style="font-size:10px;">ex: Project Buddies, ...</font>
	</td></tr></table>
</td></tr>
<?php endif;?>
<tr>
	<td align="right" width="50">From:</td>
	<td width="90%"><?=$my_name[0].' '.$my_name[1]?></td>
</tr>
<tr>
	<td align="right" width="50">Subject:</td>
	<td align="left" width="90%"><input type="text" name="subject" value="" size="60" maxlength="100"></td>
</tr>
<tr><td>&nbsp;</td><td>
<div id="btns_div_dsp"></div>
</td></tr>
<tr valign="top">
	<td align="right" width="50">Message:</td>
	<td width="90%">
	<div id="txt_div_dsp"></div>
	<script language="javascript" type="text/javascript">
	<!--
	initRTE("/images/rte/", "/html/", "/styles/rte.css",'btns_div_dsp','txt_div_dsp');
	//-->
	</script>
	<script language="javascript" type="text/javascript">
	<!--
	writeRichText('bodytxt',getObjectByID('fld_body').value, 480, 150, true, false);
	//-->
	</script>
	</td>
</tr>
<tr><td colspan="2" align="center"><input type="button" onClick="handlesubmit()" value="send" class="custButton" style="border-width:1px;"></td></tr>
</table>
</td></tr>
	<tr><td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
	<td class="roundedgebt" width="100%">&nbsp;</td>
	<td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td></tr>
</table>
</form>
<?php ob_end_flush();?>
<?php
function &processForm(){
	global $fields;
	$contents =& ob_get_contents();
	if (empty($fields)){
		return $contents;
	}
	//if send failed, then insert post data back
	foreach ($fields as $key => $value){
		$fields[$key] = toHTML($value);
	}
	$contents = funcs_processForm($contents,$fields);
	return $contents;
}
?>