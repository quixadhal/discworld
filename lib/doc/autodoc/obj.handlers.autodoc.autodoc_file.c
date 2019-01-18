.DT
autodoc_file.c
Disk World autodoc help
autodoc_file.c

.SH Description
.SP 5 5

The automatic document generator.  It takes source files from various
directories and creates help files from the comments embeded in the
code.

.EP
.SP 10 5


Written by Pinkfish

Started Fri Oct 24 16:03:57 EDT 1997

.EP



.SH See also
.SP 5 5
/obj/handlers/autodoc/autodoc_handler
.EP
.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^load_file%^RESET%^
.EI
.SI 5
void load_file()
.EI
.SP 7 5

Loads up the currently set file name from the archives.

.EP

.SI 3
* %^BOLD%^parse_file%^RESET%^
.EI
.SI 5
void parse_file(string name, function func, int only_load)
.EI
.SP 7 5

Parses the input file figuring out all the documentation bits of it.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the file to parse
.EP
.SP 9 5
func - the function to call when the parsing is finished
.EP
.SP 9 5
only_load - a flag telling us to only load the information

.EP

.SI 3
* %^BOLD%^query_changed%^RESET%^
.EI
.SI 5
int query_changed()
.EI
.SP 7 5

Did the file change?  Checks to see if the file changed since it
was last read.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it changed, 0 if it has not changed

.EP

.SI 3
* %^BOLD%^query_class_docs%^RESET%^
.EI
.SI 5
mapping query_class_docs()
.EI
.SP 7 5

The documentation for the classes.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of the class names to documentation

.EP

.SI 3
* %^BOLD%^query_define_docs%^RESET%^
.EI
.SI 5
mapping query_define_docs()
.EI
.SP 7 5

The documentation for the defines.  This is mostly used by the include
file documentation system.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of define names to documentation

.EP

.SI 3
* %^BOLD%^query_defines%^RESET%^
.EI
.SI 5
mapping query_defines()
.EI
.SP 7 5

The defines which were setup in the class.  This is the mapping of the
defines which were processed.  The format of the mapping is
([ "name" : "value" ]), where the name is the name of the define and
the value is what to replace it with.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of defines

.EP

.SI 3
* %^BOLD%^query_file_name%^RESET%^
.EI
.SI 5
string query_file_name()
.EI
.SP 7 5

The file name being processed.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the file being processed

.EP

.SI 3
* %^BOLD%^query_includes%^RESET%^
.EI
.SI 5
string * query_includes()
.EI
.SP 7 5

The files included by this one.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of included files

.EP

.SI 3
* %^BOLD%^query_inherits%^RESET%^
.EI
.SI 5
mapping query_inherits()
.EI
.SP 7 5

Returns the inherits mapping of the system.  This returns a mapping of the
form ([ inherit_name : ({ flags }) ]).  Where the name of the inherit is
something like "/std/object" and the flags are things you can apply to
an inherit, like "protected" or "private".  If there are no flags then
the flags will be an empty array.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping of things inherited by this file

.EP

.SI 3
* %^BOLD%^query_main_docs%^RESET%^
.EI
.SI 5
mapping query_main_docs()
.EI
.SP 7 5

Returns the main docs for the class.  The mapping is of
the form ([ "tag" : ({ data }) ]), where each reference
to a tag creates a new element in the data array.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping containing the main docs for the file

.EP

.SI 3
* %^BOLD%^query_num_failed_tries%^RESET%^
.EI
.SI 5
int query_num_failed_tries()
.EI
.SP 7 5

This method returns the number of times the file was attempted to
be read and it was discovered not to exist at all.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of times it was unable to be read

.EP

.SI 3
* %^BOLD%^query_private_functions%^RESET%^
.EI
.SI 5
mapping query_private_functions()
.EI
.SP 7 5

The mapping of private functions.
The mapping is of the form ([ func_name : ({ type, args, docs }) ]).
The type bit is an array of the type name, ie: ({ "int" }) or
({ "mixed", "*" }).  The args bit looks like ({ "name", type }),
where the type is the same as in the previous array.  The docs is
a mapping of the form ([ "tag" : ({ data }) ]), where each reference
to a tag creates a new element in the data array.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping containing the private functions

.EP

.SI 3
* %^BOLD%^query_protected_functions%^RESET%^
.EI
.SI 5
mapping query_protected_functions()
.EI
.SP 7 5

The mapping of protected functions.
The mapping is of the form ([ func_name : ({ type, args, docs }) ]).
The type bit is an array of the type name, ie: ({ "int" }) or
({ "mixed", "*" }).  The args bit looks like ({ "name", type }),
where the type is the same as in the previous array.  The docs is
a mapping of the form ([ "tag" : ({ data }) ]), where each reference
to a tag creates a new element in the data array.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping containing the protected functions

.EP

.SI 3
* %^BOLD%^query_public_functions%^RESET%^
.EI
.SI 5
mapping query_public_functions()
.EI
.SP 7 5

The mapping of public functions.
The mapping is of the form ([ func_name : ({ type, args, docs }) ]).
The type bit is an array of the type name, ie: ({ "int" }) or
({ "mixed", "*" }).  The args bit looks like ({ "name", type }),
where the type is the same as in the previous array.  The docs is
a mapping of the form ([ "tag" : ({ data }) ]), where each reference
to a tag creates a new element in the data array.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping containing the public functions

.EP

.SI 3
* %^BOLD%^save_file%^RESET%^
.EI
.SI 5
void save_file()
.EI
.SP 7 5

Saves the current file name to the archives.

.EP


