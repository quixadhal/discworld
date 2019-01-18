.DT
key.c
Disk World autodoc help
key.c

.SH Description
.SP 5 5

A key object.  This provides a nice easy way to create keys.

.EP
.SP 10 5


Written by ceres

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/engrave.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^set_key%^RESET%^
.EI
.SI 5
void set_key(string str, string prop)
.EI
.SP 7 5

Setup the key.  This method handles setting up the short, plural,
adjectives etc.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - The short of the key (minus the word 'key')
.EP
.SP 9 5
prop - The key property (should match the lock)

.EP


