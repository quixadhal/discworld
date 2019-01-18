.DT
condition.c
Disk World autodoc help
condition.c

.SH Description
.SP 5 5

This class keeps track of the condition of a particular object.  It
handles damaging the object when it is hit and the lowest conditon to
which the object has been damaged.  The lowest conditon keeps track of
how hard the object is to fix.  If an object has been reduced to really
shaggy condition at one point, it will be a lot harder to fix from then
on.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h and /include/armoury.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_cond%^RESET%^
.EI
.SI 5
int adjust_cond(int i)
.EI
.SP 7 5

This method adjust the current condition by the given amount.  If
resultant condition is less than the lowest condition, then the lowest
condition is set to this value.  If it is greater than the maximum
condition.  Then the condition is set to the maximum condition.  If the
condition is less than or equal to 0, then the object is attempted to
be broken.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the amount to adjust the condition by
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new condition
.EP

.SI 3
* %^BOLD%^break_me%^RESET%^
.EI
.SI 5
void break_me()
.EI
.SP 7 5

This method breaks the object.  This moves it into the recycled
object section of the armoury or dests itself if this is not
possible.

.EP

.SI 3
* %^BOLD%^cond_string%^RESET%^
.EI
.SI 5
string cond_string()
.EI
.SP 7 5

This method returns the current condition as a string.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string value of the condition

.EP

.SI 3
* %^BOLD%^do_damage%^RESET%^
.EI
.SI 5
void do_damage(string type, int amount)
.EI
.SP 7 5

This method damages the object.  The conditon is reduced by
the amount multiplied by 1 + 2 * random(damage_chance) ewhic
is then divided by 100.  This means it does approximately
the percentage of of the damage done to the object to its condition.
.EP

.SI 3
* %^BOLD%^query_cond%^RESET%^
.EI
.SI 5
int query_cond()
.EI
.SP 7 5

This method returns the current conditon.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current condition

.EP

.SI 3
* %^BOLD%^query_damage_chance%^RESET%^
.EI
.SI 5
int query_damage_chance()
.EI
.SP 7 5

This method returns the percentage of the damage which is done to the
object which will be carried through.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the damage chance
.EP

.SI 3
* %^BOLD%^query_hide_cond%^RESET%^
.EI
.SI 5
int query_hide_cond()
.EI
.SP 7 5

This method returns whether the condition string returns "" or something 
meaning full.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current lowest condition

.EP

.SI 3
* %^BOLD%^query_lowest_cond%^RESET%^
.EI
.SI 5
int query_lowest_cond()
.EI
.SP 7 5

This method returns the current lowest conditon.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current lowest condition

.EP

.SI 3
* %^BOLD%^query_max_cond%^RESET%^
.EI
.SI 5
int query_max_cond()
.EI
.SP 7 5

This method returns the current maximum conditon.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current maximum condition

.EP

.SI 3
* %^BOLD%^set_cond%^RESET%^
.EI
.SI 5
void set_cond(int i)
.EI
.SP 7 5

This method sets the current condition of the object.  If this is lower
than the current lowest conditon, then the lowest condition will be modified
to be this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new condition
.EP

.SI 3
* %^BOLD%^set_damage_chance%^RESET%^
.EI
.SI 5
void set_damage_chance(int number)
.EI
.SP 7 5

This method sets the percentage of the damage which is done to the
object which will be carried through.


Use the following chart to set the damage chance. 
.EP
.SP 7 5

.EP
.SP 7 5
o==================o=====================o
.EP
.SP 7 5
| Material | Damage Chance |
.EP
.SP 7 5
o==================o=====================o
.EP
.SP 7 5
| Cloth | 20 |
.EP
.SP 7 5
| Hide | 17 |
.EP
.SP 7 5
| Bone | 16 |
.EP
.SP 7 5
| Leather | 15 |
.EP
.SP 7 5
| Wood | 12 |
.EP
.SP 7 5
| Copper | 10 |
.EP
.SP 7 5
| Bronze | 8 |
.EP
.SP 7 5
| Iron | 6 |
.EP
.SP 7 5
| Steel | 5 |
.EP
.SP 7 5
| Klatchian Steel | 3 |
.EP
.SP 7 5
| Stone | 3 |
.EP
.SP 7 5
| Octiron | 0 |
.EP
.SP 7 5
o==================o=====================o 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new damage chance

.EP

.SI 3
* %^BOLD%^set_hide_cond%^RESET%^
.EI
.SI 5
void set_hide_cond(int i)
.EI
.SP 7 5

This sets whether the condition string should be returned or not.  This
is for objects what indicate their condition with different long
descriptions instead of the default condition string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - set it to 1 to hide the condition string.

.EP

.SI 3
* %^BOLD%^set_lowest_cond%^RESET%^
.EI
.SI 5
void set_lowest_cond(int i)
.EI
.SP 7 5

This sets the lowest condition for the object.  The lowest cond
variable is used for repairing the armour...
See where it was repaired to last time, this determines how difficult
repairs will be...
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new lowest condition
.EP

.SI 3
* %^BOLD%^set_max_cond%^RESET%^
.EI
.SI 5
void set_max_cond(int i)
.EI
.SP 7 5

This sets up how much damage an object can take before it 
breaks.  This should not be used in the actual object files
unless under special circumstances.  In weapon files, use 
new_weapon().  In armour files use setup_armour().  In 
clothing files, use setup_clothing().  Use ::Setup:: in 
clo, arm, and wep files.


The values may seem strange, as cloth can take more damage per unit weight than steel. This is because steel weighs so much, and cloth weighs so little. 
.EP
.SP 7 5

.EP
.SP 7 5
o==================o=====================o
.EP
.SP 7 5
| Material | Maximum Condition |
.EP
.SP 7 5
| | per unit weight |
.EP
.SP 7 5
o==================o=====================o
.EP
.SP 7 5
| Cloth | 90 |
.EP
.SP 7 5
| Hide | 80 |
.EP
.SP 7 5
| Leather | 70 |
.EP
.SP 7 5
| Wood | 60 |
.EP
.SP 7 5
| Bone | 45 |
.EP
.SP 7 5
o==================o=====================o
.EP
.SP 7 5
| Copper | 30 |
.EP
.SP 7 5
| Stone | 40 |
.EP
.SP 7 5
| Bronze | 40 |
.EP
.SP 7 5
| Iron | 50 |
.EP
.SP 7 5
| Steel | 60 |
.EP
.SP 7 5
| Klatchian Steel | 70 |
.EP
.SP 7 5
| Octiron | 80 |
.EP
.SP 7 5
o==================o=====================o 
.EP

.SI 3
* %^BOLD%^set_percentage%^RESET%^
.EI
.SI 5
void set_percentage(int i)
.EI
.SP 7 5

This method is used to setup the objects condition.  Once the maximum
condition has been set, this is used to set the maximum condition to
this percentage of the maximum condition.
It sets the lowest conditon a random value from 70-100% of the
current condition.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the percentage condiion
.EP


