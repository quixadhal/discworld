/* Coded by Talonia, Nov., '95*/
/* Modified to expound on multiplaying by Arby, June 16, 1998 */

#include "path.h"
#include <library.h>

inherit "/obj/monster";

void setup() {
  set_name("womble");
  set_short("womble");
  set_long("This is a small, furry womble.  She is about four feet "
           "tall and looks very cuddly.  She also looks very sad and "
           "is sobbing loudly.\n");
  add_property( "unique", 1 );
  set_gender(2);
  add_plural("wombles");
  set_race("human");
  set_level(100);
  set_language("general");
  add_move_zone("Newbie");
  set_move_after(30, 30);
  set_response_mon_understand_anything(1);
  set_respond_to_with(({
    ({"@comfort", "womble"}),
      "'Thank you, will you find my brooch for me?",
    ({({"@kick", "@punch"}), "womble"}),
      "cry"}));

  add_respond_to_with(({ ({ "hello", "hi", "gday", }),
                     ({ "womble", "all" }) }), ({ "'Hello $hcname$",
                                                    "greet $hname$" }));
  add_respond_to_with(
                           ({ ({"good day", "greetings"}), 
         ({ "womble", "all" }) }), ({ "'Hello $hcname$",
                                        "bow deep $hname$" }));
  add_respond_to_with(
   ({ "fuck", ({ "off", "you" }) }), ({ "'Same to you!", "skick $hname$" }));
  add_respond_to_with(({ "fuck" }), "waggle $hname$");
  add_respond_to_with(
                      ({ "@smile", "womble" }), "smile $hname$");
  add_respond_to_with(
     ({ ({ "@skick", "@spunch", "@jump", "@pat", "@slap" }),
          "womble" }), "glare $hname$");
  add_respond_to_with(
                      ({ ({ "@poke", "@glare", "@point" }),
                           "womble" }), "peer sus at $hname$");
  add_respond_to_with(
                      ({ "@grin", "womble" }), "grin $hname$");
  add_respond_to_with(
                ({ ({ "@caress", "@snuggle", "@nestle", "@love", "@throw" }),
                     "womble" }), "waggle $hname$");
  add_respond_to_with(
                      ({ ({ "@kiss", "@french", }),
                           "womble" }), "slap $hname$");
  add_respond_to_with(({ ({ "@bing", "@womble", }),
                           "womble" }), "smile bri $hname$");

  load_chat(50, ({
    1, ":sobs loudly.",
    1, "'Boo Hoo!  I've lost my brooch!",
    1, "@weep",
    1, "'I really loved that brooch.",
    1, "@cry",
    1, "'Oh I wish someone would find it for me.",
    2, "'Please find my brooch for me.",
    2, "'I lost my brooch in one of the rooms while I was reading all "+
       "the wonderful information here."}) );
} /* setup() */

void event_enter(object obj, string mess, object from) {
  if( (obj->query_property("wombles brooch")) &&
      (obj->query_name() == "brooch") &&
      (environment(obj) == this_object()) ) {
    call_out("brooch", 1, ({ this_player() }) );
  }
} /* event_enter() */

void brooch(mixed arg) {
  object item, tp;
  tp = arg[0];
  if(tp->query_property("womble's brooch")) {
    do_command("'Thank you for returning my brooch again.");
  } else {
    tp->add_property("womble's brooch", 1, 86400);
    tp->adjust_xp(10000);
    do_command("'Wow!  My brooch!");
    do_command("'Thank you very much!");
  }

  do_command("'You may now enter the game by typing \"out\" in the foyer.");
  do_command("'Good Luck!");
  do_command("huggle "+(string)tp->query_short());
  do_command("emote sets the brooch down, and quickly forgets where she left it.");
  foreach(item in all_inventory()) {
    item->dest_me();
  }
} /* brooch() */

int attack_by(object thing) {
  write("The womble smacks you round the head with her paw sending you "
  "flying across the room.\n");
  write("You bash your head against the wall and the world starts "
  "spinning.\n");
  do_command("'Hey, that's not nice!\n");
  thing->add_effect("/std/effects/ingested/tranquil_sleep", 120);
  this_object()->stop_fight(thing);
  thing->stop_fight(this_object());
  return 0;
} /* attack_by() */

int attack_ob(object thing) {
  this_object()->stop_fight(thing);
  thing->stop_fight(this_object());
  return 0;
} /* attack_ob() */

