/** This is an effect to give the player a nasty headache.
*   It lowers their dex, con, wis, and int by 1, as well as 
*   Making them unable to concentrate enough to do spells or 
*   special fighting commands.  Ouch!  It has a classification 
*   of disease.pain.headache
*/
#include <effect.h>
#include "path.h"
string query_classification() { return "disease.pain.headache"; }

/** @ignore yes */
int beginning(object player, mixed arg) {
   tell_object(player, "Your head hurts.\n");
   player->submit_ee(0, arg, EE_REMOVE);
   player->submit_ee("pains", ({30, 60}), EE_CONTINUOUS);
   player->adjust_bonus_con(-1);
   player->adjust_bonus_dex(-1);
   player->adjust_bonus_int(-1);
   player->adjust_bonus_wis(-1);
   return arg;
}

/** @ignore yes */

int restart(object player, mixed arg) {
   player->submit_ee("pains", ({30, 60}), EE_CONTINUOUS);
   player->adjust_bonus_con(-1);
   player->adjust_bonus_dex(-1);
   player->adjust_bonus_int(-1);
   player->adjust_bonus_wis(-1);
   return arg;
}

/** @ignore yes */

int merge_effect( object player, int time1, int time2 ) {
   int duration;
   duration = time2 + player->expected_tt();
   player->submit_ee( "end", duration, EE_REMOVE );
   player->submit_ee("pains", ({30, 60}), EE_CONTINUOUS);
   return duration;
}

/** @ignore yes */

void pains( object player) {
   switch (random(5)) {
    case 0:
      tell_object( player, "Your head hurts.\n");
      tell_room( environment( player ), player->the_short()
           + " looks pained.\n",  ({ player }) );
      break;
    case 1:
      tell_object( player, "You clutch your aching head.\n");
      tell_room( environment( player ), player->the_short()
           + " clutches " + player->query_possessive() + " head.\n",
          ({ player }) );
      break;
    case 2:
      tell_object( player, "Owwwwwwwwwwww!\n");
      tell_room( environment( player ), player->the_short()
           + " moans.\n",  ({ player }) );
      break;
    case 3:
      tell_object( player, "Your head pounds!\n");
      tell_room( environment( player ), player->the_short()
           + " clutches " + player->query_possessive()
           + " head and moans.\n",  ({ player }) );
      break;
    case 4:
      tell_object( player, "Arrrrgh!  Migraine!\n");
      tell_room( environment( player ), player->the_short()
           + " whimpers.\n",  ({ player }) );
      break;
   }
}


/** @ignore yes */
void end(object player) {
   tell_object(player, "Your head feels better.\n");
   player->adjust_bonus_con(1);
   player->adjust_bonus_dex(1);
   player->adjust_bonus_wis(1);
   player->adjust_bonus_int(1);
}
/** @ignore yes */
string query_shadow_ob(){ return SHADOWS+"disease/headache_shadow"; }
