<?php
$friends = $controller->awaitingFriendConfirm();
?>

<table width="100%">
  <tr>
    <td colspan="3"><p>You have the following friends awaiting your confirmtation</p></td>
  </tr>
  <?php foreach($friends as $afriend):
			$inviter = $afriend['inviter'];
			$is_default = ((empty($afriend['picture']))?true:false);
			$picture = funcs_getPictureURL($afriend['picture']);
			$date = $afriend['date'];
			$name = funcs_getNames($afriend['name']);
			//set new image dimensions
			$array = getimagesize($_SERVER['DOCUMENT_ROOT'].$picture);
			$width = $array[0];
			$height = $array[1];
			funcs_imageResize($width, $height, 50, true);
		?>
  <tr align="center">
    <td width="50"><a href="/profile?id=<?=$inviter?>"><img src="<?=$picture?>" border="0" <?=((!$is_default)?'style="border-color:#000000;border-style:solid;border-width:1px;"':'')?> width="<?=$width?>" height="<?=$height?>"></a></td>
    <td align="left"><a href="/profile?id=<?=$inviter?>"><font color="#336699"><b>
      <?=$name[0].' '.$name[1]?>
      </b></font></a></td>
    <td align="right"><table width="120" cellpadding="3" cellspacing="0">
        <tr>
          <td align="center" style="border-style:solid;border-width:1px;border-color:#000000;"><a href="/myconnections?addfriend=<?=$inviter?>">confirm friendship</a></td>
        </tr>
        <tr>
          <td align="center" style="border-style:solid;border-width:1px;border-color:#000000;border-top-width:0px;"><a href="/home?refusefriend=<?=$inviter?>">refuse friendship</a></td>
        </tr>
        <tr>
          <td align="center" style="border-style:solid;border-width:1px;border-color:#000000;border-top-width:0px;"><a href="/profile?id=<?=$inviter?>">who the ... is this?</a></td>
        </tr>
      </table></td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <?php endforeach;?>
</table>
