.DT
salve.c
DW_wibble autodoc help
salve.c

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/food.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/drinks.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_apply_effect%^RESET%^
.EI
.SI 5
int add_apply_effect(string word, int number)
.EI

.SI 3
* %^BOLD%^being_joined_by%^RESET%^
.EI
.SI 5
void being_joined_by(object thing)
.EI

.SI 3
* %^BOLD%^do_apply%^RESET%^
.EI
.SI 5
int do_apply(object * indir, string dir_s, string indir_s, mixed * args)
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map)
.EI

.SI 3
* %^BOLD%^query_apply_effects%^RESET%^
.EI
.SI 5
mapping query_apply_effects()
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mapping query_static_auto_load()
.EI

.SI 3
* %^BOLD%^remove_apply_effect%^RESET%^
.EI
.SI 5
void remove_apply_effect(string word)
.EI

.SI 3
* %^BOLD%^set_apply_effects%^RESET%^
.EI
.SI 5
void set_apply_effects(mapping map)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


