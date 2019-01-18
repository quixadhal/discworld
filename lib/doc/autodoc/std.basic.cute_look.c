.DT
cute_look.c
Disk World autodoc help
cute_look.c

.SH Description
.SP 5 5


This object handles all the inventory listing for rooms, when long() is 
called.
It should convert like this: ({ /w/terano/womble#1234, /std/object#1324, 
/global/creator#2523 }) into "Lady Midnite is standing here.\nA key and a 
womble are floating happily.\n"

.EP
.SP 10 5


Written by Lynscar
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/position.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^ignore_livings_in_inventory%^RESET%^
.EI
.SI 5
int ignore_livings_in_inventory()
.EI
.SP 7 5

Returns the ignore_living flag.  If livings are ignored, they are 
displayed the same way as non-livings, ie: by short alone.  If they are 
not ignored, livings are displayed on a separate line and show their
positions. NB: passing a value to the 'ignore_livings' parameter of 
query_contents overrides the ignore_living flag.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if livings are ignored, 0 if not.
.EP

.SI 3
* %^BOLD%^list_contents%^RESET%^
.EI
.SI 5
void list_contents(string words)
.EI
.SP 7 5

This method prints out the contents of the object matching.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the string to match

.EP

.SI 3
* %^BOLD%^query_contents%^RESET%^
.EI
.SI 5
varargs string query_contents(string start, object * things, int ignore_living)
.EI
.SP 7 5

This method returns a string contents of description of the container.
NB: this_player() is _not_ included in the array if (s)he is present.
The ignore_living flag tells us if we should process living
objects as different or not.  If we do not process them as
different (as you do in room descriptions), then this flag should
be 0.  This is so that living objects can be kept an an
inventory of a player without stuff it up.  ie: Pets.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
start - the start message before the contents
.EP
.SP 9 5
things - the things to give the message for
.EP
.SP 9 5
ignore_living - treat livings differently
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string contents description

.EP

.SI 3
* %^BOLD%^set_ignore_livings_in_inventory%^RESET%^
.EI
.SI 5
void set_ignore_livings_in_inventory(int ignore)
.EI
.SP 7 5

Sets the ignore_living flag.  Default is 1.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
1 - to ignore livings, 0 not to
.EP


