/**This is the shadow object for the headache effect.  It
 * handles the inability to cast and do special manoeuvres.
 */

inherit "/std/effect_shadow";

/**
 *@ignore yes
 */
int query_special_manoeuvre() {
   tell_object(player, "Your head hurts too much to try anything.\n");
   return 0;
} /* query_special_manoeuvre() */

/**
 *@ignore yes
 */
int command_shadowed(string verb, string arg){
   if(verb=="cast"){
     write("Your head hurts too much to muster your might.\n");
     return 1;
   }
   return player->command_shadowed(verb, arg);
} 

