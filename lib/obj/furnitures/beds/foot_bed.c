/* A nice bed, with duck feet.
 * by Ptoink
 *  -01/02/2003
*/

#include <forn.h>

inherit "/std/room/furniture/basic";

void setup(){

  set_name("bed");
  set_short ("duck-footed bed");
  add_alias ("duck-footed");

  set_long(
    "A fairly solid timber bed, with intricate scrollwork around the "
    "headboard.  It is supported by four duck's feet, presumably made from "
    "painted wood.  The dark wood that form its sides is inset with short "
    "strips made from a lighter wood, creating an interestingly random "
    "pattern.\n");

  set_weight( 800 );
  set_value( 100000 );

  set_allowed_positions(({"sitting", "meditating", "lying", "standing", 
                          "kneeling", "crouching"}));
 
  set_allowed_room_verbs((["sitting" : "sits" ]));
}