/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ken_curse.c,v 1.1 1998/01/06 04:07:14 ceres Exp $
 * $Log: ken_curse.c,v $
 * Revision 1.1  1998/01/06 04:07:14  ceres
 * Initial revision
 * 
*/
/**
 * Makes the player speak like an obnoxious english person.
 * The classification of the effect is "misc.curse.ken".
 * <p>
 * The argument to the effect specifies how long the effect will lasyt
 * for.
 * <p>
 * This effect has  ashadw associated with it
 * @classification misc.curse.ken
 * @example
 * // give them the curse for 3 minutes
 * add_effect("/std/effects/curses/ken_curse", 360);
 */
#include <effect.h>

/** @ignore yes */
int beginning( object player, int time ) {
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
    tell_object( player, "Your craving for fish & chips passes.\n");
} /* end() */

/** @ignore yes */
string query_classification() { return "misc.curse.ken"; }

/** @ignore yes */
string query_shadow_ob() { return "/std/shadows/curses/ken_curse"; }
