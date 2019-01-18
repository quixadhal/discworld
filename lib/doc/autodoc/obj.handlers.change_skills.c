.DT
change_skills.c
Disk World autodoc help
change_skills.c

.SH Description
.SP 5 5

Handles updating the skill systems to new levels when the skill tree
is changed.
.EP
.SP 10 5


Written by Deutha
.EP



.SH See also
.SP 5 5
/std/living/skills.c
.EP
.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/player.h, /include/tune.h, /include/language.h, /include/combat.h, /include/skills.h, /include/login_handler.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^change_language_skills%^RESET%^
.EI
.SI 5
void change_language_skills(object thing, int lev)
.EI
.SP 7 5

Changes the skill array up to the new tree involving the
languages.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object to upgrade
.EP
.SP 9 5
lev - the level we are changing up to.

.EP

.SI 3
* %^BOLD%^change_skills%^RESET%^
.EI
.SI 5
void change_skills(object thing)
.EI
.SP 7 5

Update the skills from the original skills system to the
beta skills system.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the person to update

.EP

.SI 3
* %^BOLD%^change_skills_epsilon%^RESET%^
.EI
.SI 5
void change_skills_epsilon(object thing)
.EI
.SP 7 5

Update the skills from the original skills system to the
beta skills system.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the person to update

.EP

.SI 3
* %^BOLD%^change_skills_eta%^RESET%^
.EI
.SI 5
void change_skills_eta(object thing)
.EI

.SI 3
* %^BOLD%^change_skills_zeta%^RESET%^
.EI
.SI 5
void change_skills_zeta(object thing)
.EI
.SP 7 5

Change everyones skill levels (downwards).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the person to update

.EP

.SI 3
* %^BOLD%^entering%^RESET%^
.EI
.SI 5
void entering(string name, string type)
.EI
.SP 7 5

Called by the login handler, checks to see if an upgrade
is neede.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player logging in
.EP
.SP 9 5
type - the type of the login
.EP

.SI 3
* %^BOLD%^fixup_stats%^RESET%^
.EI
.SI 5
void fixup_stats(object thing)
.EI

.SI 3
* %^BOLD%^new_skill_value%^RESET%^
.EI
.SI 5
int new_skill_value(int sk)
.EI
.SP 7 5

Calulate the new skill value for the skills version zeta

.EP

.SI 3
* %^BOLD%^query_minimum_skill_xp%^RESET%^
.EI
.SI 5
int query_minimum_skill_xp(int lvl)
.EI
.SP 7 5

Figure out thge minimum amount of xp that could have been used to
get the skill level.  This assumes a leaf skill
This appears to be the xp equation.  Using it anyway...
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lvl - the level to get to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skill cost in xp

.EP

.SI 3
* %^BOLD%^query_version%^RESET%^
.EI
.SI 5
string query_version()
.EI
.SP 7 5

The current version of the skill tree.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the version of the skill system

.EP

.SI 3
* %^BOLD%^recalc_parents%^RESET%^
.EI
.SI 5
mapping recalc_parents(mapping skills, string skill)
.EI


