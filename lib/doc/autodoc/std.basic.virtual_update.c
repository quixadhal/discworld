.DT
virtual_update.c
Disk World autodoc help
virtual_update.c

.SH Description
.SP 5 5

This is the inherit used to update virtual objects from their base files
if something changes.  This will call the method init_static_arg with
any updated data.
.EP
.SP 10 5


Written by Dragonkin
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^init_virtual_object%^RESET%^
.EI
.SI 5
void init_virtual_object()
.EI
.SP 7 5

THis method should be called in the inheritable to deal with virtual
objects.  It will check to see if the data has changed and update the
object to reflect any of the changes.

.EP

.SI 3
* %^BOLD%^replace_me%^RESET%^
.EI
.SI 5
void replace_me()
.EI
.SP 7 5

This method turns the current object into a missing item
receipt.

.EP


