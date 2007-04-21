/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: chef_curse.c,v 1.1 1998/01/06 04:07:14 ceres Exp $
 * $Log: chef_curse.c,v $
 * Revision 1.1  1998/01/06 04:07:14  ceres
 * Initial revision
 * 
*/
/**
 * This curse makes the players talk as if they are the sweedish chef.
 * The classification of the effect is "misc.curse.chef".
 * <p>
 * The argument is an integer and specifies how long the
 * curse will last.
 * <p>
 * This effect has a shadow associated with it.
 * @classification misc.curse.chef
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
    tell_object( player, "Your craving for swedish meatballs goes away.\n");
} /* end() */

/** @ignore yes */
string query_classification() { return "misc.curse.chef"; }

/** @ignore yes */
string query_shadow_ob() { return "/std/shadows/curses/chef_curse"; }
