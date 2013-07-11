<?php
$mycourses_t = new table_mycourses();
$dep_t = new table_departments();
?>
<!-- courses -->
<span class="prfTbHdr">Courses</span>
      <!-- body -->
      <p class="txt">
        <?php foreach($courses as $course):?>
        <a class="prfCourse"
	href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&course=<?=urlencode($course[$mycourses_t->field_course])?>">
        <?=$course[$mycourses_t->field_course]?>
        </a>&nbsp;- (<a href="/discussion?msgs=1&tpc=<?=urlencode($course[$dep_t->field_description])?>"><?=funcs_cutLength($course[$dep_t->field_description],30)?></a>),
        <?php endforeach;?>
      </p>
	  <!--
	  <table cellpadding="0" cellspacing="0" width="100%">
  <tr>
    <td align="left" valign="top" class="prfTbCrnlt"><img src="/images/c_lt.gif"></td>
    <td class="prfedge" width="100%">&nbsp;</td>
    <td align="right" valign="top" class="prfTbCrnrt"><img src="/images/c_rt.gif"></td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides"><span class="prfTbHdr">Courses</span> </td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides" style="padding:5px;">
      <p class="txt">
        <?php foreach($courses as $course):?>
        <a class="prfCourse"
	href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&course=<?=urlencode($course[$mycourses_t->field_course])?>">
        <?=$course[$mycourses_t->field_course]?>
        </a>&nbsp;- (<a href="/discussion?msgs=1&tpc=<?=urlencode($course[$dep_t->field_description])?>"><?=funcs_cutLength($course[$dep_t->field_description],30)?></a>),
        <?php endforeach;?>
      </p>
    </td>
  </tr>
  <tr>
    <td align="left" valign="bottom" class="prfTbCrnlb"><img src="/images/c_lb.gif"></td>
    <td class="prfedgebt" width="100%">&nbsp;</td>
    <td align="right" valign="bottom" class="prfTbCrnrb"><img src="/images/c_rb.gif"></td>
  </tr>
</table>
-->
      <!-- body -->
<!-- courses -->
