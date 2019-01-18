.DT
respond_cmd.c
Disk World autodoc help
respond_cmd.c

.SH Description
.SP 5 5

This inheritable will handle all the generic stuff for commands that
need a response.  Like 'teach' and 'offer'.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Aug 17 04:13:10 EDT 1998

.EP

.SH Classes

.SI 3
* %^BOLD%^respond_data%^RESET%^
class respond_data {
object ob;
mixed data;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_respond_command%^RESET%^
.EI
.SI 5
void add_respond_command(string type, object ob, mixed data)
.EI
.SP 7 5

This method adds a responable command set of values to the
living object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of thing to set as a response
.EP
.SP 9 5
ob - the object which is giving us the thing
.EP
.SP 9 5
data - the data associated with the command

.EP

.SI 3
* %^BOLD%^clear_respond_commands%^RESET%^
.EI
.SI 5
void clear_respond_commands()
.EI

.SI 3
* %^BOLD%^query_respond_command%^RESET%^
.EI
.SI 5
mixed query_respond_command(string type, object ob)
.EI
.SP 7 5

This method returns the data associated with specified type
and object for a respondable command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of the respondable
.EP
.SP 9 5
ob - the object we are checking against
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the data associated with the response, 0 if not found

.EP

.SI 3
* %^BOLD%^remove_respond_command%^RESET%^
.EI
.SI 5
void remove_respond_command(string type, object ob)
.EI
.SP 7 5

This method removes the response for this command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of the responable
.EP
.SP 9 5
ob - the object that we are removing a responable for

.EP


