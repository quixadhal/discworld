.DT
reaction.c
Disk World autodoc help
reaction.c

.SH Includes
.SP 5 5
This class includes the following files /include/reaction.h and /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_reaction%^RESET%^
.EI
.SI 5
void check_reaction(object a)
.EI

.SI 3
* %^BOLD%^dest_substance%^RESET%^
.EI
.SI 5
void dest_substance(object ob)
.EI

.SI 3
* %^BOLD%^merge_cont_medium%^RESET%^
.EI
.SI 5
object merge_cont_medium(object a, string medium_alias)
.EI

.SI 3
* %^BOLD%^merge_effects%^RESET%^
.EI
.SI 5
mapping merge_effects(mapping effects_a, mapping effects_m, int q_a, int q_m)
.EI

.SI 3
* %^BOLD%^move_substance%^RESET%^
.EI
.SI 5
void move_substance(object * ob)
.EI

.SI 3
* %^BOLD%^query_reaction%^RESET%^
.EI
.SI 5
mixed * query_reaction(string name_a, string name_b)
.EI

.SI 3
* %^BOLD%^query_reactions%^RESET%^
.EI
.SI 5
mapping query_reactions()
.EI

.SI 3
* %^BOLD%^restore_from_files%^RESET%^
.EI
.SI 5
void restore_from_files()
.EI

.SI 3
* %^BOLD%^update_from_files%^RESET%^
.EI
.SI 5
void update_from_files(string fn)
.EI

.SI 3
* %^BOLD%^write_dbg%^RESET%^
.EI
.SI 5
void write_dbg(string str)
.EI


