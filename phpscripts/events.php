<?php
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/main.php");
auth_authCheck();
require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/pagedata/quickAddIn.inc");
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/events/event_control.php");
drawMain_1("Events");
$event = new eventster(AUTH_USERID,AUTH_COLLEGEID);
?>
<script language="javascript">
<!--
function selectedEvent(obj){
	if (typeof(document.new_event) != "undefined" 
		&& typeof(document.new_event.oldid) != "undefined"){
		var oldid = document.new_event.oldid;
	}else{
		var oldid = null;
	}
	if (obj.value != "-1" && (oldid == null || obj.value != oldid.value)){
		window.location = "/events?newevent=1&edit="+obj.value;
	}
}
//-->
</script>
<?php 
if (isset($_GET['newevent'])):
	require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/events/add_edit_event.inc");
else:
	drawMain_2();
	//if removing a general event
	if (isset($_GET['remove'])){
		$event->removeGeneralEvent($_GET['remove']);
	}
	$my_events = $event->getMyEventTitles();
?>
<table width="500" align="left" cellpadding="5" cellspacing="0"><tr><td align="center">
<table width="100%" cellpadding="0" border="0" cellspacing="0">
<tr>
	<td bgcolor="#336699" style="padding-left:10px;">
		<table cellpadding="3" cellspacing="0">
		<tr>
			<td align="right" nowrap>
			<a href="/events" style="font-size:12px;color:#ffffff;font-weight:bold;
			<?=(isset($_GET['event'])) ? '' : 'text-decoration:none;'?>">Upcoming</a>
			</td>
			<td><font color="#FFFFFF">|</font></td>
			<td align="left" nowrap>
			<a href="/events?newevent=1" style="font-size:12px;color:#ffffff;font-weight:bold;">Add New Event</a>
			</td>
		</tr>
		</table>
	</td>
	<td bgcolor="#336699" align="right" nowrap>
	<b><font color="#ffffff">Edit: </font></b>
	<?php ob_start('processForm');?>
	<select name="event_select" style="font-size:8pt" on onChange="selectedEvent(this)">
	<option value="-1">select my event</option>
	<?php foreach ($my_events as $event_id => $event_title):?>
	<option value="<?=$event_id?>"><?=toHTML(funcs_cutLength($event_title,20))?></option>
	<?php endforeach;?>
	</select>&nbsp;
	</td>
</tr>
<tr>
	<td colspan="2" style="padding:10px" align="left" valign="top">
	<?php if (!isset($_GET['event'])):?>
		<?php require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/events/upcoming.inc");?>
	<?php else:?>
		<?php require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/events/showevent.inc");?>
	<?php endif;?>
	</td>
</tr>
</table>
<?php
//now display events based on get parameters
if (!isset($_GET[CALENDAR_MONTH]) 
	|| !is_numeric($_GET[CALENDAR_MONTH]) || $_GET[CALENDAR_MONTH]>12
	|| $_GET[CALENDAR_MONTH]<1){
	$_GET[CALENDAR_MONTH] = date('n',time());
}
if (!isset($_GET[CALENDAR_DATE]) 
	|| !is_numeric($_GET[CALENDAR_DATE]) || $_GET[CALENDAR_DATE]>31
	|| $_GET[CALENDAR_DATE]<1){
	unset($_GET[CALENDAR_DATE]);
	$days = array();
}else{
	$days = array(date('d',mktime(0,0,0,0,$_GET[CALENDAR_DATE])));
}
//find out which year does this month belong to
if(isset($_GET[CALENDAR_YEAR]) && preg_match('/200[5-9]/',$_GET[CALENDAR_YEAR])){
	$_GET[CALENDAR_YEAR] = $_GET[CALENDAR_YEAR];
}elseif(!isset($_GET[CALENDAR_DATE])){
	//compare based on months
	if (mktime(0,0,0,$_GET[CALENDAR_MONTH]) < mktime(0,0,0)){
		//year = current year + 1
		$_GET[CALENDAR_YEAR] = (int)date('Y',time())+1;
	}else{
		$_GET[CALENDAR_YEAR] = (int)date('Y',time());
	}
}else{
	//compare based on months and days
	if (mktime(0,0,0,$_GET[CALENDAR_MONTH],$_GET[CALENDAR_DATE]) < mktime(0,0,0)){
		//year = current year + 1
		$_GET[CALENDAR_YEAR] = (int)date('Y',time())+1;
	}else{
		$_GET[CALENDAR_YEAR] = (int)date('Y',time());
	}
}
if (!isset($_GET['limit']) || !is_numeric($_GET['limit'])
	|| $_GET['limit'] > 99 || $_GET['limit'] < 1){
	$_GET['limit'] = 20;
}
if (!isset($_GET['start']) || !is_numeric($_GET['start'])){
	$_GET['start'] = 0;
}
$result = $event->getEvents($_GET[CALENDAR_YEAR],$_GET[CALENDAR_MONTH],$days,null,true,true,false,
						$_GET['limit'], $_GET['start']);
		//buffer all events to find out the number of results on this page
		$events = array();
		while($event = db::fetch_assoc($result)){
			$events[] = $event;
		}
		//number of events on this page:
		$numbResults = count($events);
		printControlHeader();
		if ($numbResults == 0){
			echo '<br><span style="color:#663333">there are currently no events for you '
				.date((!isset($_GET[CALENDAR_DATE]))?'\i\n F, Y':'\o\n F jS, Y',
					mktime(0,0,0,$_GET[CALENDAR_MONTH],
						((!isset($_GET[CALENDAR_DATE])) ? 1 : $_GET[CALENDAR_DATE]),$_GET[CALENDAR_YEAR]))
							.'</span>';
		}else{
			require_once($_SERVER['DOCUMENT_ROOT']."/phpscripts/includes/events/displayEvents.inc");
		}	
	?>
</td></tr></table>
	<?php
	endif;
	drawMain_3(true,false);
function printControlHeader(){
	global $numbResults;
	echo '<table width="100%" cellspacing="5"><tr>';
		/*if there is another previous page, then show "prev link" that will have its last result
		 *being this page's first*/
		if ($_GET['start']>=($_GET['limit']-1)){
			echo '<td width="40" align="left"><a href="/events?';
			echo preg_replace('/[&]?start=[^&]+/i','',$_SERVER['REQUEST_URI'])
				.'&start='.($_GET['start']-$_GET['limit']+1).'">&#60;&#60;Prev</a></td>';
		}
		echo '<td align="center">';
		//draw date arrow
		if (!isset($_GET[CALENDAR_DATE])){
			//if showing by month, then show only if either month is bigger than 1 or year is bigger than todays
			if ($_GET[CALENDAR_MONTH]>1 || $_GET[CALENDAR_YEAR]>(int)date("Y")){
				echo '<a href="/events?';
				if ($_GET[CALENDAR_MONTH]>1){
					echo CALENDAR_MONTH.'='.($_GET[CALENDAR_MONTH]-1);
				}else{
					echo CALENDAR_MONTH.'=12&'.CALENDAR_YEAR.'='.($_GET[CALENDAR_YEAR]-1);
				}
				echo '" style="font-size:10px;color:#660000">&#60;&#60;</a>&nbsp;&nbsp;';
			}
		}else{
			//if showing by date, first check wherever we should show this date
			if ($_GET[CALENDAR_DATE]>1 || $_GET[CALENDAR_MONTH]>1 || $_GET[CALENDAR_YEAR]>(int)date("Y")){
				echo '<a href="/events?';
				if ($_GET[CALENDAR_DATE]>1){
					echo CALENDAR_MONTH.'='.$_GET[CALENDAR_MONTH].'&';
					echo CALENDAR_DATE.'='.($_GET[CALENDAR_DATE]-1);
				}elseif($_GET[CALENDAR_MONTH]>1){
					echo CALENDAR_MONTH.'='.($_GET[CALENDAR_MONTH]-1).'&';
					echo CALENDAR_DATE.'=31';
				}else{
					echo CALENDAR_MONTH.'=12&'
					.CALENDAR_YEAR.'='.($_GET[CALENDAR_YEAR]-1).'&'
					.CALENDAR_DATE.'=31';
				}
				echo '" style="font-size:10px;color:#660000">&#60;&#60;</a>&nbsp;&nbsp;';
			}
		}
		//draw date display
		echo '<font style="font-size:12px;">';
		if (!isset($_GET[CALENDAR_DATE])){
			echo date('F',mktime(0,0,0,$_GET[CALENDAR_MONTH]));
		}else{
			echo date('F jS',mktime(0,0,0,$_GET[CALENDAR_MONTH],$_GET[CALENDAR_DATE]));
		}
		echo '</font>';
		//draw right date arrow
		if (!isset($_GET[CALENDAR_DATE])){
			if ($_GET[CALENDAR_MONTH]<12 || $_GET[CALENDAR_YEAR]<((int)date("Y")+1)){
				echo '&nbsp;&nbsp;<a href="/events?';
				if ($_GET[CALENDAR_MONTH]<12){
					echo CALENDAR_MONTH.'='.($_GET[CALENDAR_MONTH]+1);
				}else{
					echo CALENDAR_MONTH.'=1'
							.'&'.CALENDAR_YEAR.'='.($_GET[CALENDAR_YEAR]+1);
				}
				echo '" style="font-size:10px;color:#660000">&#62;&#62;</a>';
			}
		}else{
			if ($_GET[CALENDAR_DATE]<31 || $_GET[CALENDAR_MONTH]<12 || $_GET[CALENDAR_YEAR]<((int)date("Y")+1)){
				echo '&nbsp;&nbsp;<a href="/events?';
				if ($_GET[CALENDAR_DATE] < 31){
					echo CALENDAR_MONTH.'='.$_GET[CALENDAR_MONTH].'&';
					echo CALENDAR_DATE.'='.($_GET[CALENDAR_DATE]+1);
				}elseif($_GET[CALENDAR_MONTH]<12){
					echo CALENDAR_MONTH.'='.($_GET[CALENDAR_MONTH]+1).'&';
					echo CALENDAR_DATE.'=1';
				}else{
					echo CALENDAR_MONTH.'=1&'
					.CALENDAR_YEAR.'='.($_GET[CALENDAR_YEAR]+1).'&'
					.CALENDAR_DATE.'=1';
				}
				echo '" style="font-size:10px;color:#660000">&#62;&#62;</a>';
			}
		}
		echo '</td>';
		/*check wherever this page is full with results and if yes, then
		*print link to the next page with the start of the last event on
		 *this page*/
		if ($numbResults == $_GET['limit']){
			echo '<td width="40" align="right"><a href="/events?';
			echo preg_replace('/[&]?start=[^&]+/i','',$_SERVER['REQUEST_URI'])
				.'&start='.($_GET['start']+$_GET['limit']-1).'">Next&#62;&#62;</a></td>';
		}
		echo '</tr></table>';
}
?>