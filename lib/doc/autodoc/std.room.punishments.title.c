.DT
title.c
Disk World autodoc help
title.c

.SH Description
.SP 5 5

This is the method for forcing someone to have a title of some sort.
It allows a range of different titles to be chosen from.
This punihshment needs to
be inherited to work correctly.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Jun 26 14:46:36 PDT 2003

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/punishment_inherit.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/nomic_system.h, /include/player.h, /include/shops/bank.h, /include/playtesters.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^choose_timeout%^RESET%^
.EI
.SI 5
void choose_timeout(string str, string area, class nomic_case the_case, string type, function finish, string title)
.EI

.SI 3
* %^BOLD%^query_titles%^RESET%^
.EI
.SI 5
string * query_titles()
.EI
.SP 7 5

This method returns the list of allowable titles to use for this
area.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of allowable titles

.EP

.SI 3
* %^BOLD%^set_titles%^RESET%^
.EI
.SI 5
void set_titles(string * titles)
.EI
.SP 7 5

This method sets the allowable list of titles used for this area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
titles - the list of titles to use

.EP


