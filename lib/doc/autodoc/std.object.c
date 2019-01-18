.DT
object.c
Disk World autodoc help
object.c

.SH Description
.SP 5 5

The basic object.  This pulls together all the bits needed to create
a basic object.

.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/std/room.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/enchant, /std/basic/help_files, /std/basic/extra_look, /std/basic/auto_load, /std/basic/id, /std/basic/property, /std/basic/effects, /std/basic/misc, /std/basic/theft_callback and /std/basic/read_desc.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/cmr.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_material%^RESET%^
.EI
.SI 5
void add_material(mixed material)
.EI
.SP 7 5

This method adds a material to the list of current materials.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
material - the material to add

.EP

.SI 3
* %^BOLD%^clean_up%^RESET%^
.EI
.SI 5
int clean_up(int inherited)
.EI

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string word, int dark)
.EI
.SP 7 5

The description of an object when someone looks at it.  This handles
the extra code for messages, colour and material.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the word it was matched on
.EP
.SP 9 5
dark - is it dark from the player's perspecitive
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the long description

.EP

.SI 3
* %^BOLD%^query_cloned_by%^RESET%^
.EI
.SI 5
string query_cloned_by()
.EI
.SP 7 5

Who cloned the object.  Used for security reasoin to trace who have
been cloning up objects for players.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the person who cloned the object

.EP

.SI 3
* %^BOLD%^query_colour%^RESET%^
.EI
.SI 5
string query_colour()
.EI
.SP 7 5

The colour of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the colour of the objecy

.EP

.SI 3
* %^BOLD%^query_material%^RESET%^
.EI
.SI 5
string query_material()
.EI
.SP 7 5

The material of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the material

.EP

.SI 3
* %^BOLD%^query_materials%^RESET%^
.EI
.SI 5
string * query_materials()
.EI
.SP 7 5

This returns all of the materials of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the materials

.EP

.SI 3
* %^BOLD%^query_objective%^RESET%^
.EI
.SI 5
string query_objective()
.EI
.SP 7 5

The objective of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns "it"

.EP

.SI 3
* %^BOLD%^query_possessive%^RESET%^
.EI
.SI 5
string query_possessive()
.EI
.SP 7 5

The possessive of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns "its"

.EP

.SI 3
* %^BOLD%^query_pronoun%^RESET%^
.EI
.SI 5
string query_pronoun()
.EI
.SP 7 5

The pronoun of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns "it"

.EP

.SI 3
* %^BOLD%^query_quality%^RESET%^
.EI
.SI 5
int query_quality()
.EI
.SP 7 5

This method returns the quality of the object.  The quality is used
in the crafts system, amongst other things.

.EP

.SI 3
* %^BOLD%^replace_long_dollars%^RESET%^
.EI
.SI 5
string replace_long_dollars(object player, string text)
.EI
.SP 7 5

This method replaces any colours of materials in the given string
with the appropriate ccolours or materials for this object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player attempting to check the colour/material.
.EP
.SP 9 5
text - the text to replace
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a new string with the dollar identities expanded.

.EP

.SI 3
* %^BOLD%^set_colour%^RESET%^
.EI
.SI 5
void set_colour(string word)
.EI
.SP 7 5

Sets the colour of the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the colour to make the object

.EP

.SI 3
* %^BOLD%^set_material%^RESET%^
.EI
.SI 5
void set_material(mixed word)
.EI
.SP 7 5

Sets the material of the object, this adds this material to the
front of the material list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the material to set it to

.EP

.SI 3
* %^BOLD%^set_name%^RESET%^
.EI
.SI 5
void set_name(string word)
.EI
.SP 7 5

Gives the object a name.  Sets the name to the word and makes it the
short description  and attempts to create the pural short description
from it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name to use

.EP

.SI 3
* %^BOLD%^set_player%^RESET%^
.EI
.SI 5
void set_player(object thing)
.EI
.SP 7 5

Sets the player associated with the object.  This is used during the
autoload sequence.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the player object we belong to
.EP

.SI 3
* %^BOLD%^set_quality%^RESET%^
.EI
.SI 5
void set_quality(int quality)
.EI
.SP 7 5

This method sets the quality of the object.  All objects are by
default 'normal' quality.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
quality - the quality of the object

.EP


