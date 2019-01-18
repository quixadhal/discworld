.DT
engrave_inherit.c
Disk World autodoc help
engrave_inherit.c

.SH Description
.SP 5 5

This is a small inheritable to allow places to do engraving.  POut a
piece of code inthe init function which calls the engrave_init()
function.
.EP
.SP 10 5


Written by Macchirton
.EP

.SH Example
.SI 5
inherit "/std/room/basic_room";
inherit "/std/shops/engrave";

void init() {
   basic_room::init();
   engrave_init();
} /* init() */
.EI
.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/shops/engrave.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^engrave_init%^RESET%^
.EI
.SI 5
void engrave_init()
.EI
.SP 7 5

This method should be called in the inheriting room's init function.  IUt
will setup the commands to allow the object to be engraved.
.EP

.SI 3
* %^BOLD%^engrave_objects%^RESET%^
.EI
.SI 5
int engrave_objects(object * obs, int cost)
.EI
.SP 7 5

This method is called when the engrave is successful.
This should be overridden in the inheritable to print out the
message you wish to say.  If this function returns 1 then the
default failed message is not used.
.EP

.SI 3
* %^BOLD%^set_engrave_language%^RESET%^
.EI
.SI 5
void set_engrave_language(string lang)
.EI
.SP 7 5

This method sets which language (for example, "agatean", or
"morporkian") to be used when engraving.  It defaults to "general",
which is not to be recommended. 
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^do_engrave%^RESET%^
.EI
.SI 5
int do_engrave(object * things, string message)
.EI
.SP 7 5

This method does the actual engraving.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
things - the things to get engraved.
.EP
.SP 9 5
message - the message to engarve on the objects
.EP

.SI 3
* %^BOLD%^engrave_living%^RESET%^
.EI
.SI 5
int engrave_living(object * obs)
.EI
.SP 7 5

This method is called when a living object is attempted to be engraved.
This should be overridden in the inheritable to print out the
message you wish to say.  If this function returns 1 then the
default failed message is not used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
liv - the living objects
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 use default fail mesasage, 1 override default fail message
.EP

.SI 3
* %^BOLD%^engrave_no_money%^RESET%^
.EI
.SI 5
int engrave_no_money(object * obs, int cost)
.EI
.SP 7 5

This method is called if the player does not have enough money to
complete the engraving.
This should be overridden in the inheritable to print out the
message you wish to say.  If this function returns 1 then the
default failed message is not used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects which were unable to be engraved
.EP
.SP 9 5
cost - the cost of the objects to be engraved
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 use default fail mesasage, 1 override default fail message
.EP

.SI 3
* %^BOLD%^engrave_wrong_items%^RESET%^
.EI
.SI 5
int engrave_wrong_items(object * obs)
.EI
.SP 7 5

This method is called if the objects in question are unable to be
engraved.
This should be overridden in the inheritable to print out the
message you wish to say.  If this function returns 1 then the
default failed message is not used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects which could not be engraved.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 use default fail mesasage, 1 override default fail message
.EP


