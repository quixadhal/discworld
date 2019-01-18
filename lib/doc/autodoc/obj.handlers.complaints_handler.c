.DT
complaints_handler.c
Disk World autodoc help
complaints_handler.c

.SH Description
.SP 5 5

This handler deals with complaints.  It manages posting to the "complaints"
board and numbering the posts sequentially (helps to follow threads if
anybody follows up the posts on the board).

.EP
.SP 10 5


Written by Arienne

Started Wed Apr  4 14:11:50 BST 2001

.EP



.SH See also
.SP 5 5
/std/room/inherit/complaints_room.c and /std/room/complaints_room.c
.EP
.SH Includes
.SP 5 5
This class includes the following files /include/board.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_submit_complaint%^RESET%^
.EI
.SI 5
int do_submit_complaint(string str)
.EI
.SP 7 5

Submits a complaint to the complaints board anonymously.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - The text of the complaint.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The note number, or 0 on failure.

.EP


