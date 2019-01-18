.DT
path.c
Disk World autodoc help
path.c

.SH Description
.SP 5 5

This method expands the specified path to get a nice new path
based on the home directory or expanding the ~ stuff.
.EP
.SP 10 5

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^get_cfiles%^RESET%^
.EI
.SI 5
string * get_cfiles(string str)
.EI
.SP 7 5

This method expands the wildcard based file name and turns it
into a list of c files.  This automaticaly adds a .c to any
files.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the wildcard based file name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of file names

.EP

.SI 3
* %^BOLD%^get_files%^RESET%^
.EI
.SI 5
string * get_files(string str)
.EI
.SP 7 5

This method expands the wildcard based file name and turns it
into a list of files
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the wildcard based file name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of file names

.EP

.SI 3
* %^BOLD%^get_path%^RESET%^
.EI
.SI 5
string get_path(string str)
.EI

.SI 3
* %^BOLD%^query_current_path%^RESET%^
.EI
.SI 5
string query_current_path()
.EI
.SP 7 5

This method returns the current path.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current path

.EP

.SI 3
* %^BOLD%^query_home_dir%^RESET%^
.EI
.SI 5
string query_home_dir()
.EI
.SP 7 5

This method returns the home directory of the creator.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the home directory

.EP

.SI 3
* %^BOLD%^set_current_path%^RESET%^
.EI
.SI 5
void set_current_path(string str)
.EI
.SP 7 5

This method sets the current path for the creator.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the current path

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^set_home_dir%^RESET%^
.EI
.SI 5
int set_home_dir(string dir)
.EI
.SP 7 5

This method sets the current home directory of the creator.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the new home directory

.EP


