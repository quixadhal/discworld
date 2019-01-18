.DT
firework.c
Disk World autodoc help
firework.c

.SH Description
.SP 5 5

This handler helps control the fireworks used in the wizards spells.

.EP
.SP 10 5

.EP

.SH Classes

.SI 3
* %^BOLD%^firework_running%^RESET%^
class firework_running {
string * messages;
int index;
string caster;
string text;
int * co_ord;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_effects%^RESET%^
.EI
.SI 5
mixed * query_effects()
.EI
.SP 7 5

This method returns the list of fireworks currently available.
The list constists of an array of firework effects.  Each effect
consists of a list of strings.  The strings are printed one by
one with a delay between each. 
.EP
.SP 7 5
ie: 
.EP
.SP 7 5
({
.EP
.SP 7 5
  ({
.EP
.SP 7 5
     mess1,
.EP
.SP 7 5
     mess2,
.EP
.SP 7 5
     ...
.EP
.SP 7 5
   }),
.EP
.SP 7 5
  ({
.EP
.SP 7 5
     /* Second effect */
.EP
.SP 7 5
   })
.EP
.SP 7 5
});
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the effects array

.EP

.SI 3
* %^BOLD%^random_firework%^RESET%^
.EI
.SI 5
varargs void random_firework(object caster, string text)
.EI
.SP 7 5

This method creates a random firework at the casters location from the
current list of fireworks.  If the text field is not filled in then
the casters name is used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
caster - the caster of the spell
.EP
.SP 9 5
text - the text to print in the firework
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^do_effects%^RESET%^
.EI
.SI 5
void do_effects()
.EI
.SP 7 5

This method broadcasts the actual firework messages.
.EP


