<!-- buttons -->
<table cellpadding="0" cellspacing="0">
  <tr>
    <?php if($GLOBALS['IS_ME']):?>
	 <td>
	 <a href="/editp/general" class="prfBtn">edit profile</a>
	 | <a href="/blogger?usrblgs=<?=AUTH_USERID?>" class="prfBtn">my blog page</a>
	 | <a href="/search?type=1&friends=<?=AUTH_USERID?>" class="prfBtn">view friends</a>
	  </td>
    <?php else:?>
		<td>
		<a href="/message?outbox=1&id=<?=$_GET['id']?>" class="prfBtn">message</a>
		| <a href="/mypictures?usr=<?=$_GET['id']?>" class="prfBtn">photos</a>
		| <a href="/blogger?usrblgs=<?=$_GET['id']?>" class="prfBtn">view blogs</a>		
		<?php if($profile_access->show_friends):?>
		| <a href="/search?type=1&friends=<?=$_GET['id']?>" class="prfBtn">friends</a>
		<?php endif;?>
		| 
		<?php if(!$profile_access->is_friends()):?>
		<a href="/myconnections?addfriend=<?=$_GET['id']?>" class="prfBtn">fecome friends</a>
		<?php else:?>
		<a href="javascript:window.confirm('Are you sure you want to remove <?=$names[0]?> from your list of friends?')?window.location = '/myconnections?removefriend=<?=$_GET['id']?>' :void(null);" class="prfBtn">break friendship</a>
		<?php endif;?>
		</td>
    <?php endif;?>
  </tr>
</table>
<!-- body -->
<!-- buttons -->
