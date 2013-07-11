<!--
var stp = 30;    // step increment size
var spd = 20;   // speed of increment
var tim;         // timer variable
var scroll_div_element;
var upprIndx = 0;
var lwrIndx = 0;
var size; 
var scroll_name;
var temp = null;
var scr_checked = 0; //keeps track of how many are checked
var sex_images = [new Image,new Image,new Image];
sex_images[0].src = "/images/memb0.gif";
sex_images[1].src = "/images/memb1.gif";
sex_images[2].src = "/images/memb2.gif";
/**requires 3 arrays to be decalred: scroll_array_nms, scroll_array_ids scroll_array_sxs*/
function scroll_setInitial(element){
	scroll_name = element;
	scroll_div_element = document.getElementById(element);
	if (scroll_array_nms == null){
		scroll_do();
		return;
	}
	size = Math.min(10,scroll_array_nms.length);
	lwrIndx = Math.min(upprIndx+size-1,scroll_array_nms.length-1)
	if (scroll_array_checked == null){
		scroll_array_checked = new Array(scroll_array_nms.length);
		for (var i=0;i<scroll_array_nms.length;i++){
			scroll_array_checked[i] = 0;;
		}
	}
	scroll_do();
}
function scroll_up() {
	if (scroll_array_nms == null){
		return;
	}
	if (temp == null){
		temp = Math.max(upprIndx-size,0);
	}
	if (upprIndx > temp){
		upprIndx--;
		lwrIndx--;
		scroll_do();
		tim = setTimeout("scroll_up()", spd);
	}else{
		temp = null;
	}
}

function scroll_dn() {
	if (scroll_array_nms == null){
		return;
	}
	if (temp == null){
		temp = Math.min(lwrIndx+size,scroll_array_nms.length-1);
	}
	if (lwrIndx < temp){
		upprIndx++;
		lwrIndx++;
		scroll_do();
		tim = setTimeout("scroll_dn()", spd);
	}else{
		temp = null;
	}
}
function scroll_do(){
	if (scroll_array_nms == null){
		scroll_div_element.innerHTML = "<font style=\"font-size:10px;\">your buddy list is empty</font>";
		return;
	}
	var text = "<form name=\""+scroll_name+"\" method=\"POST\" style=\"margin:0px;\"><table cellpadding=\"0\" cellspacing=\"0\">";
	for (var i= upprIndx;i<=lwrIndx;i++){
		text+= "<tr><td><img src=\""+sex_images[scroll_array_sxs[i]].src+"\"></td><td><input id=\"scrlFr_"
			+i+"\" "+((scroll_array_checked[i] == 1)?"checked":"")
			+" onClick=\"javascript:scroll_check('"+i+"')\" type=\"checkbox\" name=\"id_"
			+scroll_array_ids[i]+"\"></td><td><label for=\"scrlFr_"+i
			+"\" style=\"font-size:11px;color:#008000;\">"+scroll_array_nms[i]+"</label></td></tr>";
	}
	scroll_div_element.innerHTML = text+"</table></form>";
}
function scroll_toString(){
	if (scroll_array_ids == null){
		return '';
	}
	var text = '';
	var flag = false;
	for (var i=0;i<scroll_array_ids.length;i++){
		if (scroll_array_checked[i] == 1){
			if (flag){
				text += ',';
			}
			text += scroll_array_ids[i];
			flag = true;
		}
	}
	return text;
}
function scroll_check(index){
	if (scroll_array_checked[index] == 0){
		scroll_array_checked[index]=1;
		scr_checked++;
	}else{
		scroll_array_checked[index]=0;
		scr_checked--;
	}
}
function scrool_checkAll(){
	if (scroll_array_nms != null){
		for (var i=0;i<scroll_array_nms.length;i++){
			scroll_array_checked[i] =1;
		}
		scr_checked = scroll_array_checked.length;
		scroll_do();
	}
}
function scrool_checkNone(){
	if (scroll_array_nms != null){
		for (var i=0;i<scroll_array_nms.length;i++){
			scroll_array_checked[i] =0;
		}
		scr_checked = 0;
		scroll_do();
	}
}
function scroll_no() {
	clearTimeout(tim);
}
function scroll_friends_submit(path){
	//submit only if something is checked
	if (scr_checked > 0){
		eval("document."+scroll_name+".action = \""+path+"\"");
		eval("document."+scroll_name+".submit()");
	}
}
//-->
