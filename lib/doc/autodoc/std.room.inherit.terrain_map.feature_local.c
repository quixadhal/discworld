.DT
feature_local.c
Disk World autodoc help
feature_local.c

.SH Description
.SP 5 5

This method adds an item with a night/day description.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/setup and /std/room/inherit/terrain_map/simple_feature.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/terrain_map.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_item%^RESET%^
.EI
.SI 5
void add_item(string item, string day, string night)
.EI

.SI 3
* %^BOLD%^query_day_items%^RESET%^
.EI
.SI 5
string * query_day_items()
.EI
.SP 7 5

This method returns the names of all the items on this feature.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the names of the items

.EP

.SI 3
* %^BOLD%^query_items%^RESET%^
.EI
.SI 5
string * query_items()
.EI
.SP 7 5

This method returns the names of all the items on this feature.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the names of the items

.EP

.SI 3
* %^BOLD%^query_night_items%^RESET%^
.EI
.SI 5
string * query_night_items()
.EI
.SP 7 5

This method returns the names of all the items on this feature.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the names of the items

.EP


