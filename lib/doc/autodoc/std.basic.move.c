.DT
move.c
Disk World autodoc help
move.c

.SH Description
.SP 5 5

This class defines the basic move function and support functions.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^drop%^RESET%^
.EI
.SI 5
int drop(mixed dest)
.EI
.SP 7 5

The current value of the drop flag. This function is called by the
test_remove() function in the living object.  So it allows you to
move the object out of inanimate objects, just not out of the
players.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is set, 0 if it is reset
.EP

.SI 3
* %^BOLD%^dwep%^RESET%^
.EI
.SI 5
nomask mixed dwep()
.EI
.SP 7 5

Dest with extreme predejuce.  This is the last way to destruct an
object.  Used if all other methods fail.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string "Destructed With Extreme Prejudice"

.EP

.SI 3
* %^BOLD%^get%^RESET%^
.EI
.SI 5
int get(mixed dest)
.EI
.SP 7 5

The current value of the get flag.  This function is called by the
test_add() function in the living object.  So it allows you to
move the object into innimate objects, just not into player
characters.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is set, 0 if it is reset
.EP

.SI 3
* %^BOLD%^move%^RESET%^
.EI
.SI 5
varargs int move(mixed dest, string messin, string messout)
.EI
.SP 7 5

The basic move method.  The destination can be an object or a string.
if it is neither of these then the failure MOVE_EMPTY_DEST is
returned.  If the function test_add() was called on the
destination and it returns 0 then MOVE_NO_GET is returned as
a failure.  If the destination is this_object() then
MOVE_INVALID_DEST is returned as the failure.  If the
function test_remove is called on the current environment and it
returns 0 then MOVE_NO_DROP is returned as the move failure.


The events "enter" and "exit" are generated as part of the move process. Both of these events are passed two arguments. The exit event is passed the exit message and the destination. The enter event is passed the enter message and the place we came from. 

The function event_move_object() is called (call_other) on the object if the move was successful (MOVE_OK).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dest - where to move to
.EP
.SP 9 5
messin - the message to print when entering
.EP
.SP 9 5
messout - the message to print when leaving
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
MOVE_OK on success, or a failure reason on failure
.EP

.SI 3
* %^BOLD%^reset_drop%^RESET%^
.EI
.SI 5
void reset_drop()
.EI
.SP 7 5

Turns off the drop flag.
.EP

.SI 3
* %^BOLD%^reset_get%^RESET%^
.EI
.SI 5
void reset_get()
.EI
.SP 7 5

Turns off the get flag.
.EP

.SI 3
* %^BOLD%^set_drop%^RESET%^
.EI
.SI 5
void set_drop()
.EI
.SP 7 5

Turns on the drop flag.
.EP

.SI 3
* %^BOLD%^set_get%^RESET%^
.EI
.SI 5
void set_get()
.EI
.SP 7 5

Turns on the get flag.
.EP


