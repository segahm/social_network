//returns an array of all indexes that match criteria
function binarySearch(array, find, caseInsensitive, getSubstring, arrayCheckThisIndex){
	find=(!caseInsensitive)?find:find.toLowerCase();
	//Initialize:
	var low=0;
	var high=array.length-1;
	//find out which way is the array sorted
	var elem1 = array[0];
	elem1=(!caseInsensitive)?elem1:elem1.toLowerCase();
	var elemn = array[array.length-1];
	elemn=(!caseInsensitive)?elemn:elemn.toLowerCase();
	
	var highOnTop=(elem1>elemn)?true:false;
	var aTry ;
	if (arrayCheckThisIndex != null){
			aTry = arrayCheckThisIndex;
	}
	var flag = false;
	//run search
	while(low<=high){
		//chooses the rightest if even
		//if by default we need to check a certain index
		if (flag || arrayCheckThisIndex == null){
			aTry=((low+high)==1)?0:Math.ceil((low+high)/2.0);
		}
		flag = true;
		var checkThis=array[aTry];
		checkThis=(!caseInsensitive)?checkThis:checkThis.toLowerCase();
		checkThis=(!getSubstring)?checkThis:checkThis.substring(0, find.length);
		if(!highOnTop){
			if(checkThis<find){low=aTry+1;continue;};
			if(checkThis>find){high=aTry-1;continue;};
		}else{
			if(checkThis>find){low=aTry+1;continue;};
			if(checkThis<find){high=aTry-1;continue;};
		};
		result = [aTry];
		//check all neihboring indexes
		var index = aTry-1;
		while (index>=0){
			var checkThis=array[index];
			checkThis=(!caseInsensitive)?checkThis:checkThis.toLowerCase();
			checkThis=(!getSubstring)?checkThis:checkThis.substring(0, find.length);
			if (checkThis != find){
				break;
			}
			result.push(index);
			--index;
		}
		++aTry;
		while (aTry<array.length){
			var checkThis=array[aTry];
			checkThis=(!caseInsensitive)?checkThis:checkThis.toLowerCase();
			checkThis=(!getSubstring)?checkThis:checkThis.substring(0, find.length);
			if (checkThis != find){
				break;
			}
			result.push(aTry);
			++aTry;
		}
		return result;
	}
	return null;
}