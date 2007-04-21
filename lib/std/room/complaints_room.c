
/**
 * This inherit allows people to complain in unhappy times.
 *
 * @see /std/room/inherit/complaints_room.c
 * @author Arienne
 * @started Wed Apr  4 14:11:50 BST 2001
**/

inherit "/std/room/inherit/complaints_room";
inherit "/std/room/basic_room";

/**
 * @ignore
**/
void create() {
    add_help_file("complaints_room");
    do_setup++;
    basic_room::create();
    do_setup--;

    set_not_replaceable( 1 );

    if ( !do_setup ) {
        this_object()->setup();
        this_object()->reset();
    }
} /* create() */

/**
 * @ignore
**/
void init() {
    complaints_room::init();
    basic_room::init();
} /* init() */

