/*  -*- LPC -*-  */
#include "path.h"

inherit "/obj/monster";

void respond_to_soul(object play);

void setup() {
   int i;
   string *types = ({ "playful", "quiet", "thin", "fat", "noisy",
         "thoughtful", "happy", "sad", "boring", "interesting" });
   set_name( "child" );
   i = random( sizeof( types ) );
   set_short( types[ i ] +" child" );
   add_adjective( types[ i ] );
   set_main_plural( types[ i ] +" children" );
   set_race( "human" );
   set_gender( 1 + random( 2 ) );
/* Some sort of "young" property for the race object to check? */
   i = (int)"/std/races/human"->query_weight();
   i += (int)"/std/races/human"->modifier( weight );
   set_base_weight( ( 3 * i ) / ( 4 + random( 6 ) ) );
   i = (int)"/std/races/human"->query_height();
   i += (int)"/std/races/human"->modifier( height );
   set_height( ( 2 * i ) / ( 3 + random( 4 ) ) );
   set_con( ( 2 * query_con() ) / ( 3 + random( 4 ) ) );
   set_dex( ( 2 * query_dex() ) / ( 3 + random( 4 ) ) );
   set_str( ( 2 * query_str() ) / ( 3 + random( 4 ) ) );
   set_al( -50 + (int)HOSPITAL->pick_al() / 10 );
   set_level( random( 5 ) );
   set_wimpy( 90 );
   set_long( "This is one of the inhabitants of Pumpkin.\n" );
   add_effect( "/std/effects/npc/gossip", 5);
   load_chat(40, ({ 1, "#do_gossip_say:$mess$" }));

   add_respond_to_with(({ ({ "@skick", "@spunch", "@jump", "@push",
                             "@shove", "@knee" }) }),
                       (: respond_to_soul :));
} /* setup() */

void respond_to_soul(object play) {
   switch (random(6)) {
   case 0:
      do_command("'Thats not very nice!");
      break;
   case 1:
      do_command("'Pick on someone your own size.");
      do_command("glare " + player->query_name());
      break;
   case 2:
      do_command("cry");
      do_command("accuse " + player->query_name() + " of child abuse");
      break;
   case 3:
      do_command("spunch " + player->query_name());
      do_command("'Take that!  and that!");
      do_command("skick " + player->query_name());
      break;
   case 4:
      do_command("accuse " + player->query_name() + " of child abuse");
      break;
   case 5:
      do_command("'I will tell my brother!");
      break;
   }
   call_out("run_away", 2);
} /* respond_to_soul() */
