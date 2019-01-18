.DT
newbie_junk.c
Disk World autodoc help
newbie_junk.c

.SH Description
.SP 5 5

Handles the new player stuff and the login gifts.

.EP
.SP 10 5


Written by Pinkfish

.EP



.SH See also
.SP 5 5
/global/player
.EP
.SH Includes
.SP 5 5
This class includes the following files /include/priest.h, /include/config.h and /include/armoury.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^give_junk%^RESET%^
.EI
.SI 5
void give_junk(object pl)
.EI

.SI 3
* %^BOLD%^logged_on_times%^RESET%^
.EI
.SI 5
void logged_on_times(int num)
.EI
.SP 7 5

Gives out the login gifts and anything else that should be done on
a certain login number.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the number of logins

.EP

.SI 3
* %^BOLD%^start_player%^RESET%^
.EI
.SI 5
void start_player(object pl)
.EI
.SP 7 5

This method is called when a player first starts on the mud.  It gives
out all of the newbiew equipment and stuff.

.EP


