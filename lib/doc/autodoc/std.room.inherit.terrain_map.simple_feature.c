.DT
simple_feature.c
Disk World autodoc help
simple_feature.c

.SH Description
.SP 5 5

This type of feature generates distance messages to the feature
based on how far away it is.  It uses a small basic string as the
main part of the description and adds in distance messages to show
how far away the item is.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Apr  1 12:50:02 PST 2002

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/terrain_map/feature_base.
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
* %^BOLD%^query_base_description%^RESET%^
.EI
.SI 5
string query_base_description()
.EI
.SP 7 5

This method returns the basic description for the feature.

.EP

.SI 3
* %^BOLD%^set_base_description%^RESET%^
.EI
.SI 5
void set_base_description(string desc)
.EI
.SP 7 5

This method sets the basic description for the feature.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
desc - the basic description

.EP

.SI 3
* %^BOLD%^set_distance_multiplier%^RESET%^
.EI
.SI 5
void set_distance_multiplier(int mult)
.EI
.SP 7 5

This method sets the distance multiplier for the feature.  This is amount
to multiply the range by to find the actual distance.

.EP


