<?php
header('Content-type: text/html; charset=iso-8859-1');
?>
<html>
<head>
<title>partyHub - Help</title>
<link rel="Stylesheet" type="text/css" href="/styles/styles.css">
<style type="text/css">
.cntcthdr{
	color:#FAFAFA;font-weight:bold;padding:0px;padding-left:10px;font-size:12px;
}
.cntcbrd{
	border-width:1px;border-style:solid;padding-left:10px;padding:10px;
}
td{
	font-family:Verdana, Arial, Helvetica, sans-serif
}
</style>
<script language="javascript" type="text/javascript">
<!--
function openExamples(){
	window.open("/html/blogExamples.html","_blank","directories=no,status=no,toolbar=no,menubar=no,location=no,width=550,height=450,scrollbars=yes");
}
//-->
</script>
</head>
<body bgcolor="#FFFFFF">
<table align="center" width="810" cellpadding="0" cellspacing="0" border="0">
<tr><td><img src="/images/logo1.jpg"></td></tr>
<tr>
	<td class="cntcthdr" style="font-size:14px;" bgcolor="#336699">
		Help Page
	</td>
</tr>
<tr><td>
	<table width="100%" cellpadding="5" cellspacing="0">
	<tr><td>
		<table width="300" cellpadding="2" cellspacing="2">
				<tr bgcolor="#993333">
					<td class="cntcthdr">Rejecting a friendship request?</td>
				</tr>
				<tr valign="top">
				<td align="left" class="cntcbrd" style="border-color:#993333">
					<p>The other user is <b>not</b> notified and everything remains as if you didn't respond at all.</p>
				</td>
				</tr>
		</table>
	</td>
	<td>
		<table width="300" cellpadding="2" cellspacing="2">
				<tr bgcolor="#999966">
					<td class="cntcthdr">What's the point of groups?</td>
				</tr>
				<tr valign="top">
				<td align="left" class="cntcbrd" style="border-color:#999966">
					<p>partyHub groups help you organize your friends based on interests, activities, work, etc... Hence, instead of granting access individually to your friends for your events/parties/pictures, groups help make your life easier by allowing you to grant access specifically to all the people in a certain group.</p>
				</td>
				</tr>
		</table>
	</td>
	</tr>
	<tr><td style="padding-left:30px;">
		<table width="350" cellpadding="2" cellspacing="2">
				<tr bgcolor="#006633">
					<td class="cntcthdr">How come I can't access the person's profile</td>
				</tr>
				<tr valign="top">
				<td align="left" class="cntcbrd" style="border-color:#006633">
					<p>On partyHub you can set permission to determine who can access and see different parts of your profile. By default we made profile access restricted to the person’s college, however, this can be changed and further restricted to only friends.</p>
				</td>
				</tr>
		</table>
	</td><td style="padding-top:40px;padding-left:50px;">
		<table width="320" cellpadding="2" cellspacing="2">
				<tr bgcolor="#663333">
					<td class="cntcthdr">Why can't I send messages?</td>
				</tr>
				<tr valign="top">
				<td align="left" class="cntcbrd" style="border-color:#663333">
					<p>partyHub message system is restricted to your college and friends only. Hence if you are trying to send a message to someone at a different college (not your friend) it will give you an ugly message.</p>
				</td>
				</tr>
		</table>
	</td></tr>
	<tr><td colspan="2">
		<table width="100%">
		<tr valign="top"><td>
		<table width="290" cellpadding="2" cellspacing="2">
				<tr bgcolor="#999999">
					<td class="cntcthdr">Why is my calendar marked with colors?</td>
				</tr>
				<tr valign="top">
				<td align="left" class="cntcbrd" style="border-color:#999999">
					<p>partyHub automatically searches events that you have access to either publicly or privately. Public events are those where you were specified indirectly (i.e. event was posted for your college, course, major…). Private events, on the other hand, are those where one of your friends specifically wants you to see the event. You can determine which events you wish to see by setting the appropriate options.</p>
				</td>
				</tr>
		</table>
		</td>
		<td style="padding-top:30px;">
			<table width="470" cellpadding="2" cellspacing="2">
				<tr bgcolor="#339933">
					<td class="cntcthdr">Can you explain what is this Trading Space you keep talking about?</td>
				</tr>
				<tr valign="top">
				<td align="left" class="cntcbrd" style="border-color:#339933">
				<p>Here we go...uhhh... Trading Space is a peer-to-peer exchange network that allows you to buy/sell your stuff from/to other people at your college. Since there are no fees or shipping costs attached--trading space makes trading fast and (most importantly) cheap. Hence, instead of going to the local bookstore and getting screwed by those high prices (i.e. bureaucrats and publishers), you can now buy your textbooks from your fellow "colleagues". Besides, what can be better than to be able to see the picture of the person you are dealing with (unless you are a big believer in - "it is what's on the inside that matters").</p>
				</td>
				</tr>
			</table>
		</td></tr>
		</table>
	</td></tr>
	<tr><td colspan="2" align="center">
		<table width="600" cellpadding="2" cellspacing="2">
				<tr bgcolor="#CC9999">
					<td class="cntcthdr">How do I customize colors, fonts, pictures... (using CSS)?</td>
				</tr>
				<tr valign="top">
				<td align="left" class="cntcbrd" style="border-color:#CC9999">
				The truth is - this stuff is rather complicated, so how about you figure it out yourself :-); besides what’s the fun of it if we do it for you, right? Just kidding guys (and gals). O.K, basically you can customize CSS in 3 ways:
				<ul>
				<li>you can customize the way your profile page looks and how others will see it</li>
				<li>how your blogs look (this needs be customized for every blog)</li>
				<li>and if you'd like, how you view the entire site – yes, you heard it right, 
				you can customize the general look of the site (which reminds me, perhaps we should 
				create a depository for all different templates, hmmmm... - soon to come if there is enough demand for it)</li>
				</ul>
				But back to our donkeys... All css properties are organized around a simple fact that you can create a 
				css property like such - .mytags{color:#FFFFFF} - and give it different color, font, or size value. However, 
				you can't just invent names <font style="font-size:10px;">(well, you can but it won't do you any good)</font> 
				since these names have to be present in our code. But don't worry we made sure that everything is tagged with a different name and 
				so you can customize almost everything. There are also global property (class) names such as body, td, table, etc... 
				that allow you to also set your own styles. The nice thing about these, of course, is that they do not 
				have to be hard coded into our code since they are always present. For more information take a look at 
				<a href="javascript:openExamples()">our blog examples</a> or do any of the following:<br>
				<ul>
					<li>google it</li>
					<li>ask around</li>
					<li>take a look at our style sheets: <a href="/styles/styles.css">main</a>, 
					<a href="/styles/rte.css">rich formatting</a>, <a href="/styles/blog.css">blogs</a></li>
				</ul>
				just in case you can't wait any longer:</p>
<pre>
<font color="#FF3399">
body{</font>
	<span style="color:#336699">background-repeat: </span>no-repeat;
	<span style="color:#336699">background-image: </span>url(http://someurl_address);
<font color="#FF3399">}</font>
</pre>
				- will set your background picture
				</td>
				</tr>
		</table>
	</td></tr>
	</table>
</td></tr>
<tr><td>
<? include_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/general/footer.html");?>
</td></tr>
</td></tr>
</table>
</body>
</html>