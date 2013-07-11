<?php
define("TABLE_ALBUMS","albums");
define("TABLE_BLOCKLIST","blocklist");
define("TABLE_BLOG","blog");
define("TABLE_BLOGCOMMENTS","blogcomments");
define("TABLE_BLOGSETTINGS","blogsettings");
define("TABLE_COLLEGES","colleges");
define("TABLE_COURSES","courses");
define("TABLE_DELETEDEVENTS","deletedevents");
define("TABLE_DEPARTMENTS","departments");
define("TABLE_EVENTS","events");
define("TABLE_FACEBOOK","facebook");
define("TABLE_FRIENDGROUPS","friendgroups");
define("TABLE_FRIENDS","friends");
define("TABLE_FRIENDS_IN_GROUPS","friends_in_groups");
define("TABLE_IDS","ids");
define("TABLE_INVITE_EMAILS","invite_emails");
define("TABLE_ISBNS","isbns");
define("TABLE_MAIL","mail");
define("TABLE_MESSAGES","messages");
define("TABLE_MYCOURSES","mycourses");
define("TABLE_PICTURES","pictures");
define("TABLE_PROFILE","profile");
define("TABLE_RIDEBOARD","rideboard");
define("TABLE_SESSION","session");
define("TABLE_TEMPEMAILS","tempemails");
define("TABLE_TEMPFRIENDS","tempfriends");
define("TABLE_TEMPISBN","tempisbn");
define("TABLE_TEMPUSERS","tempusers");
define("TABLE_TOPICMESSAGES","topicmessages");
define("TABLE_TOPICS","topics");
define("TABLE_TRADEBIDS","tradebids");
define("TABLE_TRADEFEEDBACK","tradefeedback");
define("TABLE_TRADEHISTORY","tradehistory");
define("TABLE_TRADEITEMS","tradeitems");
define("TABLE_USERS","users");
define("TABLE_USERS_IN_EVENTS","users_in_events");

class table_albums{
	public $field_ownerid = "ownerid";
	public $field_ownerid_length = 5;
	public $field_albumid = "albumid";
	public $field_albumid_length = 6;
	public $field_title = "title";
	public $field_title_length = 40;
	public $field_isdalbum = "isdalbum";
	public $field_permissions = "permissions";
	public $field_date = "date";
}

class table_blocklist{
	public $field_blockedid = "blockedid";
	public $field_blockedid_length = 5;
	public $field_blockerid = "blockerid";
	public $field_blockerid_length = 5;
	public $field_date = "date";
}

class table_blog{
	public $field_id = "id";
	public $field_id_length = 5;
	public $field_settingsid = "settingsid";
	public $field_settingsid_length = 7;
	public $field_timedate = "timedate";
	public $field_text = "text";
	public $field_title = "title";
	public $field_title_length = 40;
}

class table_blogcomments{
	public $field_blogid = "blogid";
	public $field_blogid_length = 7;
	public $field_commentid = "commentid";
	public $field_commentid_length = 5;
	public $field_userid = "userid";
	public $field_userid_length = 5;
	public $field_time = "time";
	public $field_text = "text";
}

class table_blogsettings{
	public $field_id = "id";
	public $field_id_length = 7;
	public $field_owner = "owner";
	public $field_owner_length = 5;
	public $field_permissions = "permissions";
	public $field_date = "date";
	public $field_header = "header";
	public $field_header_length = 40;
	public $field_custom = "custom";
}

class table_colleges{
	public $field_id = "id";
	public $field_fullname = "fullname";
	public $field_fullname_length = 50;
	public $field_emailformat = "emailformat";
	public $field_emailformat_length = 20;
	public $field_shortname = "shortname";
	public $field_shortname_length = 6;
	public $field_majors = "majors";
	public $field_houses = "houses";
	public $field_more = "more";
}

class table_courses{
	public $field_id = "id";
	public $field_number = "number";
	public $field_number_length = 10;
	public $field_description = "description";
	public $field_description_length = 60;
	public $field_fieldid = "fieldid";
	public $field_collegeid = "collegeid";
}

class table_deletedevents{
	public $field_eventid = "eventid";
	public $field_eventid_length = 8;
	public $field_myid = "myid";
	public $field_myid_length = 5;
	public $field_date = "date";
}

class table_departments{
	public $field_id = "id";
	public $field_description = "description";
	public $field_description_length = 50;
	public $field_collegeid = "collegeid";
}

class table_events{
	public $field_id = "id";
	public $field_id_length = 8;
	public $field_title = "title";
	public $field_title_length = 60;
	public $field_text = "text";
	public $field_date = "date";
	public $field_postedby = "postedby";
	public $field_postedby_length = 5;
	public $field_permissions = "permissions";
	public $field_type = "type";
	public $field_type_length = 20;
	public $field_location = "location";
	public $field_location_length = 60;
	public $field_host = "host";
	public $field_host_length = 60;
	public $field_time = "time";
	public $field_time_length = 10;
}

class table_facebook{
	public $field_userid = "userid";
	public $field_userid_length = 5;
	public $field_fb_user = "fb_user";
	public $field_fb_user_length = 60;
	public $field_fb_pass = "fb_pass";
	public $field_fb_pass_length = 20;
	public $field_time = "time";
	public $field_isread = "isread";
	public $field_message = "message";
}

class table_friendgroups{
	public $field_ownerid = "ownerid";
	public $field_ownerid_length = 5;
	public $field_groupid = "groupid";
	public $field_groupid_length = 9;
	public $field_title = "title";
	public $field_title_length = 20;
	public $field_permissions = "permissions";
}

class table_friends{
	public $field_user1 = "user1";
	public $field_user1_length = 5;
	public $field_user2 = "user2";
	public $field_user2_length = 5;
}

class table_friends_in_groups{
	public $field_ownerid = "ownerid";
	public $field_ownerid_length = 5;
	public $field_groupid = "groupid";
	public $field_groupid_length = 9;
	public $field_userid = "userid";
	public $field_userid_length = 5;
}

class table_ids{
	public $field_ownerid = "ownerid";
	public $field_ownerid_length = 15;
	public $field_subjectid = "subjectid";
	public $field_subjectid_length = 15;
}

class table_invite_emails{
	public $field_email = "email";
	public $field_email_length = 60;
	public $field_userid = "userid";
	public $field_userid_length = 5;
	public $field_time = "time";
	public $field_id = "id";
	public $field_id_length = 5;
	public $field_isread = "isread";
	public $field_name = "name";
	public $field_name_length = 30;
}

class table_isbns{
	public $field_isbn = "isbn";
	public $field_isbn_length = 13;
	public $field_title = "title";
	public $field_title_length = 200;
	public $field_author = "author";
	public $field_author_length = 100;
	public $field_text = "text";
}

class table_mail{
	public $field_id = "id";
	public $field_message = "message";
}

class table_messages{
	public $field_messageid = "messageid";
	public $field_messageid_length = 10;
	public $field_to = "to";
	public $field_to_length = 5;
	public $field_from = "from";
	public $field_from_length = 5;
	public $field_subject = "subject";
	public $field_subject_length = 100;
	public $field_body = "body";
	public $field_date = "date";
	public $field_isread = "isread";
	public $field_isread_length = 1;
}

class table_mycourses{
	public $field_id = "id";
	public $field_id_length = 7;
	public $field_myid = "myid";
	public $field_myid_length = 5;
	public $field_course = "course";
	public $field_course_length = 60;
	public $field_department = "department";
	public $field_number = "number";
	public $field_number_length = 10;
}

class table_pictures{
	public $field_albumid = "albumid";
	public $field_albumid_length = 6;
	public $field_pictureid = "pictureid";
	public $field_pictureid_length = 10;
	public $field_title = "title";
	public $field_title_length = 100;
	public $field_public = "public";
	public $field_public_length = 5;
	public $field_datetime = "datetime";
}

class table_profile{
	public $field_id = "id";
	public $field_id_length = 5;
	public $field_status = "status";
	public $field_major = "major";
	public $field_majorsecond = "majorsecond";
	public $field_house = "house";
	public $field_room = "room";
	public $field_room_length = 60;
	public $field_mailbox = "mailbox";
	public $field_mailbox_length = 60;
	public $field_phone = "phone";
	public $field_phone_length = 20;
	public $field_cellphone = "cellphone";
	public $field_cellphone_length = 20;
	public $field_year = "year";
	public $field_year_length = 4;
	public $field_address = "address";
	public $field_address_length = 60;
	public $field_city = "city";
	public $field_city_length = 60;
	public $field_state = "state";
	public $field_state_length = 2;
	public $field_zip = "zip";
	public $field_zip_length = 5;
	public $field_picture = "picture";
	public $field_picture_length = 14;
	public $field_AIM = "AIM";
	public $field_AIM_length = 60;
	public $field_lastupdated = "lastupdated";
	public $field_hs = "hs";
	public $field_hs_length = 60;
	public $field_sex = "sex";
	public $field_style = "style";
	public $field_aboutme = "aboutme";
	public $field_interests = "interests";
}

class table_rideboard{
	public $field_uniqueid = "uniqueid";
	public $field_to = "to";
	public $field_to_length = 100;
	public $field_from = "from";
	public $field_from_length = 100;
	public $field_date = "date";
	public $field_user = "user";
	public $field_user_length = 5;
	public $field_school = "school";
	public $field_school_length = 3;
	public $field_seats = "seats";
	public $field_seats_length = 2;
	public $field_kind = "kind";
	public $field_kind_length = 1;
	public $field_time = "time";
	public $field_time_length = 7;
}

class table_session{
	public $field_sessionid = "sessionid";
	public $field_sessionid_length = 20;
	public $field_userid = "userid";
	public $field_userid_length = 5;
	public $field_time = "time";
	public $field_storage = "storage";
}

class table_tempemails{
	public $field_genid = "genid";
	public $field_genid_length = 10;
	public $field_email = "email";
	public $field_email_length = 60;
	public $field_date = "date";
	public $field_userid = "userid";
	public $field_userid_length = 5;
}

class table_tempfriends{
	public $field_inviter = "inviter";
	public $field_inviter_length = 5;
	public $field_invited = "invited";
	public $field_invited_length = 5;
	public $field_date = "date";
}

class table_tempisbn{
	public $field_isbn = "isbn";
	public $field_isbn_length = 13;
}

class table_tempusers{
	public $field_id = "id";
	public $field_id_length = 10;
	public $field_name = "name";
	public $field_name_length = 40;
	public $field_email = "email";
	public $field_email_length = 60;
	public $field_school = "school";
	public $field_date = "date";
}

class table_topicmessages{
	public $field_topicid = "topicid";
	public $field_topicid_length = 9;
	public $field_messageid = "messageid";
	public $field_messageid_length = 5;
	public $field_message = "message";
	public $field_userid = "userid";
	public $field_userid_length = 5;
	public $field_time = "time";
}

class table_topics{
	public $field_id = "id";
	public $field_id_length = 4;
	public $field_topic = "topic";
	public $field_topic_length = 100;
	public $field_date = "date";
	public $field_userid = "userid";
	public $field_userid_length = 5;
}

class table_tradebids{
	public $field_buyer = "buyer";
	public $field_buyer_length = 5;
	public $field_seller = "seller";
	public $field_seller_length = 5;
	public $field_itemid = "itemid";
	public $field_itemid_length = 9;
	public $field_date = "date";
}

class table_tradefeedback{
	public $field_owner = "owner";
	public $field_owner_length = 5;
	public $field_user = "user";
	public $field_user_length = 5;
	public $field_itemid = "itemid";
	public $field_itemid_length = 9;
	public $field_comment = "comment";
	public $field_date = "date";
	public $field_score = "score";
}

class table_tradehistory{
	public $field_id = "id";
	public $field_id_length = 9;
	public $field_seller = "seller";
	public $field_seller_length = 5;
	public $field_buyer = "buyer";
	public $field_buyer_length = 5;
	public $field_content = "content";
	public $field_date = "date";
	public $field_type = "type";
}

class table_tradeitems{
	public $field_id = "id";
	public $field_id_length = 9;
	public $field_content = "content";
	public $field_type = "type";
	public $field_user = "user";
	public $field_user_length = 5;
	public $field_date = "date";
	public $field_school = "school";
}

class table_users{
	public $field_id = "id";
	public $field_id_length = 5;
	public $field_name = "name";
	public $field_name_length = 30;
	public $field_email = "email";
	public $field_email_length = 60;
	public $field_password = "password";
	public $field_password_length = 10;
	public $field_permissions = "permissions";
	public $field_school = "school";
	public $field_membersince = "membersince";
	public $field_lastseen = "lastseen";
	public $field_preferedemail = "preferedemail";
	public $field_preferedemail_length = 60;
	public $field_feedback = "feedback";
	public $field_feedback_length = 9;
	public $field_purchases = "purchases";
	public $field_style = "style";
}

class table_users_in_events{
	public $field_event = "event";
	public $field_event_length = 8;
	public $field_user_group_id = "user_group_id";
	public $field_user_group_id_length = 9;
}
?>