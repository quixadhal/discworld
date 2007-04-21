/**
 * A Room Rental Office inheritable
 * that includes the /std/room/basic_room
 * inheritable
 * 
 * by goldenthread started 14 Jul y2k2
 *
 */

#include <room/room_rental.h>

inherit "/std/room/basic_room";
inherit ROOM_RENTAL_INHERIT_OFFICE;

/** @ignore yes */
void create() {
   basic_room::create();
   room_rental_office::create();
} /* create() */

/** @ignore yes */
void init() {
   basic_room::init();
   room_rental_office::init();
} /* init() */
