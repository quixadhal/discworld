#include "snowball.h"
inherit "std/object";

int strength;

void set_strength(int arg) { strength = arg; }
int query_strength() { return strength; }

void setup() {
  set_name("snowball");
  add_alias("ball");
  add_adjective("snow");
  set_long("A small hastily made snowball, just ready for throwing at"+
           " someone.\n");
  set_strength(99);
  set_heart_beat(1);
}

string long(string str, int dark) {
  return ({ "A small soggy remains of a snowball.\n", /* 0-9 */
            "A small lump of well, it was probably snow.\n", /* 10-19 */
            "A small snowball.\n", /* 20-29 */
            "A smallish snowball.\n", /* 30-39 */
            "A bigger than small but smaller than medium snowball.\n", /* 40-49 */
            "A medium snowball.\n", /* 50-59 */
            "A just reasonable snowball.\n", /* 60-69 */
            "A snowball for throwing.\n", /* 70-79 */
            "A good sized snowball.\n", /* 80-89 */
            "A nice big firm snowball, just right to throw at someone.\n"})[strength/10];/* 90-99 */
}

void init() {
  this_player()->add_command("throw", this_object(), "%D %p %I");
  this_player()->add_command("frog", this_object(), "%D %p %I");
}

void heart_beat() {
  strength--;
  if (strength<=0) {
    set_heart_beat(0);
    dest_me();
  }
}

int do_throw(object *obs) {
  object ob;

  if (!sizeof(obs))
    return 0;
  if (!(ob = present(SNOW_THING, obs[0]))) {
    ob = clone_object(SNOW_EXTRA);
    if (ob->move(obs[0]))
      ob->dest_me();
  }
  if (ob)
    ob->snowball(strength);
  this_player()->add_succeeded(obs[0]);
  tell_object(obs[0],this_player()->query_cap_name()+
                " hits you with a snowball!\n");
  call_out("dest_me",0);
  move("/room/void");
  return 1;
}

int do_frog(object *obs) {
  return do_throw(obs);
}
