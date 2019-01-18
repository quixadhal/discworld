.DT
carrying.c
Disk World autodoc help
carrying.c

.SH Description
.SP 5 5

This file contains all the code need to handle the types of carrying
which npcs tend to involve themselves in.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/money.h and /include/clothing.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_living_contents%^RESET%^
.EI
.SI 5
string query_living_contents(int self)
.EI
.SP 7 5

This method does the inventory of the living objects we all know and
love.  It is used in the long description and the inventory commands.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
self - are we showing our own inventory
.EP

.SI 3
* %^BOLD%^query_wearing_hidden%^RESET%^
.EI
.SI 5
object * query_wearing_hidden(object looker, object * things)
.EI
.SP 7 5

This method returns the items that are hidden by something on top of
them.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
looker - the person looking at the items
.EP
.SP 9 5
things - an optional set of things
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the hidden worn types

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^armour_list%^RESET%^
.EI
.SI 5
string armour_list(object * things, int hidden_only, int self)
.EI
.SP 7 5

This method returns a string list of the armours which the player
is wearing.  It will show also modify the list to show only hidden
items (items under other items are considered hidden).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
things - the things to get the list of armours from
.EP
.SP 9 5
hidden_only - only show the list of hidden objects
.EP

.SI 3
* %^BOLD%^query_types_worn%^RESET%^
.EI
.SI 5
varargs string * query_types_worn(object * things)
.EI
.SP 7 5

This method returns the list of types of things which are passed into
the array.


Two elements in an array is returned, the first element is the list of all of the different types which are worn and the second is the list of types corresponding to each of the inputs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
things - the things to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a two element array
.EP


