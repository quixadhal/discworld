.DT
scroll_book_react.c
Disk World autodoc help
scroll_book_react.c

.SH Description
.SP 5 5

This is the reaction handling code for both spell books and scrolls.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_react%^RESET%^
.EI
.SI 5
void check_react(object player)
.EI
.SP 7 5

This method checks to see if the spell reacts with anything.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
paper - who to check (us hopefull)

.EP

.SI 3
* %^BOLD%^destroy_this_scroll%^RESET%^
.EI
.SI 5
void destroy_this_scroll()
.EI
.SP 7 5

This method is called to destroy the scroll.  This must be overridden
at a higher level.

.EP


