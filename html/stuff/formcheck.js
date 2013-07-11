var formClean = null;
var formCollection = null;
var isSubmit = false;

// Loop through form(s)... storing data in an array (cleanForm)
function cleanForm() {
  formCollection = document.body.getElementsByTagName("FORM");
	
  formClean = new Array(formCollection.length);
  var c = 0;
	
  for(var i = 0; i < formCollection.length; i++) {
    thisForm = formCollection[i];
		
    for(var x = 0; x < thisForm.elements.length; x++) {
			
      formClean[c] = getFormElementValue(thisForm, x);
      c = c+1;
    }
  }
}


// Run through the dirty form(s), checking against the clean one for a miss-match
function dirtyForm() {
	/* If cleanForm() has not run we assume that user has not modified the
	   form.  This only occurs when the user changes pages before the
           page loads. */
	if (formClean == null) { 
		return true;
	}

	var c = 0;

	for(var i = 0; i < formCollection.length; i++) {
		thisForm = formCollection[i];
		
		for(var x = 0; x < thisForm.elements.length; x++) { 
			if (formClean[c] != getFormElementValue(thisForm, x)) {
				return false;
			}
			c = c+1;
		}
	}	
	return true;
}

function FlexibleSubmit(frm) {
  isSubmit=true;
  frm.submit();
}

function getFormElementValue(form, intElementIndex) {
  var el = form[intElementIndex];
  if (el.type == "radio") {
    return getRadioElementValue(form[el.name]);
  } else if (el.type == "checkbox") {
    return getCheckboxElementValue(form[el.name]);
  } else {
    return el.value;
  }
}

function getCheckboxElementValue(checkbox) {
  if (checkbox.checked) {
    return checkbox.value;
  } else {
    return null;
  }
}

function getRadioElementValue(radio) {
  for (var j = 0; j < radio.length; j++) {
    if (radio[j].checked) {
      return radio[j].value;
    } else {
      return null;
    }
  }
}

// onbeforeunload function. if dirty, prompt for action
function compareForm(event_) {
	if (!event_ && window.event) {
          event_ = window.event;
        }

	// don't run if submit button was clicked
	if (!isSubmit) {
		var results = dirtyForm();

		if (!results) {
      //unsaved_changes is defined in JsConstants.gxp
			event_.returnValue = unsaved_changes;

			return unsaved_changes;
		}
	}
}
