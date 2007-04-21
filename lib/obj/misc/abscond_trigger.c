/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: abscond_trigger.c,v 1.1 2001/04/21 11:26:33 shrike Exp $
 *
 *
 */

/* This object serves one purpose: to make sure that this_player() is
 * correct in the abscond command.  See the code in
 * /std/effects/other/abscond for a more complete explanation.
 */

#define CLASS "fighting.combat.special.abscond"

#include <effect.h>

inherit "/std/object";

void init() {
   if( !environment()
    || file_name( environment( this_object() ) ) == "/room/rubbish" ) {
      return;
   }

   if( this_player()
    && this_player() == query_property( "abscond player" ) ) {
      this_player()->submit_ee2( this_player()->effects_matching(CLASS)[0],
                                 "after_command", 0, EE_REMOVE );
   }
}
