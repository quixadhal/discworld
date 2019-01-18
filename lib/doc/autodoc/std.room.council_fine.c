.DT
council_fine.c
Disk World autodoc help
council_fine.c

.SH Description
.SP 5 5

This is the room to handle paying for fines.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Sep 21 15:58:48 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room and /std/room/inherit/council_base.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/nomic_system.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_fine%^RESET%^
.EI
.SI 5
int do_fine()
.EI
.SP 7 5

This method tells them some information about their current and previous
fines.

.EP

.SI 3
* %^BOLD%^do_pay%^RESET%^
.EI
.SI 5
int do_pay()
.EI
.SP 7 5

This method handles a person trying to pay their fine.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_show_all_fines%^RESET%^
.EI
.SI 5
int do_show_all_fines()
.EI

.SI 3
* %^BOLD%^do_show_fine%^RESET%^
.EI
.SI 5
int do_show_fine(string person)
.EI
.SP 7 5

This method shows fines about someone else.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - to show the fines about

.EP

.SI 3
* %^BOLD%^query_fine_handler%^RESET%^
.EI
.SI 5
string query_fine_handler()
.EI
.SP 7 5

This method returns the current fine handler for this counci;.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the fine handler for the council

.EP

.SI 3
* %^BOLD%^query_fine_information%^RESET%^
.EI
.SI 5
string query_fine_information(string person)
.EI
.SP 7 5

This method returns a string detailing information about their fines.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
information about the fines

.EP

.SI 3
* %^BOLD%^set_fine_handler%^RESET%^
.EI
.SI 5
void set_fine_handler(string fine)
.EI
.SP 7 5

This method sets the current fine handler for this council.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fine - the fine handler

.EP


