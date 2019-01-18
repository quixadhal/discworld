.DT
club_personal.c
Disk World autodoc help
club_personal.c

.SH Description
.SP 5 5

This is the handler for all things clubby, a club being a group of
players.  Each club must have a unique name.  It also handles elections
for various club positions.
.EP
.SP 10 5


Written by Pinkfish

Started Sun Sep 27 03:35:42 EDT 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/handlers/inherit/club_basic.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h and /include/clubs.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^personal_info%^RESET%^
class personal_info {
}

.EI
.SP 7 5

This is for a personal club, a personal club will be removed when
the founder disappears.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^create_extra_data%^RESET%^
.EI
.SI 5
void create_extra_data(string name)
.EI
.SP 7 5

This is for a personal club, a personal club will be removed when
the founder disappears.

.EP


