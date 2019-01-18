.DT
owned.c
Disk World autodoc help
owned.c

.SH Description
.SP 5 5

Common functionality for an npc that is owned (ie summoned/controlled
by someone else).
.EP
.SP 10 5


Written by Aquilo

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_owner%^RESET%^
.EI
.SI 5
object query_owner()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the ob pointing to the thing that owns/controls us

.EP

.SI 3
* %^BOLD%^set_owner%^RESET%^
.EI
.SI 5
object set_owner(mixed owner)
.EI
.SP 7 5

Set who owns us. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
owner - either player name, player ob, or other living ob

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


