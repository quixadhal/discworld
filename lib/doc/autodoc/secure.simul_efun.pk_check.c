.DT
pk_check.c
Disk World autodoc help
pk_check.c

.SH Description
.SP 5 5

Contains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/player.h, /include/login_handler.h, /include/login.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^pk_assist%^RESET%^
.EI
.SI 5
int pk_assist(mixed assister, mixed assistee, int off_line)
.EI
.SP 7 5

This method checks to see if both of the objects are able to assist each
other. This will return 1 if they cannot complete the action and 0
They are not able to assist each other if one of them is not PK and
the other is and the PK is in combat with other players.


If an object is passed into this method, it is first checked with the method query_owner_pk_check(), if this returns a non-zero value it is used as the person who owns the object. This allows objects to be pk checked when they are not players.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
assister - thing doing the assisting
.EP
.SP 9 5
assistee - thing being assisted
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they cannot attack each other

.EP

.SI 3
* %^BOLD%^pk_check%^RESET%^
.EI
.SI 5
int pk_check(mixed thing1, mixed thing2, int off_line)
.EI
.SP 7 5

This method checks to see if both of the objects are able to do a
pk action on each other.  This will return 1 if they cannot
complete the action, ie: either of them are not player killers.
It will take an object or a string as the first parameter and attempt
to check for pkness, if the off_line flag is set it will even check for
players that are not even logged on.


If an object is passed into this method, it is first checked with the method query_owner_pk_check(), if this returns a non-zero value it is used as the person who owns the object. This allows objects to be pk checked when they are not players.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing1 - first object to check
.EP
.SP 9 5
thing2 - second object to check
.EP
.SP 9 5
off_line - allow the check to occur if they are not logged on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they cannot attack each other

.EP


