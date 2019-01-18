.DT
lord.c
Disk World autodoc help
lord.c

.SH Description
.SP 5 5

This is the lord player object and contains any needed bits to
handle lord specific commands and things.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/wiz_file_comm.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/command.h and /include/mail.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_director%^RESET%^
.EI
.SI 5
nomask int query_director()
.EI

.SI 3
* %^BOLD%^query_lord%^RESET%^
.EI
.SI 5
nomask int query_lord()
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^bulk_delete%^RESET%^
.EI
.SI 5
int bulk_delete(string word)
.EI
.SP 7 5

This method forces a bulk delete on a certain letter.
This letter will then be checked to see if any of
those players have timed out and should be deleted.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the letter to bulk delete
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on falure and 1 on success

.EP

.SI 3
* %^BOLD%^clean_up_files%^RESET%^
.EI
.SI 5
int clean_up_files(string word)
.EI
.SP 7 5

This method forces the clean up of certain file son the
bulk deleter.  This cleans up all the files associated with
the given player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the player to clean up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_dismiss%^RESET%^
.EI
.SI 5
int do_dismiss(string str, string reason)
.EI
.SP 7 5

This method is called when the director attempts to dismiss
a creator who is currently working (or not working
if they are being dimissed I guess).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the creator to dismiss
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_qsnoop%^RESET%^
.EI
.SI 5
int do_qsnoop(object * obs)
.EI
.SP 7 5

This method is used to do a quiet snoop on a player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the player to snoop
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^employ%^RESET%^
.EI
.SI 5
int employ(string str)
.EI
.SP 7 5

This method is called when a director  attempts to employ
someone in their domain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the player to employ
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^end_it_all%^RESET%^
.EI
.SI 5
int end_it_all()
.EI
.SP 7 5

Very fast shutdown.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^new_domain%^RESET%^
.EI
.SI 5
int new_domain(string dom, string director)
.EI
.SP 7 5

This method is used to create a new domain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the input parameters
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^process_input%^RESET%^
.EI
.SI 5
mixed process_input(string input)
.EI
.SP 7 5

No priorities etc anymore, so directors get there own very small
process_input parser(tm)

.EP


