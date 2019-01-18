.DT
state_change.c
Disk World autodoc help
state_change.c

.SH Description
.SP 5 5

This handler is meant to simulate the changing of an object's
"state", such as melting, grinding, etc.  In reality, rather than
changing the source object, the transform() method clones a
completely new object, based on the internal lookup table; it is
the caller's responsibility to dest the source object and put the
new one in its place.


The handler's lookup table is stored in a series of files: any files in /obj/state_change ending in .dat will be added to the table when the handler is loaded. See /include/state_change.h for field definitions. The data files are loaded by the data handler, and can contain fancy things like expressions and function pointers.
.EP
.SP 10 5


Written by Jeremy
.EP



.SH See also
.SP 5 5
/include/state_change.h and /obj/handlers/data.c

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/state_change.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_table%^RESET%^
.EI
.SI 5
mixed query_table()
.EI
.SP 7 5

This method returns the entire lookup table.  It is mainly for
debugging.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the state_change lookup table

.EP

.SI 3
* %^BOLD%^transform%^RESET%^
.EI
.SI 5
varargs object transform(object ob, string transformation, object player)
.EI
.SP 7 5

This method is generally called by the object doing the state
change (e.g., a food grinder).  It looks up the specified source
object in the table and clones the "transformed" object as specified.


The table is indexed by the source object as follows: if the source object has a property "state name", the value of this is used. If not, the object's medium alias (if any, regardless of whether the object is continuous) is used. Failing these, the object's short description is used. The first one resulting in a valid string is concatenated with the transformation string (with a ":" between) and the result is used as the index. If the lookup fails, 0 is returned. 

Note that the resulting object is only cloned. It is the responsibility of the caller to dest the source and put the result in its place.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the source object
.EP
.SP 9 5
transformation - a string specifying the transformation (e.g., "grind")
.EP
.SP 9 5
The - player to whom this state change will be attributed to! [optional]
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the result of the transformation 
.EP


