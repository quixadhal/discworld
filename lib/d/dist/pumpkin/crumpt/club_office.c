#include "path.h"
#include <clubs.h>

inherit "/std/room/club_control_room";

void setup() {
   set_light(70);
   set_club_control_type(CLUB_ROOM_CLUB_ONLY);
   set_short("Club control room");
   set_long("This is a small cosy room with a nice comradely atmosphere "
            "about it.  There is a nice sofa and a desk with little club "
            "ornaments all over it.\n");

   add_item("sofa", ({ "long",
                      "The sofa looks nice and comfortable, just the spot "
                      "after a hard days hacking people into small bits.\n",
               "position", "the comfy sofa" }));
   add_item("desk",
            ({ "long",
                   "The brown leather covered desk looks really nice "
                   "and comforable sitting in the middle of the room like "
                   "that.  It is covered in small club shaped ornaments.\n",
               "position", "the large black desk" }));
   add_item("club ordanment",
            "The desk is covered in them, 'Real genuine minature babarian "
            "clubs'.  Looks like someone has a club fetish.\n");

   set_not_replaceable(1);

   clone_object("/obj/misc/top_clubs")->move(this_object());

   add_exit("south", PATH + "crumpt6", "door");

   add_property("place", "Pumpkin");
} /* setup() */
