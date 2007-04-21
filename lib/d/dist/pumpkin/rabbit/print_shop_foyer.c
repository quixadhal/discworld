#include "path.h"

inherit "/std/room/basic_room";

void setup() {
   set_light(70);
   set_room_size(5);
   set_short("print shop's foyer");
   add_property("determinate", "the ");
   set_long(
"A busy room full of people running around carrying trays full of moving "
"type and ink.  There is a coffee table in the middle of the chaos with "
"a chair sitting beside it.  To the west there is a door with a "
"brass plaque attached to it.\n");
   add_item("coffee table",
({ "long", "The coffee table is old and very coffee and tea stained, you are "
"not sure just how undisturbed you would be sitting at the table, the whole "
"place looks a little busy.",
"position", "the coffee table" }) );
   add_item("rickety chair",
({ "long", "The rickety chair sits beside the coffee table, it looks "
"pretty precarious about its role in life.  Sitting on it might be a "
"dangerous occupation.",
"position", "the rickety chair" }) );
   add_item("moving type",
"Lots of little letters placed in rows on boards, or heaped in buckets.  "
"They don't actually appear to move, but is hard to tell because no one "
"with them ever stops long enough for you to look.");
   add_item("ink",
"Large pots of ink, small pots of ink, ink!  All colours of ink, black, "
"red, green, although mostly black, are being carted around.  You are sure "
"there is a purpose to all the movement even if it is a little unclear.");
   add_item("people",
"They are all in a hurry, they look determined and stressed as if they "
"could never manage to get to whatever their destination is fast enough.  "
"Most of them do not appear to really know where their destination is, "
"they rush in and out of the room carrying the same items, look around "
"wildly and dash out again.");
   add_item("brass plaque",
({ "long", "It is a nice polished brass plaque on a surprisingly nice "
"looking door.\nIt appears to have something written on it.",
"read", "Office.\n" }) );

   add_exit("west", PATH + "print_shop_office", "door");
   add_exit("east", PATH + "print_shop_press", "corridor");
   add_exit("south", PATH + "rabbit3", "door");

   room_chat( ({ 120, 240, ({
       "Someone rushes into the room, looks around wildly and rushes back "
       "out again.",
         "You hear a CLANK, WHAM, HISS from the east",
    }) }) );

   add_property("place", "Pumpkin");
} /* setup() */
