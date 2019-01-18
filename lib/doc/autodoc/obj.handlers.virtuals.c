.DT
virtuals.c
Disk World autodoc help
virtuals.c

.SH Description
.SP 5 5

This handler fixes up virtual objects if their files change, so that their static variables will be adjusted.
.EP
.SP 10 5


Written by Dragonkin


.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_data%^RESET%^
.EI
.SI 5
void add_data(string path)
.EI
.SP 7 5

Adds a path and its static auto load information to the mapping
and cleans the mapping and paths array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the path of the object to add

.EP

.SI 3
* %^BOLD%^add_forbidden%^RESET%^
.EI
.SI 5
int add_forbidden(string path)
.EI
.SP 7 5

Called to add a path to the list of forbidden paths.  A forbidden virtual object being loaded will transform into a receipt.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the path of the virtual object to forbid
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the path is added, 0 if it is already in the list

.EP

.SI 3
* %^BOLD%^add_missing%^RESET%^
.EI
.SI 5
int add_missing(string path)
.EI
.SP 7 5

Called by the handler to add a faulty path to the list
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the path of the virtual object
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if added, 0 if not (because it's already in the list or in a /w directory)

.EP

.SI 3
* %^BOLD%^new_data%^RESET%^
.EI
.SI 5
mapping new_data(string path)
.EI
.SP 7 5

Called by virtual objects as they are being cloned, if their timestamp is older than their file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the path of the virtual object
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object's static auto load info if any, otherwise 0

.EP

.SI 3
* %^BOLD%^query_forbidden%^RESET%^
.EI
.SI 5
int query_forbidden(string path)
.EI
.SP 7 5

A function to see if a path is forbidden
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the path of the virtual object to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if forbidden, 0 if not

.EP

.SI 3
* %^BOLD%^query_forbidden_paths%^RESET%^
.EI
.SI 5
string * query_forbidden_paths()
.EI
.SP 7 5

A function to see what paths are forbidden
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all forbidden paths in the handler

.EP

.SI 3
* %^BOLD%^query_missing_paths%^RESET%^
.EI
.SI 5
string * query_missing_paths()
.EI
.SP 7 5

A function to see what items have faulty paths
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all faulty paths in the handler

.EP

.SI 3
* %^BOLD%^remove_forbidden%^RESET%^
.EI
.SI 5
void remove_forbidden(string path)
.EI
.SP 7 5

Called to remove a forbidden path from the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the path of the virtual object

.EP

.SI 3
* %^BOLD%^remove_missing%^RESET%^
.EI
.SI 5
void remove_missing(string path)
.EI
.SP 7 5

Called to remove a faulty path from the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the path of the virtual object

.EP


