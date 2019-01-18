.DT
mag_eff_handler.c
Disk World autodoc help
mag_eff_handler.c

.SH Description
.SP 5 5

This handler controls the ambient enchantment effects and effects
from solid grains of octarine.  Ambient enchantment is caused by
the use of spells in certain locations.  Ambient enchantment can
be blocked by use of the property "enchantment block".
.EP
.SP 10 5


Written by Deutha

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_octonite_crystal%^RESET%^
.EI
.SI 5
void add_octonite_crystal(object crystal)
.EI
.SP 7 5

This method adds an octonite crystal into the current list of
octonite crystals.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
crystal - the crystal to add
.EP

.SI 3
* %^BOLD%^ambient_enchantment%^RESET%^
.EI
.SI 5
int ambient_enchantment(object thing)
.EI
.SP 7 5

This method returns the current ambient enchantment for the particular
object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object to get the ambient enchantment for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the ambient enchantment of the object
.EP

.SI 3
* %^BOLD%^block%^RESET%^
.EI
.SI 5
int block(object thing)
.EI
.SP 7 5

This method checks to see if the particular object is blocking the use
of enchantments.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object to check for enchantment block
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is blocking enchanments, 0 if not
.EP

.SI 3
* %^BOLD%^choose_effect%^RESET%^
.EI
.SI 5
int * choose_effect(int number)
.EI
.SP 7 5

This method chooses a random effect from the curent list of effects
defined in the handler.  It returns an array consisting of
two elements, the first is the number, the second is a random
number between the input number multiplied by 6 and then divided
by 1000.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the intput number
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array as described above
.EP

.SI 3
* %^BOLD%^do_effect%^RESET%^
.EI
.SI 5
void do_effect(int number, string name, object place)
.EI
.SP 7 5

This method causes an effect to occur.  The number is the severity
of the effect.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the severity of the magic infestation
.EP
.SP 9 5
name - the name of the thing being effected
.EP
.SP 9 5
place - the place being effected (room)
.EP

.SI 3
* %^BOLD%^extra_look%^RESET%^
.EI
.SI 5
string extra_look(object thing)
.EI
.SP 7 5

This method returns the extra look information for a piece of octonite.
It gives different messages about size depending on its weight.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object to give na extra look for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the extra information about the octonite pebbles

.EP

.SI 3
* %^BOLD%^query_octonite_crystals%^RESET%^
.EI
.SI 5
object * query_octonite_crystals()
.EI
.SP 7 5

This method returns the current list of octonite cystals handled
by this handler.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of octonite crystals
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^schedule_next%^RESET%^
.EI
.SI 5
void schedule_next()
.EI
.SP 7 5

This schedules the next update to occur.  The updates are for the
octonite crystals.
.EP

.SI 3
* %^BOLD%^update_octonite%^RESET%^
.EI
.SI 5
void update_octonite()
.EI
.SP 7 5

This updatest he values associated with the octonite pebbles.  They
decay away slowly releasing ambient magic into the background.
.EP


