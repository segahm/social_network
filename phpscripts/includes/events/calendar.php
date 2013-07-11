<?php
require_once($_SERVER['DOCUMENT_ROOT'] . "/phpscripts/includes/events/event_control.php");
function calendar_draw(){
	$eventster = new eventster(AUTH_USERID,AUTH_COLLEGEID);
	//set default month if it wasn't specified directly
	if (!isset($_GET[CALENDAR_MONTH]) || $_GET[CALENDAR_MONTH]>12 
			|| $_GET[CALENDAR_MONTH]<1){
		$_GET[CALENDAR_MONTH] = (int)date("n");
	}
	//set default year
	if (!isset($_GET[CALENDAR_YEAR]) 
		|| !preg_match('/^200[0-9]$/',$_GET[CALENDAR_YEAR])
		|| $_GET[CALENDAR_YEAR]>((int)date("Y")+2)){
		$_GET[CALENDAR_YEAR] = (int)date("Y");
	}
	$IS_SEARCH_PRIVATE = ((AUTH_PERMISSIONS & Permissions::PERM_EVENT_PRIVATE) != 0);
	$IS_SEARCH_PUBLIC = ((AUTH_PERMISSIONS & Permissions::PERM_EVENT_PUBLIC) != 0);
	$dates = $eventster->calendarEvents($_GET[CALENDAR_YEAR],
		$_GET[CALENDAR_MONTH],AUTH_PERMISSIONS);
	$calendar_month = "";
	$calendar_calendar = "";
	//work on left arrow
	if ($_GET[CALENDAR_MONTH]>1 || $_GET[CALENDAR_YEAR]>(int)date("Y")){
		$calendar_month .= '<a href="';
		$calendar_month .= clean_date_url();
		if ($_GET[CALENDAR_MONTH]>1){
			$calendar_month .= CALENDAR_MONTH.'='.($_GET[CALENDAR_MONTH]-1)
							.'&'.CALENDAR_YEAR.'='.$_GET[CALENDAR_YEAR];
		}else{
			$calendar_month .= CALENDAR_MONTH.'=12'
							.'&'.CALENDAR_YEAR.'='.($_GET[CALENDAR_YEAR]-1);
		}
		$calendar_month .= '" style="font-size:10px;color:#660000">&#60;&#60;</a>&nbsp;&nbsp;';	
	}
	//print month
	$calendar_month .= '<a style="color:#000000" href="/events?'.CALENDAR_MONTH.'='.$_GET[CALENDAR_MONTH].'">';
	$calendar_month .= date("F",mktime(0,0,0,$_GET[CALENDAR_MONTH],1,$_GET[CALENDAR_YEAR]));
	//if not the current year display year
	if($_GET[CALENDAR_YEAR]!=(int)date("Y")){
		$calendar_month .= ' ('.preg_replace('/^([0-9]{2,2})([0-9]{2,2})$/','\\2',$_GET[CALENDAR_YEAR]).')';
	}
	$calendar_month .= '</a>';
	//work on the right arrow
	if ($_GET[CALENDAR_MONTH]<12 || $_GET[CALENDAR_YEAR]<((int)date("Y")+1)){
		$calendar_month .= '&nbsp;&nbsp;<a href="';
		$calendar_month .= clean_date_url();
		if ($_GET[CALENDAR_MONTH]<12){
			$calendar_month .= CALENDAR_MONTH.'='.($_GET[CALENDAR_MONTH]+1)
							.'&'.CALENDAR_YEAR.'='.$_GET[CALENDAR_YEAR];
		}else{
			$calendar_month .= CALENDAR_MONTH.'=1'
							.'&'.CALENDAR_YEAR.'='.($_GET[CALENDAR_YEAR]+1);
		}
		$calendar_month .= '" style="font-size:10px;color:#660000">&#62;&#62;</a>';	
	}
	//do a week offset for dates that start not on sunday
	$daysOfTheWeek = (int)date("w",mktime(0,0,0,$_GET[CALENDAR_MONTH],1,$_GET[CALENDAR_YEAR]));
	for ($i=0;$i<$daysOfTheWeek;$i++){
		if ($i==0){
			$calendar_calendar .= '<tr>';
		}
		$calendar_calendar .= '<td class="weekstd">&nbsp;</td>';
	}
	//print dates
	$days_in_month = (int)date('t',mktime(1,0,0,$_GET[CALENDAR_MONTH],1,date("Y")));
	for ($daysOfTheMonth=1;$daysOfTheMonth<=$days_in_month;$daysOfTheMonth++){
		if ($daysOfTheWeek==7){
			$calendar_calendar .= "</tr>";
			$daysOfTheWeek = 0;
		}
		if ($daysOfTheWeek==0){
			$calendar_calendar .="<tr>";
		}
		$calendar_calendar .= '<td class="weekstd"><a href="/events?' 
			. CALENDAR_MONTH . '=' . $_GET[CALENDAR_MONTH] 
			. '&' . CALENDAR_DATE . '=' . $daysOfTheMonth . '" ';
		$today = (mktime(0,0,0,$_GET[CALENDAR_MONTH],$daysOfTheMonth,$_GET[CALENDAR_YEAR]) 
					== mktime(0,0,0,(int)date("n"),(int)date("j"),(int)date("Y")));
		if(isset($dates[$daysOfTheMonth])){
			$event = $dates[$daysOfTheMonth];
			/*there exists an event on this date*/
			//create text for the popup
			$text = '&quot;'.$event['title'].'&quot; posted by '.$event['name'].' ...';
			if ($today){
				$calendar_calendar .= 'class="weekdates" title="'.$text.'" style="color:#1BD500;">[' . $daysOfTheMonth . ']';
			}elseif($event[EVENT_RIGHTS] == EVENT_RIGHTS_MY){
				$calendar_calendar .= 'class="weekdates" title="'.$text.'" style="color:#CC9900;">'.$daysOfTheMonth;
			}elseif($event[EVENT_RIGHTS] == EVENT_RIGHTS_PRIVATE){
				$calendar_calendar .= 'class="weekdates" title="'.$text.'" style="color:#FF0000;">'.$daysOfTheMonth;
			}elseif($event[EVENT_RIGHTS] == EVENT_RIGHTS_PUBLIC){
				$calendar_calendar .= 'class="weekdates" title="'.$text.'" style="color:#660066;">'.$daysOfTheMonth;
			}
		}elseif($today){
			$calendar_calendar .= 'class="weekdates" style="color:#1BD500;">[' . $daysOfTheMonth . ']';
		}else{
			$calendar_calendar .= 'class="weekdates">' . $daysOfTheMonth;
		}
		$calendar_calendar .= '</a></td>';
		$daysOfTheWeek++;
	}
	for ($i=$daysOfTheWeek;$i<7;$i++){
		$calendar_calendar .= '<td class="weekstd">&nbsp;</td>';
	}
	$calendar_calendar .= '</tr>';
?>
<table width="100%" cellpadding="0" cellspacing="3">
		<tr><td align="center" colspan="7" nowrap><?=$calendar_month?></td></tr>
		<tr>
		<td class="weekstd"><span class="weekdays">Sun</span></td>
		<td class="weekstd"><span class="weekdays">M</span></td>
		<td class="weekstd"><span class="weekdays">T</span></td>
		<td class="weekstd"><span class="weekdays">W</span></td>
		<td class="weekstd"><span class="weekdays">Th</span></td>
		<td class="weekstd"><span class="weekdays">F</span></td>
		<td class="weekstd"><span class="weekdays">S</span></td>
		</tr>
		<?=$calendar_calendar?>
	<tr><td colspan="7" align="left">
		<?php if($IS_SEARCH_PRIVATE):?>
		<span style="color:#FF0000;font-size:10px">-private events</span><br>
		<?php endif;?>
		<?php if($IS_SEARCH_PUBLIC):?>
		<span style="color:#660066;font-size:10px">-public events</span><br>
		<?php endif;?>
		<span style="color:#CC9900;font-size:10px">-posted by me</span>
	   </td></tr>
</table>
<?php
}
function clean_date_url(){
	$string = preg_replace('/^([^\?]+)(\?.*)?$/','\\1',$_SERVER['REQUEST_URI']).'?'
		.preg_replace('#(&?'.CALENDAR_MONTH.'=[^&]+)|(&?'.CALENDAR_YEAR.'=[^&]+)#','',preg_replace('#^(.+)$#','\\1&',$_SERVER['QUERY_STRING']));
	return $string;
}
calendar_draw();
?>