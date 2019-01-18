.DT
safe_room.c
Disk World autodoc help
safe_room.c

.SH Description
.SP 5 5

This is the object that is the keeper of the safe room. This should "
be an NPC.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/armoury.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_dream%^RESET%^
.EI
.SI 5
int do_dream(string str)
.EI

.SI 3
* %^BOLD%^do_rest%^RESET%^
.EI
.SI 5
int do_rest(string str)
.EI

.SI 3
* %^BOLD%^event_enter%^RESET%^
.EI
.SI 5
void event_enter(object ob, string stringy, string stringy2)
.EI

.SI 3
* %^BOLD%^event_exit%^RESET%^
.EI
.SI 5
void event_exit(object ob, string message, object to)
.EI

.SI 3
* %^BOLD%^event_fight_in_progress%^RESET%^
.EI
.SI 5
void event_fight_in_progress(object ob1, object ob2)
.EI

.SI 3
* %^BOLD%^make_calm%^RESET%^
.EI
.SI 5
void make_calm(object blue)
.EI

.SI 3
* %^BOLD%^set_keeper%^RESET%^
.EI
.SI 5
void set_keeper(object ob)
.EI

.SI 3
* %^BOLD%^stop_sneaking%^RESET%^
.EI
.SI 5
void stop_sneaking(object sneaker)
.EI

.SI 3
* %^BOLD%^trap_action%^RESET%^
.EI
.SI 5
int trap_action(string str)
.EI


