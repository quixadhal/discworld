#include <command.h>

inherit "/std/room/basic_room";

#define BLOCKED_COMMANDS ({ "quit", "quit2", "save", "su", "godmother", "tell", "t", "shout", "remote", "newbie", "gs", "group say" })

int squish(string cmd);

void setup() {
  set_short( "jail" );
  add_property( "determinate", "the " );
  add_property("no teleport", 1);
  add_property("no godmother", 1);
  set_light( 100 );
  set_long("This is where you are put when you've been a naughty boy (or "
           "girl).  It's a very boring place with nothing to do.\n");
} /* setup() */

void clean_up(int i) { return; }

// prevents people doing things they shouldn't in here. It will screw up their
// start position and allow them to use this to trans to the drum.
void init() {
  ::init();
  if (!this_player()) return;
  this_player()->command_override((: squish :));
}

int squish(string cmd) {
  string *bits;

  if(this_player()->query_creator())
    return 0;
  
  bits = explode(cmd, " ");
  if(member_array(bits[0], BLOCKED_COMMANDS) != -1) {
    write( "I don't think so.\n" );
    return 1;
  } else {
    return 0;
  }
}
