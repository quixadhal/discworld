.DT
move_failures.h
Disk World autodoc help
move_failures.h

.SH Description
.SP 5 5

List of defines for the various typoes of move failures.  So you can figure
out why the move failed (if this is important to you).

.EP
.SP 10 5

.EP

.SH Defines
.SI 3
* %^BOLD%^MOVE_EMPTY_DEST%^RESET%^
.EI

.SI 3
* %^BOLD%^MOVE_INVALID_DEST%^RESET%^
.EI

.SI 3
* %^BOLD%^MOVE_NO_DROP%^RESET%^
.EI
.SP 7 5

Unable to remove the object from the players inventory.  This is controlled
by the set_drop and reset_drop functions.

.EP

.SI 3
* %^BOLD%^MOVE_NO_GET%^RESET%^
.EI
.SP 7 5

The remove function returned a fail result when the object was attempted
to be removed.  This can be changed with the set_get and reset_get functions.

.EP

.SI 3
* %^BOLD%^MOVE_NO_UNHOLD%^RESET%^
.EI

.SI 3
* %^BOLD%^MOVE_OK%^RESET%^
.EI
.SP 7 5

The move succeeded.

.EP

.SI 3
* %^BOLD%^MOVE_TOO_HEAVY%^RESET%^
.EI
.SP 7 5

The object ius too heavy to go into the inventory.

.EP

