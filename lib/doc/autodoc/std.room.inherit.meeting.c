.DT
meeting.c
Disk World autodoc help
meeting.c

.SH Description
.SP 5 5

This file is the inheritable for a meeting room.  It handles the floor
and a few other things neatly.
.EP
.SP 10 5


Written by Pinkfish 

Started 25th of June

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^create_floor%^RESET%^
.EI
.SI 5
object create_floor()
.EI
.SP 7 5

This method creates the floor for the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the floor for the room

.EP

.SI 3
* %^BOLD%^do_bang%^RESET%^
.EI
.SI 5
int do_bang()
.EI
.SP 7 5

This method is called when the gavel is going to banged.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_finish%^RESET%^
.EI
.SI 5
int do_finish()
.EI
.SP 7 5

This method is called to finish the meeting.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_recover%^RESET%^
.EI
.SI 5
int do_recover()
.EI
.SP 7 5

This method is called when the floor is recovered by the leader.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_start%^RESET%^
.EI
.SI 5
int do_start()
.EI
.SP 7 5

Called to start a meeting.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP


