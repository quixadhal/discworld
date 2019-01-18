.DT
fairy_godmothers.c
Disk World autodoc help
fairy_godmothers.c

.SH Description
.SP 5 5

Keep track of all the fairy godmothers.  Make sure they don't do any
thing naughty.  This also controls the costs for the godmother
service.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/config.h, /include/playerinfo.h, /include/panic.h and /include/cwc.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_hurry_up%^RESET%^
.EI
.SI 5
void do_hurry_up()
.EI
.SP 7 5

Give the players a nudge if they are taking to long.

.EP

.SI 3
* %^BOLD%^do_move%^RESET%^
.EI
.SI 5
void do_move(object fairy)
.EI

.SI 3
* %^BOLD%^do_panic_callout%^RESET%^
.EI
.SI 5
void do_panic_callout(object godmother, object ob)
.EI

.SI 3
* %^BOLD%^do_startup%^RESET%^
.EI
.SI 5
int do_startup(object panicer)
.EI
.SP 7 5

This method handles a player panicing.


This is the fairy godmother controller for panicing. 

When someone panics it calls on this object to help fix them up.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
panicer - the person panicing
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if they were unmable to panic, 1 if they were

.EP

.SI 3
* %^BOLD%^finish_panic%^RESET%^
.EI
.SI 5
void finish_panic(object person, int ok)
.EI
.SP 7 5

Called by the godmother when the person has completed their
paniced transaction.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person who paniced
.EP
.SP 9 5
ok - did they complete it ok?

.EP

.SI 3
* %^BOLD%^free_godmothers%^RESET%^
.EI
.SI 5
int free_godmothers()
.EI
.SP 7 5

Check to see if there are any free godmothers.  Are you being
served?
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is a free one, 0 if there is not

.EP

.SI 3
* %^BOLD%^query_godmothers%^RESET%^
.EI
.SI 5
object * query_godmothers()
.EI
.SP 7 5

Return an array of all the godmothers.  This is the current set of
used godmothers.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of objects of godmothers

.EP


