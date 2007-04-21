/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: bit_set.c,v 1.2 1998/01/23 21:38:08 dragonkin Exp $
 *
 * $Log: bit_set.c,v $
 * Revision 1.2  1998/01/23 21:38:08  dragonkin
 * Fixed to work with new bits.
 *
 * Revision 1.1  1998/01/15 19:20:53  dragonkin
 * Initial revision
 *
*/
#include "corpse.h"
inherit "std/bit";

#include <bit.h>

void create() {
  ::create();

  set_short("anonymous set of bits");
  set_long("This is an unknown set of bits of some creature.\n");
  corpse_weight = STD_CORPSE_WEIGHT;
  set_weight(5);
  set_name("bit");
  add_property( "corpse bit", 1 );
  add_property("cureable", 1);
} /* create() */

void setup_long() {
  if ( !bit_data )
    return;
  if (no_decay()) {
    if (race_name) {
      set_short( "set of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_long("This is a set of " + (bit_data[BIT_NAME]) +
               " from " + add_a(race_name) + ".\n");
    } else {
      set_short( "set of "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ bit_data[ BIT_NAME ] );
      set_long("This is a set of " + (bit_data[BIT_NAME]) +
               " from an unknown creature.\n");
    }
  } else if (decay > 80) {
    if (race_name) {
      set_short( "set of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_long("This is a fresh set of " + (bit_data[BIT_NAME]) +
               " severed from the corpse of " + add_a(race_name) + ".\n");
    } else {
      set_short( "set of "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ bit_data[ BIT_NAME ] );
      set_long("This is a fresh set of " + (bit_data[BIT_NAME]) +
               " severed from the corpse of an unknown creature.\n");
    }
  } else if (decay > 50) {
    if (race_name) {
      set_short( "set of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_long("This is a set of " + bit_data[BIT_NAME] + " severed from the " +
               "corpse of " + add_a(race_name) + ".\n");
    } else {
      set_short( "set of "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ bit_data[ BIT_NAME ] );
      set_long("This is a set of " + bit_data[BIT_NAME] + " severed from the " +
               "corpse of an unknown creature.\n");
    }
  } else if (decay > 30) {
    if (race_name) {
      set_short( "set of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_long("This is the partially decayed remains of a set of " +
               bit_data[BIT_NAME] +" severed from the "
               "corpse of " + add_a(race_name) + ".\n");
    } else {
      set_short( "set of "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ bit_data[ BIT_NAME ] );
      set_long("This is the partially decayed remains of a set of " +
               bit_data[BIT_NAME] + " severed from the " +
               "corpse of an unknown creature.\n");
    }
  } else {
    if (race_name) {
      set_short( "set of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ race_name +" "+ bit_data[ BIT_NAME ] );
      set_long("This is the almost unrecognizable remains of a set of " +
         bit_data[BIT_NAME] +" severed from the "
         "corpse of " + add_a(race_name) + ".\n");
    } else {
      set_short( "set of "+ bit_data[ BIT_NAME ] );
      set_main_plural( "sets of "+ bit_data[ BIT_NAME ] );
      set_long("This is the almost unrecognizable remains of a set of " +
         bit_data[BIT_NAME] + " severed from the " +
         "corpse of an unknown creature.\n");
    }
  }
  if ( cured )
    set_long( query_long() +"It seems to have been pickled.\n" );
} /* setup_long() */

