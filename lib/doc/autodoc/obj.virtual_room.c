.DT
virtual_room.c
DS2 Discworld autodoc help
virtual_room.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h and /include/virtual_room.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_cloned_object%^RESET%^
.EI
.SI 5
void add_cloned_object(int flag, int delay, mixed clone_info, string entry_mess)
.EI

.SI 3
* %^BOLD%^query_cloned_definitions%^RESET%^
.EI
.SI 5
mixed * query_cloned_definitions()
.EI

.SI 3
* %^BOLD%^query_cloned_objects%^RESET%^
.EI
.SI 5
mixed * query_cloned_objects()
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^make_object%^RESET%^
.EI
.SI 5
void make_object(int i)
.EI


