.DT
room.h
Disk World autodoc help
room.h

.SH Description
.SP 5 5

This is the main include file for rooms.  It contains all the defines
needed by vairous bits of rooms.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Defines
.SI 3
* %^BOLD%^CHATTER_OBJECT%^RESET%^
.EI
.SP 7 5

This is the path used to the object used to set up room chats.

.EP

.SI 3
* %^BOLD%^DOOR_OBJECT%^RESET%^
.EI
.SP 7 5

This is the path used to the object used for doors.

.EP

.SI 3
* %^BOLD%^ENCHANT_HALF%^RESET%^
.EI
.SP 7 5

This is the 'halflife' of magic in a room, it's used for calculating
the enchantment level

.EP

.SI 3
* %^BOLD%^ITEM_OBJECT%^RESET%^
.EI
.SP 7 5

This is the path used to the object used for items.

.EP

.SI 3
* %^BOLD%^LINKER_OBJECT%^RESET%^
.EI
.SP 7 5

This is the path used to the object used to link connected rooms
together.

.EP

.SI 3
* %^BOLD%^ROOM_DAY_INDEX%^RESET%^
.EI
.SP 7 5

This is index for day items/chats.

.EP

.SI 3
* %^BOLD%^ROOM_DEFAULT_INDEX%^RESET%^
.EI
.SP 7 5

THis is the index for default item/chats.

.EP

.SI 3
* %^BOLD%^ROOM_DELTA%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_DEST%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_ENTER%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_EXIT%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_EXTRA_MOVE_MESS%^RESET%^
.EI
.SP 7 5
This is used for extra messages on a move 
.EP

.SI 3
* %^BOLD%^ROOM_FUNC%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_GRADE%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_HANDLER%^RESET%^
.EI
.SP 7 5

This is the room handler objects path.

.EP

.SI 3
* %^BOLD%^ROOM_HELP_FILE_DIR%^RESET%^
.EI
.SP 7 5

This is the directory to make all the help files relative to.

.EP

.SI 3
* %^BOLD%^ROOM_HELP_FILE_PROPERTY%^RESET%^
.EI
.SP 7 5

This is the property used to keep track of the help file.

.EP

.SI 3
* %^BOLD%^ROOM_IDLE_TIMEOUT%^RESET%^
.EI
.SP 7 5

This is used to control how long it must be since a player visited
this room in order for it to clean_up() and dest itself.

.EP

.SI 3
* %^BOLD%^ROOM_KEEP_PROP%^RESET%^
.EI
.SP 7 5

This is the property used for the keep room loaded attribute.
We use a property to reduce the memory usage.

.EP

.SI 3
* %^BOLD%^ROOM_LINK_MESS%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_LOOK%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_LOOK_FUNC%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_MESS%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_NIGHT_INDEX%^RESET%^
.EI
.SP 7 5

This is the index for night item/chats.

.EP

.SI 3
* %^BOLD%^ROOM_NO_MAP%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_NOT_REPLACE_PROGRAM_PROP%^RESET%^
.EI
.SP 7 5

This is the property we use to keep to stop the room being replaced with
replace_program().

.EP

.SI 3
* %^BOLD%^ROOM_NPC_STOP%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_OBV%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_REL%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_SIZE%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOM_VOID%^RESET%^
.EI
.SP 7 5

This is the path to the void.  Used as an error return in a few
function calls.

.EP

.SI 3
* %^BOLD%^SITUATION_CHANGER_OBJECT%^RESET%^
.EI
.SP 7 5

This is the path used to the object used to set up room situations.

.EP

.SI 3
* %^BOLD%^TERRAIN_OBJECT%^RESET%^
.EI
.SP 7 5

This is the path to the object used by the terain handler.

.EP

.SI 3
* %^BOLD%^WALL_OBJECT%^RESET%^
.EI
.SP 7 5

This is the path used to the object use for walls.

.EP

