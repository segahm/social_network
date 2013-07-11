
DOM = (document.getElementById);
if (DOM) Detect = new BrowserDetector();


/**
 * Author: Chris Wetherell
 * BrowserDetector (object)
 *
 * A class for detecting version 5 browsers by the Javascript objects 
 * they support and not their user agent strings (which can be 
 * spoofed).
 *
 * Warning: Though slow to develop, browsers may begin to add 
 * DOM support in later versions which might require changes to this 
 * file.
 *
 * Warning: No one lives forever.  Presumably.
 *
 * Typical usage:
 * Detect = new BrowserDetector();
 * if (Detect.IE()) //IE-only code...
 */
function BrowserDetector()
{

  //IE 4+
  this.IE = function()
  {
    try {
      return this.Run(document.all && !document.contains)!=false;
    } catch(e) {
      /* IE 5.01 doesn't support the 'contains' object and 
         fails the first test */
      if (document.all) return true;
      return false;
    }
  }
  
  //IE 5.5+
  this.IE_5_5_newer = function()
  {
    try { 
      return this.Run(this.IE() && Array.prototype.pop && !this.OPERA())!=false;
    } catch(e) {return false;}
  }
  
  //IE 5, Macintosh
  this.IE_5_Mac = function()
  {
      try {
        return (true == undefined);
      } catch(e) {
        return (
          document.all
          && document.getElementById 
          && !document.mimeType
          && !this.OPERA()
        )!=false;
      }
  }

  //Opera 7+
  this.OPERA = function()
  {
    try { 
      return this.Run(window.opera)!=false;
    } catch(e) {return false;}
  }

  //Gecko, actually Mozilla 1.2+
  this.MOZILLA = function()
  {
    try { 
      return this.Run(
          document.implementation
          && document.implementation.createDocument
          && !document.contains
          && !this.OPERA()
          )!=false;
    } catch(e) {return false;}
  }

  //Safari
  this.SAFARI = function()
  {
    try {
      return this.Run(
          document.implementation
          && document.implementation.createDocument
          && document.contains
          )!=false;
      } catch(e) {return false;}
  }

  //Any browser which supports the W3C DOM
  this.DOM = function()
  {
    return (document.getElementById);
  }

  this.Run = function(test)
  {
    if (test==undefined) {
      return false;
    } else {
      return test;
    }
  }

}

