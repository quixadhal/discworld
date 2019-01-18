.DT
newspaper_archive.c
Disk World autodoc help
newspaper_archive.c

.SH Description
.SP 5 5

This is the newspaper archive inherit.  Allows you to buy back issues of the
paper.
.EP
.SP 10 5


Written by Pinkfish

Started Wed May 23 15:56:13 PDT 2001

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/shop_event.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/shops/bank.h, /include/shops/newspaper_archive.h, /include/room/newspaper.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_buy%^RESET%^
.EI
.SI 5
int do_buy(string paper, int edition)
.EI
.SP 7 5

This method buys a back issue of the paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - the paper to buy a back issue of
.EP
.SP 9 5
issue - the issue number to buy

.EP

.SI 3
* %^BOLD%^do_list_papers%^RESET%^
.EI
.SI 5
int do_list_papers()
.EI
.SP 7 5

This method shows which papers are currently available for buying back
issues of.

.EP

.SI 3
* %^BOLD%^query_paper%^RESET%^
.EI
.SI 5
string query_paper()
.EI
.SP 7 5

This method returns the paper for use in getting information form
the archive.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the paper area

.EP

.SI 3
* %^BOLD%^query_paper_area%^RESET%^
.EI
.SI 5
string query_paper_area()
.EI
.SP 7 5

This method returns the paper area for use in getting information form
the archive.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the paper area

.EP

.SI 3
* %^BOLD%^set_paper%^RESET%^
.EI
.SI 5
void set_paper(string paper)
.EI
.SP 7 5

This sets the paper to use for getting information from the archive.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to set

.EP

.SI 3
* %^BOLD%^set_paper_area%^RESET%^
.EI
.SI 5
void set_paper_area(string area)
.EI
.SP 7 5

This sets the paper area to use for getting information from the archive.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to set

.EP


