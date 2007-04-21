/**
 * This is the main include file for rooms.  It contains all the defines
 * needed by vairous bits of rooms.
 * @author Pinkfish
 */
#ifndef __SYS__ROOM
/** @ignore yes */
#define __SYS__ROOM

/**
 * This is the room handler objects path.
 */
#define ROOM_HANDLER "/obj/handlers/room_handler"

#define ROOM_DEST       0
#define ROOM_EXIT       1
#define ROOM_OBV        2
#define ROOM_SIZE       3
#define ROOM_FUNC       4
#define ROOM_ENTER      5
#define ROOM_REL        6
#define ROOM_LOOK       7
#define ROOM_LOOK_FUNC  8
#define ROOM_GRADE      9
#define ROOM_MESS      10
#define ROOM_LINK_MESS 11
#define ROOM_NO_MAP    12
#define ROOM_DELTA     13
#define ROOM_NPC_STOP  14
/** This is used for extra messages on a move */
#define ROOM_EXTRA_MOVE_MESS 15

/**
 * This is the path used to the object used for doors.
 */
#define DOOR_OBJECT "/std/room/basic/door"
/**
 * This is the path used to the object used for items.
 */
#define ITEM_OBJECT "/std/room/basic/item"
/**
 * This is the path used to the object used to set up room chats.
 */
#define CHATTER_OBJECT "/std/room/basic/chatter"
/**
 * This is the path used to the object used to set up room situations.
 */
#define SITUATION_CHANGER_OBJECT  "/std/room/basic/situation_changer"
/**
 * This is the path used to the object used to link connected rooms
 * together.
 */
#define LINKER_OBJECT "/std/room/basic/linker"
/**
 * This is the path to the object used by the terain handler.
 */
#define TERRAIN_OBJECT "/std/room/basic/terrain"
/**
 * This is the path used to the object use for walls.
 */
#define WALL_OBJECT "/std/room/basic/wall"

/**
 * This is the path to the void.  Used as an error return in a few
 * function calls.
 */
#define ROOM_VOID "/room/void"

/**
 * This is the 'halflife' of magic in a room, it's used for calculating
 * the enchantment level
 */
#define ENCHANT_HALF 3600
/**
 * This is the property used for the keep room loaded attribute.
 * We use a property to reduce the memory usage.
 */
#define ROOM_KEEP_PROP "room keep"

/**
 * This is the property we use to keep to stop the room being replaced with
 * replace_program().
 */
#define ROOM_NOT_REPLACE_PROGRAM_PROP "replace program"

/**
 * This is used to control how long it must be since a player visited
 * this room in order for it to clean_up() and dest itself.
 */
#define ROOM_IDLE_TIMEOUT 900

/**
 * This is the directory to make all the help files relative to.
 */
#define ROOM_HELP_FILE_DIR "/doc/room/"
/**
 * This is the property used to keep track of the help file.
 */
#define ROOM_HELP_FILE_PROPERTY "help file name"

/**
 * This is index for day items/chats.
 */
#define ROOM_DAY_INDEX 1
/**
 * This is the index for night item/chats.
 */
#define ROOM_NIGHT_INDEX 0
/**
 * THis is the index for default item/chats.
 */
#define ROOM_DEFAULT_INDEX 2

#endif /* __SYS__ROOM */
