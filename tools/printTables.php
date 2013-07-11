<? require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/DBconnect.inc");?>
<?php
	db::connect();
	$outputFile = $_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/tables.inc";
	$result = db::query("SHOW TABLES");
	//iterate through tables
	$out1 = "<?php\n";
	$out = "";
	while ($row = db::fetch_row($result)){
		$tablename = $row[0];
		$out1 .= "define(\"TABLE_" . strtoupper($tablename) . "\",\"" . $tablename . "\");\n";
		$out .= "\nclass table_" . $tablename . "{\n";
		$result2 = db::query("describe " . $tablename);
		while ($row2 = db::fetch_row($result2)){
			$fieldname = $row2[0];
			$out .= "\tpublic \$field_" . $fieldname . " = \"" . $fieldname . "\";\n";
			$type = $row2[1];
			if (!eregi('int',$type) && eregi('^.+\([0-9]+\)$',$type)){
				$out .= "\tpublic \$field_" . $fieldname . "_length = " . ereg_replace('^.+\(([0-9]+)\)$','\\1',$type) . ";\n";
			}
		}
		$out .= "}\n";
	}
	$out .= "?>";
	//writing output 
	@ $fp = fopen($outputFile,'w');
	if ($fp){
		fwrite($fp,$out1,strlen($out1));
		fwrite($fp,$out,strlen($out));
		fclose($fp);
	}
	db::close();
?>