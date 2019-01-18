.DT
board.c
Disk World autodoc help
board.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/board.h and /include/mail.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^info_written%^RESET%^
class info_written {
string subject;
int note_num;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_commands%^RESET%^
.EI
.SI 5
void add_commands()
.EI

.SI 3
* %^BOLD%^do_eat%^RESET%^
.EI
.SI 5
int do_eat(int num)
.EI

.SI 3
* %^BOLD%^do_followup%^RESET%^
.EI
.SI 5
int do_followup(int num)
.EI

.SI 3
* %^BOLD%^do_post%^RESET%^
.EI
.SI 5
int do_post(string str)
.EI

.SI 3
* %^BOLD%^do_read%^RESET%^
.EI
.SI 5
int do_read(int num)
.EI

.SI 3
* %^BOLD%^do_read_new%^RESET%^
.EI
.SI 5
int do_read_new()
.EI

.SI 3
* %^BOLD%^do_read_next%^RESET%^
.EI
.SI 5
int do_read_next()
.EI

.SI 3
* %^BOLD%^do_reply%^RESET%^
.EI
.SI 5
int do_reply(int num)
.EI

.SI 3
* %^BOLD%^do_subjects%^RESET%^
.EI
.SI 5
int do_subjects(string search)
.EI

.SI 3
* %^BOLD%^end_of_thing%^RESET%^
.EI
.SI 5
void end_of_thing(string body)
.EI

.SI 3
* %^BOLD%^include_post%^RESET%^
.EI
.SI 5
void include_post(string str, int num, mixed * stuff)
.EI

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string str, int dark)
.EI

.SI 3
* %^BOLD%^mark_mine%^RESET%^
.EI
.SI 5
void mark_mine(mixed * stuff, mapping news_rc)
.EI

.SI 3
* %^BOLD%^query_new_messages%^RESET%^
.EI
.SI 5
int query_new_messages()
.EI

.SI 3
* %^BOLD%^query_plural%^RESET%^
.EI
.SI 5
string query_plural()
.EI

.SI 3
* %^BOLD%^set_board_name%^RESET%^
.EI
.SI 5
void set_board_name(string str)
.EI

.SI 3
* %^BOLD%^set_datafile%^RESET%^
.EI
.SI 5
void set_datafile(string str)
.EI

.SI 3
* %^BOLD%^short%^RESET%^
.EI
.SI 5
string short(int dark)
.EI

.SI 3
* %^BOLD%^the_date%^RESET%^
.EI
.SI 5
string the_date(int i)
.EI


