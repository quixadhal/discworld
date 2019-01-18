.DT
new_soul.c
Disk World autodoc help
new_soul.c

.SH Description
.SP 5 5

The soul handler for Discworld.  Handles the soul definitions and
the mangling needed to print the output for the players.

.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/user_parser.h, /include/soul.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_soul_command%^RESET%^
.EI
.SI 5
void add_soul_command(string name, mixed data)
.EI
.SP 7 5

Adds in a soul command.  Only allows additions from
the soul compiler.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the soul command
.EP
.SP 9 5
data - the data associated with the soul command

.EP

.SI 3
* %^BOLD%^clean_cache%^RESET%^
.EI
.SI 5
void clean_cache()
.EI

.SI 3
* %^BOLD%^command_control%^RESET%^
.EI
.SI 5
int command_control(string verb, object * obs,  string, string in_dir_match, string * args, string pattern)
.EI
.SP 7 5

The main soul handling bit.  This is called by the add_command code
when a soul command is matched.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the verb matched
.EP
.SP 9 5
obs - the objects to do the soul command on
.EP
.SP 9 5
in_dir_match - the name which was matched for the peoples names
.EP
.SP 9 5
args - the values of the string and stuff
.EP
.SP 9 5
pattern - the pattern which was matched.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the command succeeded, 0 if it failed

.EP

.SI 3
* %^BOLD%^delete_soul_command%^RESET%^
.EI
.SI 5
void delete_soul_command(string name)
.EI
.SP 7 5

Deletes the soul command.  This is used to remove soul commands
that are no longer used.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the soul command name to delete

.EP

.SI 3
* %^BOLD%^do_force%^RESET%^
.EI
.SI 5
void do_force(mixed * arr)
.EI
.SP 7 5

Attempts to do the force on the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arr - the args used to force

.EP

.SI 3
* %^BOLD%^get_name%^RESET%^
.EI
.SI 5
string get_name(object ob, int use_name, int type)
.EI

.SI 3
* %^BOLD%^help_list%^RESET%^
.EI
.SI 5
string help_list()
.EI
.SP 7 5

The list of soul comands in the look at soul function.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of all the soul commands formated for the screen

.EP

.SI 3
* %^BOLD%^help_string%^RESET%^
.EI
.SI 5
string help_string(string verb)
.EI
.SP 7 5

Returns the help string for the soul ocmmand.  Creates a nice helkp
message for the passed soul command.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the soul command to get help on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the soul command help string

.EP

.SI 3
* %^BOLD%^load_it%^RESET%^
.EI
.SI 5
void load_it()
.EI
.SP 7 5

Loads the previous state of the soul object off the disc.

.EP

.SI 3
* %^BOLD%^query_pcache_size%^RESET%^
.EI
.SI 5
int query_pcache_size()
.EI

.SI 3
* %^BOLD%^query_scache_size%^RESET%^
.EI
.SI 5
int query_scache_size()
.EI

.SI 3
* %^BOLD%^query_soul_command%^RESET%^
.EI
.SI 5
mixed * query_soul_command(string name)
.EI
.SP 7 5

This returns the arrays that are used by the pattern
matcher in the player object. Called from inside add_command interface.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the souul command name to find
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if no command found, otherwise an array of patterns
.EP

.SI 3
* %^BOLD%^query_soul_command_stuff%^RESET%^
.EI
.SI 5
mixed * query_soul_command_stuff(string str)
.EI
.SP 7 5

Returns the data associated with soul command.
Probably not very useful, but useful for debugging.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the soul command to get the data for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the data associated with the soul command

.EP

.SI 3
* %^BOLD%^query_soul_commands%^RESET%^
.EI
.SI 5
string * query_soul_commands()
.EI
.SP 7 5

The name of all the soul commands.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array containing the names of all the soul commands

.EP

.SI 3
* %^BOLD%^save_it%^RESET%^
.EI
.SI 5
void save_it()
.EI
.SP 7 5

Saves the current state of the soul object.

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^query_soul_command_data%^RESET%^
.EI
.SI 5
mixed * query_soul_command_data(string str)
.EI
.SP 7 5

Used internally to get the soul command data.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the soul command to get data for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the soul command data
.EP


