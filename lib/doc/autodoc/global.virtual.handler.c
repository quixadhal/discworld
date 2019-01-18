.DT
handler.c
Disk World autodoc help
handler.c

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_item%^RESET%^
.EI
.SI 5
mixed * add_item(mixed * args, int m, string str)
.EI

.SI 3
* %^BOLD%^add_level%^RESET%^
.EI
.SI 5
mixed * add_level(mixed * args, int m)
.EI

.SI 3
* %^BOLD%^clone_virtual_object%^RESET%^
.EI
.SI 5
object clone_virtual_object(string file)
.EI

.SI 3
* %^BOLD%^destruct_virtual_object%^RESET%^
.EI
.SI 5
int destruct_virtual_object(object obj)
.EI

.SI 3
* %^BOLD%^error%^RESET%^
.EI
.SI 5
int error(string str)
.EI

.SI 3
* %^BOLD%^find_virtual_object%^RESET%^
.EI
.SI 5
object find_virtual_object(string name)
.EI

.SI 3
* %^BOLD%^query_cloned_objects%^RESET%^
.EI
.SI 5
mapping query_cloned_objects()
.EI

.SI 3
* %^BOLD%^self_dest%^RESET%^
.EI
.SI 5
int self_dest()
.EI

.SI 3
* %^BOLD%^sort_arguments%^RESET%^
.EI
.SI 5
mixed * sort_arguments(mixed * args)
.EI

.SI 3
* %^BOLD%^str_to_int%^RESET%^
.EI
.SI 5
int str_to_int(string str)
.EI

.SI 3
* %^BOLD%^use_efun_clone%^RESET%^
.EI
.SI 5
object use_efun_clone(string file)
.EI


