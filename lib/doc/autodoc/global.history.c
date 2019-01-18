.DT
history.c
Disk World autodoc help
history.c

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_history%^RESET%^
.EI
.SI 5
void add_history(string arg)
.EI

.SI 3
* %^BOLD%^history_commands%^RESET%^
.EI
.SI 5
void history_commands()
.EI

.SI 3
* %^BOLD%^ignore_from_history%^RESET%^
.EI
.SI 5
void ignore_from_history(string str)
.EI

.SI 3
* %^BOLD%^print_history%^RESET%^
.EI
.SI 5
nomask int print_history(string arg)
.EI

.SI 3
* %^BOLD%^substitute_history%^RESET%^
.EI
.SI 5
string substitute_history(string arg)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^expand_history%^RESET%^
.EI
.SI 5
string expand_history(string arg)
.EI

.SI 3
* %^BOLD%^get_history%^RESET%^
.EI
.SI 5
nomask string * get_history()
.EI


