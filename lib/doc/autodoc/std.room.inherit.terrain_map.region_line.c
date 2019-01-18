.DT
region_line.c
Disk World autodoc help
region_line.c

.SH Description
.SP 5 5

This is region handler for a polygon region.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Apr  1 12:15:24 PST 2002

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/terrain_map/geometry.
.EP

.SH Classes

.SI 3
* %^BOLD%^vertex%^RESET%^
class vertex {
int x;
int y;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_vertex%^RESET%^
.EI
.SI 5
void add_vertex(int x, int y)
.EI
.SP 7 5

This method sets the position of the feature.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x coordinate of the feature
.EP
.SP 9 5
y - the y coordinate of the feature
.EP
.SP 9 5
width - the width of the feature
.EP
.SP 9 5
height - the height of the feature

.EP

.SI 3
* %^BOLD%^is_inside_region%^RESET%^
.EI
.SI 5
int is_inside_region(int x1, int y1, int x2, int y2, int range)
.EI
.SP 7 5

Find out if this polygon is inside the region, given the specified range
at which it can be seen.  This will work by adding the ranges onto the
lines and checking for distance from the points of the polygon to the line
bordering the region and also checking for intersection with the bordering
line.  It also checks to see if all of the points are inside the region.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x1 - the top x
.EP
.SP 9 5
y1 - the top y
.EP
.SP 9 5
x2 - the bottom x
.EP
.SP 9 5
y2 - the bottom y

.EP

.SI 3
* %^BOLD%^query_feature_desc_from%^RESET%^
.EI
.SI 5
mapping query_feature_desc_from(int x, int y, int z)
.EI
.SP 7 5

This is the function that finds the distance and direction to the
current region.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x-coordinate
.EP
.SP 9 5
y - the y-coordinate
.EP
.SP 9 5
z - the z-coordinate
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
({ direction, distance [, direc2, dist2, [...]] })

.EP


