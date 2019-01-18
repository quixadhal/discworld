.DT
bath_house.c
Disk World autodoc help
bath_house.c

.SH Description
.SP 5 5

Bath house inheritable!
.EP
.SP 10 5


Written by Taffyd

Started 30/04/00 12:08

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/effect.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_baths%^RESET%^
.EI
.SI 5
int query_baths()
.EI

.SI 3
* %^BOLD%^query_wash_classifications%^RESET%^
.EI
.SI 5
string * query_wash_classifications()
.EI
.SP 7 5

This returns a list of all the effect classifications that are currently
being managed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of strings.

.EP

.SI 3
* %^BOLD%^wash_player%^RESET%^
.EI
.SI 5
int wash_player(object ob)
.EI
.SP 7 5

This method washes a player.  Any wash commands that are defined
must ultimately call this function, so that all of the wash
functions can be evaluated.


When an effect is found on the player for a matching wash effect classification, the function specified is evaluated with the following parameters: 
.EP
.SO 8 2 -12

	*	ob, the player who is being washed 
	*	classification, the classification of the effect that was matched. 
	*	arg, the arguments to the effect. 
	*	effect_enum, the position of the effect in the player's effect list. 
	*	effect_ob, the object that the effect is found in. 
.EO
.SP 7 5


After this, the finished functions are evaluated with two arguments, the first one being ob, the second one being an array of all the wash effect classifications that were matched correctly.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to wash
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the player was washed successfully, 0 if they weren't.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^add_finished_func%^RESET%^
.EI
.SI 5
int add_finished_func(function func)
.EI
.SP 7 5

This adds a function to be evalauted when all of the wash effects
have been evaluated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to be evaluated
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1

.EP

.SI 3
* %^BOLD%^add_wash_effect%^RESET%^
.EI
.SI 5
int add_wash_effect(string classification, function func)
.EI
.SP 7 5

This method adds an effect to be removed when washing here.
If the effect is matched, then func is evaluated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
classification - the effect classification to match.
.EP
.SP 9 5
func - the function to evaluate when the effect is matched.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the effect classification was added, 0 if it could not be
(most likely a duplicate effect classification).
.EP

.SI 3
* %^BOLD%^advanced_antiseptic%^RESET%^
.EI
.SI 5
void advanced_antiseptic(object ob, string classification, int effect_enum, mixed arg, object effect_ob)
.EI
.SP 7 5

This is a basic cleaning method that can be used to remove a few
default things.

.EP

.SI 3
* %^BOLD%^apply_soap%^RESET%^
.EI
.SI 5
void apply_soap(object ob, string * applied_effects)
.EI
.SP 7 5

This is the default finished func. It adds soap.

.EP

.SI 3
* %^BOLD%^basic_cleaning%^RESET%^
.EI
.SI 5
void basic_cleaning(object ob, int effect_enum, string message)
.EI
.SP 7 5

This is a small cut down cleaning effect that displays a message.

.EP

.SI 3
* %^BOLD%^remove_wash_effect%^RESET%^
.EI
.SI 5
int remove_wash_effect(string classification)
.EI
.SP 7 5

This method removes a wash effect.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the - classification of the effect to be removed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the effect was successfully removed, 0 if it wasn't.

.EP

.SI 3
* %^BOLD%^wet_person%^RESET%^
.EI
.SI 5
void wet_person(object ob, string * applied_effects)
.EI


