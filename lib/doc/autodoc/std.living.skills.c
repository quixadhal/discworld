.DT
skills.c
Disk World autodoc help
skills.c

.SH Description
.SP 5 5

This skills modules for living creates.  This deals will all the
skill interactions needed for the living objects.

.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h, /include/tune.h and /include/skills.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_skill_level%^RESET%^
.EI
.SI 5
varargs int add_skill_level(string skill, int lvl, mixed exp)
.EI
.SP 7 5

This method adds a skill level to the specified skill to the
system.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to add a level to
.EP
.SP 9 5
lvl - the number of levels to add
.EP
.SP 9 5
exp - the amount of exp spent on the skill
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the skill level was changed
.EP

.SI 3
* %^BOLD%^add_teach_offer%^RESET%^
.EI
.SI 5
void add_teach_offer(object ob, string skill, int num, int lvl, int xp)
.EI
.SP 7 5

This method adds a teaching offer to the living object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object teaching us
.EP
.SP 9 5
skill - the skill they are teaching
.EP
.SP 9 5
num - the number of levels they are teaching
.EP
.SP 9 5
lvl - the level they are teaching us from
.EP
.SP 9 5
xp - the cost of the level increase in xp

.EP

.SI 3
* %^BOLD%^calc_bonus%^RESET%^
.EI
.SI 5
varargs int calc_bonus(int lvl, string skill, int use_base_stats)
.EI
.SP 7 5

This method calculates the bonus for the skill.  It takes the raw
level and turns that into a bonus and then adds on the stats
modifications.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lvl - the level to turn into bonus
.EP
.SP 9 5
skill - the skill to modify the bonus of
.EP
.SP 9 5
use_base_stats - use the real unmodified stats
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the bonus associated with the skill

.EP

.SI 3
* %^BOLD%^not_there%^RESET%^
.EI
.SI 5
int not_there(string skill)
.EI
.SP 7 5

This method checks to see if the skill exists in the skill array or
not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to check for non-existance
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if it does not exist, 1 if it does

.EP

.SI 3
* %^BOLD%^query_bonus_cache%^RESET%^
.EI
.SI 5
mapping query_bonus_cache()
.EI
.SP 7 5

This method returns the current bonus cache for the living thing.
The bonus cache is where the calculated bonuses for the skills are
kept.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the bonus cache mapping

.EP

.SI 3
* %^BOLD%^query_complete_skill_branch%^RESET%^
.EI
.SI 5
mapping query_complete_skill_branch(string branch)
.EI
.SP 7 5

This method fills out a complete skill branch, complete with ALL child skills.
It saves using many call_others to check skills.
.EP

.SI 3
* %^BOLD%^query_skill%^RESET%^
.EI
.SI 5
int query_skill(string skill)
.EI
.SP 7 5

This returns jus the skill level.  Used a lot to determine if you
can use/teach/whatever a skill.
This also uses a cache.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to return the level of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skill level

.EP

.SI 3
* %^BOLD%^query_skill_bonus%^RESET%^
.EI
.SI 5
varargs int query_skill_bonus(string skill, int use_base_stats)
.EI
.SP 7 5

This method returns the skill bonus for the specified skill.
It returns the skill + all its bonsues for stats/whatever.
It first checks to see if the skill is in it's cache.   THe
real stat values are ones not modified by bonuses or temporary
values.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
skill - the skill to get the bonus for
.EP
.SP 9 5
use_base_stats - tells the system not to use the real stat values
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skill bonus

.EP

.SI 3
* %^BOLD%^query_skill_successful%^RESET%^
.EI
.SI 5
int query_skill_successful(string str, int mod)
.EI
.SP 7 5

This method does a skill successful check.  Does this check:
.EP
.SP 7 5
(bonus + mos) >= random(200)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the skill to check
.EP
.SP 9 5
mod - the modification value
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the skill check is successful

.EP

.SI 3
* %^BOLD%^query_skills%^RESET%^
.EI
.SI 5
mapping query_skills()
.EI

.SI 3
* %^BOLD%^query_stat_cache%^RESET%^
.EI
.SI 5
mapping query_stat_cache()
.EI
.SP 7 5

This method returns the cached values for the stats.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the caches stat values

.EP

.SI 3
* %^BOLD%^query_teach_offer%^RESET%^
.EI
.SI 5
mapping query_teach_offer()
.EI
.SP 7 5

This method returns the current list of teach offerings on the
living object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping containing the teach offerings

.EP

.SI 3
* %^BOLD%^set_skills%^RESET%^
.EI
.SI 5
void set_skills(mapping map)
.EI

.SI 3
* %^BOLD%^stat_modify%^RESET%^
.EI
.SI 5
varargs int stat_modify(int lvl, string skill, int use_base_stats)
.EI
.SP 7 5

This method returns the skill as it should be modified by the
stats associated with it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lvl - the level to modify
.EP
.SP 9 5
skill - the skill the modify the bonus of
.EP
.SP 9 5
use_base_stats - use the real unmodified stat values
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the stat modification

.EP

.SI 3
* %^BOLD%^stop_teaching_skills%^RESET%^
.EI
.SI 5
void stop_teaching_skills(int left, mixed bing)
.EI
.SP 7 5

The method to call when we stop teaching skills.  THis will stop the
stuff being taught if the stop is successful, and only teach partial
amounts if we are not finished yet.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
left - the amount of time left
.EP
.SP 9 5
bing - the data associated with the command

.EP

.SI 3
* %^BOLD%^tm_check_ok%^RESET%^
.EI
.SI 5
int tm_check_ok(string skill, object exp)
.EI

.SI 3
* %^BOLD%^totaly_zap_bonus_cache%^RESET%^
.EI
.SI 5
void totaly_zap_bonus_cache()
.EI
.SP 7 5

This method zaps the bonus cache.

.EP

.SI 3
* %^BOLD%^zap_stat_cache%^RESET%^
.EI
.SI 5
void zap_stat_cache()
.EI
.SP 7 5

This method zaps the stat cache when a certain stat changes.
It calls the function stats_to_zap() on the living object to
figure out which stats have changed.
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^flatten_it%^RESET%^
.EI
.SI 5
void flatten_it(string skill)
.EI
.SP 7 5

This is used to convert a previously not only_leaf tree into an only_leaf
tree.

.EP

.SI 3
* %^BOLD%^totaly_zap_stat_cache%^RESET%^
.EI
.SI 5
void totaly_zap_stat_cache()
.EI
.SP 7 5

This method zaps the stat cache.

.EP


