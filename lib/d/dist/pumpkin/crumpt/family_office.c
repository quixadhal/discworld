#include "path.h"
#include <clubs.h>

inherit "/std/room/club_control_room";

int check_playtester(string str, object ob, string special_mess);

void setup() {
   object table;

   set_light(100);
   set_club_control_type(CLUB_ROOM_FAMILY_ONLY);
   set_short("Family control room");
   set_long("White boards cover the walls of the room, in the middle of "
            "the boards is a large black desk.  Scribblings and weird lines "
            "joining squiggly blobs to other squiggly blobs cover the white "
            "boards.\n");
   add_sign("A large imposing 'read me' type sign.\n",
            (: query_club_warning_message() :),
            "sign");

   add_item("white board", "They are everywhere, the drawings all look "
                   "somewhat meaningless, although perhaps they are "
                   "trying to convey some sort of family tree feeling.  "
                   "All the drawings look rather old, odd really.  "
                   "A singular lack of white board markers or erasers "
                   "could be the reason behind it.\n");
   add_item("white board eraser",
                   "There are none of them here, not one, not even...  No "
                   "none at all.\n");
   add_item("white board marker",
                   "Looking around the place, there is a singular lack of "
                   "white board markers.\n");
   add_item("large black desk",
            ({ "long",
                   "The desk looks imposing in its bulk and girth.  It "
                   "looks a bit unused, although there is a register "
                   "sitting on top of the desk which seems to contain the "
                   "list of all the current clubs in Ankh-Morpork.\n",
               "position", "the large black desk" }));
   add_item("squiggly blob",
                   "It appears to be joined to another squiggly blob.\n");
   add_item("weird line",
                   "This line darts across the board at an angle that "
                   "is hard to determine, you guess 16.123678 degrees.\n");
   add_item("scribblings",
                   "They are not recognisably words, they just look like they "
                   "could be words until you take a closer look.\n");
   set_not_replaceable(1);

   add_exit("south", PATH + "crumpt4", "door");

   table = clone_object("/obj/misc/top_families");
   table->move(this_object());

   add_property("place", "Pumpkin");
} /* setup() */
