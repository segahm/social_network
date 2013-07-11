/**
 * Using a factory class to avoid namespace collisions
 * of common ID names for different purposes
 */
function Preview() {}
function Posting() {}
function HtmlSource() {}

// Common IDs, class names, and other constants
HtmlSource.TEXTAREA = "textarea";

Preview.ID = "preview";
Preview.TEXTAREA = HtmlSource.TEXTAREA;
Preview.PREVIEW_BUTTON = "formatbar_PreviewAction";
Preview.HTML_PREVIEW_BUTTON = "htmlbar_PreviewAction";

Posting.PUBLISH_BUTTON = "publishPost";
Posting.DRAFT_BUTTON = "saveDraft";
Posting.OPTIONS = "postoptions";
Posting.TITLE = "f-title";
Posting.URL = "f-address";