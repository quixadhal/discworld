inherit "/obj/monster";
#define COMM_ROOM "/d/am/uu/uni/comm_room"
#define CHAR "/net/daemon/chars/"

object my_player,
       notebook;
string thingy;

void setup() {
  set_name("blue demon");
  set_long("A blue small demon with a large green note book.  He is a demon "+
           "with a purpose.  He scrutinises everyone who passes by looking "+
           "for that special person.  He is on a mission from a frog.\n");
  add_alias("blue");
  add_adjective(({ "small", "blue" }));
  set_level(-2);
  set_short("Blue demon");
  notebook = clone_object("/std/object");
  notebook->set_name("notebook");
  notebook->add_alias("book");
  notebook->add_plural("books");
  notebook->add_adjective(({ "large", "note", "green" }));
  notebook->set_long("A large note book, it looks almost brand new.\n");
  notebook->set_short("large note book");
  notebook->move(this_object());
  add_property("demon", 1);
  add_property("emote", 1);
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
  call_out("init_command", 1, "follow "+my_player->query_name());
  call_out("init_command", 1, "'Ahhhh ha!  I have found you.");
  init_command("eye "+my_player->query_name());
  call_out("init_command", 2, "mutter");
  call_out("init_command", 3, "emote scribbles something in his notebook.");
  call_out("init_command", 4, "peer care at "+my_player->query_name());
  call_out("init_command", 5, "mutter purple aardvarks");
  call_out("init_command", 6, "smile bri");
  call_out("init_command", 7, "'Right, got all the info.");
  call_out("init_command", 8, "wave mad");
  call_out("init_command", 9, "unfollow "+my_player->query_name());
  call_out("go_home", 11);
} /* got_player() */

void setup_finger(string who) {
  move_player("X", COMM_ROOM);
  tell_room(environment(),
            short(0)+" scribbles something hurridly down in its notebook.\n");
  my_player = find_player(who);
/* Ok, now we try and find our pooooor little player.  pat pat */
  add_property("goto player", my_player->query_name());
  add_triggered_action("player", "goto_player", this_object(),
                       "got_player");
} /* setup_tell() */

/* Now try and get back to the communication room */
void go_home() {
  add_property("goto co-ordinates", COMM_ROOM->query_co_ord());
  add_property("goto property", "communication room");
  add_triggered_action("player", "goto_co_ord", this_object(),
                       "got_home");
} /* go_home() */

void got_home(int bing) {
  if (!bing)
    move_player("X", COMM_ROOM);
  command("'Home sweet home!");
  call_out("go_away", 3);
} /* got_home() */

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
