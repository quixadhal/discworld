.DT
corpse.c
DW_wibble autodoc help
corpse.c

.SH Description
.SP 5 5

This method returns the id associated with the corpse.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/container, /std/living/carrying and /std/basic/virtual_quit_control.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/corpse.h, /include/move_failures.h and /include/bit.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_bit_gone%^RESET%^
.EI
.SI 5
mixed * add_bit_gone(string bit)
.EI

.SI 3
* %^BOLD%^can_find_match_recurse_into%^RESET%^
.EI
.SI 5
int can_find_match_recurse_into(object player)
.EI

.SI 3
* %^BOLD%^do_decay%^RESET%^
.EI
.SI 5
void do_decay()
.EI
.SP 7 5

This is called to decay the corpse.  It is done in a continuous
call out loop until all of the decay has been completed.

.EP

.SI 3
* %^BOLD%^find_inv_match%^RESET%^
.EI
.SI 5
varargs object * find_inv_match(string s, object looker)
.EI

.SI 3
* %^BOLD%^get%^RESET%^
.EI
.SI 5
int get(mixed dest)
.EI

.SI 3
* %^BOLD%^give_permission%^RESET%^
.EI
.SI 5
void give_permission(string words)
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map,  object)
.EI

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string words, int dark)
.EI

.SI 3
* %^BOLD%^make_bit%^RESET%^
.EI
.SI 5
object make_bit(string which_bit)
.EI

.SI 3
* %^BOLD%^make_bits%^RESET%^
.EI
.SI 5
object * make_bits(string * what_bits)
.EI

.SI 3
* %^BOLD%^move_or_destruct%^RESET%^
.EI
.SI 5
int move_or_destruct(object dest)
.EI

.SI 3
* %^BOLD%^query_armours%^RESET%^
.EI
.SI 5
object * query_armours()
.EI

.SI 3
* %^BOLD%^query_bit_left%^RESET%^
.EI
.SI 5
string query_bit_left(string s)
.EI

.SI 3
* %^BOLD%^query_bit_left_pl%^RESET%^
.EI
.SI 5
string * query_bit_left_pl(string s)
.EI

.SI 3
* %^BOLD%^query_bits_gone%^RESET%^
.EI
.SI 5
string * query_bits_gone()
.EI

.SI 3
* %^BOLD%^query_bits_left%^RESET%^
.EI
.SI 5
string * query_bits_left()
.EI

.SI 3
* %^BOLD%^query_corpse%^RESET%^
.EI
.SI 5
int query_corpse()
.EI

.SI 3
* %^BOLD%^query_corpse_id%^RESET%^
.EI
.SI 5
int query_corpse_id()
.EI

.SI 3
* %^BOLD%^query_decay%^RESET%^
.EI
.SI 5
int query_decay()
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_edible_bits_left%^RESET%^
.EI
.SI 5
string * query_edible_bits_left()
.EI

.SI 3
* %^BOLD%^query_holding%^RESET%^
.EI
.SI 5
object * query_holding()
.EI

.SI 3
* %^BOLD%^query_name%^RESET%^
.EI
.SI 5
string query_name()
.EI

.SI 3
* %^BOLD%^query_owner%^RESET%^
.EI
.SI 5
string query_owner()
.EI

.SI 3
* %^BOLD%^query_permitted%^RESET%^
.EI
.SI 5
string * query_permitted()
.EI

.SI 3
* %^BOLD%^query_race_name%^RESET%^
.EI
.SI 5
string query_race_name()
.EI

.SI 3
* %^BOLD%^query_race_ob%^RESET%^
.EI
.SI 5
string query_race_ob()
.EI

.SI 3
* %^BOLD%^query_save_data%^RESET%^
.EI
.SI 5
string query_save_data()
.EI
.SP 7 5

This is used by the corpse handler to get the data used to save
the corpse.

.EP

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mapping query_static_auto_load()
.EI

.SI 3
* %^BOLD%^query_weapon%^RESET%^
.EI
.SI 5
object query_weapon()
.EI

.SI 3
* %^BOLD%^query_wearing%^RESET%^
.EI
.SI 5
object * query_wearing()
.EI

.SI 3
* %^BOLD%^remove_armour%^RESET%^
.EI
.SI 5
void remove_armour(object arm)
.EI

.SI 3
* %^BOLD%^remove_array_parts%^RESET%^
.EI
.SI 5
string * remove_array_parts(string * a1, string * a2)
.EI

.SI 3
* %^BOLD%^remove_creator_corpse%^RESET%^
.EI
.SI 5
void remove_creator_corpse(object ob)
.EI

.SI 3
* %^BOLD%^set_armours%^RESET%^
.EI
.SI 5
void set_armours(object * things)
.EI

.SI 3
* %^BOLD%^set_bits_gone%^RESET%^
.EI
.SI 5
void set_bits_gone(string * bits)
.EI

.SI 3
* %^BOLD%^set_decay%^RESET%^
.EI
.SI 5
void set_decay(int dec)
.EI

.SI 3
* %^BOLD%^set_decay_messages%^RESET%^
.EI
.SI 5
void set_decay_messages()
.EI
.SP 7 5

This method sets up the various 'states; of the corpse to show 
how decayed it is.

.EP

.SI 3
* %^BOLD%^set_hold%^RESET%^
.EI
.SI 5
int * set_hold(object ob, int pos)
.EI

.SI 3
* %^BOLD%^set_holding%^RESET%^
.EI
.SI 5
void set_holding(object * hold)
.EI

.SI 3
* %^BOLD%^set_owner%^RESET%^
.EI
.SI 5
void set_owner(string words, object thing)
.EI
.SP 7 5

This method initially sets up the corpse object.  Generally
speaking words should be 0 and the 'thing' variable should be
set to the object which just died.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the owner of the object
.EP
.SP 9 5
thing - the object to get the ownership data from

.EP

.SI 3
* %^BOLD%^set_race_name%^RESET%^
.EI
.SI 5
void set_race_name(string str)
.EI

.SI 3
* %^BOLD%^set_race_ob%^RESET%^
.EI
.SI 5
void set_race_ob(string s)
.EI

.SI 3
* %^BOLD%^set_unhold%^RESET%^
.EI
.SI 5
int * set_unhold(object ob)
.EI

.SI 3
* %^BOLD%^set_weapon%^RESET%^
.EI
.SI 5
void set_weapon(object wpn)
.EI

.SI 3
* %^BOLD%^setup_corpse_from_save%^RESET%^
.EI
.SI 5
void setup_corpse_from_save(string str_data, string name, int id)
.EI
.SP 7 5

This method is called by the corpse handler to setup the corpse
properly after it loads.

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI

.SI 3
* %^BOLD%^test_remove%^RESET%^
.EI
.SI 5
int test_remove(object thing, int flag, mixed dest)
.EI

.SI 3
* %^BOLD%^unwield_weapon%^RESET%^
.EI
.SI 5
void unwield_weapon(object wpn)
.EI


