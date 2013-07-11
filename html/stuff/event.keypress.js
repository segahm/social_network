
BACKSPACE = null;
DELETE = null;
RETURN = null;
TAB = null;
CTRL_SHFT_A = null;
CTRL_SHFT_B = null;
CTRL_SHFT_T = null;
CTRL_SHFT_L = null;
CTRL_SHFT_P = null;
CTRL_SHFT_S = null;
CTRL_SHFT_D = null;
CTRL_SHFT_U = null;
CTRL_SHFT_Z = null;
CTRL_B = null;
CTRL_D = null;
CTRL_I = null;
CTRL_L = null;
CTRL_P = null;
CTRL_S = null;
CTRL_Y = null;
CTRL_Z = null;

IE_KEYSET = (Detect.IE() || Detect.SAFARI());

function setKeysetByEvent(e) {
  
  // IE delivers a different keyset per key event type.  Additionally, 
  // 'keydown' is different than 'keypress' but only in terms of the 
  // ctrl + shift combination.  Ugh.  Safari is more consistent.  Gecko is
  // right on.
  IE_CTRL_SHIFT_KEYSET = IE_KEYSET;  
  if (Detect.IE() && e && e.type == "keydown") IE_CTRL_SHIFT_KEYSET = false;
  
  BACKSPACE = (getKey(e) == 8);
  DELETE = (getKey(e) == 46);
  RETURN = (getKey(e) == 13);
  TAB = (getKey(e) == 9);
  
  CTRL_SHFT_A = (IE_CTRL_SHIFT_KEYSET) ? 
    isKeyPressedWithCtrlShift(1, e) : isKeyPressedWithCtrlShift(65, e);

  CTRL_SHFT_B = (IE_CTRL_SHIFT_KEYSET) ? 
    isKeyPressedWithCtrlShift(2, e) : isKeyPressedWithCtrlShift(66, e);
  
  CTRL_SHFT_D = (IE_CTRL_SHIFT_KEYSET) ? 
    isKeyPressedWithCtrlShift(4, e) : isKeyPressedWithCtrlShift(68, e);
  
  CTRL_SHFT_L = (IE_CTRL_SHIFT_KEYSET) ? 
    isKeyPressedWithCtrlShift(12, e) : isKeyPressedWithCtrlShift(76, e);
  
  CTRL_SHFT_P = (IE_CTRL_SHIFT_KEYSET) ? 
    isKeyPressedWithCtrlShift(16, e) : isKeyPressedWithCtrlShift(80, e);
  
  CTRL_SHFT_S = (IE_CTRL_SHIFT_KEYSET) ? 
    isKeyPressedWithCtrlShift(19, e) : isKeyPressedWithCtrlShift(83, e);

  CTRL_SHFT_T = (IE_CTRL_SHIFT_KEYSET) ? 
    isKeyPressedWithCtrlShift(20, e) : isKeyPressedWithCtrlShift(84, e);
  
  CTRL_SHFT_U = (IE_CTRL_SHIFT_KEYSET) ? 
    isKeyPressedWithCtrlShift(21, e) : isKeyPressedWithCtrlShift(85, e);
  
  CTRL_SHFT_Z = (IE_CTRL_SHIFT_KEYSET) ? 
    isKeyPressedWithCtrlShift(26, e) : isKeyPressedWithCtrlShift(90, e);
  
  CTRL_B = (IE_KEYSET) ? 
    isKeyPressedWithCtrl(66, e) : isKeyPressedWithCtrl(98, e);
  
  CTRL_D = (IE_KEYSET) ? 
    isKeyPressedWithCtrl(68, e) : isKeyPressedWithCtrl(100, e);
  
  CTRL_I = (IE_KEYSET) ? 
    isKeyPressedWithCtrl(73, e) : isKeyPressedWithCtrl(105, e);
  
  CTRL_L = (IE_KEYSET) ? 
    isKeyPressedWithCtrl(76, e) : isKeyPressedWithCtrl(108, e);
  
  CTRL_P = (IE_KEYSET) ? 
    isKeyPressedWithCtrl(80, e) : isKeyPressedWithCtrl(112, e);
  
  CTRL_S = (IE_KEYSET) ? 
    isKeyPressedWithCtrl(83, e) : isKeyPressedWithCtrl(115, e);
  
  CTRL_Y = (IE_KEYSET) ? 
    isKeyPressedWithCtrl(89, e) : isKeyPressedWithCtrl(121, e);
  
  CTRL_Z = (IE_KEYSET) ? 
    isKeyPressedWithCtrl(90, e) : isKeyPressedWithCtrl(122, e);

}



/**
* isCtrlShiftKeyPressed()
 *
 * Determine by char index whether a certain key's been pressed in conjunction 
 * with the CTRL and SHIFT keys.
 */
function isKeyPressedWithCtrlShift(num, e) {
  var key = getKeyAfterCtrlAndShift(e);
  if (key) return (key == num);
  return false;
}

function isKeyPressedWithCtrl(num, e) {
  var key = getKeyAfterCtrl(e);
  if (key) return (key == num);
  return false;
}

function isKeyPressedWithShift(num, e) {
  var key = getKeyAfterShift(e);
  if (key) return (key == num);
  return false;
}



// The following functions help manage some differing browser event models and  
// key detection.
function getKeyAfterCtrl(e) {
  if (isCtrlKeyPressed(e)) { return getKey(e); }
  return false;   
}

function getKeyAfterShift(e) {
  if (isShiftKeyPressed(e)) { return getKey(e); }
  return false;   
}

function getKeyAfterCtrlAndShift(e) {
  if (isCtrlKeyPressed(e) && isShiftKeyPressed(e)) { return getKey(e); }
  return false;   
}

function isCtrlKeyPressed(e) {
  return getEvent(e).ctrlKey;
}

function isShiftKeyPressed(e) {
  return getEvent(e).shiftKey;
}

function isAltKeyPressed(e) {
  return getEvent(e).altKey;
}

function getKey(e) {
  var key = getEvent(e).keyCode;
  if (!key) key = getEvent(e).charCode; 
  return key;
}

function getEventSource(evt) { 
  if (Detect.IE()) {
    return evt.srcElement; 
  } else {
    return evt.target;
  }
}

function getEvent(e) {
  return (!e) ? event : e;
}