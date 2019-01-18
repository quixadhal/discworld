.DT
wiz_object_comm.c
Disk World autodoc help
wiz_object_comm.c

.SH Description
.SP 5 5

This file conatins all the object based creator commands.
.EP
.SP 10 5


Written by Ember

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/creator.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^affirmative%^RESET%^
.EI
.SI 5
int affirmative(string s)
.EI

.SI 3
* %^BOLD%^del_obvar%^RESET%^
.EI
.SI 5
void del_obvar(string key)
.EI
.SP 7 5

This method deletes the given object variable,
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
key - the object variable to delet.
.EP

.SI 3
* %^BOLD%^get_obvar%^RESET%^
.EI
.SI 5
object get_obvar(string key)
.EI
.SP 7 5

This method returns the object associated witht the object
variable.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
key - the object variablename
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object associated with the name
.EP

.SI 3
* %^BOLD%^get_obvars%^RESET%^
.EI
.SI 5
mapping get_obvars()
.EI
.SP 7 5

This method returns the mapping of all the object variables
currently set on this creator.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the object variables as a mapping
.EP

.SI 3
* %^BOLD%^set_obvar%^RESET%^
.EI
.SI 5
void set_obvar(string key, object value)
.EI
.SP 7 5

this method sets the given object variable to the given
value.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
key - the variable name
.EP
.SP 9 5
value - the object variable name
.EP


