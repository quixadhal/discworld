.DT
placement.h
Disk World autodoc help
placement.h

.SH Description
.SP 5 5

This is used in conjunction with the placement inherit to handle
how things are placed into the room.

.EP
.SP 10 5

.EP

.SH Defines
.SI 3
* %^BOLD%^PLACEMENT_CURRENT_ROOM_OTHER%^RESET%^
.EI
.SP 7 5

This property is used for handling the current object 'other'.  The
other is the 'against', 'beside' etc things.
This is used
after the item has been placed.

.EP

.SI 3
* %^BOLD%^PLACEMENT_CURRENT_ROOM_POSITION%^RESET%^
.EI
.SP 7 5

This property is used for handling the current object position.  The
position is what it is beside, like 'wall', 'ceiling' etc.
This is used
after the item has been placed.

.EP

.SI 3
* %^BOLD%^PLACEMENT_CURRENT_ROOM_VERB%^RESET%^
.EI
.SP 7 5

This property is used for handling the current room verb.  This is used
after the item has been placed.

.EP

.SI 3
* %^BOLD%^PLACEMENT_IMMOVABLE%^RESET%^
.EI
.SP 7 5

This property is used to make items in the room as fixed in their current
place.  This way they cannot be removed or replaced.

.EP

