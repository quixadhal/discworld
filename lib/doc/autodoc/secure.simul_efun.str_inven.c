.DT
str_inven.c
Disk World autodoc help
str_inven.c

.SH Description
.SP 5 5

Cotains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_strange_inventory%^RESET%^
.EI
.SI 5
mixed * query_strange_inventory(mixed * arr)
.EI
.SP 7 5

This method returns an inventory sorted on descriptions.
The format of the return array is:

.EP
.SP 7 5
({
.EP
.SP 7 5
   "desc", ({ ob, ... }),
.EP
.SP 7 5
   "desc2", ({ ob11, ob12, ... })
.EP
.SP 7 5
   ...
.EP
.SP 7 5
 })
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arr - the array to get the inventory for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array structured as above

.EP


