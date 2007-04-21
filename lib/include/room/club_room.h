/**
 * This it the include file for club rooms.  It contains all the useful stuff
 * that makes club rooms thatm uch more fun.
 * @author Pinkfish
 * @started June 15th 1999
 * @see /std/room/club_room.c
 * @see /std/room/controller/club_room_controller.c
 */
#ifndef __ROOM_CLUB_ROOM_H
/** @ignore yes */
#define __ROOM_CLUB_ROOM_H

/**
 * This define sets the owned item to be controlled by discussion items.
 * @see /std/room/club_room->add_controlled_ownership()
 */
#define CLUB_ROOM_CONTROLLER_DISCUSSED 0
/**
 * This define sets the owned item to be fixed in its control.  So that
 * the default owner is the person who really owns it all the time.
 * @see /std/room/controller/club_room_controller->add_controlled_ownership()
 */
#define CLUB_ROOM_CONTROLLED_FIXED     1

/**
 * This means that the controlled item is owned by a person.
 */
#define CLUB_ROOM_OWNED_PERSON   0
/**
 * THis means that the controlled item is owned by a position.  A
 * position is something like 'president' or whatever.
 */
#define CLUB_ROOM_OWNED_POSITION 1

/**
 * This is the option stored for the club on the club controller to keep
 * track of how many boards a club has.  This allows the board to be
 * removed when the club runs out of boards.
 * @see /std/room/controller/club_room_controller.c
 */
#define CLUB_ROOM_BOARD_COUNT "board count"

/**
 * This is the bit of string put at the start of the name to check to see if
 * the marked position is a position or a person.
 * @see /std/room/controller/club_room_controller.c
 */
#define CLUB_ROOM_POSITION_START "position "

/**
 * This is the name of the main club rooms save file used by the club room
 * controller.
 */
#define CLUB_ROOM_CONTROLLER_SAVE_FILE "club_controller"

#endif
