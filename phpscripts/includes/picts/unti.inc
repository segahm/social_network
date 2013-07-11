<?php
$user_name = funcs_getNames(AUTH_USER_NAME);
$albm_t = new table_albums();
$fields = $control->response_codes['fields'];
drawMain_1("My Photos");
?>
<style>
.sides{
	border-left-width:1px;border-left-style:solid;border-left-color:#C5C5C5;
		border-right-width:1px;border-right-style:solid;border-right-color:#C5C5C5;
}
.albm_btn{
	font-family:tahoma,arial,sans-serif;font-size: 10pt;
	font-weight: normal;font-Style: normal;font-variant: normal;
	border-width:1px;
	color: #ffffff;background-color:#999999;
}
</style>
<script language="javascript" src="/html/utils.js"></script>
<script language="javascript">
<!--
<?=printArrays()?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/match.js");?>
<?php require_once($_SERVER['DOCUMENT_ROOT']."/html/binarySearch.js");?>
function submitForm(){
	document.crtalbm_frm.selfriends.value = scroll_toString();
	document.crtalbm_frm.submit();
}
//-->
</script>
<?php
drawMain_2(null,true);
?>
<table width="100%" cellpadding="0" cellspacing="0">
  <tr>
    <td style="padding:10px;"><table width="100%" cellpadding="0" cellspacing="0">
        <tr>
          <td align="left"><h1><font style="color:#993333;font-size:16px;">Create Album</font></h1></td>
        </tr>
        <tr>
          <td style="padding:10px;"><table width="500" cellpadding="0" cellspacing="0">
              <tr>
                <td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
                <td class="roundedge" width="100%">&nbsp;</td>
                <td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
              </tr>
              <tr>
                <td colspan="3" class="sides" style="padding:5px;"><table width="100%">
                    <tr>
                      <td bgcolor="#996666" style="font-size:2px;">&nbsp;</td>
                    </tr>
                  </table>
                  <?php
		$genid = str_shuffle('qwertyuiopasdfghjklzxcvbnm123456789');
		$genid = substr($genid, 0, $albm_t->field_albumid_length);
	?>
                  <form name="crtalbm_frm" action="/mypictures?newalbm=1&pf=1&albm=<?=$genid?>" method="post">
                    <?php ob_start('funcs_fillForm');?>
                    <input type="hidden" name="selfriends">
                    <p>Please enter the name for this album and choose a sharing settings, then click "continue" to begin uploading photos.</p>
                    <table cellpadding="0" cellspacing="3">
                    <tr>
                      <td><table cellpadding="0" cellspacing="0">
                          <tr>
                            <td><font color="#669966"><b>Album Name:</b></font>&nbsp;&nbsp;&nbsp;</td>
                            <td><input type="text" name="name" maxlength="40" size="40"></td>
                          </tr>
                        </table></td>
                    </tr>
                    <tr>
                      <td><font color="#669966"><b>Sharing Setting</b></font><br>
                        <p>Select a sharing setting that best fits the purpose of this album in terms of who can access it within our site</p></td>
                    </tr>
                    <tr>
                      <td><table cellpadding="0" cellspacing="5">
                          <tr>
                            <td><input type="radio" name="perm" value="0"></td>
                            <td><img width="20" height="20" src="/images/secure.gif"></td>
                            <td>Private</td>
                          </tr>
                          <tr>
                            <td>&nbsp;</td>
                            <td><img src="/images/corner_arrow.gif"></td>
                            <td><font style="font-size:11px;">Only I have access to these pictures and (optionaly) the following people</font></td>
                          </tr>
                        </table></td>
                    </tr>
                    <tr>
                      <td style="padding-left:10px;">
					  <table cellspacing="5">
                          <tr>
                            <td>
                              <input type="radio" name="perm_friends" value="<?=Picts::PERM_ALLFRIENDS?>">
							  All of my friends
							</td>
                          </tr>
                          <tr>
                            <td style="padding-left:10px;"><table>
                                <tr>
                                  <td><input type="radio" name="perm_friends" value="<?=Picts::PERM_SPECIFIED?>"></td>
                                  <td>The following friends &/or groups</td>
                                </tr>
                                <tr valign="top">
                                  <td><img src="/images/corner_arrow.gif"></td>
                                  <td><table width="100%" cellpadding="0" cellspacing="5">
                                      <tr valign="top">
                                        <td width="50%"><font class="event_add_color">friends:</font></td>
                                        <td width="50%"><font class="event_add_color">groups:</font></td>
                                      </tr>
                                      <tr valign="top">
                                        <td width="50%"><input type="text" name="users" size="30" onBlur="javascript:displayMatch('1shMtch','',null);" onKeyUp="javascript:matchFriend(0,this,'1shMtch')"></td>
                                        <td width="50%"><input type="text" name="groups" size="30" onBlur="javascript:displayMatch('2shMtch','',null);" onKeyUp="javascript:matchFriend(1,this,'2shMtch')"></td>
                                      </tr>
                                      <tr valign="top">
                                        <td nowrap><div id="1shMtch" style="display:none;position:absolute;"></div>
                                          <font class="dimmedTxt">ex: John Doe, mike@ucsc.edu, ...</font> </td>
                                        <td><div id="2shMtch" style="display:none;position:absolute;"></div>
                                          <font class="dimmedTxt">ex: Project Buddies, ...</font> </td>
                                      </tr>
                                    </table></td>
                                </tr>
                              </table></td>
                          </tr>
                          <tr>
                            <td><table cellpadding="0" cellspacing="0">
                                <tr>
                                  <td><input type="radio" name="perm" value="<?=Picts::PERM_PUBLIC?>"></td>
                                  <td><img width="20" height="20" src="/images/group.gif"></td>
                                  <td>&nbsp;Public</td>
                                </tr>
                              </table></td>
                          </tr>
                          <tr>
                            <td><input type="radio" name="perm" value="<?=Picts::PERM_SCHOOL?>">
&nbsp;&nbsp;&nbsp;People in my school</td>
                          </tr>
                        </table>
                        <?php ob_end_flush();?>
                        <br>
                        <table width="100%">
                          <tr>
                            <td bgcolor="#996666" align="right"><input class="albm_btn" type="button" value="cancel" onClick="document.location = '/mypictures';">
&nbsp;
                              <input class="albm_btn" style="background-color:#CC9966" type="button" value="continue" onClick="submitForm();"></td>
                          </tr>
                        </table>
                  </form></td>
              </tr>
              <tr>
                <td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
                <td class="roundedgebt" width="100%">&nbsp;</td>
                <td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
              </tr>
            </table></td>
        </tr>
      </table></td>
  </tr>
</table>
<?php
drawMain_3(false);
function printArrays(){
	$quickAddIn = new quickAddIn(AUTH_USERID);
	return printMatchArrays($quickAddIn->getFriends(),$quickAddIn->getGroups());
}
?>
