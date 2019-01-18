.DT
light.c
Disk World autodoc help
light.c

.SH Description
.SP 5 5

This method handles the light changing and control.
.EP
.SP 10 5


Written by Pinkfish
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_inform_light_change%^RESET%^
.EI
.SI 5
void add_inform_light_change(object ob)
.EI
.SP 7 5

This method adds an object to be informed of light change events.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to be informed of light change events

.EP

.SI 3
* %^BOLD%^adjust_light%^RESET%^
.EI
.SI 5
int adjust_light(int number)
.EI
.SP 7 5

This changes the current light level. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the amount to chage it by
.EP

.SI 3
* %^BOLD%^inform_of_light_level_change%^RESET%^
.EI
.SI 5
void inform_of_light_level_change(object ob)
.EI
.SP 7 5

This method tells all our environments that the light level has
changed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object whose environment we should tell about the change
.EP

.SI 3
* %^BOLD%^light_level_changed%^RESET%^
.EI
.SI 5
void light_level_changed()
.EI
.SP 7 5

This method tells us that the light levels have changed and should
be recalculated when queried.
.EP

.SI 3
* %^BOLD%^new_recalc_light%^RESET%^
.EI
.SI 5
varargs void new_recalc_light()
.EI
.SP 7 5

This method forces a recalculation of the current light level in this
object.

.EP

.SI 3
* %^BOLD%^query_light%^RESET%^
.EI
.SI 5
int query_light()
.EI
.SP 7 5

This returns the total light level of the object.   This includes the
light levels of anything that happens to be inside us.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
current light level
.EP

.SI 3
* %^BOLD%^query_light_needs_inform%^RESET%^
.EI
.SI 5
int query_light_needs_inform()
.EI

.SI 3
* %^BOLD%^query_my_light%^RESET%^
.EI
.SI 5
int query_my_light()
.EI
.SP 7 5

This method returns the light associated with this object.   This does
not count the light levels of anything inside us.

.EP

.SI 3
* %^BOLD%^query_opaque%^RESET%^
.EI
.SI 5
int query_opaque()
.EI
.SP 7 5

This method checks to see if the object is opaque or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is opaque, 0 if not

.EP

.SI 3
* %^BOLD%^remove_inform_light_change%^RESET%^
.EI
.SI 5
void remove_inform_light_change(object ob)
.EI
.SP 7 5

This method removes an object from the current list of things to
be informed of a light change.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to remove from the inform list

.EP

.SI 3
* %^BOLD%^reset_opaque%^RESET%^
.EI
.SI 5
void reset_opaque()
.EI
.SP 7 5

This method makes the object transparent (default).   This means that the
light from items inside does escape.
.EP

.SI 3
* %^BOLD%^set_light%^RESET%^
.EI
.SI 5
int set_light(int number)
.EI
.SP 7 5

This sets the current light level.   This method calls adjust_light() 
with the correct value to set the light level.

.EP
.SP 7 5

.EP
.SP 7 5
Light Levels: 
.EP
.SP 7 5
Magically Darkened Room 0- 
.EP
.SP 7 5
Absolutely Dark Room 0 
.EP
.SP 7 5
Standard Dark Room 5 
.EP
.SP 7 5
Dimly Lit Room 20 
.EP
.SP 7 5
Mine Shaft with Candles 30 
.EP
.SP 7 5
Partially Lit Room 50 
.EP
.SP 7 5
Well Lit Room 60 
.EP
.SP 7 5
Shaded Forest 60 
.EP
.SP 7 5
Brightly Lit Room 80 
.EP
.SP 7 5
Direct Sunlight 100 
.EP
.SP 7 5
Explosion or flash 200+ 
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - The new light level.
.EP

.SI 3
* %^BOLD%^set_opaque%^RESET%^
.EI
.SI 5
void set_opaque()
.EI
.SP 7 5

This method makes this object opaque.   This means that light does not
escpae from it.
.EP


