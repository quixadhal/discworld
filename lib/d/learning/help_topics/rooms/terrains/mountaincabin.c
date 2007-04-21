/* -*- LPC -*- */
#include "path.h"

inherit "/std/room";

void setup() {
   set_terrain( "tutorial_mountain" );
   set_short( "rustic mountain cabin" );
   set_long( "This is the ground floor of a comfortable mountain cabin, "
      "complete with walls made of chinked logs, a bearskin rug, "
      "a huge, comfy sofa, and a bare lumber staircase leading "
      "up and down.  Unfortunately, since someone forgot to put a "
      "front door on this place, it's damned cold in here.\n" );
   add_item( ({ "log", "wall" }), 
      "Formed of whole logs, sanded to a satin "
      "finish, laid together, and with the gaps chinked with mud." );
   add_item( "bearskin rug", ({
      "long", "Looks like a large blackbear gave his life that you can be "
         "comfortable lying before the fire.",
      "position", "the rug" }), 0 );
   add_item( "comfy sofa", ({
      "long", "Not very sophisticated, but it sure looks comfortable.",
      "position", "the sofa" }), 0 );
   
   set_light( 50 );

   room_chat( ({ 100, 300,
      ({ "A chill wind blows some snow in through the open doorway.",
         "A sudden shiver runs down your spine.",
         "Maybe you should have packed some cold weather gear?",
         "Oh, a mug of hot cocoa would about hit the spot right now."
      }) }) );

   add_exit( "down", PATH "foyer", "stair" );
} /* setup() */

string query_exit_type( string direc, string dest) {
   if (direc == "up" || direc == "down")
      return "stair";
   else if (direc == "west")
      return "corridor";
   else
      return "none";
} /* query_exit_type() */
