/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: when.c,v 1.19 2003/04/10 06:05:44 ceres Exp $
 *
 *
 */

#include <login.h>

inherit "/cmds/base";

int cmd(string who)   {
   object player;
   int last;
   int caller;
   string* bits;
   string womble;
   int bing;

   bits = explode(who, ",");
   if (sizeof(bits) > 1) {
      bing = 0;
      foreach (womble in bits) {
         if (cmd(womble)) {
            bing = 1;
         }
      }
      return bing;
   }

   caller = this_player()->query_invis(); 
   who = this_player()->expand_nickname(lower_case(who));
   player = find_player( who );
   if (player &&
       (caller >= player->query_invis() ||
        reference_allowed(player, this_player()))) {
      if ( interactive( player ) ) {
         write( who + " has been on for " +
                   query_time_string(time() - player->query_last_log_on(), -1) +
                   ".\n");
      } else {
         write( who + " is currently net-dead.\n" );
      }
      return 1;
   } else {
      /* Should be a nice number.... */
      last = PLAYER_HANDLER->test_last(who);
      if ( last < 1 ) {
         return notify_fail( "Cannot find any record of $C$" + who + ".\n" );
      } else   {
         if (time() - last < 30) {
            write(who + " only just logged off.\n");
         } else {
            write(who + " last logged off " +
                  query_time_string(time() - last, -1) +
                  " ago.\n");
         }
         return 1;
      }
   }
}


mixed *query_patterns()   {
   return ({ "<string'player name'>", (: cmd( $4[0] ) :) });
} /* query_patterns() */
