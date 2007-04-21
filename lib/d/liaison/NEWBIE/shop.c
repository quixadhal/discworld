#include "path.h"

inherit "/std/shops/general_shop";
inherit PATH+"common";

void setup() {
  
  set_zone("Newbie");
  set_light( 75 );
  add_property("no teleport", 1);
  add_property("place", "Newbie Area");
  
  set_short("Commerce room");
  set_room_size( 5 );
  set_long( "This room is dedicated to trade and commerce.  There is a "
            "shiny counter, behind which is a shopkeeper.\n"
            "Type 'list' to get a list of the "
            "items, 'browse' to examine an item before buying it, "
            "'value' to value your items before you 'sell' them and, "
            "of course, 'buy' to buy an object from the shop.\n"
            "There is a brightly coloured sign here marked 'commerce'.\n");
  
  add_item( "shiny counter", "That's a display counter for the wares of the "+
            "shop.  Use the command 'list' to get a list of them." );
  add_item( ({ "object", "item", "ware" }), "Use 'list' to get a list of the "+
  "items for sale here." );
  add_item("shopkeeper", "The shopkeeper is a small man apparently in his "
           "mid-thirties.  He seems to be ready to serve you.");
  
  add_sign("a brightly coloured sign marked 'commerce'.\n",
           "/d/liaison/NEWBIE/foyer"->format_sign(
           "On Discworld different regions use different types of money.  "
           "Type 'help currency' for a list of conversion rates.  Money is "
           "needed to buy equipment, to learn at your guild and for a "
           "variety of other purposes.\n\n"
           "Making money can be difficult.  You can loot the corpses of "
           "your victims of course, but you can also scavenge money using "
           "the 'search' command and make money performing services for your "
           "fellow players.\n\n"
           "Spending money is the fun part.  This shop has instructions "
           "on how to use it.  Take a few minutes to buy yourself some "
           "equipment with the money you were given on login.\n"), 0,
           ({"commerce", "sign"}), "general" )->add_property("determinate","the ");
  add_exit( "foyer", PATH +"foyer", "door" );
  set_store_room( PATH +"store" );
  set_min_amount(1);
  set_max_inventory(200);
} /* setup() */

void init() {
  ::init();
  this_player()->add_command("speak", this_object(), "<string'language'>" );
}

int do_speak(string str) {
  write("Sorry, you cannot do that here.\n");
  return 1;
}
