.DT
spells.c
Disk World autodoc help
spells.c

.SH Includes
.SP 5 5
This class includes the following files /include/spells.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_spell_effect%^RESET%^
.EI
.SI 5
int add_spell_effect(int no_rnds, string type, string name, object callee, string func, mixed params)
.EI

.SI 3
* %^BOLD%^do_call_out_effect%^RESET%^
.EI
.SI 5
void do_call_out_effect(mixed * params)
.EI

.SI 3
* %^BOLD%^do_spell_effects%^RESET%^
.EI
.SI 5
int do_spell_effects(object attacker)
.EI

.SI 3
* %^BOLD%^query_effects%^RESET%^
.EI
.SI 5
mixed query_effects()
.EI

.SI 3
* %^BOLD%^query_spell_effect%^RESET%^
.EI
.SI 5
mixed query_spell_effect(string name)
.EI

.SI 3
* %^BOLD%^query_spell_effects_type%^RESET%^
.EI
.SI 5
mixed * query_spell_effects_type(string type)
.EI

.SI 3
* %^BOLD%^queue_commands%^RESET%^
.EI
.SI 5
int queue_commands()
.EI

.SI 3
* %^BOLD%^remove_spell_effect%^RESET%^
.EI
.SI 5
int remove_spell_effect(string name)
.EI


