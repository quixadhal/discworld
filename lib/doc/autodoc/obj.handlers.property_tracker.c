.DT
property_tracker.c
Disk World autodoc help
property_tracker.c

.SH Description
.SP 5 5

Property tracker. 
.EP
.SP 10 5


Written by Taffyd

Started 10/06/99 3:42AM
.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/login_handler.h and /include/login.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_invalid_property%^RESET%^
.EI
.SI 5
varargs int add_invalid_property(string property, mixed func, mixed obj)
.EI
.SP 7 5

This function allows a new invalid property to be added to the list
of invalid ones.  The last two parameters are used so that you can
add a callback if you want to convert a property over to the quest
handler or whatever. They are optional.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
property - the property name to be added
.EP
.SP 9 5
func - this parameter specifies the name of the function to be 
called when the property is removed
.EP
.SP 9 5
obj - this parameter specifies the path of the object on which func
is called when the property is removed
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the property was added as invalid, 0 if it was not.
.EP

.SI 3
* %^BOLD%^check_properties%^RESET%^
.EI
.SI 5
void check_properties(mixed person, string type)
.EI
.SP 7 5

This method is called by the login handler every time someone logs onto
the MUD. It checks through any properties that they have, and removes
them if they are marked as invalid.


If a property function was set, then this function is called now when the property is removed. It is passed three parameters, string property, object player, and mixed property_data. 


.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person who is logging on
.EP
.SP 9 5
type - the type of login event. Only LOGIN events are used
by this object. 
.EP

.SI 3
* %^BOLD%^convert_no_score%^RESET%^
.EI
.SI 5
void convert_no_score(string property, object who, mixed data)
.EI

.SI 3
* %^BOLD%^query_invalid_properties%^RESET%^
.EI
.SI 5
string query_invalid_properties()
.EI
.SP 7 5

This returns a list of all the properties that are marked as invalid.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of invalid properties

.EP

.SI 3
* %^BOLD%^remove_invalid_property%^RESET%^
.EI
.SI 5
int remove_invalid_property(string property)
.EI

.SI 3
* %^BOLD%^remove_warmth%^RESET%^
.EI
.SI 5
int remove_warmth(string property, object who, mixed data)
.EI

.SI 3
* %^BOLD%^reset_data%^RESET%^
.EI
.SI 5
void reset_data()
.EI

.SI 3
* %^BOLD%^save_file%^RESET%^
.EI
.SI 5
void save_file()
.EI


