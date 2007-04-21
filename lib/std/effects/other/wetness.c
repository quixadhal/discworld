/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wetness.c,v 1.14 2002/09/24 02:30:10 presto Exp $
 * $Log: wetness.c,v $
 * Revision 1.14  2002/09/24 02:30:10  presto
 * No squelching underwater!
 *
 * Revision 1.13  2002/02/24 04:27:15  ceres
 * Stopped you drying out in waer
 *
 * Revision 1.12  2001/11/04 05:33:32  presto
 * Added another parameter to wet_string so I can get both 'wet' and 'look' to work
 * correctly.
 *
 * Revision 1.11  2001/10/02 02:30:33  presto
 * Fixed wet_string to work right.  Um, I hope.
 *
 * Revision 1.10  2001/08/20 03:06:28  presto
 * Changed the range for "slightly wet" to be 0 .. 5 rather than 1 .. 5
 *
 * Revision 1.9  2001/08/19 02:11:03  presto
 * Misspelled "extra_look" and "exta_look".  Surprisingly, it didn't work.  Also changed
 * how it decides which string to build
 *
 * Revision 1.8  2001/08/15 04:10:02  presto
 * added wet_string function to help out with the 'wet' command
 *
 * Revision 1.7  2001/06/01 21:18:17  ceres
 * Can't recall
 *
 * Revision 1.6  2000/03/03 07:11:44  ceres
 * Can't remember
 *
 * Revision 1.5  2000/01/12 00:29:31  ceres
 * Tweaked drying out.
 *
 * Revision 1.4  1999/12/31 03:19:34  pinkfish
 * Hopefully fix up a runtime in the wetness effect.
 *
 * Revision 1.3  1999/07/30 17:55:41  tannah
 * Added an initial "Squelch" in beginning() so that players never
 * get the "dry off" message without being told they were wet.
 *
 * Revision 1.2  1998/06/07 00:42:35  gruper
 * Gruper added extra_look in restart().
 *
 * Revision 1.1  1998/01/06 04:20:30  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "body.wetness".
 * <p>
 * Describe the arguments in here.
 * @classification body.wetness
 * @see help::effects
 */
#include "path.h"
#include <effect.h>
#include <weather.h>

#define MAX_EFFECT 200
#define MAX_DURATION 120
#define MAX_STRENGTH 240
#define MIN_STRENGTH 20

/** @ignore yes */
string query_classification() { return "body.wetness"; }

/** @ignore yes */
int beginning( object player, int wetness, int id ) {
  int dryness;
  
  tell_object( player, "Squelch.\n" );
  player->submit_ee( "squelch", ({ 20, 20 }), EE_CONTINUOUS );
  player->add_extra_look( this_object() );

  // check for magical (or other) protection from the rain.
  if(dryness = (int)player->check_dryness())
    wetness -= dryness;
  
  if ( wetness > (int)player->query_weight() )
    wetness = (int)player->query_weight();

  return wetness;
}

/** @ignore yes */
void restart( object player ) {
  player->add_extra_look( this_object() );
}

/** @ignore yes */
int merge_effect( object player, int new_wetness, int old_wetness, int id ) {
  int wetness, dryness;

  wetness = new_wetness + old_wetness;

  // check for magical (or other) protection from the rain.
  if(dryness = (int)player->check_dryness())
    wetness -= dryness;

  if ( wetness > (int)player->query_weight() )
      wetness = (int)player->query_weight();

  if ( wetness <= 0 )
    player->submit_ee(0, 0, EE_REMOVE );

  return wetness;
}

// End the effect
/** @ignore yes */
void end( object player, int strength, int id ) {
  tell_object(player, "You feel dry now.\n");
  player->remove_extra_look( this_object() );
}

void squelch(object player, int wetness, int id) {
  string loc;
  object env;
  int bingle;

  env = environment(player);

  if(!env)
    return;
  
  loc = (string)env->query_property("location");
  
  if(loc == "outside" && (int)WEATHER->query_temperature(env) > 0) {
    bingle = (int)WEATHER->query_temperature(env) / 2 +
      env->query_property("warmth");
  } else if(loc == "inside")
    bingle = (20 / 2) + env->query_property( "warmth" );
  
  if(player->query_personal_temp() / 3 > bingle)
    bingle -= (player->query_personal_temp() / 3);
  
  if(!env->query_water() && bingle > 0)
    wetness -= bingle;

  if(player->query_property("dead"))
    wetness = 0;

  if(wetness <= 0)
    player->submit_ee(0, 0, EE_REMOVE );

  if (wetness > 10 && !random(3) && !env->query_water() ) {
    tell_object(player, "Squelch.\n" );
  }
  /*  
  if ( ( ( 100 * wetness ) > ( 80 * (int)player->query_weight() ) ) &&
      !random( 1440 ) && !( player->query_creator() ) )
    player->add_effect( "/std/effects/disease/common_cold", wetness);
  */
  player->set_arg_of(player->sid_to_enum(id), wetness);
  
}

string wet_string( object player, int self ) {
  int wetness;
  int *enums;

  enums = (int *)player->effects_matching( "body.wetness" );
  if ( !sizeof( enums ) )
    return "";

  wetness = (int)player->arg_of(enums[0]);

  if (!intp(wetness)) {
     wetness = 0;
     player->submit_ee(0, 0, EE_REMOVE );
  }
  
  switch ( ( wetness * 100 ) / ( 1 +
      (int)player->query_weight() ) ) {
    case 0 .. 5 :
      if (self)
        return "look slightly wet";
      else
        return "looks slightly wet";
    case 6 .. 10 :
      if (self)
        return "look rather wet";
      else
        return "looks rather wet";
    case 11 .. 30 :
      if (self)
        return "look wet and bedraggled";
      else
        return "looks wet and bedraggled";
    case 31 .. 80 :
      if (self)
        return "are soaked and don't look that well at all";
      else
        return "is soaked and doesn't look that well at all";
    case 81 .. 100 :
      if (self)
        return "look like you have just been dunked in a lake";
      else
        return "looks like "+ player->query_pronoun() +
          " has just been dunked in a lake";
      break;
    default :
      return "";
  }
} /* extra_look() */

/** ignore yes */
string extra_look(object player)  {
   return capitalize(player->query_pronoun()) + " " + wet_string(player, 0) +
             ".\n";
}
