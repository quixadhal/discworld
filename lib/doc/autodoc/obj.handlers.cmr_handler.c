.DT
cmr_handler.c
Disk World autodoc help
cmr_handler.c

.SH Description
.SP 5 5

This handler deals with identifying materials and colours. Your skill and
experience are used to determine if you can successfully identify a
particular material or colour.

.EP
.SP 10 5


Written by Deutha

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/cmr.h, /include/colour.h and /include/library.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_colour%^RESET%^
.EI
.SI 5
string add_colour(string word, int fine, int crude, int qp_thresh, int sp_thresh)
.EI
.SP 7 5

This function is used to add a known colour to the handler.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The name of the colour
.EP
.SP 9 5
fine - Not sure
.EP
.SP 9 5
crude - Not sure
.EP
.SP 9 5
qp_thresh - Quest points required to help identify this colour
.EP
.SP 9 5
sp_thresh - Social points required to help identify this colour

.EP

.SI 3
* %^BOLD%^add_material%^RESET%^
.EI
.SI 5
string add_material(string word, int colour, int type, int skill_reqd, int skill_thresh)
.EI
.SP 7 5

Add a new material to the handler.
The colour of the material is an index into the COLOUR array defined in
colour.h. The type of material is an index into the MATERIALS array defined
in cmr.h. skill_reqd is an index into the SKILLS array defined in cmr.h but
can also take the special value of 99 to indicate that the material can only
be identified if someone has told the person what the material is or 0 to
indicate that everyone knows what the material is.
If the skill_reqd is a skill the skill_thresh is the bonus needed in that
skill to identify the material.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The name of the material
.EP
.SP 9 5
colour - The index into the colour array for the colour of the
                material. (see colour.h)
.EP
.SP 9 5
type - Type of material
.EP
.SP 9 5
skill_reqd - Index into the array of skills (see cmr_handler.h)
.EP
.SP 9 5
skill_thresh - The bonus required in the appropriate skill.

.EP

.SI 3
* %^BOLD%^delete_colour%^RESET%^
.EI
.SI 5
int delete_colour(string word)
.EI
.SP 7 5

Remove a colour from the handler.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The colour to remove.

.EP

.SI 3
* %^BOLD%^delete_material%^RESET%^
.EI
.SI 5
int delete_material(string word)
.EI
.SP 7 5

Remove a material from the handler.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The name of the material to remove.

.EP

.SI 3
* %^BOLD%^identify_colour%^RESET%^
.EI
.SI 5
string identify_colour(string word, object player)
.EI
.SP 7 5

Used to determine if a player can identify a particular colour. The
function returns the colours name or a string giving a rough description
if they're unable to identify it.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The colour they're trying to identify.
.EP
.SP 9 5
player - The player trying to identify the colour.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Description.

.EP

.SI 3
* %^BOLD%^identify_material%^RESET%^
.EI
.SI 5
string identify_material(string word, object player, int article)
.EI
.SP 7 5

Used to determine if a player can identify a particular material.
The function returns the materials name or a string giving a rough
description if they're unable to identify it.

Some materials can be identified by being high enough at a particular skill
(defined in cmr.h) while others are only learned by being taught and others
are known to everyone.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The name of the material
.EP
.SP 9 5
player - The player trying to identify it.
.EP
.SP 9 5
article - Definite or indefinite article.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Description.

.EP

.SI 3
* %^BOLD%^query_colour_details%^RESET%^
.EI
.SI 5
int * query_colour_details(string word)
.EI
.SP 7 5

Returns the array of information stored about a particular colour. The
array is setup using add_colour.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The colour in question
.EP

.SI 3
* %^BOLD%^query_colour_names%^RESET%^
.EI
.SI 5
string * query_colour_names()
.EI
.SP 7 5

List all the known colours in the handler.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
An array of strings of colour names.

.EP

.SI 3
* %^BOLD%^query_material_adjective%^RESET%^
.EI
.SI 5
string query_material_adjective(string word)
.EI
.SP 7 5

Not sure about this. Seems to return a colour and adjective for a material.
eg. slate returns 'grey mineral'.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
The - name of the material.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A description.

.EP

.SI 3
* %^BOLD%^query_material_ansi_colour%^RESET%^
.EI
.SI 5
string query_material_ansi_colour(string word)
.EI
.SP 7 5

Returns the ansi colour associated with the material eg. yellow for gold,
red for a red mineral pebble etc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the material's name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
ansi colour sequence for that colour.

.EP

.SI 3
* %^BOLD%^query_material_details%^RESET%^
.EI
.SI 5
int * query_material_details(string word)
.EI
.SP 7 5

Return the details of a material set with add_material

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The material in question
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
An integer array of material details.

.EP

.SI 3
* %^BOLD%^query_material_names%^RESET%^
.EI
.SI 5
string * query_material_names()
.EI
.SP 7 5

List all the known materials by name.

.EP


