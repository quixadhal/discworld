.DT
council_base.c
Disk World autodoc help
council_base.c

.SH Description
.SP 5 5

This inherit has all the basic things that all the council rooms
of various sorts share.
.EP
.SP 10 5


Written by Pinkfish

Started Wed Oct 25 21:44:37 PDT 2000

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nomic_system.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_council_area%^RESET%^
.EI
.SI 5
string query_council_area()
.EI
.SP 7 5

This method returns the current area this council chambers deals with.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the area the council chambers deal with

.EP

.SI 3
* %^BOLD%^send_council_inform%^RESET%^
.EI
.SI 5
void send_council_inform(int magistrate_only, string message)
.EI
.SP 7 5

This method broadcasts a message to the citizens or magistrates of
the area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
magistrate_only - 1 if only for magistrates, 0 for citizens
.EP
.SP 9 5
message - the message to send

.EP

.SI 3
* %^BOLD%^set_council_area%^RESET%^
.EI
.SI 5
void set_council_area(string area)
.EI
.SP 7 5

This method sets the current area the council chambers deal with.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the new council area

.EP


