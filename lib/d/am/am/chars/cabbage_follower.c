#include "path.h"
inherit "/obj/monster";

int eaten;

string my_owner;

void setup() {
  set_level(-10);
  set_name("cabbage");
  set_gender(1+random(2));
  set_long(
"A small cute looking cabbage.\n");
} /* setup() */

void set_owner(string name) {
  set_long(
"A small cute looking cabbage, it looks adoringly at "+name+".\n");
  load_chat(30, ({
1, "@roll up",
1, "@purr",
1, ":ruffles it's fronds.",
1, ":looks green.",
1, "@snuggle "+name,
1, "@stare dre at "+name,
1, "@nestle "+name,
1, "@roll over",
      }));
  my_owner = name;
} /* set_owner() */

void init() {
  ::init();
  if (my_owner && !find_player(my_owner)) {
    command("cry");
    command("'Woe is me!  Where for art thou "+my_owner);
    eaten = 1;
    call_out("dest_me", 2);
  }
  this_player()->add_command("eat", this_object());
} /* init() */

int do_eat() {
  if ((string)this_player()->query_name() != my_owner) return 0;
  write("The cabbage lovingly and adoringly purrs quietly as you "+
        "visciously rip off its fronds and eat it.  It dies looking "+
        "wonderingly into your face.\n");
  eaten = 1;
  call_out("dest_me", 2);
  return 1;
} /* do_eat() */

int attack_by(object ob) {
  ob->stop_fight(this_object());
  tell_object(ob, "The cabbage is a terrible pasifist.\n");
} /* attack_by() */

int attack_ob(object ob) {
  ob->stop_fight(this_object());
  tell_object(ob, "The cabbage is a terrible pasifist.\n");
} /* attack_by() */

event_exit(object ob) {
  if (!stringp(my_owner)) return ;
  if (ob == find_player(my_owner)) {
    call_out("check_env", 0);
  }
} /* event_exit() */


void event_dest_me(object ob) {
  call_out("check_env", 0);
} /* check_env() */

void check_env() {
  if (!stringp(my_owner) || !find_player(my_owner)) {
    command("cry");
    eaten = 1;
    call_out("dest_me", 2);
    return ;
  }
  if (environment() != environment(find_player(my_owner))) {
    move_player("X", environment(find_player(my_owner)));
    command("follow "+my_owner);
  }
} /* check_env() */

void dest_me() {
  object ob;

  if (previous_object() != this_object() && !eaten && environment()) {
    ob = clone_object(CHAR+"cabbage_follower");
    ob->move(environment());
    ob->set_owner(my_owner);
 }
  ::dest_me();
} /* dest_me() */

string query_owner() { return my_owner; }
