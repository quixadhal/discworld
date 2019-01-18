.DT
potion.c
Disk World autodoc help
potion.c

.SH Description
.SP 5 5

 This class is the potion information handler.


This will handle all the information related to potions: the skill required, the failure result, etc. (more to be added as necessary). Each potion will be an object in itself (generally a virtual object). There will be an index file which gives the pathname of the object for each potion "title". This will be loaded into a mapping on creation of the handler. The actual potion objects will be created as they are referenced (assuming they're in the index) and stored as the value of another mapping.
.EP
.SP 10 5


Written by Jeremy

Started ??/??/95

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^clone_reagent%^RESET%^
.EI
.SI 5
mixed clone_reagent(string s)
.EI
.SP 7 5

This method creates a reagent object.  It creates the potion object and
then creates the reagent on it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
potion - the potion to create
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the regent object

.EP

.SI 3
* %^BOLD%^query_chant%^RESET%^
.EI
.SI 5
string query_chant(string s)
.EI
.SP 7 5

This method returns the chant of the potion name.  The
expand_nickname function will be done on the input name before
looking it up.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the name to lookup
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the chant
.EP

.SI 3
* %^BOLD%^query_min_level%^RESET%^
.EI
.SI 5
int query_min_level(string s)
.EI
.SP 7 5

This method returns the minimum level of the potion name.  The
expand_nickname function will be done on the input name before
looking it up.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the name to lookup
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the minimum level
.EP

.SI 3
* %^BOLD%^query_paths%^RESET%^
.EI
.SI 5
mapping query_paths()
.EI
.SP 7 5

This method returns the paths that are searched for potion
objects.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of paths

.EP

.SI 3
* %^BOLD%^query_potion%^RESET%^
.EI
.SI 5
object query_potion(string s)
.EI
.SP 7 5

This method clones up a potion object.  It does not setup the reagent
in the potion.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the potion to create

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the potion object
.EP

.SI 3
* %^BOLD%^query_potions%^RESET%^
.EI
.SI 5
mapping query_potions()
.EI
.SP 7 5

This method returns the potion mapping.  This the list of known
potions.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the potion mapping

.EP


