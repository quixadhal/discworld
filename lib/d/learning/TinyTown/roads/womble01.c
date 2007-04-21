#include "path.h"

inherit "/std/room/outside";

void setup() {
  add_property( "determinate", "the " );
  set_short( "southwestern corner of Womble square" );
  set_long("This is the western edge of Tiny Town, a small "
           "town built as an example for new creators who might want to see "
           "how to make rooms, npc's and such without the complexity of "
           "the \"real\" domains.\n");
   
  set_zone( "Tiny Town");

  // This links the other square rooms to this one so people can see
  // movement across the square.
  set_linker(({ PATH +"womble02", PATH +"womble03", PATH +"womble04", }),
             "into", "", "Womble Square" );
  
  add_exit( "north", PATH +"womble03", "road" );  
  add_exit( "northeast", PATH +"womble04", "hidden" );
  add_exit( "east", PATH +"womble02", "road" );
  add_exit( "south", TTOWNBUILDINGS +"item-shop", "door" );

  modify_exit( "south", ({
    "exit mess", "$N leave$s through the door to the south.",
      "enter mess", ({ 
        1,
          "Chimes start playing as $N enters.",
          "Chimes start playing as $N enter." }),
      "move mess", "Chimes start playing as you enter the shop.\n"
      }));

  // Add a climable wall up to a ledge outside the window of the shop.
  // parameters are: direction, difficulty, destination, message
  set_wall( ({ "move", ({ "up", 100, TTOWNBUILDINGS + "shop-ledge",
                            "$N climb$s up from below." }) }) );
} /* setup() */

