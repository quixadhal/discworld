.DT
scabbard.c
DW_wibble autodoc help
scabbard.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/container and /std/basic/wearable.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h and /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^no_types_in_long%^RESET%^
.EI
.SI 5
int no_types_in_long()
.EI

.SI 3
* %^BOLD%^query_scabbard%^RESET%^
.EI
.SI 5
int query_scabbard()
.EI
.SP 7 5

This method determines if the object is a scabbard or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1 for a scabbard

.EP

.SI 3
* %^BOLD%^query_types%^RESET%^
.EI
.SI 5
string * query_types()
.EI
.SP 7 5

This method returns the types of weapons that can be sheathed in the
scabbard.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the types that can be sheathed

.EP

.SI 3
* %^BOLD%^set_no_types_in_long%^RESET%^
.EI
.SI 5
int set_no_types_in_long(int val)
.EI

.SI 3
* %^BOLD%^set_types%^RESET%^
.EI
.SI 5
void set_types(string * words)
.EI
.SP 7 5

This method sets the types of weapons that can be sheathed in the
scabbard.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the array of types that can be sheathed

.EP

.SI 3
* %^BOLD%^setup_scabbard%^RESET%^
.EI
.SI 5
void setup_scabbard(int number)
.EI
.SP 7 5

This method sets up the scabbards current condition.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the current condition fo the scabbard

.EP

.SI 3
* %^BOLD%^test_type_ok%^RESET%^
.EI
.SI 5
int test_type_ok(object thing, int flag)
.EI


