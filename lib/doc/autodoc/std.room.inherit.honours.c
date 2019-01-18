.DT
honours.c
Disk World autodoc help
honours.c

.SH Description
.SP 5 5

This room keeps track of the people who have been given the specified
honoury title and why.  It also makes sure that anyone who logs on
with the title is allowed to have it.


Don't forget to add the room to the title handler as controlling this title.
.EP
.SP 10 5


Written by Pinkfish

Started Wed Oct 17 13:15:03 PDT 2001

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/player.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^honour%^RESET%^
class honour {
int time_added;
string reason;
string cap_name;
string added_by;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_to_honour_roll%^RESET%^
.EI
.SI 5
void add_to_honour_roll(string person, string reason, string added_by)
.EI
.SP 7 5

This method adds a person into the honour role with the specifed
reason.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to honour
.EP
.SP 9 5
reason - the reason they were added
.EP
.SP 9 5
added_by - the person added by

.EP

.SI 3
* %^BOLD%^do_honour_add%^RESET%^
.EI
.SI 5
int do_honour_add(string person)
.EI
.SP 7 5

This allows a person to be added to the honour roll.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - person to add
.EP
.SP 9 5
reason - reason to add them

.EP

.SI 3
* %^BOLD%^do_honour_remove%^RESET%^
.EI
.SI 5
int do_honour_remove(string person, string reason)
.EI
.SP 7 5

This allows a person to be removed from the honour roll.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - person to add
.EP
.SP 9 5
reason - reason to add them

.EP

.SI 3
* %^BOLD%^do_list_honour%^RESET%^
.EI
.SI 5
int do_list_honour()
.EI
.SP 7 5

List everyone on the honour roll.

.EP

.SI 3
* %^BOLD%^end_honour_add%^RESET%^
.EI
.SI 5
void end_honour_add(string reason, string person, string bing)
.EI
.SP 7 5

This is called by the editor when the reason has been set.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
reason - the reason for the addition
.EP
.SP 9 5
persno - the person being added

.EP

.SI 3
* %^BOLD%^honour_roll_confirm%^RESET%^
.EI
.SI 5
void honour_roll_confirm(string response, string person, string reason)
.EI
.SP 7 5

This is called by the system to confirm the addition.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
reason - the reason for the addition
.EP
.SP 9 5
persno - the person being added

.EP

.SI 3
* %^BOLD%^inform_of_honour_change%^RESET%^
.EI
.SI 5
void inform_of_honour_change(string type, string person, string reason, string changed_by)
.EI
.SP 7 5

This method does any informing about the change in the honour
status.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - add/remove
.EP
.SP 9 5
person - the person being added/removed
.EP
.SP 9 5
reason - the reason for the addition/removal
.EP
.SP 9 5
changed_by - the person who did the change

.EP

.SI 3
* %^BOLD%^is_on_honour_roll%^RESET%^
.EI
.SI 5
int is_on_honour_roll(string person)
.EI
.SP 7 5

This method checks to see if the person is on the honour role.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to check

.EP

.SI 3
* %^BOLD%^query_controlled_titles%^RESET%^
.EI
.SI 5
string * query_controlled_titles()
.EI
.SP 7 5

This method returns the controlled title for the inherit.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the controlled title

.EP

.SI 3
* %^BOLD%^readd_all_titles%^RESET%^
.EI
.SI 5
void readd_all_titles()
.EI
.SP 7 5

A method to go through and specifically readd everyone to the list.
This helps if something goes wrong and all the titles get lost.

.EP

.SI 3
* %^BOLD%^remove_from_honour_roll%^RESET%^
.EI
.SI 5
void remove_from_honour_roll(string person)
.EI
.SP 7 5

This removes the person from the honour role.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to remove

.EP

.SI 3
* %^BOLD%^set_controlled_titles%^RESET%^
.EI
.SI 5
void set_controlled_titles(string * title)
.EI
.SP 7 5

This method sets the title we are to control.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the title to control

.EP


