/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gp_drain_effect.c,v 1.1 2000/11/04 12:20:02 turvity Exp $
 *
 *
 */

#include <effect.h>

#include "path.h"
#include "/std/living/stats.c"

#define CLASS "general.gp.drain"

/** @ignore yes */
string query_classification(){
  return CLASS;
}

/** @ignore yes */
string query_death_reason(){
  return "wasting away slowly";
}

/** @ignore yes */
void adjust_health( object player ){
  int damage,
    *enums = player->effects_matching( query_classification() );
  mixed *args;

  if( sizeof( enums ) ){
    args = player->arg_of( enums[ 0 ] );
  }
  
  tell_object( player, "You feel woozy.\n" );
  damage = roll_MdN( 2, ( ( args[0] ) / 2 ) );
  ( player )->adjust_gp( -damage );
  tell_creator( player,
                "Drain: " + damage + "\n" );
}

/** @ignore yes
 * Arguments in the form:
 *  int *arg = ({ drain, duration, ({ beginning_message, end_message }) })
 */
mixed  *beginning( object player, mixed *arg ){
  tell_object( player, arg[2][0] + "\n" );
  player->submit_ee( "adjust_health", ({ 5, 10 }),
                     EE_CONTINUOUS, player );
  player->submit_ee( 0, arg[1], EE_REMOVE );
  return arg;
}

/** @ignore yes */
mixed *merge_effect( object player, mixed *old_arg, mixed *arg ){
  arg[0] = ( old_arg[0] + arg[0] ) / 2;
  player->submit_ee( 0, arg[1] + player->expected_tt(), EE_REMOVE );
  return arg;
}

/** @ignore yes */
void end( object player, mixed *arg ){
  string *message = arg[2];

  tell_object( player, message[1] + "\n" );
}

/** @ignore yes */
void restart( object player, mixed *arg ){
  string *message = arg[2];
  
  tell_object( player, message[0] + "\n" );
}

