.DT
mineral.c
DW_wibble autodoc help
mineral.c

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/food.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/mineral.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_tool%^RESET%^
.EI
.SI 5
int check_tool(object * tools, string * needed)
.EI

.SI 3
* %^BOLD%^do_chip%^RESET%^
.EI
.SI 5
int do_chip(object * tools)
.EI

.SI 3
* %^BOLD%^do_eat%^RESET%^
.EI
.SI 5
int do_eat()
.EI

.SI 3
* %^BOLD%^do_pulverise%^RESET%^
.EI
.SI 5
int do_pulverise(object * tools)
.EI

.SI 3
* %^BOLD%^do_smash%^RESET%^
.EI
.SI 5
int do_smash(object * tools)
.EI

.SI 3
* %^BOLD%^do_sprinkle%^RESET%^
.EI
.SI 5
int do_sprinkle(object * obs)
.EI

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mapping map)
.EI

.SI 3
* %^BOLD%^int_query_static_auto_load%^RESET%^
.EI
.SI 5
mapping int_query_static_auto_load()
.EI

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string words, int dark)
.EI

.SI 3
* %^BOLD%^make_mineral%^RESET%^
.EI
.SI 5
varargs void make_mineral(string word, int number, string * inputs)
.EI

.SI 3
* %^BOLD%^parse_command_adjectiv_id_list%^RESET%^
.EI
.SI 5
string * parse_command_adjectiv_id_list()
.EI

.SI 3
* %^BOLD%^query_material%^RESET%^
.EI
.SI 5
string query_material()
.EI

.SI 3
* %^BOLD%^query_mineral%^RESET%^
.EI
.SI 5
string query_mineral()
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mapping query_static_auto_load()
.EI

.SI 3
* %^BOLD%^set_decay_level%^RESET%^
.EI
.SI 5
void set_decay_level(int level)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


