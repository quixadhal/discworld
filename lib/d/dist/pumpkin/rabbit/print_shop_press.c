#include "path.h"

inherit "/std/room/basic_room";

void setup() {
   set_light(70);
   set_room_size(5);
   set_short("print shop's machinery room");
   add_property("determinate", "the ");
   set_long(
"Big huge black machines, belching steam and exciting noises, completely cover "
"the floor of the room.  People scurry back and forth, carrying single "
"pages, bound books and trays of moving type.  The floor is covered in "
"spilled ink, both new and old, and little bits of metal, forgotten "
"bits of moving type and the odd rat.\n");

   add_item(({ "big huge black machine", "press", "machines" }),
"The machines are big presses that squish the moving type down onto "
"bits of paper which are then shuffled off to the binding department.  Steam "
"and unnecessary noise appear to be vital components of the printing "
"process.");
   add_item(({ "metal bit", "metal" }),
"There are a number of small bits of metal scattered over the floor, they "
"appear to be broken bits of moving type and things shaved off the "
"industriously working machines.");
   add_item("floor",
"The floor is covered in spilled ink, both new and old, and little bits of "
"metal,  forgotten bits of moving type and the odd rat.");
   add_item("people",
"There are people scurrying around in a hurry, moving pages from the "
"press into the binding room and swapping the moving type.  In a corner "
"there are a group of people arranging the type onto the trays from an "
"original book.");
   add_item("bound book",
"You have no idea why people would be walking around with bound books here, "
"especially since they seem to be reading the books and then blushing bright "
"red.  The title of the books appears to be 'The Joye of Snacks'.");
   add_item("spilled ink",
"The ink is all over the floor in a thick layer, caking everything in a "
"fine dust.");
   add_item("rat",
"The rat looks kind of, well very, covered in ink.  It sniffs around the "
"place industriously, perhaps it is a spy rat?");
   add_item("single page",
"The pages are single pages from books, they are not particularly ordered "
"from what you can tell.  You wonder how the books make it out alive.");
   add_item(({ "tray", "tray of moving type" }),
"Trays of moveable type to be inserted into the machines, they then print "
"the pages which get placed into the final book.");

   add_exit("west", PATH + "print_shop_foyer", "corridor");
   add_exit("north", PATH + "print_shop_binding", "corridor");

   room_chat( ({ 120, 240, ({
      "Someone reading a book blushes bright red and drops a tray of "
      "moving type.",
      "WHUMP!",
      "HISS!",
      "A machine clanks alarmingly.",
      "Someone wanders off with a bunch of pages to be bound to the binding "
      "room."
    }) }) );

   add_property("place", "Pumpkin");
} /* setup() */
