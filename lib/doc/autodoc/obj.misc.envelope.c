.DT
envelope.c
Disk World autodoc help
envelope.c

.SH Description
.SP 5 5

Paper envelopes that can be written on, closed, sealed and unsealed.  Use
a call to "make_envelope()" to set the size and description of the
envelope.
.EP
.SP 10 5


Written by Lemming

Started 7/1/2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/baggage.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^make_envelope%^RESET%^
.EI
.SI 5
void make_envelope(string description, int size)
.EI
.SP 7 5

This is the method used to define the envelope.  It works just like the
"make_bottle()" call on "/obj/bottle.c", and must be done whenever one is
created.  The description should be short and should not include the size
of the envelope - this will be inferred from the size specified.  A size
of 3 or less will be "small", 4 to 5 will be "medium", 6 to 8 is "large"
and anything over that will be a "large packet".  The default and minimum
size is 2.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
description - a brief description of the envelope
.EP
.SP 9 5
size - the maximum weight of the envelope plus contents
.EP

.SI 3
* %^BOLD%^query_envelope_status%^RESET%^
.EI
.SI 5
int query_envelope_status()
.EI
.SP 7 5

This is the method used to query what status the envelope is in.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for unsealed, 2 for sealed, 3 for sealed and then torn open
.EP


