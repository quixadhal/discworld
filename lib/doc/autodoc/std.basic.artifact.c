.DT
artifact.c
Disk World autodoc help
artifact.c

.SH Includes
.SP 5 5
This class includes the following files /include/artifacts.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_charges%^RESET%^
.EI
.SI 5
int adjust_charges(int number)
.EI

.SI 3
* %^BOLD%^break_on_charging%^RESET%^
.EI
.SI 5
int break_on_charging()
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map)
.EI

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mapping map)
.EI

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(int word, int dark)
.EI

.SI 3
* %^BOLD%^query_artifact_type%^RESET%^
.EI
.SI 5
string query_artifact_type()
.EI

.SI 3
* %^BOLD%^query_charges%^RESET%^
.EI
.SI 5
int query_charges()
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_level%^RESET%^
.EI
.SI 5
int query_level()
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mapping query_static_auto_load()
.EI

.SI 3
* %^BOLD%^query_total_charges%^RESET%^
.EI
.SI 5
int query_total_charges()
.EI

.SI 3
* %^BOLD%^query_zapper%^RESET%^
.EI
.SI 5
string query_zapper()
.EI

.SI 3
* %^BOLD%^set_artifact_type%^RESET%^
.EI
.SI 5
void set_artifact_type(string word)
.EI

.SI 3
* %^BOLD%^set_charges%^RESET%^
.EI
.SI 5
void set_charges(int number)
.EI

.SI 3
* %^BOLD%^set_level%^RESET%^
.EI
.SI 5
void set_level(int number)
.EI

.SI 3
* %^BOLD%^set_total_charges%^RESET%^
.EI
.SI 5
void set_total_charges(int number)
.EI

.SI 3
* %^BOLD%^set_zapper%^RESET%^
.EI
.SI 5
void set_zapper(string word)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI

.SI 3
* %^BOLD%^zapper_init%^RESET%^
.EI
.SI 5
void zapper_init(object thing)
.EI


