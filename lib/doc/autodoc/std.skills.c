.DT
skills.c
Disk World autodoc help
skills.c

.SH Description
.SP 5 5

This is the skills handler.  It controls all the skills currently in
the game and keeps track of their various properties.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/language.h and /include/skills.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^is_skill_ignored%^RESET%^
.EI
.SI 5
int is_skill_ignored(string skill)
.EI
.SP 7 5

This method returns 1 if the skill being asked about should be ignored.
An ignored skill means that up to this point the fact it is this deep
is ignored.  For example,if 'crafts' is ignored then 'crafts.smithing'
would count as if it was a top level skill.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to check

.EP

.SI 3
* %^BOLD%^query_all_children%^RESET%^
.EI
.SI 5
string * query_all_children(string skill)
.EI
.SP 7 5

This method returns all the children of the specified skill.
This does a deep children finding exercise, returing all the children
as it goes lower and lower into the array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to get the children of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the children
.EP

.SI 3
* %^BOLD%^query_allowed_to_teach%^RESET%^
.EI
.SI 5
int query_allowed_to_teach(string str)
.EI
.SP 7 5

This method checks to see if this particular skill is allowed to be
taught to people.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the skill to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are not allowed to teach it, 0 if they are

.EP

.SI 3
* %^BOLD%^query_children%^RESET%^
.EI
.SI 5
string * query_children(mixed * arr, string path)
.EI
.SP 7 5

This method returns the children of the specified skill chunk.
You need to pass into this array a skill chunk as taken from the
skill array.  It is recursive and returns all the internal children
as well.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arr - the skill chunk
.EP
.SP 9 5
path - the skill bit leading up to this section
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the children of the skill chunk

.EP

.SI 3
* %^BOLD%^query_immediate_children%^RESET%^
.EI
.SI 5
string * query_immediate_children(string skill)
.EI
.SP 7 5

This method returns the immediate children of the specified skill.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to get the children of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the immediate children of the skill

.EP

.SI 3
* %^BOLD%^query_no_bonus%^RESET%^
.EI
.SI 5
int query_no_bonus(string str)
.EI
.SP 7 5

This method checks to see if the specified skill has no bonus associated
with it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the skill to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is no bonus, 0 if there is a bonus

.EP

.SI 3
* %^BOLD%^query_only_leaf%^RESET%^
.EI
.SI 5
int query_only_leaf(string str)
.EI
.SP 7 5

This method determines if only leaf skills can be increased in this
skill tree.  This is used for the other skill tree, to avoid increasing
all the other skills at the same time.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the skill tree to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the tree is only leaf, 0 if not

.EP

.SI 3
* %^BOLD%^query_only_show_if_non_zero%^RESET%^
.EI
.SI 5
int query_only_show_if_non_zero(string str)
.EI
.SP 7 5

This method checks to see if the skill should only be shown on the
skill list if it is non-zero.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the skill to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if should only show non-zero skills, 0 if show any skill

.EP

.SI 3
* %^BOLD%^query_reg_skills%^RESET%^
.EI
.SI 5
mapping query_reg_skills()
.EI
.SP 7 5

This method returns the all of the skills with their children and the
number of children in a nice mapping.  The key of the mapping is the name
of the skill, the value is an array of two elements.  The first is an
array of all the children and the second is the number of children
of the skill.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping as described above

.EP

.SI 3
* %^BOLD%^query_related_skills%^RESET%^
.EI
.SI 5
string * query_related_skills(string skill)
.EI
.SP 7 5

This method also returns all the children of a skill.  It preforms
exactly the same action as query_all_children()
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to get the children of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the children of the skill

.EP

.SI 3
* %^BOLD%^query_skill%^RESET%^
.EI
.SI 5
string query_skill(string * bits)
.EI
.SP 7 5

This method attempts to figure out the path of a skill from an array
of bits.  It verifies that the skill actually exists on the way.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bits - the skill to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the path of the skill

.EP

.SI 3
* %^BOLD%^query_skill_cost%^RESET%^
.EI
.SI 5
int query_skill_cost(string skill)
.EI
.SP 7 5

This method returns the cost of the specified skill.  This is the
the number of children it has.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to check the cost of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the costof the skill

.EP

.SI 3
* %^BOLD%^query_skill_depth%^RESET%^
.EI
.SI 5
int query_skill_depth(string * skill)
.EI
.SP 7 5

This method returns the skill depth for teaching calculations.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to get the depth of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skill depth

.EP

.SI 3
* %^BOLD%^query_skill_path%^RESET%^
.EI
.SI 5
string * query_skill_path(string skill)
.EI
.SP 7 5

This method returns the path to skill.  This is essentially the skill
name broken up on '.'s and returned as an array.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skill path, or 0 if the skill does not exist

.EP

.SI 3
* %^BOLD%^query_skill_stat%^RESET%^
.EI
.SI 5
string query_skill_stat(string skill)
.EI
.SP 7 5

This method returns the stats that effect the skill.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the stats that effect the skill

.EP

.SI 3
* %^BOLD%^query_skill_tree%^RESET%^
.EI
.SI 5
string * query_skill_tree(string skill)
.EI
.SP 7 5

This method returns the skill and all of its parents leading up to
it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to get the tree of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skill tree of the skill

.EP

.SI 3
* %^BOLD%^query_skills%^RESET%^
.EI
.SI 5
mixed * query_skills()
.EI
.SP 7 5

This method returns the raw unfluffed skill array.  This is the same
as the skill array defined in /include/skills.h plus the language skills
and other dynamic skills added.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skills array
.EP


