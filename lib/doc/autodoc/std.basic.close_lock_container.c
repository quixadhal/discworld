.DT
close_lock_container.c
Disk World autodoc help
close_lock_container.c

.SH Description
.SP 5 5

This file should be used in containers that wish to notify the
above container that they are able to be registered or not.
The function we_moved(object from, object to) should be called
to allow the object to add and remove itself from the correct
places.


We only worry about the open/close state, if something is open and locked we don't care. (I guess stuff can be locked open anyway).
.EP
.SP 10 5


Written by Pinkfish

Started Thu Mar  5 16:20:10 CST 1998
.EP



.SH See also
.SP 5 5
we_moved()

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/close_lock.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^we_moved%^RESET%^
.EI
.SI 5
void we_moved(object from, object to)
.EI
.SP 7 5

This method should be called when an object is moved.  This will
remove itself from the leaving container and add itself to the
new one.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the object we came from
.EP
.SP 9 5
to - the object we are going to

.EP


