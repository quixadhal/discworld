/**
 * Indoors bath house inheritable.
 * @author Taffyd
 * @started 3/05/00 9:55
 */

inherit "/std/room/basic_room";
inherit "/std/room/inherit/bath_house";

/** @ignore yes */
void create() {
    basic_room::create();
    bath_house::create();
} /* create() */

/** @ignore yes */
void init() {
    basic_room::init();
    bath_house::init();
} /* init() */
