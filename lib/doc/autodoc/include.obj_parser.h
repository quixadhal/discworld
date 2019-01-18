.DT
obj_parser.h
Disk World autodoc help
obj_parser.h

.SH Description
.SP 5 5

THis is the header file for the object parser.  It is based on some
code by Scatter at Dawn Whispers to achive it's parsing.

.EP
.SP 5 5
File name    : newparser.h
.EP
.SP 5 5
Creator      : Scatter
.EP
.SP 5 5
Creation date: 2000-04-20
.EP
.SP 5 5
Revision history: Date Creator Desc 2000-04-20 Scatter File created. 2000-05-20 Pinkfish Modified for Discworld 
.EP
.SP 10 5

.EP

.SH Classes

.SI 3
* %^BOLD%^obj_match%^RESET%^
class obj_match {
int result;
object * objects;
string text;
}

.EI
.SP 7 5

The class used for object matching returns.
.EP

.SI 3
* %^BOLD%^obj_match_context%^RESET%^
class obj_match_context {
int ordinal;
int number_included;
int * fraction;
int ignore_rest;
int no_ambiguous;
object him;
object her;
object it;
object * plural;
}

.EI
.SP 7 5

This class used to retain context over various invocations of the parser.
.EP


.SH Defines
.SI 3
* %^BOLD%^OBJ_PARSER_AMBIGUOUS%^RESET%^
.EI
.SP 7 5

THis means an ambiguous match was noticed.  An ambiguous match is
where 'bag' matches more than one bag and the player did not force
it to recognise a specific one.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_AMBIGUOUS_PROP%^RESET%^
.EI
.SP 7 5

This property controls if the player will be informed of ambiguous
references or not.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_BAD_ENVIRONMENT%^RESET%^
.EI
.SP 7 5

This is only used inside the player objects.  Means that you cannot do
remove player actions from this location.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_BAD_FRACTION%^RESET%^
.EI
.SP 7 5

The fraction specified is invalid.  The fraction is in the text field of
the class.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_FRACTION%^RESET%^
.EI
.SP 7 5

It is not possible to do fractional matches on multiple objects.
The objects field has the objects it failed to match on and the text it
failed to match on.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_MATCH_PLURAL%^RESET%^
.EI
.SP 7 5
The flag to say that the match is a plural id. 
.EP

.SI 3
* %^BOLD%^OBJ_PARSER_MATCH_SINGULAR%^RESET%^
.EI
.SP 7 5
The flag to say that the match is a singular id. 
.EP

.SI 3
* %^BOLD%^OBJ_PARSER_MATCH_TYPE%^RESET%^
.EI
.SP 7 5
The index of the match type in the return array. 
.EP

.SI 3
* %^BOLD%^OBJ_PARSER_NO_MATCH%^RESET%^
.EI
.SP 7 5

There was no match on the string in the text field of the obj_match
class.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_NOT_ENOUGH%^RESET%^
.EI
.SP 7 5

This means that not enough of the specified object were matched.  For
example if someone did '5 coins' and only 3 were matched.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_NOT_LIVING%^RESET%^
.EI
.SP 7 5

This is only used inside the player object.  It says that the matched
objects were not living.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_OBJECTS%^RESET%^
.EI
.SP 7 5
The index of the objects in the return array. 
.EP

.SI 3
* %^BOLD%^OBJ_PARSER_SUCCESS%^RESET%^
.EI
.SP 7 5
The return result was correctly matched. 
.EP

.SI 3
* %^BOLD%^OBJ_PARSER_TOO_DARK%^RESET%^
.EI
.SP 7 5

It is too dark to match on the string in the text field of the obj_match
class.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_TYPE_EXISTENCE%^RESET%^
.EI
.SP 7 5

The value of the singular parameter, means it matches everything and 
ignores failure results.  These are used as flags.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_TYPE_LIVING%^RESET%^
.EI
.SP 7 5

Only match living things.  These are used as flags.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_TYPE_NO_NESTED%^RESET%^
.EI
.SP 7 5

Disallow nested matching.  These are used as flags.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_TYPE_OBJECT%^RESET%^
.EI

.SI 3
* %^BOLD%^OBJ_PARSER_TYPE_PLAYER%^RESET%^
.EI
.SP 7 5
Match players.  These are used as flags. 
.EP

.SI 3
* %^BOLD%^OBJ_PARSER_TYPE_SLOPPY_MATCHING%^RESET%^
.EI
.SP 7 5

Allow things like 1 carrots to match.  These are used as flags.

.EP

.SI 3
* %^BOLD%^OBJ_PARSER_USE_AND_AS_BREAK_PROP%^RESET%^
.EI
.SP 7 5

This property controls if the player will be use the 'and' keyword as
a break or as a regular character.

.EP

