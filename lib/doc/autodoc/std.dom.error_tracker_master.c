.DT
error_tracker_master.c
Disk World autodoc help
error_tracker_master.c

.SH Description
.SP 5 5

The nice error handler for domains to do wonderful things with.
Lots hacked by Pinkfish to make it work with the mysql handler and not
use blocking io calls.
.EP
.SP 10 5


Written by Sin

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/path.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/project_management.h, /include/config.h, /include/db.h and /include/board.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_the_work%^RESET%^
.EI
.SI 5
void do_the_work()
.EI
.SP 7 5

This bit does the actual processing and prints the results.

.EP

.SI 3
* %^BOLD%^initialise_variables%^RESET%^
.EI
.SI 5
void initialise_variables()
.EI
.SP 7 5

This method initialises all the variables for a error run.

.EP

.SI 3
* %^BOLD%^query_board%^RESET%^
.EI
.SI 5
string query_board()
.EI
.SP 7 5

This method returns the board to post the messages to.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the board to post the messages to

.EP

.SI 3
* %^BOLD%^query_classifications%^RESET%^
.EI
.SI 5
string * query_classifications()
.EI

.SI 3
* %^BOLD%^query_domain%^RESET%^
.EI
.SI 5
string query_domain()
.EI
.SP 7 5

This method returns the domain of the tracker.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the domain of the tracker

.EP

.SI 3
* %^BOLD%^query_post_message%^RESET%^
.EI
.SI 5
string query_post_message()
.EI
.SP 7 5

This method makes the message to post to the board from all the
other rubbish.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the message to post

.EP

.SI 3
* %^BOLD%^query_project_message%^RESET%^
.EI
.SI 5
string query_project_message()
.EI

.SI 3
* %^BOLD%^set_board%^RESET%^
.EI
.SI 5
void set_board(string board)
.EI
.SP 7 5

This method sets the board to post the messages to.  The board defaults
to the domain name, so this is not required for most domains.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to post to

.EP

.SI 3
* %^BOLD%^set_domain%^RESET%^
.EI
.SI 5
void set_domain(string name)
.EI
.SP 7 5

This method sets the domain of the tracker.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the domain of the tracker

.EP

.SI 3
* %^BOLD%^set_name%^RESET%^
.EI
.SI 5
void set_name(string name)
.EI
.SP 7 5

This method sets the name of the tracker.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the tracker

.EP

.SI 3
* %^BOLD%^set_other_dirs%^RESET%^
.EI
.SI 5
void set_other_dirs(string * others)
.EI
.SP 7 5

This method sets the list of other directories to use for tracking
errors.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
others - the other directories to use

.EP

.SI 3
* %^BOLD%^set_period%^RESET%^
.EI
.SI 5
void set_period(int length)
.EI
.SP 7 5

This method sets how long between each time the error tracker fires.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
length - the length of time in seconds

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^classify_directory%^RESET%^
.EI
.SI 5
string classify_directory(string path)
.EI

.SI 3
* %^BOLD%^iterate%^RESET%^
.EI
.SI 5
void iterate()
.EI
.SP 7 5

This method goes over all the directories and makes up the
complete report.

.EP

.SI 3
* %^BOLD%^post%^RESET%^
.EI
.SI 5
varargs int post(string message, string title)
.EI
.SP 7 5

This method posts the results to the correct board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
message - the message to post
.EP
.SP 9 5
title - the title of the message

.EP


