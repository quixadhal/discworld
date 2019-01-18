.DT
scripting.c
Disk World autodoc help
scripting.c

.SH Description
.SP 5 5

This is the standard inheritable to enable players to script npcs.


All you need to do is inherit this file, make sure you call the ::init, and make sure you have a few functions defined in your context. 

If you are not using a player shop inherit then you must ensure that is_allowed( name ) returns 1 if the player is allowed to edit scripts in this object. 

You will need to write your own find_npc() function, since this is extremely sensitive to the implementation. For reference check out the one which will be written for player controlled shops. 

It is assumed that a save object will be inherited, if not then event_save() will need to be defined. 
.EP
.SP 10 5


Written by Nofear

.EP



.SH See also
.SP 5 5
trigger_thingy(), return_script()

.EP
.SH Example
.SI 5
inherit "/std/room/scripting";
inherit "/std/room";

void init(){
   scripting::init();
   ..
}

void setup(){
   ..
   add_help_file( DOCS +"scripting" );
}
.EI
.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/expressions.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nroff.h and /include/expressions.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^running_script%^RESET%^
class running_script {
class script script;
int position;
object who;
string who_name;
string who_short;
int run_priority;
}

.EI
.SP 7 5

This class contains each element of the script itself.
.EP

.SI 3
* %^BOLD%^script%^RESET%^
class script {
string * valid_actors;
class script_data * data;
int priority;
}

.EI
.SP 7 5

This class contains each element of the script itself.
.EP

.SI 3
* %^BOLD%^script_data%^RESET%^
class script_data {
int type;
string actor;
string str;
class parse_node * expr;
int number;
}

.EI
.SP 7 5

This class contains each element of the script itself.
.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^delete_script_data_location%^RESET%^
.EI
.SI 5
int delete_script_data_location(string name, int pos)
.EI
.SP 7 5

This method deletes the script data at the specified location.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the script name
.EP
.SP 9 5
pos - the position to replace
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^insert_script_data_before%^RESET%^
.EI
.SI 5
int insert_script_data_before(string name, int pos, class script_data data)
.EI
.SP 7 5

This method inserts a data element before the specified position in the
script.  If the position is 0 then the element is inserted at the start,
if the position is beyond the end of the array, then the item is inserter
at the end.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the script name
.EP
.SP 9 5
pos - the position to insert before
.EP
.SP 9 5
data - the data to replace it with

.EP

.SI 3
* %^BOLD%^query_script%^RESET%^
.EI
.SI 5
class script query_script(string name)
.EI
.SP 7 5

This returns the script associated with the event.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - The name of the script.
.EP

.SI 3
* %^BOLD%^query_script_string%^RESET%^
.EI
.SI 5
string query_script_string(class scripting * script)
.EI
.SP 7 5

This returns a printable string of the script.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
 - script the script to turn into a string
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string version of the script

.EP

.SI 3
* %^BOLD%^repeat_loop%^RESET%^
.EI
.SI 5
void repeat_loop()
.EI
.SP 7 5

This function is called by trigger_thingy and by itself.


You could mask this if you need to restrict what the npcs are allowed to say. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
this_script - the script contents,
.EP
.SP 9 5
num - the line number we are up to
.EP
.SP 9 5
who - the person we are looking at

.EP

.SI 3
* %^BOLD%^set_script%^RESET%^
.EI
.SI 5
void set_script(string name, class scripting * script)
.EI
.SP 7 5

This sets the specific script to the new value.

.EP

.SI 3
* %^BOLD%^set_script_actors%^RESET%^
.EI
.SI 5
void set_script_actors(string name, string * actors)
.EI
.SP 7 5

This method sets the actors associated with the script.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the event to set the data for
.EP
.SP 9 5
actors - the actors for the script

.EP

.SI 3
* %^BOLD%^set_script_data%^RESET%^
.EI
.SI 5
void set_script_data(string name, class script_data * data)
.EI
.SP 7 5

This method sets the script data.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the event to set the data for
.EP
.SP 9 5
data - the script data

.EP

.SI 3
* %^BOLD%^set_script_data_location%^RESET%^
.EI
.SI 5
int set_script_data_location(string name, int pos, class script_data data)
.EI
.SP 7 5

This method replaces the script data at the specified location in
the script with a new item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the script name
.EP
.SP 9 5
pos - the position to replace
.EP
.SP 9 5
data - the data to replace it with
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^trigger_script%^RESET%^
.EI
.SI 5
void trigger_script(string event, object who, int override)
.EI
.SP 7 5

This function is called to activate a script in this object.
( Can be left blank, best to use file_name( this_player() ) ).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
event - the name of the event to be triggered,
.EP
.SP 9 5
who - a reference to the object who triggered the event
.EP
.SP 9 5
override - the override for the priority

.EP


