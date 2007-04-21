.DT
container.c
DW_wibble autodoc help
container.c

.SH Description
.SP 5 5

This is main container type object that players will use.  For
example, buckets and stuff would use this.  A backpack would be
a type of clothing and a potion bottle should be a vessel.
.EP
.SP 10 5


Written by Who knows, Pinkfish, Ember, Jeremy
.EP



.SH See also
.SP 5 5
/obj/vessel.c and /obj/clothing.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/container and /std/basic/close_lock_container.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/parse_command.h and /include/potion_attrs.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_ps_vect%^RESET%^
.EI
.SI 5
void add_ps_vect(int * v, int quant)
.EI
.SP 7 5

This method adds a potion space vector with a particular
quantity to the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
v - the vector to add
.EP
.SP 9 5
quant - the quantity to add

.EP

.SI 3
* %^BOLD%^begin_config%^RESET%^
.EI
.SI 5
void begin_config()
.EI

.SI 3
* %^BOLD%^change_concentration%^RESET%^
.EI
.SI 5
mixed * change_concentration(mixed * potion, int new_conc)
.EI
.SP 7 5

This method changes the concentration of the liquid to be a new
and exciting concentration.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
potion - the potion to change the concentration of
.EP
.SP 9 5
new_conc - the new concentration of the liquid

.EP

.SI 3
* %^BOLD%^check_breakages%^RESET%^
.EI
.SI 5
void check_breakages()
.EI
.SP 7 5

This method checks to see if any of the things contained in the
container should be broken.
.EP

.SI 3
* %^BOLD%^config%^RESET%^
.EI
.SI 5
void config(int vari, mixed cont)
.EI

.SI 3
* %^BOLD%^consistency_string%^RESET%^
.EI
.SI 5
string consistency_string(int cons)
.EI

.SI 3
* %^BOLD%^contents_vect%^RESET%^
.EI
.SI 5
int * contents_vect()
.EI
.SP 7 5

This method returns the vector of the contents of the container.
This is based on the vectors of all the liquid with a vector set
on them.

.EP

.SI 3
* %^BOLD%^dest_active_contents%^RESET%^
.EI
.SI 5
int dest_active_contents()
.EI
.SP 7 5

This method will destroy all the liquid contents of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of object destroyed

.EP

.SI 3
* %^BOLD%^empty_formula%^RESET%^
.EI
.SI 5
int empty_formula()
.EI
.SP 7 5

This method returns the amount of the liquid you can actually empty
from a container.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount of liquid that can be emptied

.EP

.SI 3
* %^BOLD%^end_config%^RESET%^
.EI
.SI 5
void end_config()
.EI

.SI 3
* %^BOLD%^ensure_open%^RESET%^
.EI
.SI 5
int ensure_open()
.EI
.SP 7 5

This method returns true if the object is open and prints a message
about the open status of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is open, 0 if not

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
* %^BOLD%^liquid_name%^RESET%^
.EI
.SI 5
string liquid_name()
.EI
.SP 7 5

This method returns the name of the current liquid inside the container.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the current liquid

.EP

.SI 3
* %^BOLD%^merge_potions%^RESET%^
.EI
.SI 5
mixed * merge_potions(mixed * potion1, mixed * potion2, int vol1, int vol2, int tot_vol)
.EI

.SI 3
* %^BOLD%^mix_liq%^RESET%^
.EI
.SI 5
mixed * mix_liq(mixed * arr1, mixed * arr2, int vol1, int vol2, int tot_vol)
.EI

.SI 3
* %^BOLD%^parse_command_adjectiv_id_list%^RESET%^
.EI
.SI 5
string * parse_command_adjectiv_id_list()
.EI

.SI 3
* %^BOLD%^parse_command_id_list%^RESET%^
.EI
.SI 5
string * parse_command_id_list()
.EI

.SI 3
* %^BOLD%^query_active_attrs%^RESET%^
.EI
.SI 5
int * query_active_attrs()
.EI

.SI 3
* %^BOLD%^query_all_attrs%^RESET%^
.EI
.SI 5
int * query_all_attrs()
.EI

.SI 3
* %^BOLD%^query_frac_parse_id%^RESET%^
.EI
.SI 5
object query_frac_parse_id(mixed * arr)
.EI

.SI 3
* %^BOLD%^query_full_weight%^RESET%^
.EI
.SI 5
int query_full_weight()
.EI
.SP 7 5

This method returns the full weight of the container.
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

.SI 3
* %^BOLD%^query_liquid_short%^RESET%^
.EI
.SI 5
string query_liquid_short()
.EI
.SP 7 5

This method returns the short description of the liquid.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short description of the liquid
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
* %^BOLD%^query_misc_attrs%^RESET%^
.EI
.SI 5
int * query_misc_attrs()
.EI

.SI 3
* %^BOLD%^query_parse_id%^RESET%^
.EI
.SI 5
object query_parse_id(mixed * arr)
.EI

.SI 3
* %^BOLD%^query_ps_coord%^RESET%^
.EI
.SI 5
int * query_ps_coord()
.EI
.SP 7 5

This method returns the potion space co-ordinates of the liquid.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the potion space co-ordinates
.EP

.SI 3
* %^BOLD%^query_ps_quantity%^RESET%^
.EI
.SI 5
int query_ps_quantity()
.EI
.SP 7 5

This method returns the quantity of liquid at the potion space.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the quantity of liquid
.EP

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
* %^BOLD%^query_water_volume%^RESET%^
.EI
.SI 5
int query_water_volume()
.EI
.SP 7 5

This method returns the volume of water in the container.
.EP

.SI 3
* %^BOLD%^query_weight_left%^RESET%^
.EI
.SI 5
int query_weight_left()
.EI
.SP 7 5

This method returns the current amount of weight left that can
be filled on the container.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current amount of weight left

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

.SI 3
* %^BOLD%^set_full_weight%^RESET%^
.EI
.SI 5
void set_full_weight(int i)
.EI
.SP 7 5

This method sets the weight of the container when it is full.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new weight of the containe when full.
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
void set_max_volume(int i)
.EI
.SP 7 5

This method sets the maximum volume of the container.  This also
sets the maximum weight of the container to one tenth of the
volume (if there is not current maximum weight).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new maximum volume
.EP

.SI 3
* %^BOLD%^set_max_weight%^RESET%^
.EI
.SI 5
void set_max_weight(int i)
.EI
.SP 7 5

This method sets the maximum weight of the container.  This also sets
the maximum volume of the container to 10 times its maximum weight.
(If there is no current maximum volume).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the maxium weight of the container
.EP

.SI 3
* %^BOLD%^set_misc_attrs%^RESET%^
.EI
.SI 5
void set_misc_attrs(mixed * new_misc_attrs, int misc_vol)
.EI

.SI 3
* %^BOLD%^set_ps_coord_quantity%^RESET%^
.EI
.SI 5
void set_ps_coord_quantity(int * new_coord, int new_quantity)
.EI
.SP 7 5

This method sets the quantity of the position space co-ordinate
to be the new quantity.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_coord - the new co-ordinate of the liquid
.EP
.SP 9 5
new_quantity - the new quantity of the liquid

.EP

.SI 3
* %^BOLD%^set_ps_quantity%^RESET%^
.EI
.SI 5
void set_ps_quantity(int n)
.EI

.SI 3
* %^BOLD%^set_volume%^RESET%^
.EI
.SI 5
void set_volume(int i)
.EI
.SP 7 5

This method sets the current amount of liquid in the container.  If
the container has a leak rate, it starts to leak.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new amount of liquid in the containe
.EP

.SI 3
* %^BOLD%^set_water_volume%^RESET%^
.EI
.SI 5
void set_water_volume(int n)
.EI
.SP 7 5

This method sets the volume of water in the container.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
n - the new volume of water

.EP

.SI 3
* %^BOLD%^sort_func%^RESET%^
.EI
.SI 5
int sort_func(int * x, int * y)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed stats()
.EI

.SI 3
* %^BOLD%^test_add%^RESET%^
.EI
.SI 5
int test_add(object ob, int flag)
.EI
.SP 7 5

This method stops the container being added when it is 
closed.  It also handles the same flags as the liing
object.  We assume this is not the same
as the default containers used
by things like item rooms and such.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object being added
.EP
.SP 9 5
flag - the addition flag
.EP

.SI 3
* %^BOLD%^transfer_liquid_to%^RESET%^
.EI
.SI 5
void transfer_liquid_to(object dest, int vol_misc, mixed * misc, int vol_water, int * coord, int quantity)
.EI
.SP 7 5

This method does the actual liquid transfer, as needed by the
filling, emptying and pour operations.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dest - where the liquid is to go
.EP
.SP 9 5
vol_misc - the volume of non-water to move
.EP
.SP 9 5
misc - the attributes for the non-water
.EP
.SP 9 5
vol_water - the volume of water to move
.EP
.SP 9 5
coord - the co-ordinate of the liquid
.EP
.SP 9 5
quantity - the quantity of the potion space liquid

.EP

.SI 3
* %^BOLD%^transparency_string%^RESET%^
.EI
.SI 5
string transparency_string(int trans)
.EI

.SI 3
* %^BOLD%^update_potion_parse%^RESET%^
.EI
.SI 5
void update_potion_parse()
.EI
.SP 7 5

This method updates the values used for the find_match() parseing
due to the liquids.
.EP

.SI 3
* %^BOLD%^void_liquid%^RESET%^
.EI
.SI 5
void void_liquid()
.EI
.SP 7 5

This method zaps all the liquid stuff in the container.

.EP


