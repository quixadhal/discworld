#include "path.h"
#include <armoury.h>

inherit "/std/shops/print_shop";

int is_room_open();

void setup() {
   set_light(70);
   set_room_size(5);
   set_short("print shop's binding room");
   add_property("determinate", "the ");
   set_long(
"Several tables are laid out in this room, people sit at the tables "
"placing the pages which come in from the printing press into the "
"final bound editions of the books.  This looks like the place where "
"the completed books are stored until taken away by their owners, judging "
"by the huge piles of books scattered around on the floor.\n");

   add_item("table",
({ "long", "Several tables house the equipment used to bind the books into "
"completed volumes are scattered around the room.  All the space not used by "
"the tables is covered in piles of completed, bound books.",
"position", "a binding table",
"position multiple", 1 }) );
   add_item(({ "edition", "volume", }),
"The uncompleted, or soon to be completed volumes of work are sitting "
"on the tables, when they get put on the ground they suddenly turn "
"into books.");
   add_item("people",
"They are binding the books, placing each page into (hopefully) the "
"correct place and creating a final, probably exact output.");
   add_item("book",
({ "long", "There are books all over the floor, in piles and stacks.  Some "
"of the stacks look a little precarious.",
"position", "a pile of books",
"position multiple", 1 }) );
   add_item("page",
"The pages are stacked up on the tables, somewhat neatly, beside the "
"workers as they place them into the bound copies.");

   add_exit("south", PATH + "print_shop_press", "corridor");

   /* Things to do with the print shop. */
   set_office_room(PATH + "print_shop_office");
   set_open_func((: is_room_open :));
   set_print_shop_name("Goatbergers printing shop, Pumpkin");

   add_property("place", "Pumpkin");
} /* setup() */

int is_room_open() {
   return 1;
} /* is_room_open() */
