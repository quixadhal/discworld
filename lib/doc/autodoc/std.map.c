.DT
map.c
Disk World autodoc help
map.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
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
* %^BOLD%^set_map%^RESET%^
.EI
.SI 5
void set_map(string area)
.EI
.SP 7 5

Function to set the map to one of the standard areas.  If you wish to
have the map cover a custom area use setup_map.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area of Discworld the map should cover.

.EP

.SI 3
* %^BOLD%^setup_map%^RESET%^
.EI
.SI 5
void setup_map(int x1, int y1, int x2, int y2, int detail)
.EI
.SP 7 5

Function to setup the details of the map.
The first two params give the coordinates of the top left corner
of the map and the second two the coordinates of the lower right corner.
The detail indicates the level of detail. This is given in terms of the
distance from which objects are visible. For example a low-detail map
might show objects that are visible from 200 miles or more whereas a
more detailed map may show items down to 50 miles visibility.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x1 - x part of the top left corner
.EP
.SP 9 5
y1 - y part of the top left corner
.EP
.SP 9 5
x2 - x part of the lower right corner
.EP
.SP 9 5
y2 - y part of the lower right corner

.EP


