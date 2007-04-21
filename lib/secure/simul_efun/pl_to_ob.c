/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: pl_to_ob.c,v 1.3 1998/09/30 14:54:05 wodan Exp $
 * $Log: pl_to_ob.c,v $
 * Revision 1.3  1998/09/30 14:54:05  wodan
 * Made it use the simul instead of the efun, for better lord invis
 *
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * This method turns a players name into an object, using nicknames.
 *  Turn a players name (string) into an object, nicknames are handled
 *  player_object = player_to_object("antiriad");
 * <p>
 *   Coded by Antiriad, womble on.
 * @author Antiriad
 */

object player_to_object(string str)  
{
  string tmp;
  if (this_player())
    tmp = this_player()->expand_nickname(lower_case(str));
  if (tmp) str = tmp;
  return this_object()->find_player(lower_case(str));
} /* player_to_object() */
