inherit "/obj/monster";

#define COMM_ROOM "/d/am/uu/uni/comm_room"
#define CHAR "/std/spells/wizard/chars/"

object notebook;
string thingy, home, my_player, dest;

void setup() {
  set_name("demon");
  set_long("A small demon with a small note book.  He is looking "+
           "furtively around the room.  You notice a small badge with the "+
           "ankh morpork post office symbol on it.\n");
  notebook = clone_object("/std/object");
  notebook->set_name("notebook");
  notebook->add_alias("book");
  notebook->add_plural("books");
  notebook->add_adjective(({ "small", "note" }));
  notebook->set_long("A small note book, it looks almost brand new.\n");
  notebook->set_short("small note book");
  notebook->move(this_object());
} /* setup() */

void set_home(string str) { home = str; }
string query_home() { return home; }

/* Who says you can kill my nice demon? */
void adjust_hp(int hp, object at) {
} /* adjust_hp() */

void got_home(int bing) {
  if (!bing) {
    move_player("X", home);
  }
  call_out("go_away", 2);
} /* got_home() */

void got_dest(int bing) {
  if (!bing) {
    move_player("X", dest);
  }
  command("stare dre");
  init_command("emote fiddles around with some dials.");
  init_command("emote pushes some things around absently.",2);
  call_out("go_home", 5);
} /* got_dest() */

void go_home() {
  add_property("goto destination", home);
  add_triggered_action("player", "goto_destination", this_object(),
                       "got_home");
} /* go_home() */

void goto_dest(string place, string from) {
  add_property("goto destination", place);
  dest = place;
  add_triggered_action("player", "goto_destination", this_object(),
                       "got_dest");
  notebook->set_read_mess("You have some mail from "+from+", goto the "+
                          "post office to read it.");
  notebook->add_read_mess(({ CHAR+"post_demon", "warning" }), 0,
                            "wizard spells", 0);
} /* goto_dest() */

void got_player(int bing) {
  if (!find_player(my_player)) {
    command("bing sad");
    init_command("'I wish I was a purple demon.");
    go_home();
    return ;
  }
  if (!bing) {
/* Froodle buns */
    move_player("X", environment(find_player(my_player)));
  }
  init_command("give note book to "+my_player);
  init_command("smile "+my_player);
  go_home();
} /* got_player() */

void goto_player(string to, string from) {
/* Ok, now we try and find our pooooor little player.  pat pat */
  add_property("goto player", to);
  my_player = to;
  add_triggered_action("player", "goto_player", this_object(),
                       "got_player");
  notebook->set_read_mess("You have some mail from "+from+", goto the "+
                          "post office to read it.");
  notebook->add_read_mess(({ CHAR+"post_demon", "warning" }), 0,
                            "wizard spells", 0);
} /* finger_callback() */

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
  tell_room(environment(), short(0)+" enters back into the cage and has "+
                           "a fierce argument about whos "+
                           "turn it is.\n");
  dest_me();
} /* go_away() */
