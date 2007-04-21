#include <config.h>
inherit "/std/room/basic_room";

object  button;

void setup() {
   set_short( "convention in the void" );
   add_property( "determinate", "the " );
   add_property( "no teleport", 1 );
   set_light( 100 );
   set_long("This is where you come when you fall out a room and have " 
      "nowhere to go.  You are inside a large igloo.  Penguin " 
      "waiters (in tuxedos of course) are serving drinks to the " 
      "assembled crowd.  A large wombat sits at the end of a " 
      "banquet table, his head resting peacefully in a bowl of " 
      "soup.\n");
   add_item( "igloo", "The igloo is nice and warm despite the icy decor." );
   add_item( ({ "penguin waiter", "penguin" }),
         "The penguins are all rather formally dressed." );
   add_item( "drink", "The drinks come in a variety of colours and smells." );
   add_item( "crowd", "The crowd consists mostly of iguanas." );
   add_item( "bowl", "The bowl holds the soup.  It is green and hums." );
   add_item( "soup", "The soup is pink and comfortable-looking." );
   add_item( "wombat", "Perhaps you should tell the wombat to use a spoon." );
   co_ord = 0;

   add_exit("out", CONFIG_START_LOCATION, "road");
} /* setup() */

void reset() {
   if ( !button ) {
      button = clone_object( "/obj/misc/button" );
      button->move( this_object() );
   }
} /* reset() */

int squish(string cmd) {
  if (cmd == "get" ||
      cmd == "drag" ||
      cmd == "palm" ||
      cmd == "slip" ||
      cmd == "drop" ||
      cmd == "bury" ||
      cmd == "decompose" ||
      cmd == "ensumpf" ||
      cmd == "envalise" || 
      cmd == "pyroscipate" ||
      cmd == "shroud" ||
      cmd == "ventisepelate") {
    write( "I don't think so.\n" );
    return 1;
  } else {
    return 0;
  }
}

void init() {
   ::init();
   co_ord = 0;
   if (!this_player() || this_player()->query_creator())
     return;
   this_player()->command_override((: squish :));
} /* init() */


int *query_co_ord() { return 0; }

void clean_up(int i) { return; }
