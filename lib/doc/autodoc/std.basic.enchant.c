.DT
enchant.c
Disk World autodoc help
enchant.c

.SH Description
.SP 5 5

Module to handle the amount of magical charge (enchantment) of an
object has.  The maximum enchantment of an item is determined by
by its weight - the material of composition is not yet taken into
account.


Enchanting an object will add to its value, and, if the object is an item of armour, will improve its armour class. 
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_enchant%^RESET%^
.EI
.SI 5
int add_enchant(int number)
.EI
.SP 7 5

This method changes the current enchanment level.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
numnber - the amount to chant the enchantment level by
.EP

.SI 3
* %^BOLD%^enchant_string%^RESET%^
.EI
.SI 5
string enchant_string()
.EI
.SP 7 5

This method returns the current enchantment string for the object.  The 
string is created from the percentage of the max enchantment that it is 
enchanted to.  If the object is also a talisman, an identifying string is 
appended to the enchantment message.


These messages are only visible to creators and players who can see octarine. 

If the property "octarine_mess" is set, then this will be printed instead of the automatically generated enchantment message. 

If the property "talisman_mess" is set, then this will be printed instead of the automatically generated talisman message if appropriate.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the enchantment string and talisman string
.EP

.SI 3
* %^BOLD%^query_degrade_enchant%^RESET%^
.EI
.SI 5
int query_degrade_enchant()
.EI
.SP 7 5

This method returns the maximum enchantment before the enchantment
starts to degrade.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the enchantment level which enchantment degrades to

.EP

.SI 3
* %^BOLD%^query_enchant%^RESET%^
.EI
.SI 5
int query_enchant()
.EI
.SP 7 5

This method queries the current enchantment level. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new enchantment level
.EP

.SI 3
* %^BOLD%^query_enchant_set_time%^RESET%^
.EI
.SI 5
int query_enchant_set_time()
.EI
.SP 7 5

This method returns the time the time the enchantment was set.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time the enchantment was set

.EP

.SI 3
* %^BOLD%^query_max_enchant%^RESET%^
.EI
.SI 5
int query_max_enchant()
.EI
.SP 7 5

This method returns the maximum possible enchantment for the
object.  This is based on its weight.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum enchantment

.EP

.SI 3
* %^BOLD%^query_real_enchant%^RESET%^
.EI
.SI 5
int query_real_enchant()
.EI
.SP 7 5

This method returns the actual enchant of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the actual enchant value

.EP

.SI 3
* %^BOLD%^set_degrade_enchant%^RESET%^
.EI
.SI 5
void set_degrade_enchant(int enchant)
.EI
.SP 7 5

This method sets the enchantment at which things start to degrade.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
enchant - the enchantment level to set it at

.EP

.SI 3
* %^BOLD%^set_enchant%^RESET%^
.EI
.SI 5
void set_enchant(int number)
.EI
.SP 7 5

This method sets the current enchantment level.   If it is greator than
the max enchantment level then it will be set to the maximum
enchantment level.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new enchantment level
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^set_enchant_set_time%^RESET%^
.EI
.SI 5
void set_enchant_set_time(int tim)
.EI
.SP 7 5

This method sets the time at which the enchant level was set.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
tim - the time to set it to

.EP


