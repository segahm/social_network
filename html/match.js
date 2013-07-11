//last index found for the lastField
var indexes = null;
var inner_index = 0;
//which input field did we applythe last index
var lastField = null;
var div_name = null;
var whichArray = null;
var field = null;
function match_focus(a,f,d){
	div_name = d;
	field = f;
	whichArray = a;
	if (field.attachEvent){
		field.onkeyup = matchFriend;
	}else if(field.addEventListener){
		field.onkeyup = matchFriend;
	}
}
function match_blur(){
	displayMatch(div_name,'',null,0);
}
/**requires: match_array to be declared that holds other array[s], if empty
 *then needs to == to null; field is the field that holds the search string value,
 *div_name is the id of the div element where match will be shown 
 *(where div_name+"_in") is the inner td id*/
function matchFriend(event_){
	if (!event_) var event_ = window.event;
	var str;
	var limit = 6;	//how many to show at a time
	if (match_array != null){
		var array = match_array[whichArray];
	}else{
		return false;
	}
	str = field.value;
	if (array != null){
		//last comma index
		var inx = str.lastIndexOf(",");
		(inx == -1) ? inx = 0 : inx = inx+1;
		str = Trim(str.substring(inx,str.length));
		//enter(13) or down(40) was pressed
		if (typeof(event_) != "undefined" 
				   && (event_.keyCode == 13 || event_.keyCode == 38 || event_.keyCode == 40) && indexes != null){
			//if need to move index to another found word
			if ((event_.keyCode == 40 && inner_index<(indexes.length-1)) 
				|| (event_.keyCode == 38 && inner_index>0)){
				(event_.keyCode == 40)?inner_index++:inner_index--;
				var words = Array();
				for (var i =0;i<indexes.length && i<limit;i++){
					words.push(array[indexes[i]]);
				}
				displayMatch(div_name,str,words,inner_index);
			}else{
				if (inx == -1){
					field.value = array[indexes[inner_index]];
				}else{
					field.value = field.value.substring(0,inx)+array[indexes[inner_index]];
				}
				displayMatch(div_name,"",null,0);
			}
			return true;
		}
		if (str != ""){
			//no need to perform a full search if the same word
			var index;	//index to start searching from
			if (indexes != null && lastField == field){
				index = indexes[inner_index];
			}else{
				index = null;
			}
			lastField = field;
			indexes = binarySearch(array, str, true, true,index);
			if (indexes != null){
				var words = Array();
				for (var i =0;i<indexes.length && i<limit;i++){
					words.push(array[indexes[i]]);
				}
				inner_index = 0;
				displayMatch(div_name,str,words,inner_index);
			}
		}
		//no results
		if (str == "" || indexes == null){
			displayMatch(div_name,str,null,0);
			inner_index = 0;
			indexes = null;
		}
	}
}
function displayMatch(div_name,wordmatched,words,inner_index){
	var div = getObjectByID(div_name);
	if (words != null && wordmatched != ""){
		div.style.display = "block";
		text = '<table cellpadding="0" cellspacing="0">';
		for (var i=0;i<words.length;i++){
			var half_1 = words[i].substring(0,wordmatched.length-1);
			var half_2 = words[i].substring(wordmatched.length-1,wordmatched.length);
			var half_3 = words[i].substring(wordmatched.length,words[i].length);
			//if selected and there is more than 1
			text += '<tr><td class="'+((inner_index == i && words.length != 1)?'mtchr_slctd':'mtchr')+'" style="'
				//if bottom line + if top element
				+((i==(words.length-1))?'border-bottom-width:1px;':'')+((i==0)?'border-top-width:1px;':'')+'">';
			text += '<font color=\"'+((inner_index != i || words.length == 1)?'#333399':'#CCCCCC')+'\">'
				+half_1
				+'</font>'+'<font color="#FF0000">'+half_2
				+'</font>'+((inner_index == i && words.length != 1)?'<font color="#FFFFFF">'+half_3+'</font>':half_3)+'</td></tr>';
		}
		text += '</table>';
		setInnerHTML(div,text);
	}else{
		div.style.display = "none";
	}
}