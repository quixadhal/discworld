/*  -*- LPC -*-  */
inherit "/obj/monster";

#include "/d/am/chars/path.h"
#include <money.h>

int going;
string from;
string spouse;
int start_money;

void initial_chats() {
   load_chat( 15, ({
      1, "'Money for the poor.",
      1, "'Please give me some money.",
      1, ":pulls on your leg.",
      1, ":rattles "+query_possessive()+" tin.",
      1, ":mutters something about a "+spouse+" and 7 children.",
      1, ":looks at you with big brown eyes.",
      1, "'Who will help me?",
      1, ":looks down at the ground and sobs.",
      1, "#do_gossip_say:Did you know $name$ said $mess$?",
      1, "@open door",
   }) );
}

void fix_gender( int gender ) {
   set_gender( gender );
   set_long( "This is a poor beggar.  Perhaps you should give "+
            (string)query_objective() +" some money.\n" );
/* Added so that the chat about spouse and 7 children is correct for gender */
   spouse = (query_gender()==1) ? "wife" : "husband" ;
}

void setup() {
   set_name("beggar");
   set_short( "poor beggar" );
   set_main_plural( "poor beggars" );
   add_adjective("poor");
   add_alias("beggar");
   set_race("human");
   set_level( 3 + random( 10 ) );
   set_wimpy( 80 );
   set_al( HOSPITAL->pick_al() / 10 );
   fix_gender(1+random(2));
   add_move_zone("docks");
   add_move_zone("underdocks");
   start_money = 3 + random( 300 );
   adjust_money( start_money, "Ankh-Morpork pence" );

   initial_chats();
        
   add_effect( "/std/effects/npc/gossip", 5 );
   add_effect( "/std/effects/npc/given", ({ this_object(), "given_thing" }));
//   add_effect( "/std/effects/npc/they_died", ({ this_object(), 
//        "loot_coins" }) );
} /* setup() */

/*
 * At the bank.  Dump all but some start money.
 */
void done_everything() {
   going = 0;
} /* done_everything() */

void at_pub() {
   mixed *stuff;
   string dest;

   /* Ok.  Get them to start buy stuff until they cannot any more */
   stuff = environment()->query_menu_items();
   if (!stuff) {
      /* Not at a pub yet? */
      /* Darn!  try again... */
      dest = PUBS[random(sizeof(PUBS))];
      /* Take them to a pub.  Drink their money */
      add_effect( "/std/effects/npc/goto_destination",
                  ({ dest, ({ this_object(), "at_pub" }), 20 }) );
   } else {
      /* Add the drink silly effect... */
      add_effect( "/std/effects/npc/drink_silly",
                  ({ this_object(), "done_drinking" }) );
   }
} /* at_pub() */

void hassle_opposite_gender() {
  object *things;
  string action;

  things=filter(all_inventory(environment()),
      (: $1->query_gender()==3-($2->query_gender()) :), this_object() );

  if (sizeof(things)) {
     action=({ ":leers at TARGET.",
        ":pats TARGET on the bum.",
        "say How's it going sweetheart?",
        ":grins at TARGET.",
        ":wolf whistles at TARGET." })[random(5)];
     do_command(replace(action,"TARGET",
        things[random(sizeof(things))]->the_short() ));
     return;
  }
  do_command( ({ ":cracks a rude joke.",
                "drool",
                "laugh out loud",
                "say So where are the babes?",
                "burp loudly" })[random(5)] );
}

void done_drinking() {
   /* load abusive chats */
   load_chat( 60, ({
      1, "'You rich prats never give me any respect.",
      1, "'Give me some money for booze.",
      5, "#hassle_opposite_gender",
      1, ":ponders "+query_possessive()+" empty tin.",
      1, ":chuckles something about a "+spouse+" and 7 children.",
      1, ":grins toothily at you.",
      1, "@burp",
      1, "#do_gossip_say:Do you even care that $name$ said $mess$?"
   }) );
   
   call_out( function() { this_object()->initial_chats(); 
               this_object()->add_effect( "/std/effects/npc/goto_destination",
               ({ from, ({ this_object(), "done_everything" }), 5 }) ); },
             random(300)+300 );

} /* done_drinking() */

void given_thing(object player, object ob, string mess) {
   string dest;

   if (ob->id(MONEY_ALIAS)) {
      if (!going) {
         if(ob->query_value_in("Ankh-Morpork") > 300) {
            this_player()->adjust_al( -5 );
         }
         dest = PUBS[random(sizeof(PUBS))];
         add_effect( "/std/effects/npc/no_follow", 0 );
         /* Take them to a pub.  Drink their money */
         add_effect( "/std/effects/npc/goto_destination",
                     ({ dest, ({ this_object(), "at_pub" }), 20 }) );
      }
      return ;
   }

   if (!ob->query_value()) {
      init_command("drop "+implode(ob->query_adjectives(), " ")+
                           " "+ob->query_name());
      return ;
   }

   if (!going) {
      from = file_name(environment());
      this_player()->adjust_al( -5 );
      going = 1;
      from = file_name(environment());
      init_command("'Thanks!  I always wanted "+
                   ob->a_short());
      init_command("womble");
      dest = SHOPS[random(sizeof(SHOPS))];
      add_effect( "/std/effects/npc/no_follow", 0 );
      /* Take them to a shop.  Sell everything. */
      add_effect( "/std/effects/npc/goto_destination",
                  ({ dest, "sell", 20 }) );
      dest = PUBS[random(sizeof(PUBS))];
      /* Take them to a pub.  Drink their money */
      add_effect( "/std/effects/npc/goto_destination",
                  ({ dest, ({ this_object(), "at_pub" }), 20 }) );
   }
} /* given_thing() */

void loot_coins() {
   call_out( (: do_command(":looks around nervously.") :), 2 );
   call_out( (: do_command("get coins from corpse") :), 5+random(10) );
} /* loot_coins() */







