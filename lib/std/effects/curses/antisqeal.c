
/**
 * This curse converts all a players Ks to Cs
 * The classification of the effect is "misc.curse.antisqeal".
 * <p>
 * The argument is an integer and specifies how long the
 * curse will last.
 * <p>
 * This effect has a shadow associated with it.
 * @classification misc.curse.antisqeal
 * @example
 * // give them the curse for 2 minutes
 * add_effect("/std/effects/curses/chef_curse", 120);
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
    tell_object( player, "Your ability to mangle the english language "
                         "returns.\n");
} /* end() */

/** @ignore yes */
string query_classification() { return "misc.curse.antisqeal"; }

/** @ignore yes */

string query_shadow_ob() { return "/std/shadows/curses/antisqeal"; }
