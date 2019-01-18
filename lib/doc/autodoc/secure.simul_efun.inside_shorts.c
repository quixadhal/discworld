.DT
inside_shorts.c
Disk World autodoc help
inside_shorts.c

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
* %^BOLD%^inside_a_short%^RESET%^
.EI
.SI 5
string inside_a_short(object ob, object play)
.EI
.SP 7 5

This method produces the inside messages for the given object up
to the player object.  This one generates 'a_short' messages.
This should only be called with an object that is a container.
It will produce weird and incorrect messages otherwise.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to generate the short for
.EP
.SP 9 5
play - the player to generate the short up to
.EP

.SI 3
* %^BOLD%^inside_one_short%^RESET%^
.EI
.SI 5
string inside_one_short(object ob, object play)
.EI
.SP 7 5

This method produces the inside messages for the given object up
to the player object.  This one generates 'a_short' messages.
This should only be called with an object that is a container.
It will produce weird and incorrect messages otherwise.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to generate the short for
.EP
.SP 9 5
play - the player to generate the short up to
.EP

.SI 3
* %^BOLD%^inside_the_short%^RESET%^
.EI
.SI 5
string inside_the_short(object ob, object play)
.EI
.SP 7 5

This method produces the inside messages for the given object up
to the player object.  This one generates 'the_short' messages.
This should only be called with an object that is a container.
It will produce weird and incorrect messages otherwise.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to generate the short for
.EP
.SP 9 5
play - the player to generate the short up to
.EP


