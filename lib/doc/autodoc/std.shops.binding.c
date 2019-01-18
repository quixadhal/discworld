.DT
binding.c
Disk World autodoc help
binding.c

.SH Description
.SP 5 5

A binding shop, where people can come and get books bound.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Apr  7 13:19:40 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/language.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_for_magic_writing%^RESET%^
.EI
.SI 5
int check_for_magic_writing(object ob)
.EI
.SP 7 5

Checks to see if the object has any magic writing on it.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is magic writing, 0 if not.
.EP

.SI 3
* %^BOLD%^do_bind%^RESET%^
.EI
.SI 5
int do_bind(object * obs)
.EI
.SP 7 5

This method binds up a book from a bunch of pages.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the pages to bind up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP


