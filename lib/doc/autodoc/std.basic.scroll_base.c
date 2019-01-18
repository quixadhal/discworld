.DT
scroll_base.c
Disk World autodoc help
scroll_base.c

.SH Description
.SP 5 5

This is the basic control file for scrolls.  It contains all the common
scroll handling stuff that goes into both the object and the shadow.

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/scroll_book_react.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^cast_spell%^RESET%^
.EI
.SI 5
int cast_spell(string words)
.EI
.SP 7 5

This method clears the spell off the scroll and turns it back into
a normal piece of paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
award - a tm was awarded on the way

.EP

.SI 3
* %^BOLD%^cast_spell_new%^RESET%^
.EI
.SI 5
int cast_spell_new(string words, object * targets, object * using)
.EI
.SP 7 5

This method clears the spell off the scroll and turns it back into
a normal piece of paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
award - a tm was awarded on the way

.EP

.SI 3
* %^BOLD%^clear_spell%^RESET%^
.EI
.SI 5
void clear_spell(int award)
.EI
.SP 7 5

This method clears the spell off the scroll and turns it back into
a normal piece of paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
award - a tm was awarded on the way

.EP

.SI 3
* %^BOLD%^query_binding_force%^RESET%^
.EI
.SI 5
int query_binding_force()
.EI
.SP 7 5

This method returns the current binding force of the scroll.  This is
the amount of force the paper exerts to try and keep the spell on the
paper.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the scrolls binding force

.EP

.SI 3
* %^BOLD%^query_magic_scroll%^RESET%^
.EI
.SI 5
int query_magic_scroll()
.EI
.SP 7 5

This method returns always returns 1 for a magic scroll.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^query_spell%^RESET%^
.EI
.SI 5
string query_spell()
.EI
.SP 7 5

This method returns the spell associated with the scroll.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the spell associated with the scroll

.EP

.SI 3
* %^BOLD%^query_spell_name%^RESET%^
.EI
.SI 5
string query_spell_name()
.EI
.SP 7 5

This method returns the name o fthe spell on the scroll.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the spell

.EP

.SI 3
* %^BOLD%^query_spell_power_level%^RESET%^
.EI
.SI 5
int query_spell_power_level()
.EI
.SP 7 5

This method returns the power level of the spell current associated
with the object.

.EP

.SI 3
* %^BOLD%^set_spell%^RESET%^
.EI
.SI 5
void set_spell(string word)
.EI
.SP 7 5

This method sets the spell associated with the scroll.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the spells path

.EP

.SI 3
* %^BOLD%^test_skill%^RESET%^
.EI
.SI 5
int test_skill()
.EI
.SP 7 5

This method tests the skill needed to cast the spell on the scroll.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skill needed to cast

.EP


