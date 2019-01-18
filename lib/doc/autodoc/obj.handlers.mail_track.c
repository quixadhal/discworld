.DT
mail_track.c
Disk World autodoc help
mail_track.c

.SH Description
.SP 5 5

This handler keeps track of who has a mailer at the moment and sends the
mail to there instead.  This also keeps track of the current mailing
lists in the system.
.EP
.SP 10 5


Written by Pinkfish

Started 1991 sometime
.EP

.SH Inherits
.SP 5 5
This class inherits the following classes std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/mail_track.h, /include/player_handler.h and /include/mail.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_controller%^RESET%^
.EI
.SI 5
int add_controller(string list_name, string mem)
.EI
.SP 7 5

This method adds a controller to the specified mailing list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list_name - the controller to add
.EP
.SP 9 5
mem - the controller to add
.EP

.SI 3
* %^BOLD%^add_mailer%^RESET%^
.EI
.SI 5
int add_mailer(object ob, string str)
.EI
.SP 7 5

This method adds in a mailer for a specific player into the current
list of mailers.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the mailer for the player
.EP
.SP 9 5
str - the player the mailer is for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
return 1 if successful

.EP

.SI 3
* %^BOLD%^add_member%^RESET%^
.EI
.SI 5
int add_member(string list_name, string mem)
.EI
.SP 7 5

This method adds a member to the specified mailing list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list_name - the mailing list to add a member too
.EP
.SP 9 5
mem - the member to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if not
.EP

.SI 3
* %^BOLD%^create_list%^RESET%^
.EI
.SI 5
int create_list(string list, string creator)
.EI
.SP 7 5

This method attempts to create a mailing list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list - the name of the list to create
.EP
.SP 9 5
creator - the creator of the list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if not
.EP

.SI 3
* %^BOLD%^delete_mailer%^RESET%^
.EI
.SI 5
int delete_mailer(object ob)
.EI
.SP 7 5

This method removes a mailer from the current list of mailers.
It removes the mailer based on the object itself
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the mailer object to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if unable to remove
.EP

.SI 3
* %^BOLD%^delete_member%^RESET%^
.EI
.SI 5
int delete_member(string list_name, string mem)
.EI
.SP 7 5

This method removes a member from the list if they are either a
controller or a member.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list_name - the list to remove the member from
.EP
.SP 9 5
mem - the member to remove from the list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure
.EP

.SI 3
* %^BOLD%^find_mailer%^RESET%^
.EI
.SI 5
object find_mailer(string str)
.EI
.SP 7 5

This method finds the mailer associated with a particular player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the player whose mailer to find.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mailer object associated with the player
.EP

.SI 3
* %^BOLD%^mail%^RESET%^
.EI
.SI 5
int mail(string str, string sub)
.EI
.SP 7 5

This is the main mail function.  It tries to start up a mailer with
the default address and subject.  If their is no default address then
the mailer uses it's menu mode, otherwise it sends one mail and then
leaves.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - 0 or "" for no default address
.EP
.SP 9 5
sub - the default subject

.EP

.SI 3
* %^BOLD%^query_controller%^RESET%^
.EI
.SI 5
int query_controller(string list_name, string name)
.EI
.SP 7 5

This method checks to see if the given player is a controller.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list_name - the name of the list
.EP
.SP 9 5
name - the name to check to see if they are a controller
.EP

.SI 3
* %^BOLD%^query_controllers%^RESET%^
.EI
.SI 5
string * query_controllers(string list)
.EI
.SP 7 5

This method returns all the current controllers of the mailing list.
Controllers can add and remove members from a list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list - the mailing list to query the controllers for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of controllers

.EP

.SI 3
* %^BOLD%^query_creator%^RESET%^
.EI
.SI 5
string query_creator(string list_name)
.EI
.SP 7 5

This method determines the creator of the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list_name - the list to get the creator of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the creator of the list
.EP

.SI 3
* %^BOLD%^query_list%^RESET%^
.EI
.SI 5
int query_list(string list)
.EI
.SP 7 5

This method checks to see if the specified list exists or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list - the list name to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the list exists, 0 if not
.EP

.SI 3
* %^BOLD%^query_mailing_lists%^RESET%^
.EI
.SI 5
string * query_mailing_lists()
.EI
.SP 7 5

This method returns the current mailing lists available on the
handler.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of available mailing lists

.EP

.SI 3
* %^BOLD%^query_member%^RESET%^
.EI
.SI 5
int query_member(string list_name, string name)
.EI
.SP 7 5

This method checks to see if the given player is in the selected
mailing list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list_name - the mailing list name
.EP
.SP 9 5
name - the name of the player in the list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the player is a member, 0 if not
.EP

.SI 3
* %^BOLD%^query_members%^RESET%^
.EI
.SI 5
string * query_members(string list)
.EI
.SP 7 5

This returns the current members of the specified mailing list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list - the mailing list to get the memebrs of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the members of the mailing list
.EP

.SI 3
* %^BOLD%^remove_controller%^RESET%^
.EI
.SI 5
int remove_controller(string list_name, string name)
.EI
.SP 7 5

This method removes a controller from the specified mailing list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list_name - the mailing list to remove the member from
.EP
.SP 9 5
name - the member to remove
.EP

.SI 3
* %^BOLD%^remove_member%^RESET%^
.EI
.SI 5
int remove_member(string list_name, string mem)
.EI
.SP 7 5

This method removes a member from the specified mailing list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list_name - the mailing list to remove the member from
.EP
.SP 9 5
mem - the members name to remove
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
* %^BOLD%^delete_list%^RESET%^
.EI
.SI 5
int delete_list(string list)
.EI
.SP 7 5

This method will delete a mailing list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
list - the mailing list to delete
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if failed and 1 on success

.EP


