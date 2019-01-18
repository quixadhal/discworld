.DT
ritual_system.h
Disk World autodoc help
ritual_system.h

.SH Description
.SP 5 5

Rituals.h
A header file for rituals.
.EP
.SP 10 5


Written by Terano.

.EP

.SH Classes

.SI 3
* %^BOLD%^ritual%^RESET%^
class ritual {
string name;
string family;
int base_gp_cost;
int * align;
int targets;
class stage * stages;
int pk_check;
int teach_base;
int learn_base;
string training_skill;
int movement;
int speech;
int vision;
string * materials , * consumables;
function target_check;
string resist_skill;
string help;
int offensive;
object caster;
object imbue;
object * rit_targets;
object * resistors;
int current_stage;
int start_time;
int target_count;
function valid;
int degree;
string deity;
mapping difficulties;
mapping benefits;
}

.EI
.SP 7 5

Data Storage for Rituals

.EP

.SI 3
* %^BOLD%^stage%^RESET%^
class stage {
int key;
string * messages;
string skill;
int level;
string * materials;
string * consumables;
int pause;
int time_modifier;
function success_func;
string prayer;
int imbue_source;
}

.EI
.SP 7 5

Each ritual has one or more stages.
Each stage must have messages for both the player and external parties.
Each stage can optionally have a skill check.
Each stage can optionally have items which can be consumed.
Each stage can be optionally lengthened, this will then be modified by 
other factors. [alignment and skill]. This defaults to 1 if not entered.

.EP


.SH Defines
.SI 3
* %^BOLD%^ALCOHOL_INHERIT%^RESET%^
.EI

.SI 3
* %^BOLD%^CURING_INHERIT%^RESET%^
.EI

.SI 3
* %^BOLD%^EFFECTS%^RESET%^
.EI

.SI 3
* %^BOLD%^ENVIRONMENT%^RESET%^
.EI

.SI 3
* %^BOLD%^FAILURE%^RESET%^
.EI

.SI 3
* %^BOLD%^FAMILY%^RESET%^
.EI

.SI 3
* %^BOLD%^IMBUE_PERFORMER%^RESET%^
.EI

.SI 3
* %^BOLD%^IMBUE_PRIEST%^RESET%^
.EI

.SI 3
* %^BOLD%^INHERITS%^RESET%^
.EI

.SI 3
* %^BOLD%^ITEMS%^RESET%^
.EI

.SI 3
* %^BOLD%^MENTAL%^RESET%^
.EI

.SI 3
* %^BOLD%^MISC%^RESET%^
.EI

.SI 3
* %^BOLD%^MOVEMENT%^RESET%^
.EI

.SI 3
* %^BOLD%^PASSAGE_INHERIT%^RESET%^
.EI

.SI 3
* %^BOLD%^RELIGIOUS%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUAL%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUAL_CALCULATE_DYNAMIC%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUAL_DIRECT_EXTERN%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUAL_DIRECT_LIVING%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUAL_DIRECT_MULTIPLE%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUAL_DIRECT_NON_LIVING%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUAL_DIRECT_NONE%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUAL_DIRECT_ROOM%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUAL_DIRECT_SELF%^RESET%^
.EI

.SI 3
* %^BOLD%^RITUALS%^RESET%^
.EI

.SI 3
* %^BOLD%^ROOMS%^RESET%^
.EI

.SI 3
* %^BOLD%^SHADOW%^RESET%^
.EI

.SI 3
* %^BOLD%^SHADOWS%^RESET%^
.EI

.SI 3
* %^BOLD%^SHIELD_INHERIT%^RESET%^
.EI

.SI 3
* %^BOLD%^SPEECH%^RESET%^
.EI

.SI 3
* %^BOLD%^STOPPED%^RESET%^
.EI

.SI 3
* %^BOLD%^SUCCESS%^RESET%^
.EI

.SI 3
* %^BOLD%^VISION%^RESET%^
.EI

.SI 3
* %^BOLD%^WARD_BASE%^RESET%^
.EI

