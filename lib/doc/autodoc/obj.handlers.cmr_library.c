.DT
cmr_library.c
Disk World autodoc help
cmr_library.c

.SH Description
.SP 5 5

Handler to keep track of which players know which materials. Allows you to
find out which materials a player knows about and add new ones.  This is
typically used in conjunction with the cmr_handler for materials which
the player must be taught about.

.EP
.SP 10 5


Written by Deutha

.EP



.SH See also
.SP 5 5
cmr_handler
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
* %^BOLD%^add_known_material%^RESET%^
.EI
.SI 5
int add_known_material(string pname, string material)
.EI
.SP 7 5

Mark a material as known by a given player.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - The name of the player
.EP
.SP 9 5
material - The name of the material

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Returns 0 if they already knew the material, 1 if they know it now.

.EP

.SI 3
* %^BOLD%^init_data%^RESET%^
.EI
.SI 5
void init_data(string pname)
.EI
.SP 7 5

Initialise a players array of known materials.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - The name of the player.

.EP

.SI 3
* %^BOLD%^query_known_material%^RESET%^
.EI
.SI 5
int query_known_material(string pname, string material)
.EI
.SP 7 5

Return whether the player knows a particular material.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - The name of the player.
.EP
.SP 9 5
material - The name of the material
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if known 0 if not known.

.EP

.SI 3
* %^BOLD%^query_known_materials%^RESET%^
.EI
.SI 5
string * query_known_materials(string pname)
.EI
.SP 7 5

Return the materials known by a player.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - The name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A string array of known materials.

.EP


