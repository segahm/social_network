<script language="javascript">
<!--
var dragapproved=false;
var z,x,y;
function move(){
	if (event.button==1&&dragapproved){
		z.style.pixelLeft=temp1+event.clientX-x;
		z.style.pixelTop=temp2+event.clientY-y;
		return false;
	}
}
function drags(){
	if (!document.all)
		return;
	if (event.srcElement.className=="drag"){
		dragapproved=true;
		z=event.srcElement;
		temp1=z.style.pixelLeft;
		temp2=z.style.pixelTop;
		x=event.clientX;
		y=event.clientY;
		document.onmousemove=move;
	}
}
document.onmousedown=drags();
document.onmouseup=new Function("dragapproved=false");
//-->
</script>