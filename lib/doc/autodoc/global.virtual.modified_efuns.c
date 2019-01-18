.DT
modified_efuns.c
Disk World autodoc help
modified_efuns.c

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^cat%^RESET%^
.EI
.SI 5
void cat(string file, int start_line, int number)
.EI

.SI 3
* %^BOLD%^clone_object%^RESET%^
.EI
.SI 5
object clone_object(string name)
.EI

.SI 3
* %^BOLD%^destruct%^RESET%^
.EI
.SI 5
void destruct(mixed name)
.EI

.SI 3
* %^BOLD%^file_name%^RESET%^
.EI
.SI 5
string file_name(object obj)
.EI

.SI 3
* %^BOLD%^find_living%^RESET%^
.EI
.SI 5
object find_living(string str)
.EI

.SI 3
* %^BOLD%^find_object%^RESET%^
.EI
.SI 5
object find_object(string str)
.EI

.SI 3
* %^BOLD%^find_player%^RESET%^
.EI
.SI 5
object find_player(string str)
.EI

.SI 3
* %^BOLD%^log_file%^RESET%^
.EI
.SI 5
void log_file(string name, string mess)
.EI

.SI 3
* %^BOLD%^mud_name%^RESET%^
.EI
.SI 5
string mud_name()
.EI

.SI 3
* %^BOLD%^say%^RESET%^
.EI
.SI 5
varargs void say(string str, mixed avoid)
.EI

.SI 3
* %^BOLD%^tell_object%^RESET%^
.EI
.SI 5
void tell_object(object ob, string str)
.EI

.SI 3
* %^BOLD%^tell_room%^RESET%^
.EI
.SI 5
varargs void tell_room(object ob, string str, mixed avoid)
.EI


