.DT
property.c
Disk World autodoc help
property.c

.SH Description
.SP 5 5

The property handling system.  This handles all the stuff to do with
properties.

.EP
.SP 10 5


Written by Pinkfish
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_properties%^RESET%^
.EI
.SI 5
void add_properties(mapping map)
.EI
.SP 7 5

Adds a mapping of properties in, instead of just one.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the mapping of properties

.EP

.SI 3
* %^BOLD%^add_property%^RESET%^
.EI
.SI 5
varargs int add_property(string var, mixed val, int tim)
.EI
.SP 7 5

Adds a property onto the object.  Allows you to add a property
onto the current object.  You can also set a time for the property
to last.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
var - the name of the property
.EP
.SP 9 5
val - the value to give the property
.EP
.SP 9 5
tim - the time before the property should time out

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it succeeded, 0 if it failed.

.EP

.SI 3
* %^BOLD%^query_is_timed_property%^RESET%^
.EI
.SI 5
int query_is_timed_property(string str)
.EI
.SP 7 5

Is this property value timed?

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the property to check

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is, 0 if not
.EP

.SI 3
* %^BOLD%^query_properties%^RESET%^
.EI
.SI 5
mapping query_properties()
.EI
.SP 7 5

returns a mapping with all the properties in it.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping of all the properties

.EP

.SI 3
* %^BOLD%^query_property%^RESET%^
.EI
.SI 5
mixed query_property(string str)
.EI
.SP 7 5

Returns the value of the property.  This will return the current value
of the named property.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the property to find
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value of the property
.EP

.SI 3
* %^BOLD%^query_property_exists%^RESET%^
.EI
.SI 5
int query_property_exists(string str)
.EI
.SP 7 5

Does this property exist?

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the property to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the property exists, 0 if it does not

.EP

.SI 3
* %^BOLD%^query_property_stats%^RESET%^
.EI
.SI 5
mixed query_property_stats(string var)
.EI
.SP 7 5

Returns some facts about the property, in the format
({ Filename of object which added stat, time added, time expiring })

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
The - property statistics are wanted on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
That stuff just mentioned.

.EP

.SI 3
* %^BOLD%^query_property_time_left%^RESET%^
.EI
.SI 5
int query_property_time_left(string str)
.EI
.SP 7 5

If this is a timed property it returns the amount of time left.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the property name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount ot time left

.EP

.SI 3
* %^BOLD%^query_stats%^RESET%^
.EI
.SI 5
mapping query_stats()
.EI
.SP 7 5

Return all property statistics

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A mapping full of stats
.EP

.SI 3
* %^BOLD%^query_timed_properties%^RESET%^
.EI
.SI 5
mapping query_timed_properties()
.EI
.SP 7 5

Returns a mapping of all the timed properties.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping of all the timed properties

.EP

.SI 3
* %^BOLD%^remove_property%^RESET%^
.EI
.SI 5
int remove_property(string var)
.EI
.SP 7 5

Removes a property off the object.  This removes the named property
from the object.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
var - the name of the property
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^set_properties%^RESET%^
.EI
.SI 5
void set_properties(mapping map)
.EI
.SP 7 5

Resets all properties
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the mapping of properties

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^reload_player_properties%^RESET%^
.EI
.SI 5
void reload_player_properties()
.EI
.SP 7 5

Fixes the times back up again after it has been loaded.

.EP

.SI 3
* %^BOLD%^save_player_properties%^RESET%^
.EI
.SI 5
void save_player_properties()
.EI
.SP 7 5

Updates the timeouts and stuff before saving the player object.

.EP


