<?
//will be inserted into buttons
$img_string = 'border="0" style="background-image: url(/images/edit/bg.gif);" onMouseOver="edit_highlight(this,true);" onMouseOut="edit_highlight(this,false);"';
?>
<script language="javascript">
init('rich_frmt_area','rich_frmt_disp');
var panel = getObjectByID('rich_frmt_panel');
<?
$text = <<<EOD
<table cellpadding="2" cellspacing="0">
<tr valign="middle"><td>
<select onchange="surroundText('<span style=\"font-family:'+this.value+'\">','</span>');this.value = '-1';" style="font-size:10px;margin:0px;padding:0px;;">
	<option value="-1" selected>Font</option>
	<option value="arial">Arial</option>
	<option value="courier new">Courier</option>
	<option value="georgia">Georgia</option>
	<option value="lucida grande">Lucida Grande</option>
	<option value="times new roman">Times</option>
	<option value="trebuchet ms">Trebuchet</option>
	<option value="verdana">Verdana</option>
	<option value="webdings">Webdings</option>
</select>
</td><td>
<img alt="insert url address" src="/images/edit/url.gif" $img_string onClick="surroundText('','[url:http://www.example.com]example title[/url]');">&nbsp;
<img alt="align left" src="/images/edit/pleft.gif" $img_string onClick="surroundText('<p style=\"textAlign:left\">','</p>');">&nbsp;
<img alt="align center" src="/images/edit/pcenter.gif" $img_string onClick="surroundText('<p style=\"textAlign:center\">','</p>');">&nbsp;
<img alt="align right" src="/images/edit/pright.gif" $img_string onClick="surroundText('<p style=\"textAlign:right\">','</p>');">&nbsp;
<img alt="pre" src="/images/edit/pre.gif" $img_string onClick="surroundText('<pre>','</pre>');">&nbsp;
<img alt="font weight" src="/images/edit/bold.gif" $img_string onClick="surroundText('<span style=\"font-weight:bold\">','</span>');">&nbsp;
<img alt="underline" src="/images/edit/underline.gif" $img_string onClick="surroundText('<span style=\"textDecoration:underline\">','</span>');">&nbsp;
<img alt="italicize" src="/images/edit/italicize.gif" $img_string onClick="surroundText('<span style=\"font-style:italic\">',',</span>');">&nbsp;
</td></tr>
</table>
EOD;
$reaplce = array();
$with = array();
$replace[] = "\n";
$with[] = '';
$replace[] = "\r";
$with[] = '';
$replace[] = "'";
$with[] = "\\'";
$text = str_replace($replace,$with,$text);
?>
var text = '<?=$text?>';
setInnerHTML(panel,text);
</script>