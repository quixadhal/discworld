.DT
guild.c
Disk World autodoc help
guild.c

.SH Includes
.SP 5 5
This class includes the following files /include/config.h, /include/tune.h, /include/shops/bank.h, /include/skills.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_command%^RESET%^
.EI
.SI 5
void add_command(string name, string * blue, int lvl)
.EI

.SI 3
* %^BOLD%^add_spell%^RESET%^
.EI
.SI 5
void add_spell(string name, string * blue, int lvl)
.EI

.SI 3
* %^BOLD%^check_primaries%^RESET%^
.EI
.SI 5
int check_primaries(string lpath, string g_o)
.EI

.SI 3
* %^BOLD%^check_spells_commands%^RESET%^
.EI
.SI 5
void check_spells_commands(string * sk, int lvl)
.EI

.SI 3
* %^BOLD%^confirm_leave%^RESET%^
.EI
.SI 5
void confirm_leave(string str)
.EI

.SI 3
* %^BOLD%^do_advance%^RESET%^
.EI
.SI 5
int do_advance(string skill)
.EI

.SI 3
* %^BOLD%^do_advance_by%^RESET%^
.EI
.SI 5
int do_advance_by(string skill, int num)
.EI

.SI 3
* %^BOLD%^do_advance_internal%^RESET%^
.EI
.SI 5
int do_advance_internal(string skill, int to, int by)
.EI

.SI 3
* %^BOLD%^do_advance_to%^RESET%^
.EI
.SI 5
int do_advance_to(string skill, int num)
.EI

.SI 3
* %^BOLD%^do_cost%^RESET%^
.EI
.SI 5
int do_cost(string skill)
.EI

.SI 3
* %^BOLD%^do_cost_all%^RESET%^
.EI
.SI 5
int do_cost_all(int brief)
.EI

.SI 3
* %^BOLD%^do_cost_by%^RESET%^
.EI
.SI 5
int do_cost_by(string skill, int num)
.EI

.SI 3
* %^BOLD%^do_cost_internal%^RESET%^
.EI
.SI 5
int do_cost_internal(string skill, int to, int by)
.EI

.SI 3
* %^BOLD%^do_cost_primaries%^RESET%^
.EI
.SI 5
int do_cost_primaries()
.EI

.SI 3
* %^BOLD%^do_cost_to%^RESET%^
.EI
.SI 5
int do_cost_to(string skill, int num)
.EI

.SI 3
* %^BOLD%^do_info%^RESET%^
.EI
.SI 5
int do_info()
.EI

.SI 3
* %^BOLD%^do_join%^RESET%^
.EI
.SI 5
int do_join(string str)
.EI

.SI 3
* %^BOLD%^do_leave%^RESET%^
.EI
.SI 5
int do_leave(string str)
.EI

.SI 3
* %^BOLD%^filter_primaries%^RESET%^
.EI
.SI 5
mixed * filter_primaries(mixed * tmp_skills, string skill_path, string * primaries)
.EI

.SI 3
* %^BOLD%^join2%^RESET%^
.EI
.SI 5
int join2(string str)
.EI

.SI 3
* %^BOLD%^query_commands%^RESET%^
.EI
.SI 5
mapping query_commands()
.EI

.SI 3
* %^BOLD%^query_our_guild%^RESET%^
.EI
.SI 5
string query_our_guild()
.EI

.SI 3
* %^BOLD%^query_skill_cost%^RESET%^
.EI
.SI 5
int query_skill_cost(string skill, int offset)
.EI

.SI 3
* %^BOLD%^query_spells%^RESET%^
.EI
.SI 5
mapping query_spells()
.EI

.SI 3
* %^BOLD%^query_start_pos%^RESET%^
.EI
.SI 5
string query_start_pos()
.EI

.SI 3
* %^BOLD%^query_teaching_person%^RESET%^
.EI
.SI 5
object query_teaching_person()
.EI

.SI 3
* %^BOLD%^set_cost_div%^RESET%^
.EI
.SI 5
void set_cost_div(int number)
.EI

.SI 3
* %^BOLD%^set_guild%^RESET%^
.EI
.SI 5
varargs void set_guild(string word1, string word2)
.EI

.SI 3
* %^BOLD%^set_teaching_person%^RESET%^
.EI
.SI 5
void set_teaching_person(object ob)
.EI


