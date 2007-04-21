/* -*- LPC -*- */
/**
 * This inherit allows people to create clubs and do fluffy club stuff if
 * they need to.
 * @author Arienne
 * @started Tue Feb 27 01:06:55 GMT 2001
 */
inherit "/std/room/basic_room";
inherit "/std/room/inherit/club_control_room";

void create() {
    do_setup++;
    basic_room::create();
    club_control_room::create();
    do_setup--;

    if ( !do_setup ) {
        this_object()->setup();
        this_object()->reset();
    }
} /* create() */

void init() {
    basic_room::init();
    club_control_room::init();
} /* init() */
