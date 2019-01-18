.DT
surface.c
Disk World autodoc help
surface.c

.SH Description
.SP 5 5

This file contains the methods to handle surfaces, like book
shelves and stuff.
.EP
.SP 10 5

.EP



.SH See also
.SP 5 5
/std/shadows/misc/surface.c

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/surface.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_environment%^RESET%^
.EI
.SI 5
void check_environment()
.EI
.SP 7 5

This method checks to make sure the environment has a shadow associated
with it.
.EP

.SI 3
* %^BOLD%^check_shadows%^RESET%^
.EI
.SI 5
void check_shadows()
.EI
.SP 7 5

This method checks to make sure all the shadows are currenly valid.  If
an invalid shadow is found it is removed.


The method check_environment() is called at the end of the function.
.EP

.SI 3
* %^BOLD%^make_shadow%^RESET%^
.EI
.SI 5
void make_shadow(object thing)
.EI
.SP 7 5

This method makes a surface shadow which is attached to the given object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object to attach the shadow too

.EP

.SI 3
* %^BOLD%^query_surface_shadows%^RESET%^
.EI
.SI 5
object * query_surface_shadows()
.EI
.SP 7 5

This method returns the current set of surface shadows associated with
the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current set of surface shadows
.EP


