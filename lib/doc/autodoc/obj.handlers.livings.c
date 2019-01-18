.DT
livings.c
Disk World autodoc help
livings.c

.SH Description
.SP 5 5

A handler to deal with all the living objects on the disc.  
.EP
.SP 10 5


Written by Wodan
.EP

.SH Change history
.SP 5 5
Pinkfish March 16th
After wodan didn't add any autodoc comments and made some bits quite
inefficent

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^enable_commands%^RESET%^
.EI
.SI 5
void enable_commands(object ob)
.EI
.SP 7 5

Enables commands on the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to enable the commands on

.EP

.SI 3
* %^BOLD%^find_living%^RESET%^
.EI
.SI 5
object find_living(string it)
.EI
.SP 7 5

Returns the living object associated with the name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the living object

.EP

.SI 3
* %^BOLD%^find_player%^RESET%^
.EI
.SI 5
object find_player(string it)
.EI
.SP 7 5

Returns the player associated with the name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the player object

.EP

.SI 3
* %^BOLD%^named_livings%^RESET%^
.EI
.SI 5
object * named_livings()
.EI
.SP 7 5

This method returns the name of all the named livings in the game.
Do not use this method except for debugging.

.EP

.SI 3
* %^BOLD%^remove_garbage%^RESET%^
.EI
.SI 5
void remove_garbage()
.EI
.SP 7 5

Removes all the not very useful things from the huge mapping.

.EP

.SI 3
* %^BOLD%^set_living_name%^RESET%^
.EI
.SI 5
void set_living_name(string name, object ob)
.EI
.SP 7 5

This method registers the living name with the specific object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name to register
.EP
.SP 9 5
ob - the object to register it with

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


