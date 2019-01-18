.DT
fine.c
Disk World autodoc help
fine.c

.SH Description
.SP 5 5

This is the method for fining someone in am.  This handles keeping
track fo the fines and the setup of fines.  This punihshment needs to
be inherited to work correctly.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Sep 21 14:35:32 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/punishment_inherit.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/nomic_system.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^fine_type%^RESET%^
class fine_type {
int paid;
int fines;
int last_touched;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^pay_fine%^RESET%^
.EI
.SI 5
int pay_fine(string person, int amount)
.EI
.SP 7 5

This method pays a fine.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person doing the payment
.EP
.SP 9 5
amount - the amount being paid
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^query_current_fine%^RESET%^
.EI
.SI 5
int query_current_fine(string person)
.EI
.SP 7 5

This method figures out the current fine for the person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to get the fine for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the currently outstanding fines

.EP

.SI 3
* %^BOLD%^query_money_area%^RESET%^
.EI
.SI 5
string query_money_area()
.EI
.SP 7 5

This method returns the money area of the fine.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the money area

.EP

.SI 3
* %^BOLD%^query_people_with_fines%^RESET%^
.EI
.SI 5
string * query_people_with_fines()
.EI
.SP 7 5

This method returns the list of players that have outstanding fines
in the fine handler.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of players

.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This method returns the save file of the fines.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save file name

.EP

.SI 3
* %^BOLD%^query_total_fine%^RESET%^
.EI
.SI 5
int query_total_fine(string person)
.EI
.SP 7 5

This method figures out the current total fine for the person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to get the fine for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the currently outstanding fines

.EP

.SI 3
* %^BOLD%^query_total_payments%^RESET%^
.EI
.SI 5
int query_total_payments(string person)
.EI
.SP 7 5

This method figures out the current total fine for the person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to get the fine for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the currently outstanding fines

.EP

.SI 3
* %^BOLD%^remove_old_fines%^RESET%^
.EI
.SI 5
void remove_old_fines()
.EI
.SP 7 5

This method nips through the list of fines and removes any fines that
are either for people that do not exist any more, or totals that are
old.

.EP

.SI 3
* %^BOLD%^set_money_area%^RESET%^
.EI
.SI 5
void set_money_area(string area)
.EI
.SP 7 5

This method sets the money area of the fine.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the money area

.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string fname)
.EI
.SP 7 5

This method sets the save file of the fines.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the save file name

.EP


