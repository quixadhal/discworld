.DT
mapping.c
Disk World autodoc help
mapping.c

.SH Description
.SP 5 5

Cotains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^m_delete%^RESET%^
.EI
.SI 5
mapping m_delete(mapping map, mixed key)
.EI
.SP 7 5

Deletes an element from a mapping (obsolete).  Use of this
should be avoided.  It is far more expensive than the
map_delete() efun.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the mapping to remove an element from
.EP
.SP 9 5
key - the element to remove

.EP


