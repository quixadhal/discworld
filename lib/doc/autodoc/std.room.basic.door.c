.DT
door.c
Disk World autodoc help
door.c

.SH Description
.SP 5 5

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/id_match, /std/basic/enchant, /std/basic/close_lock, /std/basic/property, /std/basic/trap and /std/basic/hide_invis.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/parse_command.h, /include/dirs.h, /include/room.h and /include/door.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^a_short%^RESET%^
.EI
.SI 5
string a_short()
.EI

.SI 3
* %^BOLD%^add_alias%^RESET%^
.EI
.SI 5
void add_alias(string word)
.EI

.SI 3
* %^BOLD%^do_close%^RESET%^
.EI
.SI 5
int do_close()
.EI

.SI 3
* %^BOLD%^do_knock%^RESET%^
.EI
.SI 5
int do_knock()
.EI

.SI 3
* %^BOLD%^do_lock%^RESET%^
.EI
.SI 5
int do_lock(object * obs)
.EI

.SI 3
* %^BOLD%^do_open%^RESET%^
.EI
.SI 5
int do_open()
.EI

.SI 3
* %^BOLD%^do_unlock%^RESET%^
.EI
.SI 5
int do_unlock(object * obs)
.EI

.SI 3
* %^BOLD%^drop%^RESET%^
.EI
.SI 5
int drop()
.EI

.SI 3
* %^BOLD%^dwep%^RESET%^
.EI
.SI 5
void dwep()
.EI

.SI 3
* %^BOLD%^get%^RESET%^
.EI
.SI 5
int get()
.EI

.SI 3
* %^BOLD%^go_away%^RESET%^
.EI
.SI 5
void go_away()
.EI

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long()
.EI

.SI 3
* %^BOLD%^long_status%^RESET%^
.EI
.SI 5
string long_status()
.EI

.SI 3
* %^BOLD%^move%^RESET%^
.EI
.SI 5
int move()
.EI

.SI 3
* %^BOLD%^moving_close%^RESET%^
.EI
.SI 5
int moving_close(object thing)
.EI

.SI 3
* %^BOLD%^moving_lock%^RESET%^
.EI
.SI 5
int moving_lock(object thing)
.EI

.SI 3
* %^BOLD%^moving_open%^RESET%^
.EI
.SI 5
int moving_open(object thing)
.EI

.SI 3
* %^BOLD%^moving_unlock%^RESET%^
.EI
.SI 5
int moving_unlock(object thing)
.EI

.SI 3
* %^BOLD%^one_short%^RESET%^
.EI
.SI 5
string one_short()
.EI

.SI 3
* %^BOLD%^parse_command_adjectiv_id_list%^RESET%^
.EI
.SI 5
string * parse_command_adjectiv_id_list()
.EI

.SI 3
* %^BOLD%^parse_command_id_list%^RESET%^
.EI
.SI 5
string * parse_command_id_list()
.EI

.SI 3
* %^BOLD%^parse_command_plural_id_list%^RESET%^
.EI
.SI 5
string * parse_command_plural_id_list()
.EI

.SI 3
* %^BOLD%^parse_match_object%^RESET%^
.EI
.SI 5
mixed parse_match_object(string * input, object viewer, class obj_match_context context)
.EI

.SI 3
* %^BOLD%^pick_lock%^RESET%^
.EI
.SI 5
int pick_lock(object thing)
.EI

.SI 3
* %^BOLD%^pick_unlock%^RESET%^
.EI
.SI 5
int pick_unlock(object thing)
.EI

.SI 3
* %^BOLD%^poss_short%^RESET%^
.EI
.SI 5
string poss_short()
.EI

.SI 3
* %^BOLD%^pretty_plural%^RESET%^
.EI
.SI 5
string pretty_plural()
.EI

.SI 3
* %^BOLD%^pretty_short%^RESET%^
.EI
.SI 5
string pretty_short()
.EI

.SI 3
* %^BOLD%^query_adjs%^RESET%^
.EI
.SI 5
string * query_adjs()
.EI

.SI 3
* %^BOLD%^query_dest%^RESET%^
.EI
.SI 5
string query_dest()
.EI

.SI 3
* %^BOLD%^query_determinate%^RESET%^
.EI
.SI 5
string query_determinate()
.EI

.SI 3
* %^BOLD%^query_door_name%^RESET%^
.EI
.SI 5
string query_door_name()
.EI
.SP 7 5

This returns the name of the door.  Note that this is different to the
actual 'name' of the door.  It is normally an exciting and original
value of 'door'. 
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string representing the door name.  

.EP

.SI 3
* %^BOLD%^query_how_many%^RESET%^
.EI
.SI 5
int query_how_many()
.EI
.SP 7 5

This method returns how many doors are stored in this particular
door object.  This is used for double doors and things like that
to make the plurals work correctly.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an integer value representing the number of doors.
.EP

.SI 3
* %^BOLD%^query_lock_owner%^RESET%^
.EI
.SI 5
mixed query_lock_owner()
.EI
.SP 7 5

This method is used to determine which player owns the lock
on this door.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string containing the name of the player who owns the
lock.
.EP

.SI 3
* %^BOLD%^query_long%^RESET%^
.EI
.SI 5
string query_long()
.EI

.SI 3
* %^BOLD%^query_my_id%^RESET%^
.EI
.SI 5
string query_my_id()
.EI

.SI 3
* %^BOLD%^query_my_room%^RESET%^
.EI
.SI 5
object query_my_room()
.EI

.SI 3
* %^BOLD%^query_name%^RESET%^
.EI
.SI 5
string * query_name()
.EI

.SI 3
* %^BOLD%^query_one_way%^RESET%^
.EI
.SI 5
int query_one_way()
.EI
.SP 7 5

This method is used to determine if a door has been flagged as being
'one way' or not. 
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an integer value which returns 1 if the door is one way,
0 if it is not. 
.EP

.SI 3
* %^BOLD%^query_other_id%^RESET%^
.EI
.SI 5
string query_other_id()
.EI

.SI 3
* %^BOLD%^query_parse_id%^RESET%^
.EI
.SI 5
object query_parse_id(mixed * arr)
.EI

.SI 3
* %^BOLD%^query_plu%^RESET%^
.EI
.SI 5
string * query_plu()
.EI

.SI 3
* %^BOLD%^query_plural%^RESET%^
.EI
.SI 5
string query_plural()
.EI

.SI 3
* %^BOLD%^query_short%^RESET%^
.EI
.SI 5
string query_short()
.EI
.SP 7 5

This method returns the short of the door.  This is normally a string
like "east door".
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string containing the short for the door.
.EP

.SI 3
* %^BOLD%^set_dest%^RESET%^
.EI
.SI 5
void set_dest(string word)
.EI

.SI 3
* %^BOLD%^set_door_name%^RESET%^
.EI
.SI 5
void set_door_name(string word)
.EI
.SP 7 5

This method sets the name of the door.  Note that this is different
to the actual visible name or short of the door.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new door name
.EP

.SI 3
* %^BOLD%^set_how_many%^RESET%^
.EI
.SI 5
void set_how_many(int number)
.EI
.SP 7 5

This method sets how many doors there are so that plurals
and everything are set correctly.  It also flags the door
with the "group object" property.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the number of doors this door should represent.
.EP

.SI 3
* %^BOLD%^set_lock_owner%^RESET%^
.EI
.SI 5
void set_lock_owner(string owner)
.EI
.SP 7 5

This method sets the owner of the lock.   This value is used in
pick_lock() to determine whether or not various PK checks etc
are passed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
owner - the name of the player who owns the lock.

.EP

.SI 3
* %^BOLD%^set_long%^RESET%^
.EI
.SI 5
void set_long(string word)
.EI

.SI 3
* %^BOLD%^set_my_id%^RESET%^
.EI
.SI 5
void set_my_id(string word)
.EI

.SI 3
* %^BOLD%^set_one_way%^RESET%^
.EI
.SI 5
void set_one_way(int number)
.EI
.SP 7 5

This method is used to set whether or not the door is one way.
A one way door won't trigger a "broken door" message when there's
no exit leading back.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - pass 1 if the door is one way, or 0 if it isn't.

.EP

.SI 3
* %^BOLD%^set_other_id%^RESET%^
.EI
.SI 5
void set_other_id(string word)
.EI

.SI 3
* %^BOLD%^set_short%^RESET%^
.EI
.SI 5
void set_short(string words)
.EI
.SP 7 5

This method sets the short for the door.  As well as setting the short,
it calculates the name, adjectives and plurals for the door.  
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the short to set for the door object. 
.EP

.SI 3
* %^BOLD%^setup_door%^RESET%^
.EI
.SI 5
void setup_door(string word, object mine, string his, mixed * args, string type)
.EI
.SP 7 5

This method is called by /std/room/basic_room.c in query_door()
to create the door. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the door being created
.EP
.SP 9 5
mine - the object that owns the door
.EP
.SP 9 5
his - the destination for the door
.EP
.SP 9 5
args - dest_other information from door.  This is mainly
the modify_exit() parameters.
.EP
.SP 9 5
type - the type of the door, eg, door, window, etc. 

.EP

.SI 3
* %^BOLD%^short%^RESET%^
.EI
.SI 5
string short()
.EI

.SI 3
* %^BOLD%^tell_door%^RESET%^
.EI
.SI 5
void tell_door(string words, object thing)
.EI

.SI 3
* %^BOLD%^the_short%^RESET%^
.EI
.SI 5
string the_short()
.EI


