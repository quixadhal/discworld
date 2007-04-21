#include <effect.h>

#define SHADOWS "/std/effects/object/"

string query_classification(){return "object.horse"; }

void beginning(object player, int amount){
    player->submit_ee(0, amount, EE_REMOVE);
    player->add_extra_look(this_object());
}/*beginning()*/

int merge_effect(object player, int old_amount, int new_amount){
   if (old_amount > new_amount)
      return old_amount;
   return new_amount;
}/*merge_effect()*/


string query_shadow_ob(){
   return SHADOWS+"shadow_horse";
}

int survive_death(){ 
   return 0; 
}

string extra_look(object player){
   return capitalize((string)player->the_short()) +
         " is mounted on a large, black horse.\n";
}/*extra_look()*/

void end(object player, int amount, int id){
   player->remove_extra_look(this_object());
} /* end() */
