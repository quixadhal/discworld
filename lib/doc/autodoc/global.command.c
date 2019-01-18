.DT
command.c
Disk World autodoc help
command.c

.SH Description
.SP 5 5

Handles commands from directories for living objects.
   /global/command.c
   from the Nightmare IVr1 Object Library
   handles commands of living objects
   created by Descartes of Borg 950323
   Hacked for Discworld by Turrican 4-11-95

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/command.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^AddSearchPath%^RESET%^
.EI
.SI 5
string * AddSearchPath(mixed val)
.EI
.SP 7 5

Add a directory(s) into the search path for commands.  If the input is
an array then the members of that are added to the search path.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
val - the path to array
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new search path

.EP

.SI 3
* %^BOLD%^GetClient%^RESET%^
.EI
.SI 5
string GetClient()
.EI
.SP 7 5

This tells us if we are a client.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 0

.EP

.SI 3
* %^BOLD%^GetForced%^RESET%^
.EI
.SI 5
int GetForced()
.EI
.SP 7 5

This method returns if we are being forced or not
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if we are being forced

.EP

.SI 3
* %^BOLD%^GetSearchPath%^RESET%^
.EI
.SI 5
string * GetSearchPath()
.EI
.SP 7 5

This method returns the current search path
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current search path

.EP

.SI 3
* %^BOLD%^RemoveSearchPath%^RESET%^
.EI
.SI 5
string * RemoveSearchPath(mixed val)
.EI
.SP 7 5

Remove a directory(s) from the search path.  If the input is
an array then the members of that are added to the search path.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
val - the directory(s) to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new search path

.EP

.SI 3
* %^BOLD%^eventForce%^RESET%^
.EI
.SI 5
int eventForce(string cmd)
.EI
.SP 7 5

A force event.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cmd - the command to force
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP

.SI 3
* %^BOLD%^query_current_cmd%^RESET%^
.EI
.SI 5
string query_current_cmd()
.EI
.SP 7 5
What are the args to the current verb? 
.EP

.SI 3
* %^BOLD%^query_current_verb%^RESET%^
.EI
.SI 5
string query_current_verb()
.EI
.SP 7 5
What is the current verb? 
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^cmdAll%^RESET%^
.EI
.SI 5
int cmdAll(string args)
.EI
.SP 7 5

Try and execute the command.  It calls the fuinction command_shadowed
with the verb and args as parameters if the command is found to exist.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the args to use for the verb
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^cmdPatterns%^RESET%^
.EI
.SI 5
mixed * cmdPatterns(string verb)
.EI
.SP 7 5

This method tries to lookup the patterns for the given verb.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the verb to get the patterns for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of patterns
.EP

.SI 3
* %^BOLD%^command_commands%^RESET%^
.EI
.SI 5
void command_commands()
.EI
.SP 7 5

Add in the command catch all action.

.EP


