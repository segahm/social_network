<?php
$fields = $_GET;
?>
<table width="100%" class="blkBrd"><tr><td>
<?php ob_start('funcs_fillForm');?>
<form method="get" action="/trades/marketplace">
	<table>
		<tr>
		<td>
		Marketplace: <input type=text name="q" value="" size="50" maxlength="255">
		</td>
		<td>
		<select name="categ">
			<option value="all">All Categories</option>
			<option value="<?=TYPE_ITEM_TEXTBOOK?>">textbooks</option>
			<option value="<?=TYPE_ITEM_READER?>">course readers</option>
			<option value="<?=TYPE_ITEM_HOUSING?>">place for rent</option>
			<option value="<?=TYPE_ITEM_OTHER?>">other</option>
		</select>
		</td>
		<td><input type="submit" class="custButton" style="font-size:11px" value="search"></td>
		</tr>
	</table>
</form>
</td>
</tr>
</table>
<?php ob_end_flush();?>