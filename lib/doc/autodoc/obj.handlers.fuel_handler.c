.DT
fuel_handler.c
Disk World autodoc help
fuel_handler.c

.SH Description
.SP 5 5

Adds a burner into the burner array.  The burner has the
function concume_fuelk called on it every fuel interval.
.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/fuel_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_burner%^RESET%^
.EI
.SI 5
void add_burner(object thing)
.EI

.SI 3
* %^BOLD%^query_burners%^RESET%^
.EI
.SI 5
object * query_burners()
.EI
.SP 7 5

Returns the complete list of the burner array.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array containing all the burners

.EP

.SI 3
* %^BOLD%^remove_burner%^RESET%^
.EI
.SI 5
void remove_burner(object thing)
.EI
.SP 7 5

Remove a burner from the burner list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the burner to remove

.EP


