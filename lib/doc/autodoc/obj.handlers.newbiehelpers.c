.DT
newbiehelpers.c
Disk World autodoc help
newbiehelpers.c

.SH Description
.SP 5 5

Newbie Helpers Handler

.EP
.SP 10 5


Written by Dasquian

Started on 26 March 2002


.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/playerinfo.h and /include/mail.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_admin%^RESET%^
.EI
.SI 5
int add_admin(string name)
.EI

.SI 3
* %^BOLD%^add_helper%^RESET%^
.EI
.SI 5
int add_helper(object adder, string player)
.EI

.SI 3
* %^BOLD%^no_apply_reason%^RESET%^
.EI
.SI 5
string no_apply_reason(object player)
.EI

.SI 3
* %^BOLD%^no_reject_reason%^RESET%^
.EI
.SI 5
string no_reject_reason(object rejecter, string player)
.EI

.SI 3
* %^BOLD%^no_vouch_reason%^RESET%^
.EI
.SI 5
string no_vouch_reason(object voucher, string applicant)
.EI

.SI 3
* %^BOLD%^query_admin%^RESET%^
.EI
.SI 5
int query_admin(string name)
.EI

.SI 3
* %^BOLD%^query_can_chat%^RESET%^
.EI
.SI 5
int query_can_chat(object who)
.EI

.SI 3
* %^BOLD%^query_check_player%^RESET%^
.EI
.SI 5
string query_check_player(object checker, string player)
.EI

.SI 3
* %^BOLD%^query_helper%^RESET%^
.EI
.SI 5
int query_helper(string name)
.EI

.SI 3
* %^BOLD%^query_list%^RESET%^
.EI
.SI 5
string query_list(object lister)
.EI

.SI 3
* %^BOLD%^query_name%^RESET%^
.EI
.SI 5
string query_name()
.EI

.SI 3
* %^BOLD%^reject_application%^RESET%^
.EI
.SI 5
void reject_application(object rejecter, string player, string reason, string mail)
.EI

.SI 3
* %^BOLD%^remove_admin%^RESET%^
.EI
.SI 5
int remove_admin(string name)
.EI

.SI 3
* %^BOLD%^remove_helper%^RESET%^
.EI
.SI 5
string remove_helper(object remover, string player, string reason)
.EI

.SI 3
* %^BOLD%^submit_application%^RESET%^
.EI
.SI 5
void submit_application(string statement, object player)
.EI

.SI 3
* %^BOLD%^submit_vouch%^RESET%^
.EI
.SI 5
void submit_vouch(string statement, mixed args)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^load_file%^RESET%^
.EI
.SI 5
void load_file()
.EI

.SI 3
* %^BOLD%^log_message%^RESET%^
.EI
.SI 5
void log_message(string message)
.EI

.SI 3
* %^BOLD%^save_file%^RESET%^
.EI
.SI 5
void save_file()
.EI


