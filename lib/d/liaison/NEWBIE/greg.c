/*  -*- LPC -*-  */
#include "path.h"
#include <armoury.h>

inherit "/obj/monster";

#define THROW_OUT 300

#define RPATHS ({ PATH + "combat_room1", PATH + "combat_room2", PATH + "combat_room3"})
#define RNAMES ({ "one", "two", "three" })

object *queue = ({ }); // the queue of people waiting to use a room.
object *offered = ({ }); // offered.
object *in_use = ({ 0, 0, 0 }); // who is using the rooms.
object combat_room;
mapping callouts;

void setup() {
  add_property("determinate", "");
  set_name( "greg" );
  set_short("Greg" );
  set_main_plural("Gregs" );
  set_race( "human" );
  set_response_mon_understand_anything(1);
  set_gender( 1 );
  set_al(-500 );
  set_class( "fighter" );
  set_language( "general" );
  set_level( 100 + random( 100 ) );
  set_long( "This is Greg the combat instructor.  He looks pretty mean, but "
            "don't let appearances deceive you, he's a softy at heart.\n" );
  adjust_money( 10 + random( 40 ), "Ankh-Morpork pence" );
  load_chat( 25, ({
    1, ":grunts.",
      1, "'Ug.",
      1, "'I am great.",
      1, "@flex",
      1, "'Look at my cool muscles.",
      }) );

  add_respond_to_with(({ "yes" }), ({ "#yes" }));
  add_respond_to_with(({ "no" }), ({ "#no" }));

  add_respond_to_with(({ ({ "can", "may" }),
                         ({ "practise", "practice", "train"}),
                         ({ "please" }) }),
                      ({ "#request" }) );

  add_respond_to_with(({ ({ "hello", "hi", "gday", }),
                           ({ "womble", "all" }) }), ({ "'Hello $hcname$",
                                                          "bow ? $hname$" }));

  add_respond_to_with(({ "fuck", ({ "off", "you" }) }), ({ "'Same to you!",
                                                             "skick $hname$"
                                                             }));
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

  add_skill_level( "other.health", 50 + random( 50 ) );
  (ARMOURY->request_weapon("long sword", 75))->move(this_object());
  (ARMOURY->request_armour("ringmail", 80))->move(this_object());
  (ARMOURY->request_armour("metal clad boots", 80))->move(this_object());
//  (ARMOURY->request_armour("metal helmet", 80))->move(this_object());
  (ARMOURY->request_armour("metal gauntlets", 80))->move(this_object());
  init_equip();
  callouts = ([ ]);
}

void event_enter(object ob, string message, object from) {
  if(environment() == find_object("/room/rubbish")) {
    return ::event_enter(ob, message, from);
  }
  if(ob->query_property("dead")) {
    return ::event_enter(ob, message, from);
  }
  if(!ob->query_visible(this_object())) {
    return ::event_enter(ob, message, from);
  }
  if(!userp(ob)) {
    return ::event_enter(ob, message, from);
  }
  if(callouts && callouts[ob]) {
    remove_call_out(callouts[ob]);
    map_delete(callouts, ob);
  }

  call_out("do_greeting", 1, ob, from);
}

int attack_by(object thing) {
  tell_room(environment(), "Greg lifts a hand and slaps " + thing->one_short()
            + " sending " + thing->query_objective() + " flying across "
            "the room.\n", thing);
  write("Greg lifts a hand, almost as large as your head, and gives you a "
        "slap that sends you flying across the room.\n");
  write("You bash your head against the wall and the world starts "
        "spinning.\n");
  do_command("'Hey, that's not nice!");
  thing->add_effect("/std/effects/ingested/tranquil_sleep", 120);
  this_object()->stop_fight(thing);
  thing->stop_fight(this_object());
  return 0;
}

// Check if there is a room free and if this person is at the top of the
// queue.
void check_room() {
  int room;
  object ob;

  queue = filter_array(queue,
                       (: environment($1) == environment(this_object()) :));

  // When we just want to shuffle someone off the queue we call this function
  // with ob of 0 and then just take the first person in line.
  if(sizeof(queue))
    ob = queue[0];

  if(member_array(ob, queue) == -1)
    return;

  room = member_array(0, in_use);

  if(room == -1) {
    do_command("'Ok " + ob->one_short() + ", unfortunately all the rooms are "
               "busy right now, but "
               "if you don't mind waiting a minute one will become free.");
    return;
  }

  do_command("'Ok " + ob->one_short() + ", you can use room " +
             RNAMES[room] + ".");
  tell_object(ob, "Greg leads you into room " +
              RNAMES[room] + ".\n");
  ob->move_with_look(RPATHS[room], "$N is propelled into the room.",
                     "$N is led out of the room.");
  in_use[room] = ob;
  queue -= ({ ob });

  callouts[ob] = call_out("throw_out", THROW_OUT, ob);
  combat_room = environment();
}

// Called when someone says yes to training
void yes(object ob) {
  if(member_array(ob, offered) == -1 || member_array(ob, queue) != -1)
    return;

  queue += ({ ob });
  check_room();
}

void no(object ob) {
  if(member_array(ob, offered) == -1)
    return;
  offered -= ({ ob });
  if ( environment(ob) == environment(this_object()) ) {
    do_command("'Okay.  Well, come back if you feel like having a go later.");
    do_command("smile " + ob->query_name());
  }
}

// Someone wants to practice.
void request(object ob) {
  do_command("'Would you like to try out your skills " + ob->one_short() +
             "?");
  if(!sizeof(filter_array(ob->query_holding(), (: $1 && $1->query_weapon()
                                                :))))
    do_command("'You may want to buy a weapon at the shop before trying out "
               "your skills.");
  if(!offered)
    offered = ({ });
  offered += ({ ob });
}

// find out who is using a particular room, this is called by the combat
// rooms exit function.
object query_using(int room) {
  return in_use[room];
}

// Throw people out of the room if they've been in there too long.
void throw_out(object ob) {
  int room;

  room = member_array(ob, in_use);

  if(room != -1) {
    if(base_name(environment(ob)) == RPATHS[room]) {
      do_command(RNAMES[room]);
      call_out("tell_them", 2, ob, room);
    } else {
      in_use[room] = 0;
    }
  }
}

void tell_them(object ob, int room) {
  if(!ob) {
    do_command("combat");
    return;
  }

  do_command("'Ok " + ob->one_short() + ", time's up.");
  do_command("'Come with me.");
  environment()->stop_them(ob);
  tell_object(ob, "Greg leads you out of the room.\n");
  do_command("combat");
  ob->move_with_look(combat_room, "$N is propelled into the room.",
                     "$N is pushed out of the room.");
}

// Welcome them and see if they want to do some combat training.
void do_greeting(object ob) {
  int room;

  if(!ob || !interactive(ob) || member_array(ob, queue) != -1)
    return;

  room = member_array(ob, in_use);

  // If they haven't just used a room, and are not marked as using a
  // room say hello.
  if(room == -1) {
    do_command("'Hello " + ob->one_short() + ", welcome to the combat "
               "training area.");
    do_command("'Would you like to try out your skills?");

    if(!sizeof(filter_array(ob->query_holding(), (: $1 && $1->query_weapon()
                                                  :))))
      do_command("'You may want to buy a weapon at the shop before trying out "
                 "your skills.");

  } else {
    do_command("'I hope you had a productive time.  Would you like to have "
               "another go?");

    // If they're still marked as using a room then mark that room as
    // available and check for someone new needing it.
    if(room != -1) {
      in_use[room] = 0;
      check_room();
    }
  }

  if(!offered)
    offered = ({ });
  offered += ({ ob });
}
