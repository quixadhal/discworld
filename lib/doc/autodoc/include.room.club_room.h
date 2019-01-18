.DT
club_room.h
Disk World autodoc help
club_room.h

.SH Description
.SP 5 5

This it the include file for club rooms.  It contains all the useful stuff
that makes club rooms thatm uch more fun.
.EP
.SP 10 5


Written by Pinkfish

Started June 15th 1999
.EP



.SH See also
.SP 5 5
/std/room/club_room.c and /std/room/controller/club_room_controller.c

.EP
.SH Defines
.SI 3
* %^BOLD%^CLUB_ROOM_BOARD_COUNT%^RESET%^
.EI
.SP 7 5

This is the option stored for the club on the club controller to keep
track of how many boards a club has.  This allows the board to be
removed when the club runs out of boards.
.EP

.SI 3
* %^BOLD%^CLUB_ROOM_CONTROLLED_FIXED%^RESET%^
.EI
.SP 7 5

This define sets the owned item to be fixed in its control.  So that
the default owner is the person who really owns it all the time.
.EP

.SI 3
* %^BOLD%^CLUB_ROOM_CONTROLLER_DISCUSSED%^RESET%^
.EI
.SP 7 5

This define sets the owned item to be controlled by discussion items.
.EP

.SI 3
* %^BOLD%^CLUB_ROOM_CONTROLLER_SAVE_FILE%^RESET%^
.EI
.SP 7 5

This is the name of the main club rooms save file used by the club room
controller.

.EP

.SI 3
* %^BOLD%^CLUB_ROOM_OWNED_PERSON%^RESET%^
.EI
.SP 7 5

This means that the controlled item is owned by a person.

.EP

.SI 3
* %^BOLD%^CLUB_ROOM_OWNED_POSITION%^RESET%^
.EI
.SP 7 5

THis means that the controlled item is owned by a position.  A
position is something like 'president' or whatever.

.EP

.SI 3
* %^BOLD%^CLUB_ROOM_POSITION_START%^RESET%^
.EI
.SP 7 5

This is the bit of string put at the start of the name to check to see if
the marked position is a position or a person.
.EP

