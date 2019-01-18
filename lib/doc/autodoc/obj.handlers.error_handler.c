.DT
error_handler.c
Disk World autodoc help
error_handler.c

.SH Description
.SP 5 5

The main error database handler.  This should be used for all calls
into the error system, setting up queries and for making changes to
the bugs.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Feb  6 17:28:47 PST 2003

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/handlers/inherit/error_handler.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^setup_error_handler%^RESET%^
.EI
.SI 5
void setup_error_handler(string user, string db, string password)
.EI


