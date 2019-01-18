.DT
applications_handler.c
Disk World autodoc help
applications_handler.c

.SH Description
.SP 5 5

 Applications handler
 @author Drakkos
 @started 30/01/2003


.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/board.h, /include/mail.h, /include/library.h, /include/applications.h, /include/player_handler.h, /include/playerinfo.h, /include/player.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_application%^RESET%^
.EI
.SI 5
int add_application(string t, string n, string te, string dom)
.EI

.SI 3
* %^BOLD%^add_application_type%^RESET%^
.EI
.SI 5
varargs void add_application_type(string t, int v, string b, int g, int q, int a, int ti, string * qu, string intro, string responsibility)
.EI

.SI 3
* %^BOLD%^application_requirements%^RESET%^
.EI
.SI 5
string application_requirements(string type)
.EI

.SI 3
* %^BOLD%^delete_application%^RESET%^
.EI
.SI 5
int delete_application(string t, string n)
.EI
.SP 7 5

 Deletes an application from the system.

 @param name The name of the aplicant
 @param type The type of application.
 @return 1 for success, 0 for failure.


.EP

.SI 3
* %^BOLD%^delete_vouch%^RESET%^
.EI
.SI 5
int delete_vouch(string who, string name, string position)
.EI

.SI 3
* %^BOLD%^find_application%^RESET%^
.EI
.SI 5
int find_application(string name, string type)
.EI
.SP 7 5

 Find a particular application.

 @param name The name of the candidate.
 @param type The type of application.
 @return the index of the candidate, or -1 if none found.


.EP

.SI 3
* %^BOLD%^find_type%^RESET%^
.EI
.SI 5
int find_type(string type)
.EI
.SP 7 5

 Find a particular application type.

 @param type The type of application.
 @return the index of the type, or -1 if none found.


.EP

.SI 3
* %^BOLD%^fix_name%^RESET%^
.EI
.SI 5
int fix_name(string n, string n2)
.EI

.SI 3
* %^BOLD%^format_application%^RESET%^
.EI
.SI 5
string format_application(int i)
.EI

.SI 3
* %^BOLD%^list_applications%^RESET%^
.EI
.SI 5
string list_applications()
.EI

.SI 3
* %^BOLD%^load_me%^RESET%^
.EI
.SI 5
void load_me()
.EI

.SI 3
* %^BOLD%^make_vouch%^RESET%^
.EI
.SI 5
int make_vouch(string t, string p, string n, string v)
.EI

.SI 3
* %^BOLD%^number_of_vouches%^RESET%^
.EI
.SI 5
int number_of_vouches(string name, string type)
.EI
.SP 7 5

 Returns the number of vouches a player has received for an application.

 @param i The index of the application.
 @return 1 for success, 0 for failure.


.EP

.SI 3
* %^BOLD%^query_application_vouches%^RESET%^
.EI
.SI 5
mapping query_application_vouches(string type, string name)
.EI

.SI 3
* %^BOLD%^query_applications_accepted%^RESET%^
.EI
.SI 5
int query_applications_accepted(string dom)
.EI

.SI 3
* %^BOLD%^query_domain_status%^RESET%^
.EI
.SI 5
mapping query_domain_status()
.EI

.SI 3
* %^BOLD%^query_intro_text%^RESET%^
.EI
.SI 5
string query_intro_text(string type)
.EI

.SI 3
* %^BOLD%^query_name%^RESET%^
.EI
.SI 5
string query_name()
.EI

.SI 3
* %^BOLD%^query_questions%^RESET%^
.EI
.SI 5
string * query_questions(string type)
.EI

.SI 3
* %^BOLD%^query_responsibility%^RESET%^
.EI
.SI 5
string query_responsibility(string type)
.EI

.SI 3
* %^BOLD%^query_types%^RESET%^
.EI
.SI 5
string * query_types()
.EI

.SI 3
* %^BOLD%^query_vouch%^RESET%^
.EI
.SI 5
string query_vouch(string t, string p, string n)
.EI
.SP 7 5

 Query the vouch for a particular player in a particular category.

 @param t The type of application.
 @param p The name of the player.
 @param n The name of the voucher.
 @return the text of the vouch or 0 if none exists.


.EP

.SI 3
* %^BOLD%^query_vouches_for_all%^RESET%^
.EI
.SI 5
mapping query_vouches_for_all(string name)
.EI

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI

.SI 3
* %^BOLD%^toggle_domain_lock%^RESET%^
.EI
.SI 5
int toggle_domain_lock(string dom)
.EI

.SI 3
* %^BOLD%^valid_application%^RESET%^
.EI
.SI 5
int valid_application(string name, string type)
.EI


