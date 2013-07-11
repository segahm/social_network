<?php
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/interface.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/DBconnect.inc");
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/auth.inc");
function drawMain_1($title){
	ob_start();
?>
<html>
<head>
<link rel="Stylesheet" type="text/css" href="/styles/styles.css">
<title>partyHub -
<?=$title?>
</title>
<style type="text/css">
.pnl{
	border-width:1px;border-style:solid;
	border-bottom-color:#333333;border-right-color:#333333;
	border-top-color:#CCCCCC;border-left-color:#CCCCCC
}
.pnlBtn{
	color:#336699;
}
.pnlBtn:hover{
	color:#000000;
}
.scrl_pnl_lnks{
	color:#000000;font-size:10px;
}
</style>
<?php
}
function drawMain_2($onload = null,$showFriends = false,$ids = null,$show_styles = true){
?>
<?php if ($showFriends):?>
<script language="javascript" src="/html/scroll_panel.js"></script>
<?php endif;?>
<?php if($show_styles && !is_null(AUTH_STYLE)):?>
<style type="text/css">
<?=AUTH_STYLE?>
</style>
<?php endif;?>
</head>
<body <?=((!is_null($onload))?'onLoad="'.$onload.'"':'')?>>
<!-- school/site image -->
<?php include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/schoolImagePanel.inc");?>
<br>
<table width="850" cellpadding="0" cellspacing="0" align="center">
  <tr>
    <td valign=top><table width="100%" cellpadding="0" cellspacing="0" align="center">
        <tr valign=top>
          <!-- left panel -->
          <td width="170" nowrap align="center">
		  <table width="100%" style="border-right-width:1px;border-right-style:solid;border-right-color:#CCCCCC;" cellpadding=0 cellspacing=0>
              <tr>
                <td colspan="2">
				 <?php require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/picts/rndm.inc");?>
				  </td>
              </tr>
              <tr>
                <td width="120">&nbsp;</td>
                <td width="50" style="border-bottom-width:1px;border-bottom-style:solid;border-bottom-color:#CCCCCC;">&nbsp;</td>
              </tr>
            </table>
            <br>
			<table width="150">
			<tr><td class="mainLeftTopPanel">Things to do...</td></tr>
			</table>
			<table>
			<tr><td><a class="pnlBtn" title="view my profile" href="/profile">My profile</a></td></tr>
			<tr><td><a class="pnlBtn" title="browse my pictures" href="/mypictures">Photos</a></td></tr>
			<tr><td><a class="pnlBtn" title="Blogger!" href="/blogger">Blogger!</a></td></tr>
			<tr><td><a class="pnlBtn" title="events" href="/events">Events & Parties</a></td></tr>
			<tr><td><a class="pnlBtn" title="discussion" href="/discussion">Discussions...</a></td></tr>
			<tr><td><a class="pnlBtn" title="my mail" href="/message">messageHub</a></td></tr>
			<tr><td><a class="pnlBtn" title="Trading Space" href="/trades">Trading Space</a></td></tr>
			<tr><td><a class="pnlBtn" title="Find My Ride!" href="/rides">Find My Ride!</a></td></tr>
			</table>
           <br>
            <?php if ($showFriends):?>
            <?php
				$sexes_array = array();
				$friends = main_getAllFriends(AUTH_USERID);
				if (empty($friends)){
					$friends_string = "var scroll_array_checked = null;\nvar scroll_array_nms = null;\nvar scroll_array_ids = null;\nvar scroll_array_sxs = null;";
				}else{
					asort($friends);
					$id_keys = array_keys($friends);
					$names_array = array();
					$sxs_array = array();
					//create string here so that we don't walk through array more than once
					$name_string = '';
					$id_string = '';
					$sexes_string = '';
					$flag = false;
					foreach ($friends as $id => $value){
						if ($flag){
							$name_string .= ',';
							$id_string .= ',';
							$sexes_string .= ',';
						}
						$flag = true;
						$name = $value[0];
						$sxs = $value[1];
						$name_string .= '"'.$name.'"';
						$id_string .= '"'.$id.'"';
						$sexes_string .= '"'.((is_null($sxs))?'0':$sxs).'"';
					}
					$friends_string = 'var scroll_array_nms = new Array('
						.$name_string.');'
						.'var scroll_array_ids = new Array('
						.$id_string.');'
						.'var scroll_array_sxs = new Array('
						.$sexes_string.');'."var scroll_array_checked = null;";
					if (!empty($ids)){
						$friends_string .= "scroll_array_checked = new Array(scroll_array_nms.length);";
						$ids = array_flip($ids);
						//iterate through all and if corresponding key exists in ids then check it
						foreach($id_keys as $key => $id){
							$friends_string .= 'scroll_array_checked['.$key.'] = '
								.((isset($ids[$id]))?'1':'0').";";
						}
					}
				}
			?>
            <table width="100%" class="sidepnl" cellpadding="0" cellspacing="3">
              <tr>
                <td style="padding-left:5px;"><table width="100%" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><span class="roundHdr">Friends' List</span></td>
                      <td align="right"><a style="font-size:10px;text-decoration:none;color:#006633" href="javascript:scrool_checkAll();">[all]</a> <a style="font-size:10px;text-decoration:none;color:#006633" href="javascript:scrool_checkNone();">[none]</a> </td>
                    </tr>
                  </table></td>
              </tr>
              <tr>
                <td style="padding:5px;"><div id="scroll_friends">
                    <form name="scroll_friends" method="POST" style="margin:0px;">
                      <font style="font-size:10px;">buddy list is empty<font>
                    </form>
                  </div>
                  <p align="center" style="font-size:10px;"> <a class="scrl_pnl_lnks" style="font-size:10px;text-decoration:none;color:#006633" href="javascript:scroll_up();" onMouseUp="javascript:scroll_no();">[up]</a>&nbsp; <a class="scrl_pnl_lnks" style="font-size:10px;text-decoration:none;color:#006633" href="javascript:scroll_dn();" onMouseUp="javascript:scroll_no();">[more]</a></p>
                  <script language="javascript">
				<!--
					<?=$friends_string?>
					<?=((!empty($friends))?'eval("scroll_setInitial(\'scroll_friends\');");':'')?>
					
				//-->
				</script>
                </td>
              </tr>
            </table>
            <?php endif;?>
            <!--		</td>-->
            <!--center panel-->
          <td width="100%" align="left" style="border-top-width:1px;border-top-style:solid;border-top-color:#CCCCCC;border-right-width:1px;border-right-style:solid;border-right-color:#CCCCCC;"><?php
}
function drawMain_3($rightpanel,$calendar = true,$rndm_img = true){
?>
          </td>
          <?php 
		if($rightpanel == true){?>
          <td style="padding-left:10px;" align="center" valign="top"><table width="100%" cellpadding="0" cellspacing="5">
              <?php if ($calendar):?>
              <tr>
                <td id="calendar" class="sidepnl"><?php require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/events/calendar.php");?>
                </td>
              </tr>
              <tr>
                <td>&nbsp;</td>
              </tr>
              <?php endif;?>
			  <tr><td>
			<table width="120" cellpadding="2" cellspacing="0" align="center">
			<tr><td><table width="100%"><tr><td class="mainRightPanel">My Stuff...</td></tr></table></td></tr>
			<tr><td align="center" nowrap><a class="pnlBtn" title="view my profile" href="/profile">My profile</a> - <a class="pnlBtn" title="edit my profile" href="/editp/general">edit</a></td></tr>
			<tr><td align="center"><a class="pnlBtn" title="manage my connections" href="/myconnections">My Buddies</a></td></tr>
			<tr><td align="center"><a class="pnlBtn" title="change my account" href="/myaccount">My Account</a></td></tr>
			<tr><td align="center"><a class="pnlBtn" title="configure my settings" href="/myprivacy">My Options</a></td></tr>
			</table>
			  </td></tr>
            </table></td>
          <?php
		}
		?>
        </tr>
      </table>
      <?php include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>
    </td>
  </tr>
</table>
</body>
</html>
<?php
	db::close();
	ob_end_flush();
}
/**returns an array of friends and their associated ids in the ascending order and automaticaly
 			*formats the names 
 			*First Last*/
function main_getAllFriends($userid){
	$users_t = new table_users();
	$friends_t = new table_friends();
	$profile_t = new table_profile();
	$query = 'SELECT '.TABLE_USERS.'.'.$users_t->field_id.','.TABLE_USERS.'.'.$users_t->field_name.','
		.TABLE_PROFILE.'.'.$profile_t->field_sex
		.' FROM '.TABLE_USERS.','.TABLE_FRIENDS.','.TABLE_PROFILE.' WHERE '
		.TABLE_FRIENDS.'.'.$friends_t->field_user1
		." = '".$userid."' AND ".TABLE_FRIENDS.'.'.$friends_t->field_user2.' = '
		.TABLE_USERS.'.'.$users_t->field_id.' AND '
		.TABLE_PROFILE.'.'.$profile_t->field_id.' = '.TABLE_USERS.'.'.$users_t->field_id;
	$result = db::query($query);
	$friends = array();
	while ($row = db::fetch_row($result)){
		$names = funcs_getNames($row[1]);
		$row[1] = $names[0].' '.$names[1];
		$friends[$row[0]] = array($row[1],$row[2]);
	}
	return $friends;
}
?>
