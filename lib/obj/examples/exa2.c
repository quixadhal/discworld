/* this is the second template room, introducing 'items'-- bil */

/* I will not discuss what has already been discussed...revise the older
 * rooms if you have problems.
 */
#include "path.h"

inherit "/std/room";

void setup()
{
  set_short("second simple room");

  set_long("This is the second example room. It introduces items, and " +
           "aliasing - again use 'more here' to look at it. Over by the " +
           "north wall you can see a table.\n");
  
  set_light(90); /* another brightly lit room */

/* >> Items <<
 * Items grew out of people moaning that they could see a 'table' in the
 * long description, yet 'examine table' declared there was no such thing!
 * So now they can. The first argument refers to the thing the player
 * must examine to see the second argument printed out.
 * eg you can 'examine table' to see "The table is...etc"
 * In general follow the guidelines for set_long(), except an item
 * should be much shorter, 1/2 to 2 lines...remember these are minor
 * things and we only have finite memory resourses.
 */
 add_item("table",
          "The table is one step away from being firewood. Upon it rests " +
          "a blotter.\n");
/* if you want more than one way to examine an item, use an array of
 * strings in the first argument - see 'help array' if this confuses you.
 * eg. 'examine north wall' and 'examine wall' both tell the player
 * "It is very...etc"
 */
 add_item( ({ "north wall", "wall" }),
          "It is very flat and is holding up rather a ricketty table.\n");
/* you can add items to things referenced in other items...clever huh!
 * but don't go mad - don't add a blots item unless they are *important*
 * otherwise it will get silly.
 */
 add_item("blotter",
          "It is covered in random ink blots.\n");

/* >> Exits <<
 * Nothing new here - back to the first example and onwards...
 */
  add_exit("west", ROOM + "exa1", "door");
  add_exit("east", ROOM + "exa3", "door");

  set_zone("examples");
}
