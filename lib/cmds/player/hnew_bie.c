/*
 * $Locker:  $
 * $Id: hnew_bie.c,v 1.3 2000/12/02 22:53:31 pinkfish Exp $
 * $Log: hnew_bie.c,v $
 * Revision 1.3  2000/12/02 22:53:31  pinkfish
 * Add in atime to hnewbie.
 *
 * Revision 1.2  1999/08/29 08:11:38  ceres
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.1  1999/01/15 00:37:46  ceres
 * Initial revision
 *
 * 
*/

#include <player.h>

inherit "/cmds/guild_base";

#define HIST "/obj/handlers/hist_handler"

int cmd() {
  mixed *history;
  string result;
  
  history = HIST->query_chat_history("newbie");
  
  if ( !arrayp( history ) || !sizeof( history ) ) {
    tell_object(this_player(), "Nothing has been said on the newbie "
                "channel.\n");
  }
  else {
    result = "$P$Newbie Channel$P$";    
    result += implode(map(history, (: "$I$5=*" + ctime($1[2])[11..18] +
                                      "* $C$" + $1[0] + $1[1] + 
                                    "%^RESET%^" :)), "\n");    
    tell_object(this_player(), result);
  }
  
  return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "", (: cmd() :) });
} /* query_patterns() */
