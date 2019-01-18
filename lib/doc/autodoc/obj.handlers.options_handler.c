.DT
options_handler.c
Disk World autodoc help
options_handler.c

.SH Description
.SP 5 5

This handler deals will the options a player has.  It will return the
list of options, set options and control how the options can be
set.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Jun  8 17:13:47 PDT 2000

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/error_handler.h, /include/ls.h, /include/cmds/options.h, /include/cmds/teach.h, /include/terrain_map.h, /include/creator.h, /include/obj_parser.h, /include/player.h, /include/am_time.h, /include/colour.h, /include/clubs.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^option%^RESET%^
class option {
mixed type;
int restriction;
function set;
function query;
mapping suboptions;
string help;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^convert_birthday%^RESET%^
.EI
.SI 5
string convert_birthday(string str)
.EI

.SI 3
* %^BOLD%^is_option%^RESET%^
.EI
.SI 5
int is_option(object player, string name)
.EI
.SP 7 5

This method checks to see if the specified option path exists.  This
will only return true if it is an actual option, not an option group.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player this is relative to
.EP
.SP 9 5
name - the option path to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is an option, 0 if not

.EP

.SI 3
* %^BOLD%^is_option_group%^RESET%^
.EI
.SI 5
int is_option_group(object player, string name)
.EI
.SP 7 5

This method checks to see if the specified option group path exists.  This
will only return true if it is an option group, not an actual option.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player this is relative to
.EP
.SP 9 5
name - the option path to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is an option group, 0 if not

.EP

.SI 3
* %^BOLD%^query_option_help%^RESET%^
.EI
.SI 5
string query_option_help(object player, string path)
.EI
.SP 7 5

This method returns the help string of the specified option.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to find the value on
.EP
.SP 9 5
path - the path to the option

.EP

.SI 3
* %^BOLD%^query_option_value%^RESET%^
.EI
.SI 5
string query_option_value(object player, string path)
.EI
.SP 7 5

This method returns the value of the specified option.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to find the value on
.EP
.SP 9 5
path - the path to the option

.EP

.SI 3
* %^BOLD%^query_option_values%^RESET%^
.EI
.SI 5
string * query_option_values(object player, string name)
.EI
.SP 7 5

This method returns the different values this option can be
set to.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player this is relative to
.EP
.SP 9 5
name - the name of the option
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the allowed parameters

.EP

.SI 3
* %^BOLD%^query_sub_options%^RESET%^
.EI
.SI 5
string * query_sub_options(object player, string name)
.EI
.SP 7 5

This method returns all the sub options and option groups at this
level.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player this is relative to
.EP
.SP 9 5
name - the path to return the suboptions of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the sub options of the path

.EP

.SI 3
* %^BOLD%^set_option_value%^RESET%^
.EI
.SI 5
int set_option_value(object player, string path, string value)
.EI
.SP 7 5

This method sets an option value on the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to find the value on
.EP
.SP 9 5
path - the path to the option
.EP
.SP 9 5
value - the value directly from the command line

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^add_option%^RESET%^
.EI
.SI 5
int add_option(string name, mixed type, int cre_only, function set_function, function query_function, string help)
.EI
.SP 7 5

This method adds in an option for the player to be able to set.
The set function will be called with two parameters, the value to
set it to and the variable being set.  The set function must return
1 if the value was successfuly set.


int set_function(variable, value);
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the path to the option eg: ({ "output", "look" })
.EP
.SP 9 5
type - the type of the option eg: OPTIONS_TYPE_BRIEF
.EP
.SP 9 5
set_function - the function to call to set the option
.EP
.SP 9 5
query_function - the function to call to query the option
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if not

.EP

.SI 3
* %^BOLD%^add_option_to_mapping%^RESET%^
.EI
.SI 5
void add_option_to_mapping(mapping array, string name, mixed type, int cre_only, function set_function, function query_function, string help)
.EI
.SP 7 5

This method adds in an option for the player to be able to set to
a mapping.  This method should be used by all the dynamic tree
methods to create leaves.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
array - the mapping to add the elements to
.EP
.SP 9 5
path - the path to the option eg: ({ "output", "look" })
.EP
.SP 9 5
type - the type of the option eg: OPTIONS_TYPE_BRIEF
.EP
.SP 9 5
set_function - the function to call to set the option
.EP
.SP 9 5
query_function - the function to call to query the option
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if not

.EP


