.DT
complaints_room.c
Disk World autodoc help
complaints_room.c

.SH Description
.SP 5 5

This inherit allows people to complain in unhappy times.  It does the
interfacing with the complaints handler and all the other gubbins.

.EP
.SP 10 5


Written by Arienne

Started Wed Apr  4 14:11:50 BST 2001

.EP



.SH See also
.SP 5 5
/std/room/inherit/complaints_room.c and /obj/handlers/complaints_handlers.c
.EP
.SH Includes
.SP 5 5
This class includes the following files /include/complaints_handler.h and /include/board.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_complain%^RESET%^
.EI
.SI 5
int do_complain()
.EI
.SP 7 5

This prints a set message to the player who wants to complain.  It also
enforces who cannot complain (eg. guest characters cannot complain).

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Should always return 1.

.EP

.SI 3
* %^BOLD%^finish_complaint%^RESET%^
.EI
.SI 5
void finish_complaint(string str)
.EI
.SP 7 5

Check to see whether the complaint was cancelled.  If not, submit it to the
complaints handler.  Print a message on success or failure.

.EP


