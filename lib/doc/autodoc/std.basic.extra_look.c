.DT
extra_look.c
Disk World autodoc help
extra_look.c

.SH Description
.SP 5 5

Handles adding bits to the long description of objects.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_extra_look%^RESET%^
.EI
.SI 5
void add_extra_look(object ob)
.EI
.SP 7 5

This method adds an object into the list of extra look objects.
ob will need to have a string extra_look( object ) function defined on
it which will actually give the extra look string. ob->extra_look() is
called with the object to which you are adding the extra look.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to add
.EP

.SI 3
* %^BOLD%^calc_extra_look%^RESET%^
.EI
.SI 5
string calc_extra_look()
.EI
.SP 7 5

This method creates the extra long stringsbased on the current list
of objects to use an extra_look on.  The function extra_look() is
called on every object with this_object() passed in as the first
(and only) parameter.  If the result is non-zero it is added onto
the output.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the extra look string

.EP

.SI 3
* %^BOLD%^query_extra_look_list%^RESET%^
.EI
.SI 5
object * query_extra_look_list()
.EI
.SP 7 5

This method returns the current list of objects to be used for
the extra look list.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of extra look objects

.EP

.SI 3
* %^BOLD%^remove_extra_look%^RESET%^
.EI
.SI 5
void remove_extra_look(object ob)
.EI
.SP 7 5

This method removes the object from the list of extra look objects.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to remove
.EP


