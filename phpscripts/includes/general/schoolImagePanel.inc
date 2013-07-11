<table cellpadding="0" cellspacing="0" width="850" align="center" class="srchBox">
	<tr valign="top">
	<td>
	<!-- search panel -->
		<?php
			function printSearchPanelTop(){
			if (defined('SEARCH_QUERY')){
				$search_query = toHTML(SEARCH_QUERY);
			}else{
				$search_query = '';
			}
			if (defined('SEARCH_TYPE')){
				$search_type = SEARCH_TYPE;
			}else{
				$search_type = 1;
			}
			$types = array(1 => 'people','market place','events','parties','my pictures','rides');
			?>
			<form method="get" action="/search" style="margin:0px;">
			<input type="hidden" name="r" value="<?=rand(1,100)?>">	
				<table>
					<tr valign="center">
					<td class="srchFont" nowrap>Search: <input type=text name="q" value="<?=$search_query?>" size=40 maxlength="255">
					</td>
					<td>
					<select name="type">
						<?php foreach($types as $key => $value):?>
						<option value="<?=$key?>"<?=($search_type == $key) ? ' selected':''?>><?=$value?></option>
						<?php endforeach;?>
					</select>
					</td>
					<td><input type="submit" class="custButton" style="font-size:11px;background-color:#CC6633" value="search"></td>
					</tr>
				</table>
			</form>
			<?php
			}
			printSearchPanelTop()
			?>
	<!-- search panel -->
	</td>
	<td align="right">
	<img src="/images/logo2.gif">
	</td><td align="right" nowrap>
	<a href="/doc/invite" class="srchLnks" style="text-decoration:none">[invite]</a>
	<a href="/home" class="srchLnks" style="text-decoration:none">[home]</a>
	<?=((defined('AUTH_USERID'))?'<a href="/signout" class="srchLnks" style="text-decoration:none">[sign out]</a>':'')?>
	<a href="/doc/help" class="srchLnks" style="text-decoration:none">[help]</a>
	</td></tr>
</table>
<table cellpadding="0" cellspacing="0" width="850" align="center">
<tr><td class="mainHdr" align="left">Hello, <?=funcs_getNames(AUTH_USER_NAME,false)?>!</td><td class="mainHdr" align="right"><?=AUTH_COLLEGEFULL?></td></tr>
</table>