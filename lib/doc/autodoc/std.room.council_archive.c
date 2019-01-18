.DT
council_archive.c
Disk World autodoc help
council_archive.c

.SH Description
.SP 5 5

This room is for looking at old closed cases.
.EP
.SP 10 5


Written by Pinkfish

Started Tue Jun  3 10:22:28 PDT 2003

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room and /std/room/inherit/council_base.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/nomic_system.h and /include/mail.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_list_all_cases%^RESET%^
.EI
.SI 5
int do_list_all_cases()
.EI
.SP 7 5

This method lists all the currently open cases.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 if failed

.EP

.SI 3
* %^BOLD%^do_list_case%^RESET%^
.EI
.SI 5
int do_list_case(int case_id)
.EI
.SP 7 5

This method lists the specified open case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
case_id - the case to list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 if failed

.EP

.SI 3
* %^BOLD%^do_list_citizens%^RESET%^
.EI
.SI 5
int do_list_citizens()
.EI
.SP 7 5

This method lists the currently online citizens in a nice list.

.EP

.SI 3
* %^BOLD%^query_case_short%^RESET%^
.EI
.SI 5
string query_case_short(class nomic_case the_case)
.EI
.SP 7 5

The short description of the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the_case - the case to get a short desc
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short description of the case

.EP


