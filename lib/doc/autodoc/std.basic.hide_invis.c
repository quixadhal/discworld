.DT
hide_invis.c
Disk World autodoc help
hide_invis.c

.SH Description
.SP 5 5

This method returns the current hide/invisible states on the object.
The mapping is of the format:\n

.EP
.SP 5 5
([ <hide type name> :
.EP
.SP 5 5
    ({
.EP
.SP 5 5
       who,
.EP
.SP 5 5
       see,
.EP
.SP 5 5
       ({ obj, method }), // Remove method
.EP
.SP 5 5
     }),
.EP
.SP 5 5
...
.EP
.SP 5 5
])
.EP
.SP 5 5

.EP
.SP 10 5

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_hide_invis%^RESET%^
.EI
.SI 5
int add_hide_invis(string type, object who, mixed see, string * gone)
.EI
.SP 7 5

This method adds a hide/invis state to the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the name of the type of hiding
.EP
.SP 9 5
who - no idea
.EP
.SP 9 5
see - no idea
.EP
.SP 9 5
gone - the method and object to call when the hiding is removed
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^hide_invis_string%^RESET%^
.EI
.SI 5
string hide_invis_string()
.EI
.SP 7 5

This method returns an extra bit to stick on the end of the players
name to show when they are hiding and so on.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the extra bit to stick after a person's name based on the state

.EP

.SI 3
* %^BOLD%^perception_check%^RESET%^
.EI
.SI 5
int perception_check(object thing)
.EI
.SP 7 5

This does a perception check to see if the person can be seen.
This is variable on light level, amongst other things.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the person doing the looking

.EP

.SI 3
* %^BOLD%^query_hide_invis%^RESET%^
.EI
.SI 5
mapping query_hide_invis()
.EI

.SI 3
* %^BOLD%^query_visible%^RESET%^
.EI
.SI 5
int query_visible(object thing)
.EI
.SP 7 5

This is the method called to check to see if the person is visible or
not.
This is masked in /global/wiz_file_comm to handle
creator invisibility.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object doing the looking
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if visible, 0 if not

.EP

.SI 3
* %^BOLD%^remove_hide_invis%^RESET%^
.EI
.SI 5
varargs int remove_hide_invis(string type, int quiet)
.EI
.SP 7 5

This method removed the hiding of the specified type off the object.
This method will call the function setup when the hiding was added
when the hiding is removed. If you pass 1 as the second argument, it
will send the 'quiet' flag to the callback function.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of hiding to remove
.EP
.SP 9 5
quiet - flag to pass along to the callback function
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP


