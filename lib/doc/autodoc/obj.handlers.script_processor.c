.DT
script_processor.c
Disk World autodoc help
script_processor.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^perform_line%^RESET%^
.EI
.SI 5
int perform_line(string script, int line)
.EI

.SI 3
* %^BOLD%^perform_script%^RESET%^
.EI
.SI 5
void perform_script(string script, int line)
.EI

.SI 3
* %^BOLD%^query_actor%^RESET%^
.EI
.SI 5
string query_actor(string word)
.EI

.SI 3
* %^BOLD%^query_actors%^RESET%^
.EI
.SI 5
mapping query_actors()
.EI

.SI 3
* %^BOLD%^query_stage%^RESET%^
.EI
.SI 5
string query_stage()
.EI

.SI 3
* %^BOLD%^set_actor%^RESET%^
.EI
.SI 5
void set_actor(string name, string path)
.EI

.SI 3
* %^BOLD%^set_stage%^RESET%^
.EI
.SI 5
void set_stage(string word)
.EI

.SI 3
* %^BOLD%^start_script%^RESET%^
.EI
.SI 5
int start_script(string script)
.EI


