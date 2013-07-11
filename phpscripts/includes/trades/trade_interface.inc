<?php
define("TYPE_ITEM_TEXTBOOK",1);
define("TYPE_ITEM_READER",2);
define("TYPE_ITEM_HOUSING",3);
define("TYPE_ITEM_OTHER",4);
define("ITEM_IMAGES_FOLDER","/img/items/");
function feedbackConvert($mixed){
	if (is_array($mixed)){
		//if converting into sql string
		return implode('/',$mixed);
	}else{
		return explode('/',$mixed);
	}
}
class face_var{
	public $iteminfo = 'iteminfo';
	public $isbninfo = 'isbninfo';
	//the following are common variables for all items
	public $common_title = 'title';
	public $common_image = 'image';
	public $common_price = 'price';
	public $common_comments = 'comments'; //or
	public $common_more = 'more';
}
class face_isbn{
	public $listprice = 'listprice';
	public $publisher = 'publisher';
	public $binding = 'binding';
	public $pages = 'pages';
	public $mimage = 'mimage';
	public $simage = 'simage';
	public $limage = 'limage';
	public $isbn = 'isbn';
	public $title = 'title';
	public $author = 'author';
	function getMajorImage($isbninfo,&$width,&$height,&$border){
		$width = null;
		$height = null;
		$border = 1;
		if (isset($isbninfo[$this->mimage])){
			$image = $isbninfo[$this->mimage];
		}elseif(isset($isbninfo[$this->limage])){
			//cut width and height
			$array = getimagesize($isbninfo[$this->limage]);
			$width = $array[0];
			$height = $array[1];
			funcs_imageResize($width,$height,120,true);
			$image = $isbninfo[$this->limage];
		}else{
			//set default image
			$image = "/images/noimage.gif";
			$border = 0;
		}
		return $image;
	}
}
class face_textbook{
	public $isbn = 'isbn';
	public $title = 'title';
	public $author = 'author';
	public $course = 'course';
	public $teacher = 'teacher';
	public $price = 'price';
	public $condition = 'condition';
	public $comments = 'comments';
}
class face_reader{
	public $title = 'title';
	public $year = 'year';
	public $course = 'course';
	public $teacher = 'teacher';
	public $price = 'price';
	public $condition = 'condition';
	public $comments = 'comments';
	public $quarter = 'quarter';
}
class face_housing{
	public $title = 'title';
	public $location = 'location';
	public $price = 'price';
	public $utils = 'utils';
	public $sex = 'sex';
	public $arrangement = 'arrangement';
	public $more = 'more';
	public $image = 'image';
}
class face_other{
	public $title = 'title';
	public $price = 'price';
	public $more = 'more';
	public $image = 'image';
}
?>