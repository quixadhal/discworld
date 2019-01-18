.DT
owned_room.c
Disk World autodoc help
owned_room.c

.SH Description
.SP 5 5

This is an owned room inherit.  It allows controllable entrance and
things like 'allow' and 'throwout' and stuff.  Make sure the create on
this is called after the create in the basic room.
.EP
.SP 10 5


Written by Pinkfish

Started 30th of June

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/room.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^is_allowed%^RESET%^
.EI
.SI 5
int is_allowed(string person)
.EI
.SP 7 5

This method tests to see if the person in question is either the owner
or is allowed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to check

.EP

.SI 3
* %^BOLD%^is_allowed_in%^RESET%^
.EI
.SI 5
int is_allowed_in(string person)
.EI
.SP 7 5

This method checks to see if they are allowed in.  Allow anyone in
if the room is not locked.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are allowed in, 0 if not

.EP

.SI 3
* %^BOLD%^load_room%^RESET%^
.EI
.SI 5
void load_room()
.EI
.SP 7 5

This method loads the room setup.

.EP

.SI 3
* %^BOLD%^query_allowed%^RESET%^
.EI
.SI 5
string * query_allowed()
.EI
.SP 7 5

This method returns the current allowed list on the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current allowed array

.EP

.SI 3
* %^BOLD%^query_front_door%^RESET%^
.EI
.SI 5
string query_front_door()
.EI
.SP 7 5

This method queries the current front door.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current front door

.EP

.SI 3
* %^BOLD%^query_owner%^RESET%^
.EI
.SI 5
string query_owner()
.EI
.SP 7 5

This method returns the owner of the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the owner of the room

.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This method returns the save file the owned room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save file for the owned room

.EP

.SI 3
* %^BOLD%^save_room%^RESET%^
.EI
.SI 5
void save_room()
.EI
.SP 7 5

This method saves the room setup.

.EP

.SI 3
* %^BOLD%^set_front_door%^RESET%^
.EI
.SI 5
void set_front_door(string exit)
.EI
.SP 7 5

This method sets the exit that is the front door.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
exit - the exit to use as the front door

.EP

.SI 3
* %^BOLD%^set_owner%^RESET%^
.EI
.SI 5
void set_owner(string owner)
.EI
.SP 7 5

This method sets the owner of the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
owner - the new owner of the room

.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string name)
.EI
.SP 7 5

This method sets the save file for the owned room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
save_file - the name of the save file

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^do_allow%^RESET%^
.EI
.SI 5
int do_allow(string name)
.EI
.SP 7 5

This the main entry point for the allow command.
.EP

.SI 3
* %^BOLD%^do_disallow%^RESET%^
.EI
.SI 5
int do_disallow(string name)
.EI
.SP 7 5

This the main entry point for the disallow command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure

.EP

.SI 3
* %^BOLD%^do_throwout%^RESET%^
.EI
.SI 5
int do_throwout(object * people)
.EI
.SP 7 5

This is the main entry point for the throwout command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the person to throwout
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_allowed%^RESET%^
.EI
.SI 5
void set_allowed(string * allowed)
.EI
.SP 7 5

This method sets the allowed array for the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
allowed - the new allowed array.

.EP


