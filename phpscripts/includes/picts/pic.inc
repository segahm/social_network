<?php drawMain_1("My Photos");
//get the name of the owner of these albums
if (isset($control->response_codes['user'])){
	$users_t = new table_users();
	$user_name = funcs_getNames($control->response_codes['user'][$users_t->field_name]);
	$user_id = $control->response_codes['user'][$users_t->field_id];
	$user_school = $control->response_codes['user'][$users_t->field_school];
	$is_owner = false;
}else{
	$user_name = funcs_getNames(AUTH_USER_NAME);
	$is_owner = true;
}
$data = $control->response_codes['pic_info'];
$pict_t = new table_pictures();
$is_party_pic = $data[$pict_t->field_public];
if (!is_null($is_party_pic)){
	$party_pic_score = explode('/',$is_party_pic);
}
//though we physicaly resize images, that happens only during cronjob and thus the user might still
//see the image unresized
$img_file = $data[$pict_t->field_pictureid];
$img_file = Picts::getPictureUrl($img_file);
//if cron job has processed this picture then there is no need to access
//to make an additional request to file to find out its dimensions since it is
//already resized
if (!Mydate::isBeforeCron(funcs_getTimeStamp($data[$pict_t->field_datetime]))){
	//resize image to the target
	$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$img_file);
	$width = $array[0];
	$height = $array[1];
	$resizeByWidth = true;
	funcs_imageResize($width,$height,Picts::MAX_WIDTH_IMAGE,$resizeByWidth);
}else{
	$width = null;
	$height = null;
}
?>
<?php if ($is_owner):?>
<script language="javascript" src="/html/utils.js"></script>
<script language="javascript">
function dispstngs(){
	var dsparea = getObjectByID('settings');
	dsparea.style.display = 'block';
}
function del_pict(){
	document.location = '/mypictures?delpict=<?=$data[$pict_t->field_pictureid]?>';
}
</script>
<?php endif;?>
<style>
.albm_btn{
	font-family:tahoma,arial,sans-serif;font-size: 10px;
	font-weight: normal;font-Style: normal;font-variant: normal;
	border-width:1px;
	color: #ffffff;background-color:#CC9966;
}
</style>
<?php drawMain_2();?>
<table width="500" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td style="padding-left:10px;"><div id="settings" style="display:none">
	<br>
        <table width="100%" cellpadding="0" cellspacing="0">
          <tr>
            <td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
            <td class="roundedge" width="100%">&nbsp;</td>
            <td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
          </tr>
          <tr>
            <td colspan="3" class="roundsides" style="padding-left:5px;"><form style="padding:0px;margin:0px;" action="/mypictures?cptn=1" method="post">
                Caption:
                <input style="color:#006633" type="text" maxlength="100" value="<?=toHTML($data[$pict_t->field_title])?>" size="50" name="caption">
&nbsp;
                <input type="submit" class="albm_btn" style="background-color:#666666" value="save settings">
                <input type="hidden" name="public" value="<?=$data[$pict_t->field_public]?>">
                <input type="hidden" name="id" value="<?=$data[$pict_t->field_pictureid]?>">
                <input type="button" onclick="del_pict();" class="albm_btn" style="background-color:#999966" value="delete">
              </form></td>
          </tr>
          <tr>
            <td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
            <td class="roundedgebt" width="100%">&nbsp;</td>
            <td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
          </tr>
        </table>
      </div></td>
  </tr>
  <tr>
    <td style="padding:10px;"><table width="100%" cellpadding="0" cellspacing="0">
        <tr>
          <td align="left" valign="top" class="roundCrnlt"><img src="/images/c_lt.gif"></td>
          <td class="roundedge" width="100%">&nbsp;</td>
          <td align="right" valign="top" class="roundCrnrt"><img src="/images/c_rt.gif"></td>
        </tr>
        <tr>
          <td colspan="3" class="roundsides" style="padding-left:5px;"><font color="#006633"><b>Photo</b></font></td>
        </tr>
        <tr>
          <td colspan="3" class="roundsides"><table width="100%" cellpadding="0" cellspacing="0">
              <tr>
                <td style="font-size:4px;" bgcolor="#663333">&nbsp;</td>
              </tr>
            </table></td>
        </tr>
        <tr>
          <td colspan="3" class="roundsides" style="padding:5px;"><p align="center"><img src="<?=$img_file?>"<?=((!is_null($width))?' width="'.$width.'"':'').((!is_null($height))?' height="'.$height.'"':'')?> border=1></p>
           <p align="center" style="color:#006633"> <?=toHTML($data[$pict_t->field_title])?></p>
           <?php if ($is_owner):?>
               <p align="center"><a href="javascript:dispstngs();"><img src="/images/edit2.gif" border="0"></a></p>
           <?php endif;?>
		</td>
        </tr>
        <?php if (isset($party_pic_score)):?>
        <tr>
          <td colspan="3" class="roundsides"><table cellpadding="3" cellspacing="0" align="center">
              <tr>
                <td valign="top"><span style="color:#CC3300;font-weight:bold;">
                  <?=$party_pic_score[1]?>
                  </span></td>
                <td valign="bottom"><img src="/images/thumbdown.gif" border="0"></td>
                <td valign="top"><img src="/images/thumbup.gif" border="0"></td>
                <td valign="baseline"><span style="color:#006633;font-weight:bold;">
                  <?=$party_pic_score[0]?>
                  </span></td>
              </tr>
            </table></td>
        </tr>
        <?php endif;?>
        <tr>
          <td colspan="3" style="padding:5px;" class="roundsides" align="center"><input type="button" onclick="document.location = '/mypictures?showalbm=<?=$data[$pict_t->field_albumid]?><?=(($is_owner)?'':'&usr='.$user_id)?>'"
		class="albm_btn" style="background-color:#CC3333;text-decoration:none" value="back to album">
&nbsp; </td>
        </tr>
        <tr>
          <td align="left" valign="bottom" class="roundCrnlb"><img src="/images/c_lb.gif"></td>
          <td class="roundedgebt" width="100%">&nbsp;</td>
          <td align="right" valign="bottom" class="roundCrnrb"><img src="/images/c_rb.gif"></td>
        </tr>
      </table></td>
  </tr>
</table>
<?php
drawMain_3(true);
?>
