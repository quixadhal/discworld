/**
 * The newspaper office room.
 * @author Obilic
 * @changed Pinkfish Fri Apr 27 12:08:29 PDT 2001
 * Turned into an inherit and made to use a newspaper handler for
 * distributing the actual text.
 */
inherit "/std/room/inherit/newspaper_office";
inherit "/std/room/basic_room";

void create() {
   newspaper_office::create();
   basic_room::create();
   add_help_file("newspaper_office");
} /* create() */

void init() {
   basic_room::init();
   newspaper_office::init();
} /* init() */

