#include "mail.h"
inherit "/std/object";

void setup() {
  set_short("wedding ring");
  set_long("A single gold band ring with no adornation "+
           "apart from a small enscription on the bottom "+
           "unreadable to human eyes.\n");
  set_name("ring");
  add_adjective("wedding");
}

void init() {
  ::init();
  add_action("mail","mail");
  add_action("home","home");
  add_action("goto","goto",-1);
  add_action("do_place", "place");
}

int do_place(string str) {
  int i;
  object *obs;

  if (!str) {
    notify_fail("You have to place it on someones finger!\n");
    return 0;
  }
  obs = find_match(str, environment(this_player()));
  for (i=0;i<sizeof(obs);i++)
    if (living(obs[i]) && interactive(obs[i])) {
      write("You place the ring on "+obs[i]->query_name()+"'s finger.\n");
      say(this_player()->query_cap_name()+" places the ring on "+
           obs[i]->query_name()+"'s finger.\n");
      this_object()->move(obs[i]);
      reset_drop();
      return 1;
    }
  return 0;
}

int query_auto_load() { return 1; }

void init_arg(mixed *bing) {
  reset_drop();
}

int goto(string str) {
  string bing;

  notify_fail("Syntax: goto <name>\n");
  if (!str)
    return 0;
  if (!find_living(str)) {
    write("Cannot find "+str+".\n");
    return 0;
  }
  bing = (string)this_player()->query_cap_name();
  this_player()->move(environment(find_living(str)),
                 bing+" appears sneakily.\n", bing+" dissapears slightly.\n");
  write("Ok.\n");
  return 1;
}

int home() {
  write("There's no place like home.  There's no place like home.  "+
        "You tap your feet and vanish.\n");
  tell_room(environment(this_player()), this_player()->query_cap_name()+
       " taps their feet together three times "+
       "and vanishes.\n", ({ this_player() }) );
  this_player()->move("/w/pinkfish/newroom");
  tell_room(environment(this_player()), this_player()->query_cap_name()+
       " appears slowly.\n", ({ this_player() }) );
  return 1;
}

int mail(string str) {
  MAIL_TRACK->mail(str);
  return 1;
}
