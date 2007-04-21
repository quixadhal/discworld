/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: in_finger_demon.c,v 1.1 1998/01/06 05:17:24 ceres Exp $
 * $Log: in_finger_demon.c,v $
 * Revision 1.1  1998/01/06 05:17:24  ceres
 * Initial revision
 * 
*/
#include <comms.h>

#define CHAR "/net/daemon/chars/"

inherit "/obj/monster";

object my_player,
       notebook;
string thingy;

void got_home(int bing);

void setup() {
  set_name("demon");
  set_short("small blue demon");
  set_long("A small blue demon with a large green note book.  He is a demon "+
           "with a purpose.  He scrutinises everyone who passes by looking "+
           "for that special person.  He is on a mission from a frog.\n");
  add_adjective(({ "small", "blue" }));
  set_race("imp");
  set_level(5);
  add_property("demon", 1);
  add_property("emote", 1);
  notebook = clone_object("/std/object");
  notebook->set_name("notebook");
  notebook->add_alias("book");
  notebook->add_plural("books");
  notebook->add_adjective(({ "large", "green", "note" }));
  notebook->set_long("A large note book, it looks almost brand new.\n");
  notebook->set_short("large green note book");
  notebook->move(this_object());
} /* setup() */

/* Who says you can kill my nice demon? */
void adjust_hp(int hp, object thing) {
  if (!objectp(thing))
    return 0;
  thing->stop_fight(this_object());
  do_command("stare "+ (string)thing->query_name());
  thing->add_effect("/std/effects/ingested/calm", 60);
  return 0;
} /* adjust_hp() */

void got_player(int bing) {
  if (!my_player) {
    /* Oh no!  Woe is me! */
    command("cry");
    command("'There goes my chance for a gold frog.");
    init_command("sigh");
    call_out("go_away", 5);
    return;
  }
  if (!bing) {
    /* Froodle buns */
    move(environment(my_player), "$N appear$s with a blue flash of light.",
         "$N implode$s in a puff of blue.");
  }
  call_out("do_command", 1, "follow "+my_player->query_name());
  call_out("do_command", 2, "'Ahhhh ha!  I have found you.");
  call_out("do_command", 3, "eye "+my_player->query_name());
  call_out("do_command", 4, "mutter");
  call_out("do_command", 5, "emote scribbles something in its notebook.");
  call_out("do_command", 6, "peer care at "+my_player->query_name());
  call_out("do_command", 7, "mutter purple aardvarks");
  call_out("do_command", 8, "smile bri");
  call_out("do_command", 9, "'Right, got all the info.");
  call_out("do_command", 10, "wave mad");
  call_out("do_command", 11, "unfollow "+my_player->query_name());
  call_out("go_home", 15);
} /* got_player() */

void setup_finger(string who) {
  move(COMM_ROOM, "$N appear$s with a blue flash of light.",
       "$N implode$s in a puff of blue.");
  tell_room(environment(),
            one_short()+" hurridly scribbles something down in its "
            "notebook.\n");
  my_player = find_player(who);
  if (!my_player || my_player->query_property("no finger daemon")) {
    command("cry");
    command("'There goes my chance for a gold frog.");
    call_out("go_away", 5);
    return;
  }
  /* 5 minutes... */
  my_player->add_property("no finger daemon", 1, 300);
  /* Ok, now we try and find our pooooor little player.  pat pat */
  add_effect("/std/effects/npc/goto_destination",
               ({ my_player, (: got_player :), 20 }));
} /* setup_finger() */

/* Now try and get back to the communication room */
void go_home() {
  add_effect("/std/effects/npc/goto_destination",
             ({ COMM_ROOM, (: got_home :), 20 }));
} /* go_home() */

void got_home(int bing) {
  if (!bing) {
    move(COMM_ROOM, "$N appear$s with a blue flash of light.",
         "$N implode$s in a puff of blue.");
  }
  command("'Home sweet home!");
  call_out("go_away", 3);
} /* got_home() */

void go_away() {
  command("wave");
  move("/room/rubbish", "$N appears.", "$N slowly becomes more transparent "
       "and then vanishes utterly.");
} /* go_away() */
