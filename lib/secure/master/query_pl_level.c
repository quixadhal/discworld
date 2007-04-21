/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: query_pl_level.c,v 1.1 1998/01/06 05:12:03 ceres Exp $
 * $Log: query_pl_level.c,v $
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
/*
 * There are several occasions when the game driver wants to check if
 * a player has permission to specific things.
 *
 * These types are implemented so far:
 * "trace":		If the player is allowed to use tracing.
 * "wizard":            Is the player considered at least a "minimal" wizard?
 * "error messages":    Is the player allowed to get run time error messages?
 */

int query_player_level( string what ) {
   if ( !this_player() )
      return 0;
   switch ( what ) {
      case "error messages" :
         return 1;
      case "trace" :
      case "wizard" :
         return (int)this_player()->query_creator();
   }
} /* query_player_level() */
