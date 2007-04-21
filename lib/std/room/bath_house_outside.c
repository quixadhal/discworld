/**
 * Outdoors bath house inheritable.
 * @author Taffyd
 * @started 3/05/00 9:55
 */

inherit "/std/room/outside";
inherit "/std/room/inherit/bath_house";

/** @ignore yes */
void create() {
    bath_house::create();
    outside::create();
} /* create() */

/** @ignore yes */
void init() {
    outside::init();
    bath_house::init();
} /* init() */
