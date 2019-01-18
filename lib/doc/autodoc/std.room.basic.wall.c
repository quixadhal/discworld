.DT
wall.c
Disk World autodoc help
wall.c

.SH Description
.SP 5 5

This is a wall object.  It is added to rooms when you use the set_wall()
function to add a climbable wall exit to a room.  If you wish to have
players climb somewhere this is the preferred method to use.

.EP
.SP 10 5


Written by Deutha
.EP



.SH See also
.SP 5 5
climb, /std/room.c and /std/rooftop.c

.EP
.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^calc_co_ord%^RESET%^
.EI
.SI 5
void calc_co_ord()
.EI

.SI 3
* %^BOLD%^fall_down%^RESET%^
.EI
.SI 5
void fall_down(object thing)
.EI

.SI 3
* %^BOLD%^query_at_bottom%^RESET%^
.EI
.SI 5
int query_at_bottom()
.EI

.SI 3
* %^BOLD%^query_belows%^RESET%^
.EI
.SI 5
string * query_belows()
.EI

.SI 3
* %^BOLD%^query_bottom%^RESET%^
.EI
.SI 5
mixed * query_bottom()
.EI

.SI 3
* %^BOLD%^query_death_mess%^RESET%^
.EI
.SI 5
string query_death_mess()
.EI

.SI 3
* %^BOLD%^query_ghost_action%^RESET%^
.EI
.SI 5
mixed query_ghost_action()
.EI

.SI 3
* %^BOLD%^query_move%^RESET%^
.EI
.SI 5
mixed * query_move(string word)
.EI

.SI 3
* %^BOLD%^query_moves%^RESET%^
.EI
.SI 5
mixed * query_moves()
.EI

.SI 3
* %^BOLD%^query_no_drop%^RESET%^
.EI
.SI 5
int query_no_drop()
.EI

.SI 3
* %^BOLD%^set_wall%^RESET%^
.EI
.SI 5
void set_wall(mixed * args)
.EI


