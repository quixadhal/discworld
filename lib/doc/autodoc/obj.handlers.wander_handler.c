.DT
wander_handler.c
Disk World autodoc help
wander_handler.c

.SH Description
.SP 5 5

Handles npcs wanderting randomly around the place and npcs
following a route to a destination.
.EP
.SP 10 5


Written by Wodan and Pinkfish

.EP



.SH See also
.SP 5 5
/obj/monster->move_me_to() and /std/effects/npc/goto_destination
.EP
.SH Includes
.SP 5 5
This class includes the following files /include/player.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^route_traveller%^RESET%^
class route_traveller {
object monster;
int movetime;
int delay;
string dest;
}

.EI

.SI 3
* %^BOLD%^wander_group%^RESET%^
class wander_group {
object * monsters;
int movetime;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^move_me_please%^RESET%^
.EI
.SI 5
int move_me_please(int delay, string dest)
.EI
.SP 7 5

Called from the npc to start them mmoving.  This should not need to
be called inside your code.  It is called from the function
move_me_to in the npc code.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
delay - the delay between each move
.EP
.SP 9 5
dest - the destination room for the npc
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if it failed and 1 on success

.EP

.SI 3
* %^BOLD%^query_moving_monster%^RESET%^
.EI
.SI 5
int query_moving_monster(object ob)
.EI
.SP 7 5

Is this npc a moving npc?
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the npc to check to see if it moves
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is moving, 0 if it is not.

.EP


