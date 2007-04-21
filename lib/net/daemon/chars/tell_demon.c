/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: tell_demon.c,v 1.1 1998/01/06 05:17:24 ceres Exp $
 * $Log: tell_demon.c,v $
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

void setup() {
  set_name("purple demon");
  set_long("A small purple demon with a large note book.  He is looking "+
           "furtively around the room.  He seems in somewhat of a hurry.\n");
  add_alias("demon");
  set_short("purple demon");
  set_race( "imp" );
  set_level(-2);
  add_adjective(({ "purple", "short" }));
  notebook = clone_object("/std/object");
  notebook->set_name("notebook");
  notebook->add_alias("book");
  notebook->add_plural("books");
  notebook->add_adjective(({ "large", "note" }));
  notebook->set_long("A large note book, it looks almost brand new.\n");
  notebook->set_short("large note book");
  notebook->move(this_object());
  add_property("demon", 1);
} /* setup() */

/* Who says you can kill my nice demon? */
void adjust_hp(int hp, object at) {
} /* adjust_hp() */

void got_player(int bing) {
  if (!my_player) {
/* Oh no!  Woe is me! */
    command("cry");
    command("'There goes my chance for a gold frog.");
    init_command("sigh");
    call_out("go_away", 2);
    return ;
  }
  if (!bing) {
/* Froodle buns */
    move_player("X", environment(my_player));
  }
  init_command("give note book to "+my_player->query_name());
  init_command("smile "+my_player->query_name());
  call_out("go_away", 20);
} /* got_player() */

void setup_tell(string person, string mud, string who, string mess) {
  move_player("X", COMM_ROOM);
  tell_room(environment(),
            short(0)+" scribbles something hurridly down in its notebook.\n");
  my_player = find_player(lower_case(who));
/* Ok, now we try and find our pooooor little player.  pat pat */
  if (!my_player->query_creator()) {
    add_property("goto player", my_player->query_name());
    add_triggered_action("player", "goto_player", this_object(),
                         "got_player");
  } else
    got_player(0);
  notebook->set_read_mess(person+"@"+mud+" sent you this message:\n"+mess);
  notebook->add_read_mess(({ CHAR+"tell_demon", "warning" }), 0,
                            "wizard spells", 0);
} /* setup_tell() */

string garble_text(mixed bing, object ob) {
  call_out("do_dest", 10, ob);
  return "Warning, this note will self destruct in ten seconds.\n";
} /* warning() */

string magic_text(mixed bing, object ob) {
  call_out("do_dest", 10, ob);
  return "Warning, this note will self destruct in ten seconds.\n";
} /* warning() */

void do_dest(object ob) {
  object ob2;

  if (!ob) return ;
  ob2 = environment(ob);
  if (living(ob2)) {
    tell_room(environment(ob2), ob2->query_cap_name()+"'s "+
              ob->short()+" spontaneously combusts.\n", ({ ob2 }));
    tell_object(ob2, "Your "+ob->short()+" spontaneously combusts.\n");
  } else {
    tell_room(ob2, ob->short()+" spontaneously combusts.\n");
  }
  ob->dest_me();
} /* do_dest() */

void go_away() {
  command("wave");
  tell_room(environment(), short(0)+" becomes slowly more transperent "+
                           "and then vanishes utterly.\n");
  dest_me();
} /* go_away() */
