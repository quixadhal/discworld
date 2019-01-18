.DT
error_handler.c
Disk World autodoc help
error_handler.c

.SH Description
.SP 5 5

This is a handler to keep track of errors, make adding them and querying
them easier.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Jun 17 17:28:04 PDT 2002

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/error_handler.h and /include/db.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^build_sql_update%^RESET%^
.EI
.SI 5
string build_sql_update(int id, string field, mixed value)
.EI
.SP 7 5

This method builds an sql update from the given inputs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the bug to update
.EP
.SP 9 5
field - the field to update
.EP
.SP 9 5
value - the new value of the field

.EP

.SI 3
* %^BOLD%^create_error_comment%^RESET%^
.EI
.SI 5
class error_comment create_error_comment(mapping map)
.EI
.SP 7 5

This method creates the error comment from the input mapping.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the mapping to create the comment from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the error comment class

.EP

.SI 3
* %^BOLD%^create_error_complete%^RESET%^
.EI
.SI 5
class error_complete create_error_complete(class error_details details)
.EI
.SP 7 5

This method creates the complete error class from the error details
passed in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
details - the details to make the complete error from

.EP

.SI 3
* %^BOLD%^create_error_details%^RESET%^
.EI
.SI 5
class error_details create_error_details(mapping map)
.EI
.SP 7 5

This method creates the error details from the return data.

.EP

.SI 3
* %^BOLD%^create_error_forward%^RESET%^
.EI
.SI 5
class error_forward create_error_forward(mapping map)
.EI
.SP 7 5

This method creates the error forward from the input mapping.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the mapping to create the forward from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the error forward class

.EP

.SI 3
* %^BOLD%^create_error_replies%^RESET%^
.EI
.SI 5
class error_replies create_error_replies(mapping map)
.EI
.SP 7 5

This method creates the error reply class from the sql query.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the mapping to convert to the class
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nice new error reply class

.EP

.SI 3
* %^BOLD%^create_error_summary%^RESET%^
.EI
.SI 5
class error_summary create_error_summary(mapping map)
.EI
.SP 7 5

This method creates the error summary from the input mapping.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
map - the mapping to create the summary from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the error summary class

.EP

.SI 3
* %^BOLD%^do_add_error_reply%^RESET%^
.EI
.SI 5
int do_add_error_reply(int bug_id, string from, string to, string subject, string message)
.EI
.SP 7 5

THis method adds an error reply into the system.  THis is a debug message
and only useful until all the error setups are connected together properly.

.EP

.SI 3
* %^BOLD%^do_assign_bug_to%^RESET%^
.EI
.SI 5
int do_assign_bug_to(int id, string assigned_to, function finished)
.EI
.SP 7 5

This method changes who the bug is assigned to.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the bug to change
.EP
.SP 9 5
assigned_to - the person it is assigned to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_change_category%^RESET%^
.EI
.SI 5
int do_change_category(int id, string category, function finished)
.EI
.SP 7 5

This method changes the category of the bug.  The category is in the set
'room', 'object', 'help', 'ritual', 'spell', 'command', 'general'.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the bug to update
.EP
.SP 9 5
category - the new category of the bug
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_change_status%^RESET%^
.EI
.SI 5
int do_change_status(int id, string status, int nomail, string subject, string user, string message, function finished)
.EI
.SP 7 5

This method changes the status of the bug, it also automaticaly
sends a message to the person who reported the bug and who the bug
is assigned to if someone else changes it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the bug to change
.EP
.SP 9 5
status - the new status of the bug
.EP
.SP 9 5
nomail - dont send any mail
.EP
.SP 9 5
user - the user who is making the change
.EP
.SP 9 5
subject - the subject of the mail
.EP
.SP 9 5
message - the main body of the mail
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP

.SI 3
* %^BOLD%^do_change_type%^RESET%^
.EI
.SI 5
int do_change_type(int id, string type, function finished)
.EI
.SP 7 5

This method changes the type of the bug.  The type is in the set
'bug', 'typo', 'idea'.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the bug to update
.EP
.SP 9 5
type - the new type of the bug
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_comment%^RESET%^
.EI
.SI 5
int do_comment(int id, string user, string comment, function finished)
.EI
.SP 7 5

This method adds a comment into the database.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the bug to comment on
.EP
.SP 9 5
user - the user making the comment
.EP
.SP 9 5
comment - the comment to make
.EP
.SP 9 5
finished - the finished function

.EP

.SI 3
* %^BOLD%^do_error_replies%^RESET%^
.EI
.SI 5
int do_error_replies(string recipient, int only_new, function finished)
.EI
.SP 7 5

This method looks for all the error replies for the specific recipient.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
recipient - the recipient
.EP
.SP 9 5
only_new - only new messages
.EP
.SP 9 5
finished - the function to call on finishing

.EP

.SI 3
* %^BOLD%^do_forward%^RESET%^
.EI
.SI 5
int do_forward(int id, string user, string dir, function finished)
.EI
.SP 7 5

This method changes the status of the bug, it also automaticaly
sends a message to the person who reported the bug and who the bug
is assigned to if someone else changes it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the bug to change
.EP
.SP 9 5
user - the user
.EP
.SP 9 5
dir - the directory
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP

.SI 3
* %^BOLD%^do_query_bug_details%^RESET%^
.EI
.SI 5
int do_query_bug_details(int bug_id, function finished)
.EI
.SP 7 5

This method sets up a call to return the details of the error.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bug_id - the id of the bug to get the details from
.EP
.SP 9 5
finished - the function to call when things are finished
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_query_bug_summary%^RESET%^
.EI
.SI 5
int do_query_bug_summary(class error_query query, function finished)
.EI
.SP 7 5

This method sets up a query for the specific data, this returns the
bug summary for all the found bugs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
query - the class to use for the query
.EP
.SP 9 5
finished - the function to call when things are finished
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_query_open_bugs_by_reporter%^RESET%^
.EI
.SI 5
int do_query_open_bugs_by_reporter(string reporter, function finished)
.EI
.SP 7 5

This returns the bug summary for all the open bugs associated with
the specified reporter.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
reporter - the reporter to find the bugs for
.EP
.SP 9 5
finished - the function to call when things are finished
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_query_open_bugs_for_fname%^RESET%^
.EI
.SI 5
int do_query_open_bugs_for_fname(string fname, function finished)
.EI
.SP 7 5

This returns the bug summary for all the open bugs associated with
the specified file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the file name
.EP
.SP 9 5
finished - the function to call when things are finished
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_query_open_bugs_in_dir%^RESET%^
.EI
.SI 5
int do_query_open_bugs_in_dir(string dir, function finished)
.EI
.SP 7 5

This method finds the bug summary for all the open bugs in the
specified directory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory to find the open bugs in
.EP
.SP 9 5
finished - the function to call when things are finished
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_status_error_reply%^RESET%^
.EI
.SI 5
int do_status_error_reply(int id, string status, function finished)
.EI
.SP 7 5

This method sets the error reply status to something useful.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the error reply
.EP
.SP 9 5
status - the new status
.EP
.SP 9 5
finished - the finished fuinction
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the setup is successful, 0 if not

.EP

.SI 3
* %^BOLD%^finish_comments%^RESET%^
.EI
.SI 5
void finish_comments(int type, mixed data, function finished, class error_complete * errors, string ids, string query)
.EI
.SP 7 5

This method updates the comments based on the query results.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - type type
.EP
.SP 9 5
fdata - the return data from the query
.EP
.SP 9 5
function - finsiished the finished function
.EP
.SP 9 5
errors - the errors results we are updating
.EP
.SP 9 5
query - the ids list we are using

.EP

.SI 3
* %^BOLD%^finish_details%^RESET%^
.EI
.SI 5
void finish_details(int type, mixed data, function finished, string query)
.EI
.SP 7 5

This method finishes a details based query.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type returned (error etc)
.EP
.SP 9 5
data - the data returned
.EP
.SP 9 5
finished - the function to call on finish

.EP

.SI 3
* %^BOLD%^finish_details_error_replies%^RESET%^
.EI
.SI 5
void finish_details_error_replies(int type, mixed data, function finished, class error_complete * errors, string ids, string query)
.EI
.SP 7 5

This method updates the comments based on the query results.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - type type
.EP
.SP 9 5
fdata - the return data from the query
.EP
.SP 9 5
function - finsiished the finished function
.EP
.SP 9 5
errors - the errors results we are updating
.EP
.SP 9 5
query - the ids list we are using

.EP

.SI 3
* %^BOLD%^finish_error_replies%^RESET%^
.EI
.SI 5
void finish_error_replies(int type, mixed data, function finished, string query)
.EI
.SP 7 5

This method finishes doing a reply lookup.

.EP

.SI 3
* %^BOLD%^finish_forward_query%^RESET%^
.EI
.SI 5
void finish_forward_query(int type, mixed data, function finished, string user, string newdir)
.EI
.SP 7 5

This method finishes a details based query.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type returned (error etc)
.EP
.SP 9 5
data - the data returned
.EP
.SP 9 5
finished - the function to call on finish

.EP

.SI 3
* %^BOLD%^finish_forwards%^RESET%^
.EI
.SI 5
void finish_forwards(int type, mixed data, function finished, class error_complete * errors, string query)
.EI
.SP 7 5

This method updates the comments based on the query results.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - type type
.EP
.SP 9 5
fdata - the return data from the query
.EP
.SP 9 5
function - finsiished the finished function
.EP
.SP 9 5
errors - the errors results we are updating
.EP
.SP 9 5
query - the ids list we are using

.EP

.SI 3
* %^BOLD%^finish_status_query%^RESET%^
.EI
.SI 5
void finish_status_query(int type, mixed data, function finished, string status, int nomail, string user, string subject, string message)
.EI

.SI 3
* %^BOLD%^finish_summary%^RESET%^
.EI
.SI 5
void finish_summary(int type, mixed data, function finished, string query)
.EI
.SP 7 5

This method finishes a summary based query.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type returned (error etc)
.EP
.SP 9 5
data - the data returned
.EP
.SP 9 5
finished - the function to call on finish

.EP

.SI 3
* %^BOLD%^finish_update%^RESET%^
.EI
.SI 5
void finish_update(int type, mixed data, function finished, string query)
.EI
.SP 7 5

This method finishes an update based query.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type returned (error etc)
.EP
.SP 9 5
data - the data returned
.EP
.SP 9 5
finished - the function to call on finish

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^build_sql_query%^RESET%^
.EI
.SI 5
string build_sql_query(string select_rows, class error_query query)
.EI
.SP 7 5

This method builds an sql query from the input query.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
query - the input query
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an sql string query

.EP

.SI 3
* %^BOLD%^setup_error_handler%^RESET%^
.EI
.SI 5
void setup_error_handler(string user, string db, string password)
.EI
.SP 7 5

This method sets up the access permissions to use for accessing
the database.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
user - the user to use
.EP
.SP 9 5
db - the database to use
.EP
.SP 9 5
password - the password to use

.EP


