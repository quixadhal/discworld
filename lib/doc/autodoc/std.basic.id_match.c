.DT
id_match.c
Disk World autodoc help
id_match.c

.SH Description
.SP 5 5

This inheritable contains all the code to deal with matching objects in
the parser.
.EP
.SP 10 5


Written by Pinkfish.

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/obj_parser.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^parse_match_object%^RESET%^
.EI
.SI 5
mixed parse_match_object(string * input, object viewer, class obj_match_context context)
.EI
.SP 7 5

This code is for the new parser...  It returns an array which contains
two elements.  The first is the type of the match, plural or singular,
and the second is the object it matched.  Usualy this_object() but
it can be used to swap new objects.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
input - the input string
.EP
.SP 9 5
viewer - the person viewing the object
.EP
.SP 9 5
context - the contex this is to be taken in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
({ type, ob }), 0 on failure

.EP

.SI 3
* %^BOLD%^query_no_all%^RESET%^
.EI
.SI 5
int query_no_all()
.EI
.SP 7 5

This method returns the no all flag's current value.
If this flag is set to true then the object
will not respond to 'all' requests and must be accessed with it's real
name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current value of the flag

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^is_matching_object%^RESET%^
.EI
.SI 5
int is_matching_object(string * input, object viewer, class obj_match_context context)
.EI
.SP 7 5

This method is used by upper objects that only wish to match the
ids and not mess with all the context stuff.  This method will
return a combination of OBJ_PARSER_MATCH_SINGULAR and
OBJ_PARSER_MATCH_PLURAL.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
input - the input string
.EP
.SP 9 5
viewer - the person viewing the object
.EP
.SP 9 5
context - the contex this is to be taken in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the singular/plural match flags

.EP

.SI 3
* %^BOLD%^set_no_all%^RESET%^
.EI
.SI 5
void set_no_all(int no_all)
.EI
.SP 7 5

This sets the no all flag.  If this flag is set to true then the object
will not respond to 'all' requests and must be accessed with it's real
name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
no_all - the no all flag value

.EP

.SI 3
* %^BOLD%^update_parse_match_context%^RESET%^
.EI
.SI 5
int update_parse_match_context(class obj_match_context context, int num, int singular)
.EI
.SP 7 5

This method updates the context.  It returns the number of the objects
we match, and you pass in the number of objects that we are.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
context - the context to update
.EP
.SP 9 5
num - the number of objects
.EP
.SP 9 5
singular - return from is_matching_object
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of matched objects

.EP


