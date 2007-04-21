/**
 * A Room Rental room inheritable
 * that include the /std/room/basic_room
 * inheritable
 *
 * @author goldenthread
 * @started 14 Jul y2k2
 *
 */

#include <room/room_rental.h>

inherit "/std/room/basic_room";
inherit ROOM_RENTAL_INHERIT_ROOM;

/** @ignore yes */
void create() {
   basic_room::create();
} /* create() */

/** @ignore yes */
void init() {
   basic_room::init();
} /* init() */

