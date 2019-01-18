.DT
book_shop.c
Disk World autodoc help
book_shop.c

.SH Description
.SP 5 5

This is a specialisation of the craft shop inheritable.  It adds in the
ability to read specified pages from books.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Sep 10 23:51:01 PDT 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/craft_shop.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/craft_shop.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^browse_pages%^RESET%^
.EI
.SI 5
int browse_pages(object * obs)
.EI
.SP 7 5

This is like a normal browse, but it allows us to look at specific pages
in the book.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the page number to browse
.EP
.SP 9 5
obs - the objects whose pages to browse
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^choose_which_page%^RESET%^
.EI
.SI 5
int choose_which_page(function end_func, int value, string name, object * sellables)
.EI
.SP 7 5

This method gets the player to choose a page to allow to browse
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
end_func - the function to evaluate when finished
.EP
.SP 9 5
value - the value of the sellables
.EP
.SP 9 5
name - the name to sell the object for
.EP
.SP 9 5
sellables - the objects to sell

.EP


