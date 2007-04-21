#include "path.h"

inherit "/std/room/rooftop";

void setup() {
  set_short( "small ledge" );
  set_long( "This is a small ledge just outside the upstairs window of "
						"the item shop.\n");

  // This is usually calculated for you and only needed if you want the room
  // to be bigger than normal.
  set_room_size( 10 ); 

	// Set the slope of this ledge. Parameters: gradient, name, destination
	set_slope(60, "ledge", TTOWNROADS + "womble01");

	// Set the messages to be shown if a player falls off the ledge due
	// to excessive weight.
	set_weak_messages("The ledge crumbles under your weight.\n",
										"$short$ falls from the crumbling ledge.",
										"$short$ comes crashing to the ground.",
                                                                                "The ledge makes a strange sound.");
	
	// make the ledge weak and crumbly. Parameters: weight, destination
	set_weak_roof(2500, TTOWNROADS + "womble01");

	/*
	 * lots of add_items are needed here to describe all the things in the
	 * room.
	 */


	add_exit("window", PATH+"shop-upstairs", "window");

	/*
	 * This sets up a wall exit so that people can climb down from here.
	 */
	// set the bottom of the wall, and a modifier to the damage taken
	// if you fall.
	set_wall( ({ "bottom", ({ TTOWNROADS + "womble01", 200 }), }) );

	/*
	 * set the parameters for a climb they are:
	 * direction, base difficulty, destination, message
	 */
	set_wall( ({ "move", ({ "down", 30, TTOWNROADS + "womble01",
														"$N climb$s down from above." }) }) );
}

