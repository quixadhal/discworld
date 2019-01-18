.DT
bank_master.c
Disk World autodoc help
bank_master.c

.SH Description
.SP 5 5

This is the bank inheritable.  It handles all the bank transactions
and other such exciting stuff.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room and /std/shops/inherit/bank_master.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^event_save%^RESET%^
.EI
.SI 5
void event_save(object ob)
.EI
.SP 7 5

This method does the actual save.

.EP

.SI 3
* %^BOLD%^make_bank_robbers%^RESET%^
.EI
.SI 5
int make_bank_robbers()
.EI

.SI 3
* %^BOLD%^query_money_place%^RESET%^
.EI
.SI 5
string query_money_place()
.EI
.SP 7 5

This method retuns the money place for the bank.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the money place

.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This method returns the save file used for the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save file used

.EP

.SI 3
* %^BOLD%^set_place%^RESET%^
.EI
.SI 5
void set_place(string word)
.EI
.SP 7 5

This method sets the place that the money will be setup from.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the place to set

.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string word)
.EI
.SP 7 5

This method sets the save file for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the save file name for the bank

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


