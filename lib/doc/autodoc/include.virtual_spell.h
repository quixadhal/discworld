.DT
virtual_spell.h
Disk World autodoc help
virtual_spell.h

.SH Includes
.SP 5 5
This class includes the following files /include/config.h and /include/db.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^eff_args%^RESET%^
class eff_args {
object obj;
class spell_info spell;
int id;
}

.EI

.SI 3
* %^BOLD%^spell_info%^RESET%^
class spell_info {
string name;
int target_type;
class spell_stage * stages;
string * needed;
string tell_final;
string say_final;
}

.EI

.SI 3
* %^BOLD%^spell_stage%^RESET%^
class spell_stage {
string skill;
int difficulty;
string tell_msg;
string say_msg;
string * consumed;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^DATA_EXISTS%^RESET%^
.EI

.SI 3
* %^BOLD%^DB%^RESET%^
.EI

.SI 3
* %^BOLD%^DIFF_POWER%^RESET%^
.EI

.SI 3
* %^BOLD%^ID_EXISTS%^RESET%^
.EI

.SI 3
* %^BOLD%^MAXID%^RESET%^
.EI

.SI 3
* %^BOLD%^PASSWORD%^RESET%^
.EI

.SI 3
* %^BOLD%^SET_STONE%^RESET%^
.EI

.SI 3
* %^BOLD%^SPELL%^RESET%^
.EI

.SI 3
* %^BOLD%^SPELL_DIRECT_LIVING%^RESET%^
.EI

.SI 3
* %^BOLD%^SPELL_DIRECT_NON_LIVING%^RESET%^
.EI

.SI 3
* %^BOLD%^SPELL_UNDIRECTED%^RESET%^
.EI

.SI 3
* %^BOLD%^STAGE%^RESET%^
.EI

.SI 3
* %^BOLD%^STAGE_COST%^RESET%^
.EI

.SI 3
* %^BOLD%^STAGE_TIME%^RESET%^
.EI

.SI 3
* %^BOLD%^TRICK_HANDLER%^RESET%^
.EI

.SI 3
* %^BOLD%^TRICKS%^RESET%^
.EI

.SI 3
* %^BOLD%^UNSET_STONE%^RESET%^
.EI

.SI 3
* %^BOLD%^USER_ID%^RESET%^
.EI

.SI 3
* %^BOLD%^VIRTUAL_SPELL_EFF%^RESET%^
.EI

.SI 3
* %^BOLD%^VIRTUAL_SPELL_OB%^RESET%^
.EI

