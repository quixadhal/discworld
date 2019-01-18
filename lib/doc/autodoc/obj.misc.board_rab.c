.DT
board_rab.c
Disk World autodoc help
board_rab.c

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/misc/board.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/board.h and /include/mail.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_add%^RESET%^
.EI
.SI 5
int do_add(string name)
.EI

.SI 3
* %^BOLD%^do_archive%^RESET%^
.EI
.SI 5
int do_archive(string name)
.EI

.SI 3
* %^BOLD%^do_arts%^RESET%^
.EI
.SI 5
int do_arts()
.EI

.SI 3
* %^BOLD%^do_board%^RESET%^
.EI
.SI 5
int do_board(string str)
.EI

.SI 3
* %^BOLD%^do_boards%^RESET%^
.EI
.SI 5
int do_boards()
.EI

.SI 3
* %^BOLD%^do_killfile%^RESET%^
.EI
.SI 5
int do_killfile(string arg)
.EI

.SI 3
* %^BOLD%^do_listkillfile%^RESET%^
.EI
.SI 5
int do_listkillfile(string arg)
.EI

.SI 3
* %^BOLD%^do_maximum%^RESET%^
.EI
.SI 5
int do_maximum(int i)
.EI

.SI 3
* %^BOLD%^do_minimum%^RESET%^
.EI
.SI 5
int do_minimum(string i)
.EI

.SI 3
* %^BOLD%^do_new%^RESET%^
.EI
.SI 5
int do_new()
.EI

.SI 3
* %^BOLD%^do_next%^RESET%^
.EI
.SI 5
int do_next()
.EI

.SI 3
* %^BOLD%^do_remove%^RESET%^
.EI
.SI 5
int do_remove(string name)
.EI

.SI 3
* %^BOLD%^do_security%^RESET%^
.EI
.SI 5
int do_security()
.EI

.SI 3
* %^BOLD%^do_skip%^RESET%^
.EI
.SI 5
int do_skip()
.EI

.SI 3
* %^BOLD%^do_store%^RESET%^
.EI
.SI 5
int do_store(int which, string file)
.EI

.SI 3
* %^BOLD%^do_storeall%^RESET%^
.EI
.SI 5
int do_storeall(string file)
.EI

.SI 3
* %^BOLD%^do_summary%^RESET%^
.EI
.SI 5
int do_summary(string str)
.EI

.SI 3
* %^BOLD%^do_timeout%^RESET%^
.EI
.SI 5
int do_timeout(int i)
.EI

.SI 3
* %^BOLD%^do_transfer%^RESET%^
.EI
.SI 5
int do_transfer(int num, string str)
.EI

.SI 3
* %^BOLD%^do_unkillfile%^RESET%^
.EI
.SI 5
int do_unkillfile(string arg)
.EI

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mixed board)
.EI

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string str, int dark)
.EI

.SI 3
* %^BOLD%^move%^RESET%^
.EI
.SI 5
varargs int move(object dest, string s1, string s2)
.EI

.SI 3
* %^BOLD%^prev%^RESET%^
.EI
.SI 5
int prev()
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mixed query_static_auto_load()
.EI

.SI 3
* %^BOLD%^set_board_name%^RESET%^
.EI
.SI 5
void set_board_name(string str)
.EI

.SI 3
* %^BOLD%^set_datafile%^RESET%^
.EI
.SI 5
void set_datafile(string str)
.EI


