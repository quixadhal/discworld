inherit "/obj/monster";
#include "path.h"

void setup() {
  add_property("demon", 1);
  set_level(-10);
  set_name("cabbage");
  set_gender(1+random(2));
  set_long(
"A large strapping looking cabbage with large hulking cabbages leaves.  "+
capitalize(query_pronoun())+" is a wandering cabbage.\n");
/*
  add_move_zone("ankh morpork");
  add_move_zone("pub");
 */
  set_move_after(10, 10);
  load_chat(400, ({
1, "'I'm off to see the cabbage, the wonderful cabbage of oz",
1, "'A rolling cabbage gathers no penguins",
1, "'A cabbage a day keeps a Penguin at bay",
1, "'How many gardens must a cabbage lie down in before he can be called a cabbage?",
1, "'Remember your cabbage and have a good weekend",
1, "'Have you hugged your cabbage today?",
1, "'Cabbages are a mans best friend",
1, "'Cabbages are a womens best friend",
1, "'Look at that cabbage!  Wow, what a frond.",
      }));
} /* setup() */

void init() {
  ::init();
  if (interactive(this_player())) {
    if (!sizeof(find_match("cabbage", this_player()))) {
      call_out("give_cabbage", 0);
    }
  }
} /* init() */

void give_cabbage() {
  object ob;

  if (!this_player() || environment(this_player()) != environment())
    return ;
  if (sizeof(find_match("cabbage", this_player())))
    return ;
  ob = clone_object("/global/special/cabbage");
  write("The cabbage swells up a lot, and suddenly a small cabbage "+
        "darts out from the folds of his monstrous skin and "+
        "starts to follow you.\n");
  tell_room(environment(),
        "The cabbage swells up a lot, and suddenly a small cabbage "+
        "darts out from the folds of his monstrous skin and "+
        "starts to follow "+this_player()->query_cap_name()+".\n",
        ({ this_player() }));
  ob->move(this_player());
  ob = clone_object(CHAR+"cabbage_follower");
  ob->move(environment());
  ob->set_owner(this_player()->query_name());
  ob->do_command("follow "+this_player()->query_name());
} /* give_cabbage() */

int attack_by(object ob) {
  ob->stop_fight(this_object());
  tell_object(ob, "The cabbage is a terrible pasifist.\n");
} /* attack_by() */

int attack_ob(object ob) {
  ob->stop_fight(this_object());
  tell_object(ob, "The cabbage is a terrible pasifist.\n");
} /* attack_by() */

void dest_me() {
  object ob;

  if (previous_object() != this_object() && environment()) {
    ob = clone_object(CHAR+"wandering_cabbage");
    ob->move(environment());
  }
  ::dest_me();
} /* dest_me() */
