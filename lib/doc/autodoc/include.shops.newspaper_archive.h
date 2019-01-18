.DT
newspaper_archive.h
Disk World autodoc help
newspaper_archive.h

.SH Description
.SP 5 5

The include file for the newspaper archive shop.
.EP
.SP 10 5


Written by Pinkfish

Started Wed May 23 16:31:59 PDT 2001

.EP

.SH Defines
.SI 3
* %^BOLD%^NEWSPAPER_ARCHIVE_EVENT_BOUGHT%^RESET%^
.EI
.SP 7 5

This event is called when the paper is finally bought.  The extra
arguments are the paper name, the edition being bought, the cost
and the paper object itself.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARCHIVE_EVENT_INVALID_EDITION%^RESET%^
.EI
.SP 7 5

This is the event called when the edition being bought is invalid.  The
extra arguments are the paper name, the edition being checked.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARCHIVE_EVENT_INVALID_PAPER%^RESET%^
.EI
.SP 7 5

This is the event called when the paper asked to be listed is 
non-existant.  The extra argyment to this is the paper which 
was entered and unable to be matched.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARCHIVE_EVENT_LIST%^RESET%^
.EI
.SP 7 5

This is the event called when the listing is attempted to be done.
The extra arguments are the default output string and the
array of papers.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARCHIVE_EVENT_MOVE_PAPER%^RESET%^
.EI
.SP 7 5

This event is called to move the paper into the person's inventory.
The extra arguements are the paper being bought, the edition number,
the cost and the newspaper object.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARCHIVE_EVENT_NO_PAPERS%^RESET%^
.EI
.SP 7 5

This is the event called when there are no papers to list.

.EP

.SI 3
* %^BOLD%^NEWSPAPER_ARCHIVE_EVENT_TOO_POOR%^RESET%^
.EI
.SP 7 5

This event is called when the paper is too expensive to be bought.
The extra arguments are the paper name, the edition being bought
and how much it cost.

.EP

