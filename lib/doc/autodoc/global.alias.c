.DT
alias.c
Disk World autodoc help
alias.c

.SH Description
.SP 5 5

The alias control module for players.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/history.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/alias.h, /include/player.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_player_alias%^RESET%^
.EI
.SI 5
int add_player_alias(string name, mixed * value)
.EI
.SP 7 5

This method sets the alias to the new value.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the alias
.EP
.SP 9 5
value - the value to set the alias too
.EP

.SI 3
* %^BOLD%^alias_commands%^RESET%^
.EI
.SI 5
void alias_commands()
.EI
.SP 7 5

This method adds all the alias commands onto the player.  The commands
are 'alias', 'unalias', 'ealias', 'END_ALIAS' and the "*" pattern
expand the alias.

.EP

.SI 3
* %^BOLD%^is_alias%^RESET%^
.EI
.SI 5
int is_alias(string verb)
.EI
.SP 7 5

This method tells us if the given name is an alias.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the verb to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is an alias, 0 if not

.EP

.SI 3
* %^BOLD%^query_aliases%^RESET%^
.EI
.SI 5
mapping query_aliases()
.EI
.SP 7 5

This method returns a complete list of all the aliases
defined on the player.  This is the internal mapping so it will
be quite unreadable.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of aliases

.EP

.SI 3
* %^BOLD%^query_player_alias%^RESET%^
.EI
.SI 5
mixed * query_player_alias(string name)
.EI
.SP 7 5

This method returns the value of the specified alias.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the alias to query
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value of the alias
.EP

.SI 3
* %^BOLD%^remove_alias_thing%^RESET%^
.EI
.SI 5
int remove_alias_thing(string verb)
.EI
.SP 7 5

This is called to signify the end of an alias.  This is needed
keep track of recursive aliases and such like.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the verb name to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1
.EP

.SI 3
* %^BOLD%^remove_all_aliases%^RESET%^
.EI
.SI 5
int remove_all_aliases()
.EI
.SP 7 5

This method zaps all the current aliases defined.  This can only be
called by a high lord.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP

.SI 3
* %^BOLD%^remove_player_alias%^RESET%^
.EI
.SI 5
int remove_player_alias(string name)
.EI
.SP 7 5

This method will remove the alias from the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the alias
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^exec_alias%^RESET%^
.EI
.SI 5
void exec_alias(string verb, string args)
.EI
.SP 7 5

This method runs the alias and executes all the commands in the
alias.  You should probably use run_alias() instead.  This calls
set_doing_alias() to setup blocking.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the name of the alias
.EP
.SP 9 5
args - the arguments associated with the alias
.EP

.SI 3
* %^BOLD%^expand_alias%^RESET%^
.EI
.SI 5
string * expand_alias(string verb, string args)
.EI
.SP 7 5

This method expands the alias from the input string thingy.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the verb to expand
.EP
.SP 9 5
args - the arguments to the verb
.EP

.SI 3
* %^BOLD%^is_doing_alias%^RESET%^
.EI
.SI 5
int is_doing_alias(string verb)
.EI
.SP 7 5

This method checks to see if the player is doing the specified alias
already.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the verb to check
.EP

.SI 3
* %^BOLD%^run_alias%^RESET%^
.EI
.SI 5
string * run_alias(string verb, string args)
.EI
.SP 7 5

Attempt to expand the alias.    This will look up the alias and
see if it is defined.  If it is, it will attempt to expand the 
alias. This does not call set_doing_alias().  This will
return 0 if the alias does not exist, or the alias is already
being run.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the name of the alias to expand
.EP
.SP 9 5
args - the arguments to the alias
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array if the alias was expanded, 0 if failed
.EP

.SI 3
* %^BOLD%^set_doing_alias%^RESET%^
.EI
.SI 5
void set_doing_alias(string verb)
.EI
.SP 7 5

This method sets us as currently doing the given alias.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the alias to do
.EP


