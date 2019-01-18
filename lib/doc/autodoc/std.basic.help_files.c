.DT
help_files.c
Disk World autodoc help
help_files.c

.SH Description
.SP 5 5

This inheritable allows objects or rooms to have help attached to them.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nroff.h and /include/room.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_help_file%^RESET%^
.EI
.SI 5
void add_help_file(string help_file)
.EI
.SP 7 5

This method sets up the help for the room.  All the help file names are
relative to /doc/room unless a complete path is specified.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
help_file - the help file name

.EP

.SI 3
* %^BOLD%^help_function%^RESET%^
.EI
.SI 5
mixed * help_function()
.EI
.SP 7 5

This method returns the help data as a function pointer.
The function pointer needs to be evaluated to create the actual text.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the function pointer for the help text

.EP

.SI 3
* %^BOLD%^help_string%^RESET%^
.EI
.SI 5
string help_string()
.EI
.SP 7 5

This method returns the help as a string.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the help as a string

.EP

.SI 3
* %^BOLD%^query_help_file_directory%^RESET%^
.EI
.SI 5
string query_help_file_directory()
.EI
.SP 7 5

This method is used to get the directory for the help files.
This should be overridden in higher up objects that use this object.

.EP

.SI 3
* %^BOLD%^query_help_files%^RESET%^
.EI
.SI 5
string query_help_files()
.EI
.SP 7 5

This method returns the help files for the current room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of help files

.EP

.SI 3
* %^BOLD%^remove_help_file%^RESET%^
.EI
.SI 5
void remove_help_file(string help_file)
.EI
.SP 7 5

This method removes the specified help file from the current ones.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
help_file - the help file name to remove

.EP


