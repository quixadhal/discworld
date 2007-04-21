/**
 * This is the include file for the room rental handler.
 * @author Goldenthread
 */
#ifndef __ROOM_RENTAL_H
/** @ignore yes */
#define __ROOM_RENTAL_H

/** The path of the handler. */
#define ROOM_RENTAL_HANDLER               "/obj/handlers/room_rental"
/** The path of the save file. */
#define ROOM_RENTAL_SAVE_FILE             "/save/rental_save_file"

/** The inherit for the rental rooms. */
#define ROOM_RENTAL_INHERIT_ROOM          "/std/room/inherit/room_rental"
/** The inherit for the rental office. */
#define ROOM_RENTAL_INHERIT_OFFICE        "/std/room/inherit/room_rental_office"

/** The inherit for the rental rooms ( including /std/room/basic_room ). */
#define ROOM_RENTAL_INHERIT_BASIC_ROOM    "/std/room/room_rental"
/** The inherit for the rental office ( including /std/room/basic_room ). */
#define ROOM_RENTAL_INHERIT_BASIC_OFFICE  "/std/room/room_rental_office"

/** Means the rental was successful. */
#define RENTAL_SUCCESS         0
/** Couldn't find the room to rent. */
#define RENTAL_NO_SUCH_ROOM    1
/** The room was already rented to someone else. */
#define RENTAL_ALREADY_RENTED  2
/** The room is available. */
#define RENTAL_AVAILABLE       3
/** The person trying to rent does not have enough money */
#define RENTAL_NO_MONEY        4
/** One day in seconds. (60 * 60 * 24) */
#define ONE_DAY                86400

#endif
