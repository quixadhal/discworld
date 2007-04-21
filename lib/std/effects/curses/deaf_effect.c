/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: deaf_effect.c,v 1.1 1998/01/06 04:07:14 ceres Exp $
 * $Log: deaf_effect.c,v $
 * Revision 1.1  1998/01/06 04:07:14  ceres
 * Initial revision
 * 
*/
/**
 * Makes the player deaf.  The classification of the effect
 * is "misc.curse.deafness".
 * <p>
 * This effect has a shadow associated with it.
 * @classification misc.curse.deafness
 */
#include "effect.h"

/** @ignore yes */
string query_classification() { return "misc.curse.deafness"; }

/** @ignore yes */
int query_indefinite() { return 1; }

/** @ignore yes */
string query_shadow_ob() { return "/std/shadows/curses/deaf_shadow"; }

/** @ignore yes */
mixed beginning( object player, mixed arg ) {
   tell_object( player, "Sounds seem dim and muffled.\n" );
   if ( pointerp( arg ) ) {
      player->submit_ee( "hear_better", arg[ 1 ], EE_CONTINUOUS );
      return arg[ 0 ];
   } else {
      return arg;
   }
}

/** @ignore yes */
void end( object player ) {
   tell_object( player, "Your hearing has returned to normal.\n" );
}

/** @ignore yes */
void hear_better( object player, mixed arg, int id ) {
   player->add_message(this_object(), "Your hearing improves somewhat.\n", 0);
   arg = 100 - ((100 - arg) * 2/3);
   if (arg == 100) player->delete_effect( player->sid_to_enum( id ) );
   else 
      player->set_arg_of( player->sid_to_enum( id ), arg );
}

/** @ignore yes */
mixed merge_effect( object player, mixed old_arg, mixed new_arg, int id ) {
   player->add_message( this_object(), "Your hearing gets worse.\n", 0 );
   player->set_arg_of( player->sid_to_enum( id ), new_arg );
}

