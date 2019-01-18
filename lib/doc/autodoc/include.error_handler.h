.DT
error_handler.h
Disk World autodoc help
error_handler.h

.SH Description
.SP 5 5

This is the include file to handler all the basic error handler
code.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Feb  6 17:40:26 PST 2003

.EP

.SH Classes

.SI 3
* %^BOLD%^error_comment%^RESET%^
class error_comment {
int date;
string commenter;
string comment;
}

.EI
.SP 7 5

The comments associated with the bug.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
date - the date the comment was added
.EP
.SP 9 5
commenter - the person who commented on the bug
.EP
.SP 9 5
comment - the comment about the bug

.EP

.SI 3
* %^BOLD%^error_complete%^RESET%^
class error_complete {
class error_details details;
class error_comment * comments;
class error_forward * forwards;
class error_replies * replies;
}

.EI
.SP 7 5

All the details of the error.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
details - the details of the error
.EP
.SP 9 5
comments - the comments associated with the error
.EP
.SP 9 5
forwards - the forwards associated with the error
.EP
.SP 9 5
replies - the replies associated with the error

.EP

.SI 3
* %^BOLD%^error_details%^RESET%^
class error_details {
class error_summary summary;
int fix_date;
string fixer;
string report;
string runtime;
}

.EI
.SP 7 5

The details of the bug.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
summary - the summary class for the basic sumamry details
.EP
.SP 9 5
fix_date - the date the bug was fixed
.EP
.SP 9 5
fixer - the person who fixed the bug
.EP
.SP 9 5
report - the report associated with the bug
.EP
.SP 9 5
runtime - the runtime error associated with the bug

.EP

.SI 3
* %^BOLD%^error_forward%^RESET%^
class error_forward {
int date;
string forwarder;
string old_directory;
}

.EI
.SP 7 5

The forwards associated with the bug.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
date - the date the forward was done
.EP
.SP 9 5
forwarder - the person who forwared the bug
.EP
.SP 9 5
old_directory - the old directory of the bug

.EP

.SI 3
* %^BOLD%^error_query%^RESET%^
class error_query {
int no_extra_details;
int recursive;
int id;
int bug_offset;
int * id_list;
string dir;
string assigned_to;
string file_name;
string reporter;
string fixer;
string * status;
string * category;
string * type;
string order_by;
}

.EI
.SP 7 5

This is the class used to generate queries through the error handler.
You do not need to fill in all the fields, only the filled in ones will
be used as restrictions on the query.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
no_extra_details - this only works with the bug details queries
                         and it means that only the main data is fetched,
                         not comments etc.
.EP
.SP 9 5
recursive - do recursive directory lookups
.EP
.SP 9 5
id - look up this id specifically
.EP
.SP 9 5
id_list - look up this list of ids, if this is specified it
                overrides id
.EP
.SP 9 5
bug_offset - the offset to use in the limit command
.EP
.SP 9 5
dir - the directory to use
.EP
.SP 9 5
assigned_to - who it is assigned to
.EP
.SP 9 5
file_name - specific file name to lookup
.EP
.SP 9 5
reporter - the reporter to lookup
.EP
.SP 9 5
fixer - the person who fixed the bug
.EP
.SP 9 5
status - the status of the bug
.EP
.SP 9 5
category - the category of the bug
.EP
.SP 9 5
type - the type of the bug
.EP
.SP 9 5
order_by - one of the ordering methods

.EP

.SI 3
* %^BOLD%^error_replies%^RESET%^
class error_replies {
int id;
int bug_id;
string sender;
string recipient;
string status;
int senddate;
string subject;
string message;
}

.EI
.SP 7 5

The reply data for the error.  This is where the bug replies are
stored.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
id - the id of the bug reply
.EP
.SP 9 5
bug_id - the id of the bug
.EP
.SP 9 5
sender - who sent the bug reply
.EP
.SP 9 5
recipient - the recipient of the bug reply
.EP
.SP 9 5
status - the status of the bug reply
.EP
.SP 9 5
senddate - when the bug was sent
.EP
.SP 9 5
subject - the subject of the reply
.EP
.SP 9 5
message - the message associated with the bug

.EP

.SI 3
* %^BOLD%^error_summary%^RESET%^
class error_summary {
int id;
int entry_date;
string filename;
string category;
string assigned_to;
string reporter;
string type;
string status;
string directory;
}

.EI
.SP 7 5

The summary plus the details is the complete
error.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
id - the unique id of the bug
.EP
.SP 9 5
entry_date - the date the bug was added
.EP
.SP 9 5
filename - the filename the bug was put against
.EP
.SP 9 5
assigned_to - who the bug is assigned to
.EP
.SP 9 5
reporter - the person who reported the bug
.EP
.SP 9 5
category - is one of ROOM | OBJECT etc
.EP
.SP 9 5
type - is one of TYPO | BUG | IDEA
.EP
.SP 9 5
status - is one of OPEN | CLOSED | etc
.EP
.SP 9 5
directory - the directory the bug is in

.EP


.SH Defines
.SI 3
* %^BOLD%^ERROR_ASSIGNED_NO_ONE%^RESET%^
.EI
.SP 7 5
This is the define for the user name to use for assigned to 'no one'. 
.EP

.SI 3
* %^BOLD%^ERROR_BAD_QUERY%^RESET%^
.EI
.SP 7 5

This is the return value when the query is bad.

.EP

.SI 3
* %^BOLD%^ERROR_CATEGORIES%^RESET%^
.EI
.SP 7 5
This is the list of valid categories. 
.EP

.SI 3
* %^BOLD%^ERROR_HANDLER%^RESET%^
.EI
.SP 7 5
This is the path to the error handler.  
.EP

.SI 3
* %^BOLD%^ERROR_ORDER_BY_DATE%^RESET%^
.EI
.SP 7 5
This define is used to order by the date. 
.EP

.SI 3
* %^BOLD%^ERROR_ORDER_BY_DIRECTORY%^RESET%^
.EI
.SP 7 5
This define is used to order by the directory. 
.EP

.SI 3
* %^BOLD%^ERROR_ORDER_BY_FILENAME%^RESET%^
.EI
.SP 7 5
This define is used to order by the filename. 
.EP

.SI 3
* %^BOLD%^ERROR_ORDER_BY_REPORTER%^RESET%^
.EI
.SP 7 5
This define is used to order by the reporter. 
.EP

.SI 3
* %^BOLD%^ERROR_STATUS%^RESET%^
.EI
.SP 7 5
This is the list of valid statuses. 
.EP

.SI 3
* %^BOLD%^ERROR_STATUS_CONSIDERING%^RESET%^
.EI
.SP 7 5
Error status considering the idea, not yet fixed. 
.EP

.SI 3
* %^BOLD%^ERROR_STATUS_DENIED%^RESET%^
.EI
.SP 7 5
Error status denied.  The bug was not fixed and is marked as not being
a bug 
.EP

.SI 3
* %^BOLD%^ERROR_STATUS_FIXED%^RESET%^
.EI
.SP 7 5
Error status fixed.  The bug has been resolved and closed. 
.EP

.SI 3
* %^BOLD%^ERROR_STATUS_FIXING%^RESET%^
.EI
.SP 7 5
Error status fixing, so it is in the middle of being fixed. 
.EP

.SI 3
* %^BOLD%^ERROR_STATUS_OPEN%^RESET%^
.EI
.SP 7 5
Error status open 
.EP

.SI 3
* %^BOLD%^ERROR_TYPE%^RESET%^
.EI
.SP 7 5
This is the list of valid types. 
.EP

