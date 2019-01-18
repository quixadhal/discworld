.DT
language_inherit.c
Disk World autodoc help
language_inherit.c

.SH Includes
.SP 5 5
This class includes the following files /include/tasks.h, /include/fixed_random.h and /include/living.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_level_increase%^RESET%^
.EI
.SI 5
void check_level_increase(object player, int difficulty, int level, string skill, string mess)
.EI

.SI 3
* %^BOLD%^do_garble%^RESET%^
.EI
.SI 5
string do_garble(string word, string type)
.EI

.SI 3
* %^BOLD%^garble_message%^RESET%^
.EI
.SI 5
string garble_message(string mess, object player, object speaker, string type, string skill, int no_increase)
.EI

.SI 3
* %^BOLD%^garble_say%^RESET%^
.EI
.SI 5
mixed garble_say(string start, string mess, object player, object from, string type, string skill, int no_increase)
.EI

.SI 3
* %^BOLD%^garble_text%^RESET%^
.EI
.SI 5
string garble_text(mixed text, object thing, object player, string skill)
.EI

.SI 3
* %^BOLD%^query_long%^RESET%^
.EI
.SI 5
string query_long()
.EI

.SI 3
* %^BOLD%^query_name%^RESET%^
.EI
.SI 5
string query_name()
.EI

.SI 3
* %^BOLD%^set_long%^RESET%^
.EI
.SI 5
void set_long(string str)
.EI

.SI 3
* %^BOLD%^set_name%^RESET%^
.EI
.SI 5
void set_name(string str)
.EI

.SI 3
* %^BOLD%^set_start_text_bit%^RESET%^
.EI
.SI 5
void set_start_text_bit(string str)
.EI


