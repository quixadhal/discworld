.DT
club_discuss.c
Disk World autodoc help
club_discuss.c

.SH Description
.SP 5 5

This file is for adding discussion items to clubs.  It should be included
into the main club control room.
.EP
.SP 10 5


Written by Pinkfish

Started April 21st 1999
.EP



.SH See also
.SP 5 5
/obj/handlers/club_handler.c

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h and /include/clubs.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^memo_item%^RESET%^
class memo_item {
string club_name;
string subject;
string description;
string * choices;
int timeout;
int open;
}

.EI
.SP 7 5

This class is used by the memo discussion item addition code.
.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_discuss%^RESET%^
.EI
.SI 5
int do_discuss(int open, string type, string club_name)
.EI
.SP 7 5

The main entry point for all the discussion item types.  Figures out
which discussion item it is and calls the correct method to start the
ball rolling.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^change_position%^RESET%^
.EI
.SI 5
void change_position(string str, int open, string club_name)
.EI
.SP 7 5

Figure out which of the choices the player wishes to change.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name of the position to be changed
.EP

.SI 3
* %^BOLD%^change_position_new_name%^RESET%^
.EI
.SI 5
void change_position_new_name(string str, string position, int open, string club_name)
.EI
.SP 7 5

This method checks to make sure that the entered name to change the
position to is valid.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the new name for the position
.EP
.SP 9 5
position - the position to change
.EP

.SI 3
* %^BOLD%^description_confirm%^RESET%^
.EI
.SI 5
void description_confirm(string confirm, string str, int open, string club_name)
.EI
.SP 7 5

This method confirms the description typed in is correct.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
confirm - the string the used typed to confirm the description
.EP
.SP 9 5
str - the description
.EP

.SI 3
* %^BOLD%^discuss_memo_menu%^RESET%^
.EI
.SI 5
void discuss_memo_menu(class memo_item memo)
.EI
.SP 7 5

This method runs the menu for the memo discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
memo - the memo item to be used in the menu

.EP

.SI 3
* %^BOLD%^discussion_add_name%^RESET%^
.EI
.SI 5
void discussion_add_name(string str, int open, string club_name)
.EI
.SP 7 5

This method checks to make sure that the entered position name is
valid.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the position to check
.EP

.SI 3
* %^BOLD%^discussion_finish_information%^RESET%^
.EI
.SI 5
void discussion_finish_information(string info, mixed * data)
.EI
.SP 7 5

This method is the return from the editing for the information
bit and will now try and confirm this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the confirmation string
.EP
.SP 9 5
data - the passed in data
.EP

.SI 3
* %^BOLD%^do_discuss_add_position%^RESET%^
.EI
.SI 5
int do_discuss_add_position(int open, string club_name)
.EI
.SP 7 5

This is the method used to start the discussion of an add position
discussion item.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_discuss_change_position_name%^RESET%^
.EI
.SI 5
int do_discuss_change_position_name(int open, string club_name)
.EI
.SP 7 5

This is the main entry point for setting up a discussion item to 
change a position name in the club.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_discuss_create_account%^RESET%^
.EI
.SI 5
int do_discuss_create_account(int open, string club_name)
.EI
.SP 7 5

This is the main entry point for setting up a discussion item to 
remove a position from the club.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_discuss_description%^RESET%^
.EI
.SI 5
int do_discuss_description(int open, string club_name)
.EI
.SP 7 5

This is the entry point for setting up a discussion on changing
the description of the club.
.EP

.SI 3
* %^BOLD%^do_discuss_memo%^RESET%^
.EI
.SI 5
int do_discuss_memo(int op, string club_name)
.EI
.SP 7 5

This is the main entry point for setting up a memo discussion type.
A memo is a user run discussion item which has no effect on any
real game mechanics.
.EP

.SI 3
* %^BOLD%^do_discuss_no_confidence%^RESET%^
.EI
.SI 5
int do_discuss_no_confidence(int open, string club_name)
.EI
.SP 7 5

This is the main entry point for the no confidence discussion votes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
open - the open flag
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_discuss_remove_account%^RESET%^
.EI
.SI 5
int do_discuss_remove_account(int open, string club_name)
.EI
.SP 7 5

This is the main entry point for setting up a discussion item to 
remove an account from the club.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_discuss_remove_member%^RESET%^
.EI
.SI 5
int do_discuss_remove_member(int open, string club_name)
.EI
.SP 7 5

This is the main entry point for the function to setup a discussion item
to remove a member from the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
open - the open flag

.EP

.SI 3
* %^BOLD%^do_discuss_remove_position%^RESET%^
.EI
.SI 5
int do_discuss_remove_position(int open, string club_name)
.EI
.SP 7 5

This is the main entry point for setting up a discussion item to 
remove a position from the club.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_discuss_secret%^RESET%^
.EI
.SI 5
int do_discuss_secret(int open, string club_name)
.EI
.SP 7 5

This method is the main entry point to adding a discussion type to make
the club secret.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
open - if the item is being added as an open item

.EP


