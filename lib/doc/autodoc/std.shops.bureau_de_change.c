.DT
bureau_de_change.c
Disk World autodoc help
bureau_de_change.c

.SH Description
.SP 5 5

This is the standard money changer inheritable.
.EP
.SP 10 5


Written by Pinkfish
.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
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
* %^BOLD%^do_buy%^RESET%^
.EI
.SI 5
int do_buy(object * things, string country)
.EI

.SI 3
* %^BOLD%^exchanges_list%^RESET%^
.EI
.SI 5
string exchanges_list()
.EI

.SI 3
* %^BOLD%^list%^RESET%^
.EI
.SI 5
int list()
.EI

.SI 3
* %^BOLD%^query_buy_only%^RESET%^
.EI
.SI 5
int query_buy_only()
.EI

.SI 3
* %^BOLD%^query_exchanges%^RESET%^
.EI
.SI 5
mapping query_exchanges()
.EI

.SI 3
* %^BOLD%^query_place%^RESET%^
.EI
.SI 5
string query_place()
.EI

.SI 3
* %^BOLD%^query_standard%^RESET%^
.EI
.SI 5
int query_standard()
.EI

.SI 3
* %^BOLD%^sell%^RESET%^
.EI
.SI 5
int sell(object * things)
.EI

.SI 3
* %^BOLD%^set_buy_only%^RESET%^
.EI
.SI 5
void set_buy_only()
.EI

.SI 3
* %^BOLD%^set_exchanges%^RESET%^
.EI
.SI 5
void set_exchanges(mapping map)
.EI

.SI 3
* %^BOLD%^set_place%^RESET%^
.EI
.SI 5
void set_place(string word)
.EI

.SI 3
* %^BOLD%^set_standard%^RESET%^
.EI
.SI 5
void set_standard(int number)
.EI


