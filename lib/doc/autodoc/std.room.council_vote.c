.DT
council_vote.c
Disk World autodoc help
council_vote.c

.SH Description
.SP 5 5

This is the room you will use to vote for magistrates.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Oct 23 22:26:11 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room, /std/room/inherit/voting_room and /std/room/inherit/council_base.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h and /include/nomic_system.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_list_motions%^RESET%^
.EI
.SI 5
int do_list_motions()
.EI
.SP 7 5

This lists all the new rules up for discussion or whatever by the
magistrates.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_nominate%^RESET%^
.EI
.SI 5
int do_nominate(string person)
.EI
.SP 7 5

This method allows you to nominate someone for a position.

.EP

.SI 3
* %^BOLD%^do_status%^RESET%^
.EI
.SI 5
int do_status()
.EI
.SP 7 5

This method prints some useful information about the election status
of the current area.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_vote%^RESET%^
.EI
.SI 5
int do_vote(string person, int force)
.EI
.SP 7 5

This method allows you to vote for someone.

.EP

.SI 3
* %^BOLD%^do_vote_on%^RESET%^
.EI
.SI 5
int do_vote_on(int num, string vote)
.EI
.SP 7 5

This method will handle voting on the specific new nomic rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the rule number to vote on
.EP
.SP 9 5
vote - their vote
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP


