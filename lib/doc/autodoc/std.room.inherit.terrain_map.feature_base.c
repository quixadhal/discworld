.DT
feature_base.c
Disk World autodoc help
feature_base.c

.SH Description
.SP 5 5

This is a stand alone feature handler for dealing with features that
span over multiple terrains.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Apr  1 12:15:24 PST 2002

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/terrain_map/distance_str.
.EP

.SH Classes

.SI 3
* %^BOLD%^feature_pos%^RESET%^
class feature_pos {
int xstart;
int ystart;
int width;
int height;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^calc_feature_desc%^RESET%^
.EI
.SI 5
string calc_feature_desc(mapping direc, int night, int visibility)
.EI
.SP 7 5

This function needs to be overridden to display the actual
message to the feature.  The direc argument is a mapping of the
form, key = direction, value = distance.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
direc - the directions and distances
.EP
.SP 9 5
night - the night feature
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the feature descriptions

.EP

.SI 3
* %^BOLD%^filter_distances%^RESET%^
.EI
.SI 5
mapping filter_distances(mapping direcs)
.EI
.SP 7 5

This method filters the returned descs to make sure they are all
within the needed range.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
direcs - the mappings of directions and distance

.EP

.SI 3
* %^BOLD%^is_inside_region%^RESET%^
.EI
.SI 5
int is_inside_region(int x1, int y1, int x2, int y2)
.EI
.SP 7 5

This method determines if the feature is inside the specified region
or not.
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
* %^BOLD%^query_blocking%^RESET%^
.EI
.SI 5
int query_blocking()
.EI
.SP 7 5

This method returns the blocking status of the feature.  This will only work
with local features and not distant features.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the blocking status of the features

.EP

.SI 3
* %^BOLD%^query_feature_desc_from%^RESET%^
.EI
.SI 5
mapping query_feature_desc_from(int x, int y, int z, int no_filter)
.EI
.SP 7 5

This is the function used by the system to find the message for this
feature.
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
.SP 9 5
no_filter - whether or not to filter by distance

.EP

.SI 3
* %^BOLD%^query_max_day_range%^RESET%^
.EI
.SI 5
int query_max_day_range()
.EI
.SP 7 5

This method returns the maximum range the feature can be seen at.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum range of the feature

.EP

.SI 3
* %^BOLD%^query_max_night_range%^RESET%^
.EI
.SI 5
int query_max_night_range()
.EI
.SP 7 5

This method returns the maximum range the feature can be seen at.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum range of the feature

.EP

.SI 3
* %^BOLD%^query_max_range%^RESET%^
.EI
.SI 5
int query_max_range()
.EI
.SP 7 5

This method returns the maximum possible range the item can be seen
at.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum possible range

.EP

.SI 3
* %^BOLD%^query_min_range%^RESET%^
.EI
.SI 5
int query_min_range()
.EI
.SP 7 5

This method returns the minimum range the  feature can be seen at.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the minimum range of the feature

.EP

.SI 3
* %^BOLD%^query_range_offset%^RESET%^
.EI
.SI 5
int query_range_offset()
.EI
.SP 7 5

This method returns the range offset.  The offset is added to the
distance of the feature for calculating the distance.  If the
point is inside the offset the distance is ignored and the
distance it then calculated from the edge of the offset.  This
can be used to generate circular features.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current range offset

.EP

.SI 3
* %^BOLD%^query_region_ob%^RESET%^
.EI
.SI 5
object query_region_ob()
.EI
.SP 7 5

This method returns the region object.  This should be used
for setup calls.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the region object

.EP

.SI 3
* %^BOLD%^set_blocking%^RESET%^
.EI
.SI 5
int set_blocking(int blocking)
.EI
.SP 7 5

This method sets the blocking status of the feature.  This will only work
with local features and not distant features.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
blocking - the blocking flag

.EP

.SI 3
* %^BOLD%^set_max_day_range%^RESET%^
.EI
.SI 5
void set_max_day_range(int range)
.EI
.SP 7 5

This method sets the maximum range for the feature to be seen at.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
range - the maximum range to be seen at

.EP

.SI 3
* %^BOLD%^set_max_night_range%^RESET%^
.EI
.SI 5
void set_max_night_range(int range)
.EI
.SP 7 5

This method sets the maximum range for the feature to be seen at.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
range - the maximum range to be seen at

.EP

.SI 3
* %^BOLD%^set_min_range%^RESET%^
.EI
.SI 5
void set_min_range(int range)
.EI
.SP 7 5

This method sets the minimum range for the feature to be seen at.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
range - the minimum range to be seen at

.EP

.SI 3
* %^BOLD%^set_range_offset%^RESET%^
.EI
.SI 5
void set_range_offset(int offset)
.EI
.SP 7 5

This method sets the range offset.  The offset is added to the
distance of the feature for calculating the distance.  If the
point is inside the offset the distance is ignored and the
distance it then calculated from the edge of the offset.  This
can be used to generate circular features.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
offset - the offset to use

.EP

.SI 3
* %^BOLD%^set_region%^RESET%^
.EI
.SI 5
void set_region(mixed region)
.EI
.SP 7 5

This method sets the region to use for the system.  If the region is
set as a string, then that region is cloned, if it is set as and
object, it is just used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
region - the region to use

.EP


