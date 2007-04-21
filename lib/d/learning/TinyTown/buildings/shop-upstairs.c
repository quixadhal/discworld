#include "path.h"

inherit "/std/room/basic_room";

void setup() {
  set_short( "bedroom" );
  set_long( "This is a small bedroom above the shop.  There is a window "
            "in the north wall.\n");
  
  set_light( 60 );

  // This is usually calculated for you and only needed if you want the room
  // to be bigger than normal.
  set_room_size( 10 ); 

  // set the movement zone.
  set_zone( "Tiny Town");

  /*
   * lots of add_items are needed here to describe all the things in the
   * room.
   */


  add_exit("window", PATH+"shop-ledge", "window");
  add_exit("down", PATH +"item-shop", "stair" );

  // make them move downwards 9feet when they go down the stairs
  modify_exit("down", ({ "downgrade", 9 }));
}

