/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: whoami.c,v 1.2 1998/04/14 02:29:47 pinkfish Exp $
 * $Log: whoami.c,v $
 * Revision 1.2  1998/04/14 02:29:47  pinkfish
 * Changed to use add_command.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* whoami command, trial out by Turrican for a commands daemon. */

#include <command.h>

inherit "/cmds/base";

mixed cmd() {
  write( "You are "+ (string)( DIR_PLAYER_CMDS +"/who" )->
         who_line(this_player(), this_player()->query_creator(),
                  this_player()->query_cols(), 1) +".\n" );
  return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd() :) });
} /* query_patterns() */
