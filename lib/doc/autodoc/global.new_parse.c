.DT
new_parse.c
Disk World autodoc help
new_parse.c

.SH Description
.SP 5 5

This file contains all the code to support and run the text parsing
system used by discworld.   This is called 'add_command', please see
help on add_command for a more detailed listing.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/command.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/obj_parser.h, /include/command.h, /include/user_parser.h, /include/soul.h, /include/function.h and /include/creator.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^fail_mess_data%^RESET%^
class fail_mess_data {
object * direct;
object * indirect;
int weight;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_command%^RESET%^
.EI
.SI 5
varargs int add_command(string cmd, object ob, mixed format, function funct)
.EI
.SP 7 5

The id is a useful thingy so that things can remember which pattern was 
parsed.

.EP

.SI 3
* %^BOLD%^add_failed_mess%^RESET%^
.EI
.SI 5
void add_failed_mess(object dir, string mess, mixed * in_dir)
.EI

.SI 3
* %^BOLD%^add_succeeded%^RESET%^
.EI
.SI 5
int add_succeeded(mixed ob)
.EI
.SP 7 5

This is called by the object the command is being passed on to find
whether or not it succeeded on the objects it was passed... and which
ones. This can be passed an object.. or an array of objects.
Share and enjoy.

.EP

.SI 3
* %^BOLD%^add_succeeded_mess%^RESET%^
.EI
.SI 5
int add_succeeded_mess(object dir, mixed incoming_mess, object * in_dir)
.EI

.SI 3
* %^BOLD%^check_if_allowed%^RESET%^
.EI
.SI 5
int check_if_allowed(object ob)
.EI

.SI 3
* %^BOLD%^check_if_creator%^RESET%^
.EI
.SI 5
int check_if_creator(object ob)
.EI

.SI 3
* %^BOLD%^check_living%^RESET%^
.EI
.SI 5
int check_living(object ob)
.EI

.SI 3
* %^BOLD%^create_message%^RESET%^
.EI
.SI 5
varargs string create_message(string * bits, int * matches, mixed * pattern, object * dir, int flag)
.EI

.SI 3
* %^BOLD%^get_fail_messages%^RESET%^
.EI
.SI 5
string get_fail_messages(string verb, object * fail_obs)
.EI

.SI 3
* %^BOLD%^my_find_match%^RESET%^
.EI
.SI 5
class obj_match my_find_match(string pattern, object * where, int type)
.EI

.SI 3
* %^BOLD%^new_parser%^RESET%^
.EI
.SI 5
nomask int new_parser(string str)
.EI
.SP 7 5

This method does all the real work for add_command parsing.

.EP

.SI 3
* %^BOLD%^pattern_match%^RESET%^
.EI
.SI 5
int * pattern_match(string * bits, mixed * pattern)
.EI

.SI 3
* %^BOLD%^print_special_messages%^RESET%^
.EI
.SI 5
void print_special_messages(string verb)
.EI

.SI 3
* %^BOLD%^query_failed_message_exists%^RESET%^
.EI
.SI 5
int query_failed_message_exists(object dir)
.EI
.SP 7 5

This method checks to see if the given object has already added a failed
message yet or not.   This is checking for a direct object, not an 
indirect object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the object adding the failed message
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if not found, 1 if found
.EP

.SI 3
* %^BOLD%^query_p_commands%^RESET%^
.EI
.SI 5
mapping query_p_commands()
.EI
.SP 7 5

This method returns the current internal set of commands.


([ "command_name" :
.EP
.SP 7 5
({ ({ pattern_weight, pattern_str, nn, object, function }) })
.EP
.SP 7 5
])
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current commands list
.EP

.SI 3
* %^BOLD%^query_p_objects%^RESET%^
.EI
.SI 5
mapping query_p_objects()
.EI
.SP 7 5

This method returns the current mapping between objects and commands.
.EP
.SP 7 5
([ object : ({ "cmd1", "cmd2", ... }), ... ]) 

This mapping is used when the object leaves the environment to make the command updating more efficent.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current object/command mapping
.EP

.SI 3
* %^BOLD%^query_parse_command%^RESET%^
.EI
.SI 5
mixed * query_parse_command(string name)
.EI
.SP 7 5

This method returns the information associated with the specific
command.  This should only be used for debug.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the command name to return info on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the information associated with the command

.EP

.SI 3
* %^BOLD%^query_parse_command_objects%^RESET%^
.EI
.SI 5
object * query_parse_command_objects(string name)
.EI
.SP 7 5

This method returns the objects associated with the parse command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the command name to return the objects for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the objects associated with the command

.EP

.SI 3
* %^BOLD%^query_succ_mess_dir%^RESET%^
.EI
.SI 5
object * query_succ_mess_dir()
.EI
.SP 7 5

This method returns the objects which have success messages already
attached for.   This allows you to determine which objects already hace
a success message available.   This array is added to by both the
add_succeeded_mess and add_failed_mess methods, it disable the
autogeneration of these messages.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the succeeded message objects
.EP

.SI 3
* %^BOLD%^query_succ_mess_indir%^RESET%^
.EI
.SI 5
object * query_succ_mess_indir()
.EI
.SP 7 5

This method returns all the indirect objects used in the success
messages.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the success message

.EP

.SI 3
* %^BOLD%^query_word_list%^RESET%^
.EI
.SI 5
string * query_word_list(string list)
.EI

.SI 3
* %^BOLD%^remove_object%^RESET%^
.EI
.SI 5
int remove_object(mixed ob, int was_env)
.EI

.SI 3
* %^BOLD%^setup_failed_mess%^RESET%^
.EI
.SI 5
void setup_failed_mess(class obj_match failed_match)
.EI

.SI 3
* %^BOLD%^syntax_messages%^RESET%^
.EI
.SI 5
int syntax_messages(string str)
.EI


