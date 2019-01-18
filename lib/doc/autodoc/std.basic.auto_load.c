.DT
auto_load.c
Disk World autodoc help
auto_load.c

.SH Description
.SP 5 5

This file has generic methods of accessing auto load information.  This
will save memory and generaly be more useful than the previous version.
It shoudl also solve a lot of the container in container problems,
.EP
.SP 10 5


Written by Pinkfish

Started Wed Feb 17 13:46:35 PST 1999

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_auto_load_value%^RESET%^
.EI
.SI 5
mapping add_auto_load_value(mapping map, string file_name, string tag, mixed value)
.EI
.SP 7 5

This method adds a value into the auto load mapping.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the mapping to add to
.EP
.SP 9 5
file_name - the file name tag of the object doing the adding
.EP
.SP 9 5
tag - the tag name to be used
.EP
.SP 9 5
value - the value to be stored with it.
.EP

.SI 3
* %^BOLD%^query_auto_load_value%^RESET%^
.EI
.SI 5
mixed query_auto_load_value(mapping map, string file_name, string tag)
.EI
.SP 7 5

This method will attempt to read the tag value for the given name out of
the auto load mapping.


This works backwards compatible with the old method of dealing with tags and file names.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the auto load mapping
.EP
.SP 9 5
file_name - the file name tag to use
.EP
.SP 9 5
tag - the tag to use in the file name
.EP


