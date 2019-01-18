.DT
soul_compiler.c
Disk World autodoc help
soul_compiler.c

.SH Description
.SP 5 5

This compiles the soul files into a non human readable format.   The
format is far more useful to the soul though :)

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/user_parser.h and /include/soul.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^compile_directory%^RESET%^
.EI
.SI 5
void compile_directory(string fname)
.EI
.SP 7 5

Compiles up a directory full of files.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the directory name to compile

.EP

.SI 3
* %^BOLD%^compile_file%^RESET%^
.EI
.SI 5
void compile_file(string fname)
.EI
.SP 7 5

Compiles up the file into the useful soul format.   It also tells
the soul about it.


See the soul data files in /save/new_soul for details of the format for the soul files. The file has to come from the soul save directory or the call will not work.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the name of the file to compile

.EP


