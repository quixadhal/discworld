.DT
food.c
DW_wibble autodoc help
food.c

.SH Description
.SP 5 5


.EP
.SP 5 5
This is the main inheritable for food objects. It allows you to create food and drink. Virtual files may also be coded if given the *.food extention. There are many examples of these virtual food files in the /obj/food directory. Food does the following things. 
.EP
.SP 5 5

.EP
.SP 5 5
- It can be eaten or drunk. 
.EP
.SP 5 5
- Food can be changed from solid to liquid with set_liquid(), and changed from liquid to solid with reset_liquid(). 
.EP
.SP 5 5
- The weight of each bite can be set with set_weight_per_bite(). 
.EP
.SP 5 5
- Effects can be added to the player when they are splashed with the food, get food rubbed on them, or get food applied to them with add_external_effect. 
.EP
.SP 5 5
- Splashing, applying, and rubbing can be pk-checked with set_external_pk_check() 
.EP
.SP 5 5
- Splashing can be activated with set_splashable() 
.EP
.SP 5 5
- Splashing can be deactivated with unset_splashable() 
.EP
.SP 5 5
- Applying can be activated with set_applicable() 
.EP
.SP 5 5
- Applying can be deactivated with unset_applicable() 
.EP
.SP 5 5
- Effects can be added to the player when they consume the food with add_eat_effect(). 
.EP
.SP 5 5
- Functions can be added to be carried out when the food is consumed with add_eat_func(). This function can be stored on another object if the set_eat_object() is used. 
.EP
.SP 5 5
- An eat message can be set with set_eat_mess(). 
.EP
.SP 5 5
- Food can be cut up into pieces with sharp weapons. 
.EP
.SP 5 5
- The description of the new 'pieces' can be set using set_piece_description(), set_piece_short(), set_piece_plural(), and set_piece_substance(). 
.EP
.SP 5 5
- Non-liquid food decays and will eventually crumble to dust. 
.EP
.SP 5 5
- The decay speed can be set with set_decay_speed(). 
.EP
.SP 5 5
- Food can be 'pickled' to prevent decay using do_cure(). 
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/virtual_update and /obj/cont_medium.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/bits_controller.h, /include/drinks.h, /include/playtesters.h and /include/weapon.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_eat_effect%^RESET%^
.EI
.SI 5
int add_eat_effect(mixed word, int number)
.EI
.SP 7 5

This adds an eat effect to the food.  This will be added to
the player or NPC when the food is eaten.


The effect is added with a number as the argument. The number is based on the amount which is eaten (in weight units), and usually represents the strength of the effect. If the object is continuous, the weight is calculated from the amount and weight_unit array... 


.EP
.SP 7 5
eff_num = (amount * number * weight_unit[0]) / weight_unit[1] 

...where the number is passed into the add_eat_effect() function. If the effect already exists, then the number is added onto the existing number. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The file name of the effect to add.
.EP
.SP 9 5
number - The number to set to the effect to.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The current value of the effect in the mapping

.EP

.SI 3
* %^BOLD%^add_external_effect%^RESET%^
.EI
.SI 5
int add_external_effect(string word, int number)
.EI
.SP 7 5

This adds an external effect to the food.  This will be added to
the player or NPC when the liquid is splashed, applied, or rubbed on them.


The effect is added with a number as the argument. The number is based on the amount which is splashed (in weight units), and usually represents the strength of the effect. If the object is continuous, the weight is calculated from the amount and weight_unit array... 


.EP
.SP 7 5
eff_num = (amount * number * weight_unit[0]) / weight_unit[1] 

...where the number is passed into the add_external_effect() function. If the effect already exists, then the number is added onto the existing number. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The file name of the effect to add.
.EP
.SP 9 5
number - The number to set to the effect to.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The current value of the effect in the mapping

.EP

.SI 3
* %^BOLD%^check_for_container%^RESET%^
.EI
.SI 5
int check_for_container()
.EI
.SP 7 5

This checks the outside of us for a container and then destroys ourselves
if it is not a container.

.EP

.SI 3
* %^BOLD%^check_sharp%^RESET%^
.EI
.SI 5
int check_sharp(object weapon)
.EI
.SP 7 5

This checks to see if a weapon is sharp.  This is used later
to determine whather an object can be cut up with the weapon.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
weapon - The weapon to check to see if it is sharp.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if it is sharp, or 0 otherwise.

.EP

.SI 3
* %^BOLD%^consume%^RESET%^
.EI
.SI 5
varargs void consume(object consumer, int amount, string type)
.EI
.SP 7 5

This consumes some of the food.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
consumer - The person doing the consumption.
.EP
.SP 9 5
amount - The amount being consumed.
.EP
.SP 9 5
type - Use "splash" or "apply" or "external" to use external
 effects, anything else for eat effects.


.EP

.SI 3
* %^BOLD%^do_apply%^RESET%^
.EI
.SI 5
int do_apply(object * things)
.EI
.SP 7 5

This handles applying things

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if it succeeded, or return 0 if it failed.

.EP

.SI 3
* %^BOLD%^do_cure%^RESET%^
.EI
.SI 5
int do_cure()
.EI
.SP 7 5

This is called by the pickling stick.  It stops the food
object decaying.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
returns 1 for suceess and 0 for failure.

.EP

.SI 3
* %^BOLD%^do_cut%^RESET%^
.EI
.SI 5
varargs int do_cut(int num_pieces)
.EI
.SP 7 5

This cuts the food up into bits.  This is the command called
with add_command() and does the actual cutting up into bits.  If
the num_pieces parameter is undefined, the food is cut in half.
If the food is continuous, the cutting is handled by
the state_change handler (all continuous objects must be cut this
way).

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num_pieces - The number of pieces to cut the food into.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it succeeded, 0 if not.

.EP

.SI 3
* %^BOLD%^do_drink%^RESET%^
.EI
.SI 5
int do_drink()
.EI
.SP 7 5

This gives the appropriate message for drinking things.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if it succeeded, or return 0 if it failed.

.EP

.SI 3
* %^BOLD%^do_dry%^RESET%^
.EI
.SI 5
int do_dry()
.EI
.SP 7 5

This method dries the food.  Dried food no longer decays.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
returns 1 for suceess and 0 for failure.

.EP

.SI 3
* %^BOLD%^do_eat%^RESET%^
.EI
.SI 5
varargs int do_eat(int no_mess)
.EI
.SP 7 5

This is the eat command call.  This is setup with add_command()
and does the actual eating.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
no_mess - don't use success messages
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 on success, 2 on success + last bit eaten and return 0 on failure.
.EP

.SI 3
* %^BOLD%^do_mince%^RESET%^
.EI
.SI 5
varargs int do_mince()
.EI

.SI 3
* %^BOLD%^do_quaff%^RESET%^
.EI
.SI 5
int do_quaff()
.EI
.SP 7 5

This gives the appropriate message for quaffing things. Its
rather like drinking but a lot of it gets splashed on you.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if it succeeded, or return 0 if it failed.

.EP

.SI 3
* %^BOLD%^do_splash%^RESET%^
.EI
.SI 5
int do_splash(object * things)
.EI

.SI 3
* %^BOLD%^query_applicable%^RESET%^
.EI
.SI 5
int query_applicable(object applier, object appliee)
.EI
.SP 7 5

This queries applicability of the food. (rubbing on someone)
Note that the food can only be applied when
set_applicable() has been called
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
applier - - Who applies it
.EP
.SP 9 5
appliee - - Who it is applied to.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it can be applied, 0 if it can't
.EP

.SI 3
* %^BOLD%^query_bites_gone%^RESET%^
.EI
.SI 5
int query_bites_gone()
.EI
.SP 7 5

This returns the number of bites that have been taken out
of the food object.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The number of bites which have been taken from
the object.
.EP

.SI 3
* %^BOLD%^query_cured%^RESET%^
.EI
.SI 5
int query_cured()
.EI
.SP 7 5

This returns the cured state of the bit.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is cured, 0 if not

.EP

.SI 3
* %^BOLD%^query_decay_divisor%^RESET%^
.EI
.SI 5
float query_decay_divisor()
.EI
.SP 7 5
This tells us the current divisor used to determine decay.  This can
 be modified with set_decay_speed.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
float the divisor
.EP

.SI 3
* %^BOLD%^query_decay_level%^RESET%^
.EI
.SI 5
int query_decay_level()
.EI
.SP 7 5

This returns the level of the decay.  This will be a number
between 1 and 6.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The level of decay, which is between 1 and 6.

.EP

.SI 3
* %^BOLD%^query_decay_speed%^RESET%^
.EI
.SI 5
int query_decay_speed()
.EI
.SP 7 5

This returns the decay speed.  If this is 0 then the object
is not decaying.  This is the length of time it takes
for the whole object to decay.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The decay speed.

.EP

.SI 3
* %^BOLD%^query_decays%^RESET%^
.EI
.SI 5
int query_decays()
.EI
.SP 7 5

This query tells us whether the food decays.  All objects
which decay should have this function on them.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if the object decays.
.EP

.SI 3
* %^BOLD%^query_dried%^RESET%^
.EI
.SI 5
int query_dried()
.EI
.SP 7 5

This returns the dried state of the bit.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is dried, 0 if not

.EP

.SI 3
* %^BOLD%^query_eat_effects%^RESET%^
.EI
.SI 5
mapping query_eat_effects()
.EI
.SP 7 5

The eat effects of the food.  The mapping contains a set of
strings (the effect) and a number associated with the
effect.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A mapping containing the eat effects.

.EP

.SI 3
* %^BOLD%^query_eat_func%^RESET%^
.EI
.SI 5
string query_eat_func()
.EI
.SP 7 5

This returns the name of the eat function to carry out
when the object is eaten.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A string which is the name of the eat function.

.EP

.SI 3
* %^BOLD%^query_eat_mess%^RESET%^
.EI
.SI 5
string query_eat_mess()
.EI
.SP 7 5

This returns the eat message of the object.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A string of text which is the eat message.

.EP

.SI 3
* %^BOLD%^query_eat_object%^RESET%^
.EI
.SI 5
string query_eat_object()
.EI
.SP 7 5

This returns the eat object on which the eat function
of the food of the food is stored.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A string which is the the name of the eat object.

.EP

.SI 3
* %^BOLD%^query_edible%^RESET%^
.EI
.SI 5
int query_edible()
.EI
.SP 7 5

This tells us if the food is edible (ie. can be
consumed using 'eat' instead of 'drink'.  If it is
edible then it is not a liquid.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if the food is a liquid, and return 0 if it is not.

.EP

.SI 3
* %^BOLD%^query_external_effects%^RESET%^
.EI
.SI 5
mapping query_external_effects()
.EI
.SP 7 5

The external (splash,apply,rub) effects of the food.  The
mapping contains a set of strings (the effect) and a number
associated with the effect.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A mapping containing the external effects.

.EP

.SI 3
* %^BOLD%^query_food_object%^RESET%^
.EI
.SI 5
int query_food_object()
.EI
.SP 7 5

This method returns 1 if the item is a food object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 when it is a food object

.EP

.SI 3
* %^BOLD%^query_in_pieces%^RESET%^
.EI
.SI 5
int query_in_pieces()
.EI
.SP 7 5

If the food has been cut up (using the 'cut' command)
then this will return true.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if it is in pieces, and return 0 otherwise.

.EP

.SI 3
* %^BOLD%^query_liquid%^RESET%^
.EI
.SI 5
int query_liquid()
.EI
.SP 7 5

This function returns one if the food is a liquid.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if the food is a liquid, and return 0 if it is not.

.EP

.SI 3
* %^BOLD%^query_long_decay_level%^RESET%^
.EI
.SI 5
string query_long_decay_level()
.EI
.SP 7 5

This method returns the decay level for the object as a string, 
it is used in the long() function.

.EP

.SI 3
* %^BOLD%^query_long_eat_level%^RESET%^
.EI
.SI 5
string query_long_eat_level()
.EI
.SP 7 5

This method returns how much of the item has been nibbled on. 
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string used in long().

.EP

.SI 3
* %^BOLD%^query_piece_description%^RESET%^
.EI
.SI 5
string query_piece_description()
.EI
.SP 7 5

This returns the long description of the food object when
it is cut up into pieces.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A string of text which is the piece description.

.EP

.SI 3
* %^BOLD%^query_rotten%^RESET%^
.EI
.SI 5
int query_rotten()
.EI
.SP 7 5
This tells us if the food has started to rot

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Return 1 if the food has started to rot, 0 otherwise.

.EP

.SI 3
* %^BOLD%^query_short_rotten_adjective%^RESET%^
.EI
.SI 5
string query_short_rotten_adjective()
.EI
.SP 7 5

This method returns the current rotten adjective that's used 
in the short description for decaying objects.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string like slightly rotten, mostly rotten, etc.

.EP

.SI 3
* %^BOLD%^query_splashable%^RESET%^
.EI
.SI 5
int query_splashable(object splasher, object splashee)
.EI
.SP 7 5

This queries splashability of the food.
Note that the food can only be splashed when liquid and when
set_splashable() has been called
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
splasher - - Who splashes.
.EP
.SP 9 5
splashee - - Who is splashed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it can splash, 0 if it can't
.EP

.SI 3
* %^BOLD%^query_weight_per_bite%^RESET%^
.EI
.SI 5
int query_weight_per_bite()
.EI
.SP 7 5

This sets the weight of each bite of the food object.
This is used along with the total weight of the object
to calculate how many bites of the food can be taken.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The weight of each bite.

.EP

.SI 3
* %^BOLD%^remove_eat_effect%^RESET%^
.EI
.SI 5
void remove_eat_effect(string word)
.EI
.SP 7 5

This removes the eat effect of the given name.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The name of the effect to delete.
.EP

.SI 3
* %^BOLD%^remove_external_effect%^RESET%^
.EI
.SI 5
void remove_external_effect(string word)
.EI
.SP 7 5

This removes the external effect of the given name.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The name of the effect to delete.
.EP

.SI 3
* %^BOLD%^reset_liquid%^RESET%^
.EI
.SI 5
void reset_liquid()
.EI
.SP 7 5

This changes the food object into a solid.  Solids are not
'continuous' and the decay speed will be set back to the
default.

.EP

.SI 3
* %^BOLD%^set_applicable%^RESET%^
.EI
.SI 5
void set_applicable()
.EI
.SP 7 5

This sets it so the food can be applied.
Note this is the default case.
.EP

.SI 3
* %^BOLD%^set_bites_gone%^RESET%^
.EI
.SI 5
void set_bites_gone(int number)
.EI
.SP 7 5

This sets the number of bites gone.  This helps determine
the total weight of the object.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - The number of bites gone.

.EP

.SI 3
* %^BOLD%^set_decay_level%^RESET%^
.EI
.SI 5
void set_decay_level(int level)
.EI
.SP 7 5

This sets the decay level.  This should be anumber
between 1 and 6.  It lets you make a food object seem
as though it is already decaying.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
level - The new decay level.

.EP

.SI 3
* %^BOLD%^set_decay_speed%^RESET%^
.EI
.SI 5
void set_decay_speed(int decay)
.EI
.SP 7 5

This sets the speed at which a food object decays.  The
speed is set in seconds and is the total amount of time
before the food decays.


If the decay speed is set to 0, then the object will never decay. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
speed - The speed at which the object will decays.
.EP

.SI 3
* %^BOLD%^set_eat_effects%^RESET%^
.EI
.SI 5
void set_eat_effects(mapping map)
.EI
.SP 7 5

This sets all the eat effects.  This function should NOT
be used.  Please use add_eat_effect.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - The eat effects mapping.

.EP

.SI 3
* %^BOLD%^set_eat_func%^RESET%^
.EI
.SI 5
void set_eat_func(string word)
.EI
.SP 7 5

This sets the eat function of the food.  This is called when
the food is eaten.


If the eat object is set, then the eat function needs to be set as well. If the function is not set then setting the object has no effect. 

The function will be called with two parameters, the first being the object being eaten and the second being the person eating the object. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The function to set as the eat function.
.EP

.SI 3
* %^BOLD%^set_eat_mess%^RESET%^
.EI
.SI 5
void set_eat_mess(mixed word)
.EI
.SP 7 5

This sets the eat function of the food.  The function is
carried out when the food is consumed.  The eat message,
set with set_eat_mess(), is displayed when this function
is carried out.


The eat mess should be treated exactly the same as a add_succeeded_mess(), as it is processed later as add_succeeded_mess(eat_mess, ({ })); 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The string or array to be used as
the add_succeeded_mess() when eating the food.

.EP

.SI 3
* %^BOLD%^set_eat_object%^RESET%^
.EI
.SI 5
void set_eat_object(string word)
.EI
.SP 7 5

This sets the object on which the eat function is stored.


If the eat object is set, then the eat function needs to be set as well. If the function is not set then setting the object has no effect. 

The function will be called with two parameters, the first being the object being eaten and the second being the person eating the object. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The object containing the eat function.
.EP

.SI 3
* %^BOLD%^set_external_effects%^RESET%^
.EI
.SI 5
void set_external_effects(mapping map)
.EI
.SP 7 5

This sets all the external effects.  This function should NOT
be used.  Please use add_external_effect.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - The external effects mapping.

.EP

.SI 3
* %^BOLD%^set_external_pk_check%^RESET%^
.EI
.SI 5
void set_external_pk_check()
.EI
.SP 7 5

This makes the external effects pk checked.
.EP

.SI 3
* %^BOLD%^set_in_pieces%^RESET%^
.EI
.SI 5
void set_in_pieces(int number)
.EI
.SP 7 5

This sets the in_pieces flag.  This allows you to make the
food seem as though it has already been cut into pieces.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - The number of pieces.
.EP

.SI 3
* %^BOLD%^set_liquid%^RESET%^
.EI
.SI 5
void set_liquid()
.EI
.SP 7 5

This changes the food object into a liquid.  Liquids are
automatically a 'continuous' medium, which means it will
combine with other liquids of the same type.


This will automatically set the decay speed to 0. Liquids do not decay. 
.EP

.SI 3
* %^BOLD%^set_piece_description%^RESET%^
.EI
.SI 5
void set_piece_description(string word)
.EI
.SP 7 5

This sets the long description of the food when it is has
'cut' into pieces.


If this is set then the short of the object is changed to 'piece of cake'. If the piece description is not set the short description of the object is not changed. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The long description to use when cut into pieces.
.EP

.SI 3
* %^BOLD%^set_piece_plural%^RESET%^
.EI
.SI 5
void set_piece_plural(string plural)
.EI
.SP 7 5

This plural description used for the pieces.  It is only
really of use if the piece short pluralises in an unusual
manner.  If it is not set the piece plural is set to what
the pice short has been set to with an "s" stuck on the
end, for example, slice to slices.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The plural used for the 'pieces' when cut into
pieces.
.EP

.SI 3
* %^BOLD%^set_piece_short%^RESET%^
.EI
.SI 5
void set_piece_short(string short)
.EI
.SP 7 5

This is the description used in the short for the pieces.
When the object is cut up this is used as part of the
short description.  For example, it can change
'piece of cake' to 'slice of cake', 'piece of chocolate'
to 'square of chocolate', and so on.  If this the piece
short has not been set it is set to the default which is
simply "piece".

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The short to use for the 'piece' when cut into
pieces.
.EP

.SI 3
* %^BOLD%^set_piece_substance%^RESET%^
.EI
.SI 5
void set_piece_substance(string substance)
.EI
.SP 7 5

This sets a new description for what there are pieces of.
For example, if you want the short of the food before it
is cut up to be 'bar of chocolate', without the piece
substance being set, when it is cut up it will become
'a piece of bar of chocolate'.  If you set this in enables
you to change that unwieldly description to 'a piece of
chocolate'.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The description of what the 'piece' is of.
.EP

.SI 3
* %^BOLD%^set_splashable%^RESET%^
.EI
.SI 5
void set_splashable()
.EI
.SP 7 5

This sets it so the food can be splashed.
Note that the food can only be splashed when liquid in any case.
Note this is NOT the default case.
.EP

.SI 3
* %^BOLD%^set_weight_per_bite%^RESET%^
.EI
.SI 5
void set_weight_per_bite(int number)
.EI
.SP 7 5

This sets weight of each bite of the food object.  This is
used along with the total weight of the object to calculate
how many bites of the food can be taken.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - The weight each bite should be.

.EP

.SI 3
* %^BOLD%^setup_eat%^RESET%^
.EI
.SI 5
void setup_eat(string ob, string func)
.EI
.SP 7 5

This sets up the eat object and function.  This is used when
the food is eaten.  The eat function is called on the eat
object.  If successful the eat message is added to the player.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - The object to use.
.EP
.SP 9 5
func - The function to use.

.EP

.SI 3
* %^BOLD%^setup_eat_piece_messages%^RESET%^
.EI
.SI 5
void setup_eat_piece_messages(mixed piece, mixed amt, mixed last)
.EI
.SP 7 5

This sets up the eat messages for pieces and stuff.  The amount message
will replace the $amt$ string in the message with the amount_string().  
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
piece - the eat piece message. This can be either a string or an
array, it is passed to add_succeeded_mess().
.EP
.SP 9 5
amt - the amount messages
.EP
.SP 9 5
last - the message to print out when the last bit gets eaten

.EP

.SI 3
* %^BOLD%^unset_applicable%^RESET%^
.EI
.SI 5
void unset_applicable()
.EI
.SP 7 5

This sets it so the food cannot be applied.
.EP

.SI 3
* %^BOLD%^unset_external_pk_check%^RESET%^
.EI
.SI 5
void unset_external_pk_check()
.EI
.SP 7 5

This makes the external effects not pk checked.
.EP

.SI 3
* %^BOLD%^unset_splashable%^RESET%^
.EI
.SI 5
void unset_splashable()
.EI
.SP 7 5

This sets it so the food cannot be splashed.
Note that the food can only be splashed when liquid in any case.
.EP


