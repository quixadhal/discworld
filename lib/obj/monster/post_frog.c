inherit "/obj/monster";
#include "mail.h"

string from, to;

void setup() {
  set_name("frog");
  set_long("A small green frog with a large brightly coloured post office "+
           "hat.  He has a small bag slung over his shoulder in which are "+
           "stuffed literaly hundreds of letters.  He has a broad smile "+
           "on his face and obviously enjoys his job.\n");
  add_adjective(({ "small", "green", "post", "office" }));
/* Yes, its a demon.  Ok?  Got a problem with that? */
  add_property("demon", 1);
} /* setup() */

/* Who says you can kill my nice demon? */
void adjust_hp(int hp, object at) {
} /* adjust_hp() */

void got_player(int bing) {
  if (!find_player(to)) {
/* Oh no!  Woe is me! */
    command("'I almost had him!");
    init_command("sigh");
    call_out("go_home", 2);
    return ;
  }
  if (!bing) {
/* Froodle buns */
    move_player("X", environment(find_player(to)));
  }
  command("point "+to);
  command("whisper You have been sent a message by "+from+", go to the post office "+
          "to read it. to "+to);
  init_command("'Cheerio!");
  init_command("wave");
  call_out("go_home", 5);
} /* got_player() */

void send_mail(string t, string f) {
  to = t;
  from = f;
/* Ok, now we try and find our pooooor little player.  pat pat */
  if (!find_player(t)) {
    call_out("go_home",1);
    return ;
  }
  add_property("goto player", t);
  add_triggered_action("player", "goto_player", this_object(),
                       "got_player");
} /* setup_tell() */

/*
 * Now...  where the heck should I send them?
 */
void go_home() {
  add_property("goto destination", FROG_HOME);
  add_triggered_action("dest", "goto_destination", this_object(),
                       "got_home");
} /* go_home() */

void got_home(int bing) {
  if (!bing)
    move_player("X", FROG_HOME);
  command("'Wimble!\n");
  tell_room(short(0)+" fades into the crowd of other frogs.\n");
  dest_me();
  return ;
} /* got_home() */

void go_away() {
  command("wave");
  tell_room(environment(), short(0)+" hops silently and dramaticaly off into "+
                           "the distance.\n");
  dest_me();
} /* go_away() */
