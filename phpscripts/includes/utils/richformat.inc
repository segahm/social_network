<?php
class BlogFormat extends RichFormat{
	const ALLOWED_TAGS = '<style>,<a>,<abbr>,<acronym>,<b>,<big>,<blockquote>,<br>,<center>,<cite>,<code>,<dd>,<div>,<dl>,<dt>,<em>,<font>,<h1>,<h2>,<h3>,<h4>,<h5>,<h6>,<hr>,<i>,<img>,<li>,<ol>,<p>,<pre>,<s>,<small>,<span>,<strike>,<strong>,<sub>,<sup>,<table>,<tbody>,<td>,<tfoot>,<thead>,<tr>,<tt>,<u>,<ul>';
	private $indexes;	//holds blog indexes informat
	function __construct($html){
		$this->html = $html;
		$this->newHtml = $this->parseDom($html);
		if($this->newHtml === false){
			//error! abort parsing
			return;
		}
		$length = 0;
		$this->newHtml = $this->ignoredChars($this->newHtml,$length);
	}
	protected function parseDom($html){
		$dom = new DOMDocument();
		//init dom properties
		$dom->encoding = 'iso-8859-1';
		$dom->resolveExternals = false;
		$dom->formatOutput = true;
		$dom->preserveWhiteSpace = false;
		$dom->validateOnParse = false;
		//set error handler to record all the errors while parsing
		set_error_handler(
			create_function('$er_code,$error,$errfile, $errline',
				'$GLOBALS["MY_DOM_ERROR"] = $error;'
			));
		$dom->loadHTML($html);
		if (isset($GLOBALS['MY_DOM_ERROR'])){
			$this->error = $GLOBALS['MY_DOM_ERROR'];
			//format error to be user friendly
			$indx = stripos($this->error,']:');
			$setDefaltError = false;
			if ($indx === false){
				//something is wrong, unknown error format
				$setDefaltError = true;
			}
			if ($setDefaltError){
				$this->error = 'we were unable to process this html; please make sure that all tags are complete';
			}else{
				$this->error = substr($this->error,$indx+2);
			}
			return false;
		}
		$dom->normalize();
		restore_error_handler();
		return $dom->saveHTML();
	}
	public function getHtml(){
		return $this->newHtml;
	}
	public function getIndexes(){
		return $this->indexes;
	}
	public function getError(){
		return $this->error;
	}
}
class RichFormat{
	public $html;
	protected $newHtml;
	protected $error = null;
	const ALLOWED_TAGS = '<a>,<abbr>,<acronym>,<b>,<big>,<blockquote>,<br>,<center>,<cite>,<code>,<dd>,<div>,<dl>,<dt>,<em>,<font>,<h1>,<h2>,<h3>,<h4>,<h5>,<h6>,<hr>,<i>,<img>,<li>,<ol>,<p>,<pre>,<s>,<small>,<span>,<strike>,<strong>,<sub>,<sup>,<table>,<tbody>,<td>,<tfoot>,<thead>,<tr>,<tt>,<u>,<ul>';
	function __construct($html){
		$this->html = $html;
		$this->newHtml = $this->parseDom($html);
		if($this->newHtml === false){
			//error! abort parsing
			return;
		}
		$length = 0;
		$this->newHtml = $this->formatHTML($this->newHtml,$length);
		$this->newHtml = $this->ignoredChars($this->newHtml,$length);
	}
	protected function parseDom($html){
		$dom = new DOMDocument();
		//init dom properties
		$dom->encoding = 'iso-8859-1';
		$dom->resolveExternals = false;
		$dom->formatOutput = true;
		$dom->preserveWhiteSpace = false;
		$dom->validateOnParse = false;
		//set error handler to record all the errors while parsing
		set_error_handler(
			create_function('$er_code,$error,$errfile, $errline',
				'$GLOBALS["MY_DOM_ERROR"] = $error;'
			));
		$dom->loadHTML($html);
		if (isset($GLOBALS['MY_DOM_ERROR'])){
			$this->error = $GLOBALS['MY_DOM_ERROR'];
			//format error to be user friendly
			$indx = stripos($this->error,']:');
			$setDefaltError = false;
			if ($indx === false){
				//something is wrong, unknown error format
				$setDefaltError = true;
			}
			if ($setDefaltError){
				$this->error = 'we were unable to process this html; please make sure that all tags are complete';
			}else{
				$this->error = substr($this->error,$indx+2);
			}
			return false;
		}
		$dom->normalize();
		restore_error_handler();
		return $dom->saveHTML();
	}
	public function getHtml(){
		return $this->newHtml;
	}
	public function getError(){
		return $this->error;
	}
	protected function formatHTML($text,&$length){
		//this should also remove comments
		$text = strip_tags($text,self::ALLOWED_TAGS);
		//this should remove all attribute as onclick onmousedown on...
		$rpls = array();
		$with = array();
		//remove all href="javascript stuff
		$rpls[] = '/[ ]?href=(["\']?)javascript:[^>\s]*/is';
		$with[] = '';
		//remove table width dimensions
		//remove all table width parameters with no percentage values
		$rpls[] = '/(<t[^>]+)\swidth=["\']?[0-9]{2,}+[^%]["\']?/is';
		$with[] = '\\1';
		//remove all on* events
		$rpls[] = '/(<[^>]+)\son[^=]*=["\']?[^>\s]+/is';
		$with[] = '\\1';
		//remove all name/id/class attributes
		$rpls[] = '/(<[^>]+\s)(?:class|id|name|CONTENTEDITABLE|HIDEFOCUS|UNSELECTABLE)=["\']?[^>\s]+/is';
		$with[] = '\\1';
		//remove all nowrap keywords
		$rpls[] = '/nowrap/i';
		$with[] = '';
		//remove all illegal styles
		$illegal_styles = array();
		$illegal_styles[] = 'z-index';
		$illegal_styles[] = 'display';
		$illegal_styles[] = 'visibility';
		$illegal_styles[] = 'position';
		$illegal_styles[] = 'ACCELERATOR';
		$illegal_styles[] = 'zoom';
		$rpls[] = '/(<[^>]+\sstyle=[^>]*)(?:'.implode('|',$illegal_styles).')[^;"\'>]*[;]?/is';
		$with[] = '\\1';
		$text = preg_replace($rpls,$with,$text);
		//remove all img tags that don't have width and height specified
		$pos = 0;
		//get length once and keep track of it
		$length = strlen($text);
		while($pos < $length && ($pos = stripos($text,'<img',$pos)) !== false){
			//find the end of the tag
			$sec_pos = stripos($text,'>',$pos);
			$img_string = substr($text,$pos,$sec_pos-$pos+1);
			//it has both width and height attributes
			if (preg_match('/height=["\']?[0-9]{1,3}["\']?[ >]/i',$img_string)
				&& preg_match('/width=["\']?[0-9]{1,3}["\']?[ >]/i',$img_string)){
				$pos++;
				continue;
			}
			$text = substr_replace($text,'',$pos,$sec_pos-$pos+1);
			//update length
			$length -= $sec_pos-$pos+1;
			$pos++;
		}
		//$text = self::formatFonts($text,$length);
		$this->length = $length;
		return $text;
	}
	/**removes \n [\s]+ from string where these chars are not in pre*/
	protected function ignoredChars($text,&$length){
		$pos = 0;
		$sec_pos = 0;
		$beg_del = 0;
		while($sec_pos < $length && $sec_pos !== false 
				&& $pos < $length && ($pos = stripos($text,'<pre',$pos)) !== false){
			$beg_del = $sec_pos;
			//remove chars starting the last $beg_del
			$str_len = $pos-$sec_pos;
			$substr = substr($text,$sec_pos,$str_len);
			$substr = preg_replace('/[\s\n]+/',' ',$substr);
			$length -= $str_len-strlen($substr);
			$text = substr_replace($text,$substr,$sec_pos,$str_len);
			$sec_pos = stripos($text,'</pre>',$pos);
			$pos = $sec_pos+6;
		}
		if ($sec_pos !== false){
			$str_len = $length-$sec_pos;
			$substr = substr($text,$sec_pos,$str_len);
			$substr = preg_replace('/[\s\n]+/',' ',$substr);
			$text = substr_replace($text,$substr,$sec_pos,$str_len);
			//update length
			$length -= $str_len-strlen($substr);
		}
		return $text;
	}
}
?>