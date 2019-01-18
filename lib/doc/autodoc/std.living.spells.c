.DT
spells.c
Disk World autodoc help
spells.c

.SH Description
.SP 5 5

This method returns the string help for the spell.  It is used by the
help command to find the help for the spell.
.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/spells.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^spell_process%^RESET%^
class spell_process {
mapping spells;
mapping process;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_spell%^RESET%^
.EI
.SI 5
int add_spell(string name, mixed ob, mixed func)
.EI
.SP 7 5

This method adds a spell into the current spell list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the spell to add
.EP
.SP 9 5
ob - the object the spell is on
.EP
.SP 9 5
func - the function to call for the spell
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^help_spell%^RESET%^
.EI
.SI 5
string help_spell(string str)
.EI

.SI 3
* %^BOLD%^query_spell%^RESET%^
.EI
.SI 5
mixed query_spell(string word)
.EI
.SP 7 5

This method finds the data for the spell of the specific name
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the spell to find data on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the data for the spell

.EP

.SI 3
* %^BOLD%^query_spells%^RESET%^
.EI
.SI 5
mapping query_spells()
.EI
.SP 7 5

This method returns the current bunch of spells for the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the spells

.EP

.SI 3
* %^BOLD%^query_spells_nocase%^RESET%^
.EI
.SI 5
mapping query_spells_nocase()
.EI
.SP 7 5

This method returns the list of spells without any case issues.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of spells without any case

.EP

.SI 3
* %^BOLD%^remove_spell%^RESET%^
.EI
.SI 5
int remove_spell(string name)
.EI
.SP 7 5

This method removes the spell of the given name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the spell to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^reset_spells%^RESET%^
.EI
.SI 5
void reset_spells()
.EI
.SP 7 5

This method resets all the spell data.

.EP


