.DT
login.c
Disk World autodoc help
login.c

.SH Description
.SP 5 5

The main login object.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/mail.h, /include/data.h, /include/access.h, /include/player_handler.h, /include/playerinfo.h, /include/random_names.h, /include/login_handler.h, /include/login.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^state%^RESET%^
class state {
string name;
string action;
string write;
int noecho;
mapping events;
}

.EI
.SP 7 5

This structure holds a single state. A state has a name,
an action or write and a set of events. 
.EP
.SP 7 5
The name is used to identify the state and must be unique within the state table. An action is a function to call while a write is a string or file to be written to the screen. The events mapping is made of event and new-state. The event is the trigger and new-state is the new state the machine should enter.

An event is a string of the form "type-data" where type is either "input" or "return" indicating user input or the return from an action function. Thus "return-ok" would be triggered if the action function in this state returned "ok" while "input-y" would be triggered if the user typed in "y". The special event "input" will trigger on any user input while the event "default" will trigger on any input or return value. 
.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^debug_log%^RESET%^
.EI
.SI 5
void debug_log(string fmt, mixed args ...)
.EI

.SI 3
* %^BOLD%^enter_state%^RESET%^
.EI
.SI 5
void enter_state(string new_state)
.EI

.SI 3
* %^BOLD%^main_random_name_menu%^RESET%^
.EI
.SI 5
mixed main_random_name_menu(string event)
.EI

.SI 3
* %^BOLD%^query_delete_player_file_name%^RESET%^
.EI
.SI 5
string query_delete_player_file_name(string name)
.EI
.SP 7 5

This method returns the path to the deleted player file.
This is the deleted player files actual normal location, it was written to
allow moving the player files aruond easier.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of hte player whose file to find
.EP

.SI 3
* %^BOLD%^query_login_ob%^RESET%^
.EI
.SI 5
int query_login_ob()
.EI
.SP 7 5

This method determines if the object is a login object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^query_player_file_name%^RESET%^
.EI
.SI 5
string query_player_file_name(string name)
.EI
.SP 7 5

This method returns the path to the player file.
This is the player files actual normal location, it was written to
allow moving the player files aruond easier.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of hte player whose file to find
.EP

.SI 3
* %^BOLD%^quit%^RESET%^
.EI
.SI 5
mixed quit(string event)
.EI


