/**
 * This file is the inheritable for a meeting room.  It handles the floor
 * and a few other things neatly.
 * @author Pinkfish 
 * @started 25th of June
 */
inherit "/std/room/basic_room";
inherit "/std/room/inherit/meeting";

/** @ignore yes */
void create() {
   add_help_file("meeting");
   basic_room::create();
   meeting::create();
} /* create() */

/** @ignore yes */
void init() {
   basic_room::init();
   meeting::init();
} /* init() */
