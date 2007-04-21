#include "path.h"
#include <armoury.h>

inherit "/std/shops/print_shop";

nosave object goatberger;

int is_office_open(object *obs);

void setup() {
   set_light(70);
   set_room_size(5);
   set_short("print shop's office");
   add_property("determinate", "the ");
   set_long(
"A big impressive table dominates the room, contrasting sharply to the "
"bare undecorated walls and single bookcase on the back wall.  "
"The soft squooshy carpet covers the floor "
"in a soft cuddly way, licking up against the walls of the room.  "
"The room gives the impression of opulence by its starkness.\n");

   add_item("impressive table",
({ "long", "The table is almost bare except for a blotting pad and a nice "
"set of gold plated quills.  It is a deep shiny maroon colour "
"and the finish glistens softly in the light.  Nestled in behind the "
"table is a wonderful looking chair.",
"position", "the impressive table" }));
   add_item("chair",
({ "long", "The chair is one of those amazing office executive chairs.  "
"It has bits that wobble, bits that zip around and generally looks "
"pretty exciting.",
"position", "the chair" }) );
   add_item("gold plated quill",
"The gold plated quills rest on a quill holder on the table, slightly "
"askew from the blotting paper.  They look hardly used.");
   add_item("bare undecorated wall",
"The walls are bare and undecorated, but nicely finished with a "
"coat of some sort of timber varnish.\n");
   add_item( ({ "blotting paper", "blotting pad" }),
"The blotting paper is bright white, it looks like it has never seen "
"anything remotely resembling ink.  Scribbled just on the corner of "
"the wonderful white paper is a small note saying, 'Please note we can "
"\"print\" books and allow you to \"claim\" copyright here.'");
   add_item("quill holder",
"Resting on the hand engraved silver quill holder are two gold plated "
"quills.");
   add_item("soft squooshy carpet",
"The light brown carpet decorates the room perfectly, running from "
"wall to wall, gently fading into the floor and not being too obtrusive.");
   add_item("bookcase",
"It is sparsely populated with a couple of new looking leather bound "
"books.  They are titled 'Two nights in a closet', 'The Joye of Snacks' "
"and 'A few bedtime poems'.\n");
   add_item(({ "two nights in a closet", "book" }),
({ "long", "The red leather cover looks posh and gorgeous in your "
"hands.  You can see the title 'Two nights in a closet' etched "
"into the cover.\n"
"It appears to have something written on it.",
"read",
"This is a long winding book about a monster having to temporarily "
"relocate from a nice cellar in which it used to live.  The cellar "
"is undergoing refurbishment and is unavailable for monster "
"habitation, so it moved to a closet in a childs bedroom.  The "
"mother of the child refuses to believe that there is a monster "
"in the closet despite the loud protestations of the child.  The "
"monster peers out at the child every night, when it leaves it "
"eats the childs favorite toy and the mother eventually agrees "
"to look in the closet.  But it is empty.\n" }));
   add_item(({ "the joye of snacks", "book" }),
({ "long", "Wonderful bright red alluring cover with an interesting "
"picture of two people, naked, embracing in a shower with a "
"coffee table lurking in the foreground.  The caption 'Joye of "
"snacks' can be seen on the binder.\n"
"It appears to have something written on it.",
"read", "The first page has 'Demo copy only' written on it in "
"big black letters.  Sounds like a really interesting book though.\n" }) );
   add_item(({ "a few bedtime poems", "book" }),
({ "long", "The mate black cover of this book hardly even gleams at "
"all, the straight time Roman lettering saying 'A few bedtime poems "
"by a small colourful fish'.\n"
"It appears to have something written on it.",
"read",
"Strawberries\n"
"------------\n"
"\n"
"Starting from a above,\n"
"Working slowly down under.\n"
"Sliding up the sides\n"
"Eating a meal, fresh cream and syrup.\n"
"\n"
"Round and round, and round again\n"
"Grinning mischeviously\n"
"One tongue at play\n"
"Firm and hard, fresh strawberries today.\n"
"\n"
"\n"
"Flirting\n"
"--------\n"
"\n"
"A sidelong glance,\n"
"A funny remark!\n"
"Glittering eyes\n"
"Meeting in the dark.\n"
"\n"
"Fear of whats to come\n"
"Glad of what is here.\n"
"Hoping, wishing, believing\n"
"We will be as one.\n"
"\n"
"\n"
"Bewitched by the highlands\n"
"--------------------------\n"
 "\n"
"Scotland reminds me\n"
"Of heather and song\n"
"Of bagpipes and dancing\n"
"Never a witch\n"
 "\n"
"Then she bewitched me\n"
"Her eyes were brown\n"
"Her hair was like satin\n"
"Skin of pure honey\n"
 "\n"
"We talked for hours\n"
"Talking of the times\n"
"Strawberries and cream\n"
"Staring at her eyes\n"
 "\n"
"We laughed and sighed\n"
"Talked far and wide\n"
"Feeling a glow inside\n"
"Hands touching in my mind\n"
 "\n"
"Many more times we met\n"
"Laughed and had fun\n"
"Flirting with abandon\n"
"More happy than ever before\n"
 "\n"
"Now Scotland and witches\n"
"Are inextricably linked\n"
"With joy and happiness\n"
"In my dreams at night\n"
 }) );

   /* Make this just the office. */
   set_no_collection(1);
   set_open_func((: is_office_open($1) :));
   set_default_language("common");
   add_translation_cost("djelian", 130);
   set_save_dir("/d/am/save/printing/");

   add_exit("east", PATH + "print_shop_foyer", "door");

   add_property("place", "Pumpkin");
} /* setup() */

void reset() {
   if (!goatberger) {
      goatberger = clone_object("/obj/monster");
      goatberger->set_name("goatberger");
      goatberger->set_short("Mr. Goatberger");
      goatberger->set_long(
"He is smartly dressed in an expensive looking imported suit, a slight smile "
"just playing on his lips.  A gold fob watch peeks out of the lapels "
"of his suit.\n");
      goatberger->set_race("human");
      goatberger->set_level(10);
      goatberger->set_gender("male");
      goatberger->add_property("determinate", "");
      ARMOURY->request_item("white linen tunic", 90)->move(goatberger);
      ARMOURY->request_item("green pants", 90)->move(goatberger);
      goatberger->set_default_position( (: goatberger->do_command("sit on chair") :) );
      goatberger->do_command("equip");
      goatberger->move(this_object(), "$N arrives from the west.\n");
      goatberger->return_to_default_position(0);
   }
} /* reset() */

int is_office_open(object *obs) {
   if (!goatberger) {
      add_failed_mess("There is no one here to serve you.\n", obs);
      return 0;
   }
   return 1;
} /* is_office_open() */
