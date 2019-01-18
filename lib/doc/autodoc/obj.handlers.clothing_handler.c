.DT
clothing_handler.c
Disk World autodoc help
clothing_handler.c

.SH Description
.SP 5 5

This object handles all the different types of clothing, their
locations etc.
.EP
.SP 10 5


Written by Pinkfish

Fiddled!  Actually added the cloak and dresses areas and fixed here and
there.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/clothing.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^can_wear_or_remove%^RESET%^
.EI
.SI 5
string can_wear_or_remove(object thing, object player)
.EI
.SP 7 5

This method returns whether or not a player can wear or remove a given item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the item to be worn or removed.
.EP
.SP 9 5
player - the person doing the wearing or removing.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 for success, a string for failure.

.EP

.SI 3
* %^BOLD%^query_all_clothing_types%^RESET%^
.EI
.SI 5
mapping query_all_clothing_types()
.EI
.SP 7 5

This method returns a mapping of the current clothing types.


The make up of this array is:
.EP
.SP 7 5
<dh>first element 
.EP
.SP 10 5
is the items which if you're wearing this one cannot be removed. <dh>second element 
.EP
.SP 10 5
is the items which hide the given item. 
.EP
.SP 7 5
i.e. The wig type cannot be removed if types hood, helmet or hat are also worn, also the wig type is not visible if the types hood, helmet or hat are worn.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of clothing types
.EP

.SI 3
* %^BOLD%^query_all_clothing_zones%^RESET%^
.EI
.SI 5
mapping query_all_clothing_zones()
.EI
.SP 7 5

This method returns the clothing zones.  The mapping consists of
arrays of types which are in the particular zones.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the clothing zones

.EP

.SI 3
* %^BOLD%^query_all_equivilant_types%^RESET%^
.EI
.SI 5
mapping query_all_equivilant_types()
.EI
.SP 7 5

This method returns all the equivilant types.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the equivilant types

.EP

.SI 3
* %^BOLD%^query_clothing_cannot_remove%^RESET%^
.EI
.SI 5
string * query_clothing_cannot_remove(string type)
.EI
.SP 7 5

This method returns the items you would be unable to remove the
type with.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of clothing to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
array of types you would be unable to remove, 0 if invalid type
.EP

.SI 3
* %^BOLD%^query_clothing_depth%^RESET%^
.EI
.SI 5
int query_clothing_depth(mixed type)
.EI
.SP 7 5

This method returns the depth of the clothing type.  The parament
can also be an array, in which case it will return the minimum
non-zero depth.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of clothing to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the depth rating of the type
.EP

.SI 3
* %^BOLD%^query_clothing_hiding%^RESET%^
.EI
.SI 5
string * query_clothing_hiding(string type)
.EI
.SP 7 5

This method returns the types which hide this type of thingy.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of clothing to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
array of types which would hide this type, 0 if invalid type
.EP

.SI 3
* %^BOLD%^query_clothing_maximum_depth%^RESET%^
.EI
.SI 5
int query_clothing_maximum_depth(mixed type)
.EI
.SP 7 5

This method returns the depth of the clothing type.  The parament
can also be an array, in which case it will return the maximum
non-zero depth.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of clothing to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the depth rating of the type
.EP

.SI 3
* %^BOLD%^query_clothing_type%^RESET%^
.EI
.SI 5
class clothing_type query_clothing_type(string name)
.EI
.SP 7 5

This method returns the information associated with the clothing
of a particular type.


This will returns a class containing the clothing type information in it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the type to lookup
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the clothing type array, 0 if not found
.EP

.SI 3
* %^BOLD%^query_clothing_zone%^RESET%^
.EI
.SI 5
string * query_clothing_zone(string zone)
.EI
.SP 7 5

This method returns the array of clothing types in the particular
zone.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zone - the zone to get the types for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the clothing in the current zone

.EP

.SI 3
* %^BOLD%^query_equivilant_type%^RESET%^
.EI
.SI 5
string query_equivilant_type(string type)
.EI
.SP 7 5

This method returns the equivilant type for the given type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to get the equivilant fo
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the equivilant type

.EP

.SI 3
* %^BOLD%^query_items_blocking%^RESET%^
.EI
.SI 5
object * query_items_blocking(object thing, object player)
.EI
.SP 7 5

This method returns the list of things that are blocking the removal
of the specific object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to find what it is being blocked by
.EP
.SP 9 5
player - the player to look it up on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of objects blocking it

.EP

.SI 3
* %^BOLD%^query_valid_type%^RESET%^
.EI
.SI 5
int query_valid_type(string type)
.EI
.SP 7 5

This method tells us if the type is valid.  It checks both normal
types and equivilant types.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is valid, 0 if not
.EP

.SI 3
* %^BOLD%^query_zone_level%^RESET%^
.EI
.SI 5
int query_zone_level(string zone)
.EI
.SP 7 5

This method returns the level for the specified zone.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zone - the zone to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the level of the zone

.EP

.SI 3
* %^BOLD%^query_zone_name%^RESET%^
.EI
.SI 5
string query_zone_name(string type)
.EI
.SP 7 5

This method finds the zone for the specific type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to find in the zones
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the zone name, 0 if not found

.EP

.SI 3
* %^BOLD%^query_zone_names%^RESET%^
.EI
.SI 5
string * query_zone_names(string type)
.EI
.SP 7 5

This method finds all the zones for the specific type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to find in the zones
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the zone names, 0 if not found

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^add_clothing_type%^RESET%^
.EI
.SI 5
void add_clothing_type(string name, string * not_remove, string * hiding)
.EI
.SP 7 5

This method adds a new clothing type into the existing clothing
type mapping.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the clothing type
.EP
.SP 9 5
not_remove - the types which if worn you cannot remove this item
.EP
.SP 9 5
hiding - the types which hides this clothing

.EP

.SI 3
* %^BOLD%^add_clothing_zone%^RESET%^
.EI
.SI 5
void add_clothing_zone(string zone, string * types)
.EI
.SP 7 5

This method adds a clothing zone.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zone - the zone name
.EP
.SP 9 5
types - the types to add to the zone

.EP

.SI 3
* %^BOLD%^add_equivilant_type%^RESET%^
.EI
.SI 5
void add_equivilant_type(string name, string equiv)
.EI
.SP 7 5

This method adds an equivilant type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the type
.EP
.SP 9 5
equiv - what it is equivilant to

.EP

.SI 3
* %^BOLD%^add_zone_coverage%^RESET%^
.EI
.SI 5
void add_zone_coverage(string type, string * zones)
.EI
.SP 7 5

This method sets the zones that a particular clothing type covers
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the clothing type
.EP


