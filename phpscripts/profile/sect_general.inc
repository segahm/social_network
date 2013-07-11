<!-- general info -->
<!--
<table cellpadding="0" cellspacing="0" width="100%">
  <tr>
    <td align="left" valign="top" class="prfTbCrnlt"><img src="/images/c_lt.gif"></td>
    <td class="prfedge" width="100%">&nbsp;</td>
    <td align="right" valign="top" class="prfTbCrnrt"><img src="/images/c_rt.gif"></td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides"><span class="prfTbHdr">School Info</span> </td>
  </tr>
  <tr>
    <td colspan="3" class="prfsides" align="center" style="padding:5px;">-->
      <!-- body -->
	  <span class="prfTbHdr">School Info</span>
      <?php
//get a string of majors and houses into arrays
$majorList = collegeinfo::funcs_getAListOf($data[$college_table->field_majors]);
$houseList = collegeinfo::funcs_getAListOf($data[$college_table->field_houses]);
?>
      <table cellpadding="3" cellspacing="0" width="100%">
        <tr>
          <td><font class="inListTxt">College: </font><a class="inLinksCat" href="/search?col=<?=$data[$users_table->field_school]?>&type=<?=SEARCH_TYPE_PEOPLE?>">
            <?=$data[$college_table->field_fullname]?>
            </a> </td>
        </tr>
        <?php if(!is_null($data[$profile_table->field_status])):?>
        <tr>
          <td><font class="inListTxt">Status: </font>
		  <a class="inLinksCat" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&status=<?=$data[$profile_table->field_status]?>">
            <?=$CONSTANT_PROFILE_STATUS[$data[$profile_table->field_status]]?></a>
          </td>
        </tr>
        <?php endif;?>
         <?php if(!is_null($data[$profile_table->field_year])):?>
        <tr>
          <td><font class="inListTxt">Year: </font><a class="inLinksCat" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&year=<?=urlencode($data[$profile_table->field_year])?>">
            <?=toHTML($data[$profile_table->field_year])?>
            </a></td>
        </tr>
         <?php endif;?>
        <?php if(!is_null($data[$profile_table->field_major])):?>
        <tr>
          <td><font class="inListTxt">First major: </font><a class="inLinksCat" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&major=<?=urlencode($data[$profile_table->field_major])?>">
            <?=ucwords($majorList[$data[$profile_table->field_major]])?>
            </a> </td>
        </tr>
        <?php endif;?>
        <?php if(!is_null($data[$profile_table->field_majorsecond])):?>
        <tr>
          <td><font class="inListTxt">Second major: </font><a class="inLinksCat" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&major=<?=urlencode($data[$profile_table->field_majorsecond])?>">
            <?=ucwords($majorList[$data[$profile_table->field_majorsecond]])?>
            </a> </td>
        </tr>
        <?php endif;?>
         <?php if(!is_null($data[$profile_table->field_house])):?>
        <tr>
          <td><font class="inListTxt">House: </font><a class="inLinksCat" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&house=<?=urlencode($data[$profile_table->field_house])?>">
            <?=ucwords($houseList[$data[$profile_table->field_house]])?>
            </a> </td>
        </tr>
        <?php endif;?>
         <?php if(!is_null($data[$profile_table->field_room])):?>
        <tr>
          <td><font class="inListTxt">Room (bld.): </font><a class="inLinksCat" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&room=<?=urlencode($data[$profile_table->field_room])?>">
            <?=toHTML($data[$profile_table->field_room])?>
            </a> </td>
        </tr>
         <?php endif;?>
        <?php if(!is_null($data[$profile_table->field_hs])):?>
        <tr>
          <td><font class="inListTxt">High School: </font><a class="inLinksCat" href="/search?type=<?=SEARCH_TYPE_PEOPLE?>&hs=<?=urlencode($data[$profile_table->field_hs])?>">
            <?=toHTML($data[$profile_table->field_hs])?>
            </a> </td>
        </tr>
         <?php endif;?>
      </table>
      <!-- body -->
  <!--  </td>
  </tr>
  <tr>
    <td align="left" valign="bottom" class="prfTbCrnlb"><img src="/images/c_lb.gif"></td>
    <td class="prfedgebt" width="100%">&nbsp;</td>
    <td align="right" valign="bottom" class="prfTbCrnrb"><img src="/images/c_rb.gif"></td>
  </tr>
</table>-->
<!-- general info -->
