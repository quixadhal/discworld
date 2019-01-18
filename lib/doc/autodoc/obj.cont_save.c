.DT
cont_save.c
Disk World autodoc help
cont_save.c

.SH Description
.SP 5 5

A container that will save it's contents in a nice happy way.  The
container will not allow stuff in or out until it has finished loading.
.EP
.SP 10 5


Written by Ceres

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/baggage.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^can_find_match_recurse_into%^RESET%^
.EI
.SI 5
int can_find_match_recurse_into(object looker)
.EI

.SI 3
* %^BOLD%^do_load%^RESET%^
.EI
.SI 5
varargs void do_load(object thing)
.EI
.SP 7 5

Loads the data for the saved container from the save file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the person to report errors to

.EP

.SI 3
* %^BOLD%^do_save%^RESET%^
.EI
.SI 5
void do_save()
.EI
.SP 7 5

This method saves the data to the save file.

.EP

.SI 3
* %^BOLD%^query_can_recurse%^RESET%^
.EI
.SI 5
int query_can_recurse()
.EI

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This method returns the current save file for the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current save file

.EP

.SI 3
* %^BOLD%^set_can_recurse%^RESET%^
.EI
.SI 5
void set_can_recurse(int recurse_flag)
.EI

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
varargs void set_save_file(string file, object thing)
.EI
.SP 7 5

This method sets the save file for the item and then does a load.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - the file name to set the save object to
.EP
.SP 9 5
thing - the thing to set as the person to report errors to

.EP


