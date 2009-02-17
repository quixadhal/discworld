.DT
vessel.c
Dead DW2 autodoc help
vessel.c

.SH Description
.SP 5 5

This is a container specifically for holding liquids.  The hope is
to reduce the memory requirements by taking all of the liquid stuff
out of /obj/container, since most of the containers are sacks and
chests.  Also, the reaction handler will replace the potion space.


Some additional notes: 
.EP
.SO 6 3 -11

	1. 	As is (hopefully) documented elsewhere, the base units of volume for most continuous media are drops and pinches, both of which are roughly 1/4 of a cc. This means that water has 200 drops per weight unit (1g/cc). Non-continuous objects are assumed to be the same density as water. 
.EO
.SP 5 5

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/baggage.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/volumes.h, /include/tasks.h and /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_volume%^RESET%^
.EI
.SI 5
int add_volume(int vol)
.EI
.SP 7 5

This method returns the amount of volume left for liquids to be
added into.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
vol - the amount of volume added
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the addition was successful, 0 if not
.EP

.SI 3
* %^BOLD%^calc_liquid%^RESET%^
.EI
.SI 5
void calc_liquid()
.EI

.SI 3
* %^BOLD%^cmp_amount_r%^RESET%^
.EI
.SI 5
int cmp_amount_r(object a, object b)
.EI

.SI 3
* %^BOLD%^cmp_weight_r%^RESET%^
.EI
.SI 5
int cmp_weight_r(object a, object b)
.EI

.SI 3
* %^BOLD%^drink_amount%^RESET%^
.EI
.SI 5
int drink_amount(int drinking, object player)
.EI
.SP 7 5

This method drinks a certain amount of the liquid in the container.
It will do all the fudging for being in combat and drinking too
much, as well as dropping the bottle and so on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
drinking - the amount to drink 

.EP

.SI 3
* %^BOLD%^fullness_adjectives%^RESET%^
.EI
.SI 5
string * fullness_adjectives()
.EI
.SP 7 5

This returns an adjective for how full the current object is with
liquid.  This is used in the parse command handling code.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the fullness adjective
.EP

.SI 3
* %^BOLD%^is_fighting_bottle_smashed%^RESET%^
.EI
.SI 5
int is_fighting_bottle_smashed(object player, object me)
.EI
.SP 7 5

This method checks to see if the person doing the drinking can hold onto
their bottle without loosing it while in combat.  Warning!  This code
may be used in other objects to deal with handling drinking while in
combat.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player doing the drinking
.EP
.SP 9 5
me - the object being drunk 
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the bottle is stopped, 0 if it is not
.EP

.SI 3
* %^BOLD%^query_cont_volume%^RESET%^
.EI
.SI 5
int query_cont_volume()
.EI

.SI 3
* %^BOLD%^query_fullness_desc%^RESET%^
.EI
.SI 5
string query_fullness_desc()
.EI
.SP 7 5

This method returns the fullness description of the vessel.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the fullness description of the vessel 
.EP

.SI 3
* %^BOLD%^query_leak_rate%^RESET%^
.EI
.SI 5
int query_leak_rate()
.EI
.SP 7 5

This method returns the leak rate of the container
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current leak rate 
.EP

.SI 3
* %^BOLD%^query_liquid%^RESET%^
.EI
.SI 5
int query_liquid()
.EI
.SP 7 5

This method returns if this vessel is currenly a liquid.  This means
it has a liquid inside it.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is a liquid, 0 if not 
.EP

.SI 3
* %^BOLD%^query_liquid_desc%^RESET%^
.EI
.SI 5
string query_liquid_desc()
.EI
.SP 7 5

This method returns the description of the liquid inside the vessel.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current liquids description 
.EP

.SI 3
* %^BOLD%^query_max_volume%^RESET%^
.EI
.SI 5
int query_max_volume()
.EI
.SP 7 5

This method returns the current maxium volume associated with this
container.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current maximum volume 
.EP

.SI 3
* %^BOLD%^query_vessel%^RESET%^
.EI
.SI 5
int query_vessel()
.EI

.SI 3
* %^BOLD%^query_volume%^RESET%^
.EI
.SI 5
int query_volume()
.EI
.SP 7 5

This method returns the current amount of liquid in the container.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current amount of liquid in the container 
.EP

.SI 3
* %^BOLD%^query_volume_left%^RESET%^
.EI
.SI 5
int query_volume_left()
.EI
.SP 7 5

This method returns the amount of volume left for liquids to be
added into.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount of volume left
.EP

.SI 3
* %^BOLD%^remove_volume%^RESET%^
.EI
.SI 5
int remove_volume(int vol_lost)
.EI
.SP 7 5

This method removes some volume of liquid from the container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
vol_lost - the amount of volume removed
.EP

.SI 3
* %^BOLD%^set_leak_rate%^RESET%^
.EI
.SI 5
void set_leak_rate(int i)
.EI
.SP 7 5

This method sets the leak rate of the container.  The leak rate is how
fast stuff leaks out of the container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new leak rate of the container
.EP

.SI 3
* %^BOLD%^set_max_volume%^RESET%^
.EI
.SI 5
void set_max_volume(int v)
.EI
.SP 7 5

This method sets the maximum volume of the container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
v - the new maximum volume
.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed stats()
.EI

.SI 3
* %^BOLD%^xfer_volume%^RESET%^
.EI
.SI 5
int xfer_volume(int vol_xferred, object dest)
.EI
.SP 7 5

This method transfers a given amount of a liquid to a new container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dest - the destination of the liquid
.EP
.SP 9 5
vol_xferred - the amount of volume transfered 
.EP


