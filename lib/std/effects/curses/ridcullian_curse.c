/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ridcullian_curse.c,v 1.1 1998/01/06 04:07:14 ceres Exp $
 * $Log: ridcullian_curse.c,v $
 * Revision 1.1  1998/01/06 04:07:14  ceres
 * Initial revision
 * 
*/
/**
 * Makes the players hair fall out.
 * The classification of this effect is "misc.curse.ridcullian".
 * <p>
 * The argument specifies how long the hair loss will last.
 * @classification misc.curse.ridcullian
 * @example
 * // Give them the curse for 5 minutes
 * add_effect("/std/effects/curse/ridcullian", 300);
 */
#include <effect.h>

/** @ignore yes */
string query_classification() { return "misc.curse.ridcullian"; }

/** @ignore yes */
int merge_effect( object player, int old_arg, int new_arg, int id ) {
   player->submit_ee( 0, player->expected_tt() + new_arg, EE_REMOVE );
   tell_object(player, "You feel more concerned about your hair loss.\n");
}

/** @ignore yes */
int test_remove( object player, int a, int id, int bonus) {
   if (bonus > 150) return 1;
   tell_object(player, "You feel less concerned about your hair loss.\n");
   player->submit_ee(0, player->expected_tt() - bonus, EE_REMOVE);
   return 0;
}

/** @ignore yes */
int beginning( object player, int a, int id) {
   player->submit_ee("hair_worry", ({ 10, 20 }), EE_CONTINUOUS);
   player->submit_ee(0, a, EE_REMOVE);
   
   tell_object( player, "You reach up to smooth your hair, and a large "+
               "chunk of hair comes out.\n");
   tell_room( environment( player ), player->the_short() + " smooths " +
             player->query_possessive() + " hair and then stares in "+
             "horror at " + player->query_possessive() + " hands.\n",
             ({ player }) );
}

/** @ignore yes */
void end( object player, mixed a, int id ) {
   tell_object( player, "Ahhhh.  Your hair seems to have stopped "
               "falling out so prematurely.\n");
}

/** @ignore yes */
void hair_worry( object player, mixed a, int id ) {
   switch (random(5)) {
    case 0:
      player->do_say("I'm not going bald!  I mean it's "+
                     "ridiculous!  My family have never gone "+
                     "bald, except for one of my aunts!");
      break;
    case 1:
      player->do_say("I mean it wasn't there yesterday!");
      break;
    case 2:
      player->do_say("I have NOT got a bald spot!");
      break;
    case 3:
      player->do_say("Something's definitely happening, "+
                     "I can feel it in my water.");
      
      break;
    case 4:
      /* need soul thingy here :( */
      player->do_say("What about this, then?");
      break;
   }
}

