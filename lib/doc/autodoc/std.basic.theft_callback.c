.DT
theft_callback.c
Disk World autodoc help
theft_callback.c

.SH Description
.SP 5 5

Theft callback system!
.EP
.SP 10 5


Written by Terano

Started 5/04/2003

.EP

.SH Classes

.SI 3
* %^BOLD%^theft_callback%^RESET%^
class theft_callback {
string func_name;
string path;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_theft_callback%^RESET%^
.EI
.SI 5
int add_theft_callback(string func_name, string path)
.EI
.SP 7 5

Add a new theft callback!
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
int -1 if failed, otherwise the ID of the callback.

.EP

.SI 3
* %^BOLD%^event_theft%^RESET%^
.EI
.SI 5
void event_theft(object command_ob, object thief, object victim, object * stolen)
.EI

.SI 3
* %^BOLD%^query_theft_callbacks%^RESET%^
.EI
.SI 5
string query_theft_callbacks()
.EI

.SI 3
* %^BOLD%^remove_theft_callback%^RESET%^
.EI
.SI 5
int remove_theft_callback(int id)
.EI
.SP 7 5

Delete a theft callback!
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
int -1 if failed, 1 otherwise

.EP


