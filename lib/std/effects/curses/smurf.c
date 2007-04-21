/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: smurf.c,v 1.1 1998/01/06 04:07:14 ceres Exp $
 * $Log: smurf.c,v $
 * Revision 1.1  1998/01/06 04:07:14  ceres
 * Initial revision
 * 
*/
/**
 * Turns the player into a smurf.  The classification of this effect is
 * is "misc.curse.smurf".
 * <p>
 * If a time is specifed then this is how long the effect lasts.
 * If no time is specifed then the effect is never removed.
 * <p>
 * This effect has a shadow associated with it.
 * @classification misc.curse.smurf
 * @example
 * // Give them the curse for 6 minutes
 * add_effect("/std/effects/curses/smurf", 360);
 * @example
 * // Give them the curse forever!
 * add_effect("/std/effects/curse/smurf");
 */
#include <effect.h>

/** @ignore yes */
string query_classification() { return "misc.curse.smurf"; }

/** @ignore yes */
string query_shadow_ob() { return "/std/shadows/curses/ensmurfed"; }

/** @ignore yes */
int beginning( object player, int time ) {
   tell_object( player, "You suddenly find yourself filled with a strange "
               "fear of someone called \"Gargamel\".\n");
   if (time)
     player->submit_ee( 0, time, EE_REMOVE );
   return time;
}

/** @ignore yes */
int merge_effect( object player, int time1, int time2 ) {
   int duration;

   duration = time2 + player->expected_tt();
   player->submit_ee( 0, duration, EE_REMOVE );
   return duration;
} /* merge_effect() */
 
/** @ignore yes */
void end( object player, int time ) {
   tell_object( player, "Your fear of Gargamel has abated.\n");
} /* end() */

