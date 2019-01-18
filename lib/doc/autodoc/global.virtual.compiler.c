.DT
compiler.c
Disk World autodoc help
compiler.c

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h, /global/virtual/compiler.h and /global/virtual/virtual.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^compile_arm%^RESET%^
.EI
.SI 5
object compile_arm(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_clo%^RESET%^
.EI
.SI 5
object compile_clo(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_clotrans%^RESET%^
.EI
.SI 5
object compile_clotrans(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_cont%^RESET%^
.EI
.SI 5
object compile_cont(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_food%^RESET%^
.EI
.SI 5
object compile_food(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_imp%^RESET%^
.EI
.SI 5
object compile_imp(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_mon%^RESET%^
.EI
.SI 5
object compile_mon(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_ob%^RESET%^
.EI
.SI 5
object compile_ob(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_r%^RESET%^
.EI
.SI 5
object compile_r(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_sca%^RESET%^
.EI
.SI 5
object compile_sca(string name, int clone)
.EI

.SI 3
* %^BOLD%^compile_wep%^RESET%^
.EI
.SI 5
object compile_wep(string name, int clone)
.EI

.SI 3
* %^BOLD%^do_clone%^RESET%^
.EI
.SI 5
void do_clone(object dest, string str)
.EI

.SI 3
* %^BOLD%^extract_arguments%^RESET%^
.EI
.SI 5
mixed * extract_arguments(string str)
.EI

.SI 3
* %^BOLD%^initialize_methods%^RESET%^
.EI
.SI 5
void initialize_methods()
.EI

.SI 3
* %^BOLD%^prop_to_fun%^RESET%^
.EI
.SI 5
object prop_to_fun(string file, mapping funs, string virt_class, string virt_include, int clone)
.EI

.SI 3
* %^BOLD%^query_return_ob%^RESET%^
.EI
.SI 5
object query_return_ob()
.EI

.SI 3
* %^BOLD%^set_debug_status%^RESET%^
.EI
.SI 5
int set_debug_status(int d_stat)
.EI

.SI 3
* %^BOLD%^set_return_ob%^RESET%^
.EI
.SI 5
void set_return_ob(object ob)
.EI

.SI 3
* %^BOLD%^str_to_var%^RESET%^
.EI
.SI 5
mixed str_to_var(string str)
.EI

.SI 3
* %^BOLD%^strip_string%^RESET%^
.EI
.SI 5
string strip_string(string str)
.EI


