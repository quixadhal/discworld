/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: time.c,v 1.15 2003/03/02 23:37:42 pinkfish Exp $
 * 
 */
#include <localtime.h>
#include <dst.h>
#include <am_time.h>
#ifndef __DISTRIBUTION_LIB__
#include <special_day.h>
#endif
#include <config.h>

inherit "/cmds/base";

mixed *_timezones;

void show_mud_time();

void create() {
   mixed *time_rubbish;

   time_rubbish = localtime(time());
   _timezones = ({
      "Seattle", - time_rubbish[LT_GMTOFF] + (US_DST * 60 * 60),
      "Toronto", - time_rubbish[LT_GMTOFF] + (3 + CA_DST) * (60 * 60),
     "London", (UK_DST * 60 * 60),
      "Sydney", (10 + AUS_DST) * (60 * 60),
                 });
} /* create() */

mixed cmd( string word ) {
   mixed *time_rubbish;
   int i;
   string mess;

   time_rubbish = localtime(time());
   /* Figure out the gmt time. */
   mess = "";

   for (i = 0; i < sizeof(_timezones); i += 2) {
      mess += sprintf( "[%s in %-9s]\n", ctime(time() + time_rubbish[LT_GMTOFF] +
                                    _timezones[i + 1]), _timezones[i]);
   }

   //Something for creators.
   if ( this_player()->query_creator() )
      show_mud_time();

   write(mess);
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd :) });
} /* query_patterns() */

void show_mud_time() {
#ifndef __DISTRIBUTION_LIB__
   string mess;
   string day = cap_words(SPECIAL_DAY_HANDLER->query_special_day());

   mess = sprintf( "In Ankh-Morpork, it is %s%s\n", amtime( time() ), 
           ( strlen(day) ? ".  Today is "+ day +".": ".") );
   write( mess );
#endif
}
