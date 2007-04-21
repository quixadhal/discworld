/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: newbie_shoes.c,v 1.4 2001/02/25 09:04:59 shrike Exp $
 * $Log: newbie_shoes.c,v $
 * Revision 1.4  2001/02/25 09:04:59  shrike
 * changed the message slightly so that it makes more sense with the untying command.
 *
 * Revision 1.3  2000/01/03 10:13:23  taffyd
 * Fixes!
 *
 * Revision 1.2  1999/10/23 11:31:50  gerbil
 *  Forcibly unlocked by taffyd
 *
 * Revision 1.1  1998/01/06 04:40:23  ceres
 * Initial revision
 *
*/
#include <move_failures.h>

#include "path.h"

inherit "/std/effect_shadow";

int query_orthopaedic_shoes() { return 1; }

string query_msgin()
{
return "$N stagger$s clumsily from $F.";
}

string query_msgout()
{
return "$N stagger$s $T clumsily.";
}

int move(mixed dest, string inmsg, string outmsg)
{
  int ret;

  ret = player->move(dest, inmsg, outmsg);
  if (ret == MOVE_OK) {
    tell_object( player, "You wince as spikes seem to drive themselves "
      "into the balls of your feet, but you get there eventually.\n" );
  }
  return ret;
}

int test_for_effect( object thing ) {
   return member_array( EFFECTS +"newbie_shoes",
      (string *)thing->query_wear_effects() ) != -1;
} /* test_for_effect() */

void taken_off(object what, string eff)
{
  if (eff != EFFECTS + "newbie_shoes")
    player->taken_off(what, eff);
  else {
    remove_this_effect();
  }
}
