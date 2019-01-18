.DT
council_cases.c
Disk World autodoc help
council_cases.c

.SH Description
.SP 5 5

This room is for handling control of cases.  It allows people to
add notes, open cases and general have a wild time in case land.
.EP
.SP 10 5


Started Fri Jul 21 18:57:34 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room and /std/room/inherit/council_base.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/nomic_system.h and /include/mail.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_punishment_type%^RESET%^
.EI
.SI 5
void add_punishment_type(string name, string file)
.EI
.SP 7 5

This method adds a type to the room.  A type is a type of
punishment that can be applied to the person when the case is close.
The different functions have the following arguments:
.EP
.SP 7 5
start_punishment(the_case, type, finish_func)
.EP
.SP 7 5
query_short(the_case, type, data)
.EP
.SP 7 5
complete_punishment(the_case, type, data)
.EP
.SP 7 5
finish_func(the_case, type, data) (from start())
.EP
.SP 7 5
suspend_punishment(the_case, type, data)
.EP
.SP 7 5


The functions are all called on a inheritable file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the type
.EP
.SP 9 5
short - the function to make the short description
.EP
.SP 9 5
start - the function to call at the start
.EP
.SP 9 5
complete - the function to call at the completion
.EP
.SP 9 5
suspend - suspend the punishment

.EP

.SI 3
* %^BOLD%^close_case_desc%^RESET%^
.EI
.SI 5
void close_case_desc(string desc, mixed * extra)
.EI
.SP 7 5

This method sets up an exciting set of closing comments for the
case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
desc - the closing desc
.EP
.SP 9 5
extra - the extra data

.EP

.SI 3
* %^BOLD%^complete_punishment%^RESET%^
.EI
.SI 5
void complete_punishment(string name, class nomic_case the_case, mixed data)
.EI
.SP 7 5

This method starts the punishment check thingy to get any extra
arguements.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the punishment to check
.EP
.SP 9 5
the_case - th case we are messing with
.EP
.SP 9 5
finish - the finish function to call

.EP

.SI 3
* %^BOLD%^do_add_note%^RESET%^
.EI
.SI 5
int do_add_note(int case_id)
.EI
.SP 7 5

This method adds a note to an open case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
case_id - the id of the case to add a note to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 if failed

.EP

.SI 3
* %^BOLD%^do_add_person%^RESET%^
.EI
.SI 5
int do_add_person(int case_id, string person)
.EI
.SP 7 5

This method adds a person to an open case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
case_id - the case to add it to
.EP
.SP 9 5
person - the person to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_appeal_case%^RESET%^
.EI
.SI 5
int do_appeal_case(int case_id)
.EI
.SP 7 5

This method appeals the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
case_id - the id of the case
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_close_case%^RESET%^
.EI
.SI 5
int do_close_case(int case_id, string type)
.EI
.SP 7 5

This method closes the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
case_id - the id of the case
.EP
.SP 9 5
type - the type of closing
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_list_case%^RESET%^
.EI
.SI 5
int do_list_case(int case_id)
.EI
.SP 7 5

This method lists the specified open case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
case_id - the case to list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 if failed

.EP

.SI 3
* %^BOLD%^do_list_closed_case%^RESET%^
.EI
.SI 5
int do_list_closed_case(int case_id)
.EI
.SP 7 5

This method lists the specified closed case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
case_id - the case to list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 if failed

.EP

.SI 3
* %^BOLD%^do_list_closed_cases%^RESET%^
.EI
.SI 5
int do_list_closed_cases()
.EI
.SP 7 5

This method lists all the recently closed cases.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 if failed

.EP

.SI 3
* %^BOLD%^do_list_open_cases%^RESET%^
.EI
.SI 5
int do_list_open_cases()
.EI
.SP 7 5

This method lists all the currently open cases.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 if failed

.EP

.SI 3
* %^BOLD%^do_list_punishments%^RESET%^
.EI
.SI 5
int do_list_punishments()
.EI
.SP 7 5

This method lists the current set of punishments known by this
area.

.EP

.SI 3
* %^BOLD%^do_list_punishments_desc%^RESET%^
.EI
.SI 5
int do_list_punishments_desc(string type)
.EI
.SP 7 5

This method lists the current set of punishments known by this
area.

.EP

.SI 3
* %^BOLD%^do_open_case%^RESET%^
.EI
.SI 5
int do_open_case(string people, string short)
.EI
.SP 7 5

Opening a case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
people - the people the case ie against
.EP
.SP 9 5
short - the short description of the case
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_player_status%^RESET%^
.EI
.SI 5
int do_player_status(string person)
.EI
.SP 7 5

This is the command to print the status of the current open and
recently closed cases.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to show
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_remove_person%^RESET%^
.EI
.SI 5
int do_remove_person(int case_id, string person)
.EI
.SP 7 5

This method adds a person to an open case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
case_id - the case to add it to
.EP
.SP 9 5
person - the person to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^is_last_appealer%^RESET%^
.EI
.SI 5
int is_last_appealer(class nomic_case the_case, string person)
.EI
.SP 7 5

This method checks and sees if the person is the last person to appeal
the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to check

.EP

.SI 3
* %^BOLD%^is_last_closer%^RESET%^
.EI
.SI 5
int is_last_closer(class nomic_case the_case, string person)
.EI
.SP 7 5

This method checks and sees if the person is the last person to close
the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to check

.EP

.SI 3
* %^BOLD%^query_all_punishment_types%^RESET%^
.EI
.SI 5
string * query_all_punishment_types()
.EI
.SP 7 5

This method returns the name of all the allowed punishment types.

.EP

.SI 3
* %^BOLD%^query_case_short%^RESET%^
.EI
.SI 5
string query_case_short(class nomic_case the_case)
.EI
.SP 7 5

The short description of the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the_case - the case to get a short desc
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short description of the case

.EP

.SI 3
* %^BOLD%^query_extra_player_status%^RESET%^
.EI
.SI 5
string query_extra_player_status(string player)
.EI
.SP 7 5

This method returns any extra information to print about a player in
the status information.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player we are checking
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
any extra information

.EP

.SI 3
* %^BOLD%^query_last_appealer%^RESET%^
.EI
.SI 5
string query_last_appealer(class nomic_case the_case)
.EI
.SP 7 5

This method finds the last person to appeal the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to check

.EP

.SI 3
* %^BOLD%^query_last_closer%^RESET%^
.EI
.SI 5
string query_last_closer(class nomic_case the_case)
.EI
.SP 7 5

This method checks and sees if the person is the last person to close
the case.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to check

.EP

.SI 3
* %^BOLD%^query_punishment_mail_info%^RESET%^
.EI
.SI 5
string query_punishment_mail_info(string name, class nomic_case the_case, mixed data)
.EI
.SP 7 5

This method returns the mail information of the punishment.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the punishment type
.EP
.SP 9 5
the_case - the caseof the thingy
.EP
.SP 9 5
data - the extra punishment type data
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short of the type

.EP

.SI 3
* %^BOLD%^query_punishment_short%^RESET%^
.EI
.SI 5
string query_punishment_short(string name, class nomic_case the_case, mixed data)
.EI
.SP 7 5

This method returns the short of the punishment.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the punishment type
.EP
.SP 9 5
the_case - the caseof the thingy
.EP
.SP 9 5
data - the extra punishment type data
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short of the type

.EP

.SI 3
* %^BOLD%^query_punishment_type%^RESET%^
.EI
.SI 5
string query_punishment_type(string name)
.EI
.SP 7 5

This method returns the file associated with the type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the nam eof the class to lookup
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file of the type

.EP

.SI 3
* %^BOLD%^start_punishment%^RESET%^
.EI
.SI 5
void start_punishment(string name, class nomic_case the_case, function finish)
.EI
.SP 7 5

This method starts the punishment check thingy to get any extra
arguements.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the punishment to check
.EP
.SP 9 5
the_case - th case we are messing with
.EP
.SP 9 5
finish - the finish function to call

.EP

.SI 3
* %^BOLD%^suspend_punishment%^RESET%^
.EI
.SI 5
void suspend_punishment(class nomic_case the_case)
.EI
.SP 7 5

This method suspends the punishment after it is started.  Used in the
appeals process.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the_case - th case we are messing with

.EP


