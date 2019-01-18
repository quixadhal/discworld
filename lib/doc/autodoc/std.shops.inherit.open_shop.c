.DT
open_shop.c
Disk World autodoc help
open_shop.c

.SH Description
.SP 5 5

This is the basic inheritable to make things check for being
open.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Oct 23 12:04:18 PDT 2000

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^is_open%^RESET%^
.EI
.SI 5
int is_open(object player, int type)
.EI
.SP 7 5

This method checks to see if the shop is open.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player who we are checking to see if it is open for

.EP

.SI 3
* %^BOLD%^query_open_function%^RESET%^
.EI
.SI 5
function query_open_function()
.EI
.SP 7 5

This method queries the open function for the room/.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the open function

.EP

.SI 3
* %^BOLD%^set_open_function%^RESET%^
.EI
.SI 5
void set_open_function(function func)
.EI
.SP 7 5

This method sets the open function for the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the open function
.EP


