.DT
council_citizenship.c
Disk World autodoc help
council_citizenship.c

.SH Description
.SP 5 5

This is the place you go to try and become a citizen of the council
area.
.EP
.SP 10 5


Written by Pinkfish

Started Wed Sep 27 18:19:22 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room, /std/room/inherit/honours, /std/room/inherit/council_base and /std/basic/expressions.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/nomic_system.h, /include/mail.h and /include/expressions.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^approval_list%^RESET%^
class approval_list {
string * sponsors;
mapping against;
int time_added;
}

.EI

.SI 3
* %^BOLD%^approved_person%^RESET%^
class approved_person {
class approval_list info;
int time_approved;
string approver;
int denied;
}

.EI

.SI 3
* %^BOLD%^blacklist%^RESET%^
class blacklist {
string blacklister;
string reason;
}

.EI

.SI 3
* %^BOLD%^denied%^RESET%^
class denied {
string denier;
string deny_reason;
int time_denied;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_add_blacklist%^RESET%^
.EI
.SI 5
int do_add_blacklist(string person, string reason)
.EI
.SP 7 5

This method adds a person to the blacklist.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to add
.EP
.SP 9 5
reason - the reason for them to be blacklisted

.EP

.SI 3
* %^BOLD%^do_apply%^RESET%^
.EI
.SI 5
int do_apply()
.EI
.SP 7 5

This is the method used for someone to apply for citizenship.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_approve%^RESET%^
.EI
.SI 5
int do_approve(string person)
.EI
.SP 7 5

This method makes the citizen approved by a happy little magistrate.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to approve

.EP

.SI 3
* %^BOLD%^do_deny%^RESET%^
.EI
.SI 5
int do_deny(string person, string reason)
.EI
.SP 7 5

This allows you to put in a reason for denying someone for citizenship.

.EP

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI
.SP 7 5

This method lists all the current applicants.

.EP

.SI 3
* %^BOLD%^do_list_blacklist%^RESET%^
.EI
.SI 5
int do_list_blacklist(int verbose)
.EI
.SP 7 5

This method lists the current blacklist.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verbose - list in verbose mode or not

.EP

.SI 3
* %^BOLD%^do_list_denied%^RESET%^
.EI
.SI 5
int do_list_denied()
.EI
.SP 7 5

This method lists the current set of denied people.

.EP

.SI 3
* %^BOLD%^do_list_expression%^RESET%^
.EI
.SI 5
int do_list_expression()
.EI
.SP 7 5

This method lists the expression used for showing if someone is
allowed to be a citizen or not.

.EP

.SI 3
* %^BOLD%^do_remove_blacklist%^RESET%^
.EI
.SI 5
int do_remove_blacklist(string person)
.EI
.SP 7 5

This method removes a person from the blacklist.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to remove

.EP

.SI 3
* %^BOLD%^do_set_expression%^RESET%^
.EI
.SI 5
int do_set_expression(string expression, int sponsor_expr)
.EI
.SP 7 5

This method sets up the expression to use to check to make sure people
are allowed to apply for citizenship.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
expression - the expression to set the check to
.EP
.SP 9 5
sponsor_expr - sets the sponsor or the citizen expression

.EP

.SI 3
* %^BOLD%^do_sponsor%^RESET%^
.EI
.SI 5
int do_sponsor(string person)
.EI
.SP 7 5

This allows someone to sponsor someone for citizenship.

.EP

.SI 3
* %^BOLD%^load_me%^RESET%^
.EI
.SI 5
void load_me()
.EI
.SP 7 5

This loads all the exciting data for the citizenship room.

.EP

.SI 3
* %^BOLD%^post_weekly_results%^RESET%^
.EI
.SI 5
void post_weekly_results()
.EI
.SP 7 5

This method posts the weekly results about the citizenship approvals
to the council board.

.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This method returns the current save file name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current save file name

.EP

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5

This method saves everything!

.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string fname)
.EI
.SP 7 5

This method sets the save file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the name of the save file

.EP


