.DT
clothing_transport.c
Disk World autodoc help
clothing_transport.c

.SH Description
.SP 5 5

This is the clothing file for transports.
.EP
.SP 10 5


Written by Pinkfish

Started Wed Nov 17 00:39:37 PST 1999
.EP



.SH See also
.SP 5 5
/std/npc/types/transport.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /obj/clothing.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_direction_difficulty_modifier%^RESET%^
.EI
.SI 5
int query_direction_difficulty_modifier()
.EI
.SP 7 5

This method queries the direction difficulty modifier.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the direction difficulty modifier

.EP

.SI 3
* %^BOLD%^query_mount_difficulty_modifier%^RESET%^
.EI
.SI 5
int query_mount_difficulty_modifier()
.EI
.SP 7 5

This method queries the mount difficulty modifier.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mount difficulty modifier

.EP

.SI 3
* %^BOLD%^query_worn_by_race%^RESET%^
.EI
.SI 5
string query_worn_by_race()
.EI
.SP 7 5

This method queries the race the clothing is for.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the race the clothing is for

.EP

.SI 3
* %^BOLD%^set_direction_difficulty_modifier%^RESET%^
.EI
.SI 5
void set_direction_difficulty_modifier(int diff)
.EI
.SP 7 5

This method sets the direction difficulty modifier.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
diff - the direction difficulty modifier

.EP

.SI 3
* %^BOLD%^set_mount_difficulty_modifier%^RESET%^
.EI
.SI 5
void set_mount_difficulty_modifier(int diff)
.EI
.SP 7 5

This method set the mount difficulty modifier.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
diff - the mount difficulty modifier

.EP

.SI 3
* %^BOLD%^set_worn_by_race%^RESET%^
.EI
.SI 5
void set_worn_by_race(string race)
.EI
.SP 7 5

This method sets the race the clothing is for.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
race - the race that the clothing is for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the race the clothing is for

.EP


