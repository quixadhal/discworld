.DT
family.c
Disk World autodoc help
family.c

.SH Description
.SP 5 5

This class contains all the stuff needed to control families in a
player.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Oct 30 07:53:30 PST 1998

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/am_time.h, /include/clubs.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^family_information%^RESET%^
class family_information {
string name;
string * adjectives;
string id;
string * titles;
string cur_title;
string * clubs;
string force_title;
int force_timeout;
}

.EI
.SP 7 5

The class for family information in the family stuff for the player.
This is used internaly to the player object and should not be used
elsewhere.
.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_player_club%^RESET%^
.EI
.SI 5
int add_player_club(string club)
.EI
.SP 7 5

This method adds a club to current list of known clubs on the
family.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the club to add

.EP

.SI 3
* %^BOLD%^add_player_title%^RESET%^
.EI
.SI 5
int add_player_title(string title)
.EI
.SP 7 5

This method adds the players title into the list of allowed titles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the title to add into the list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the title was added successfuly, 0 if it exists
.EP

.SI 3
* %^BOLD%^check_family_name%^RESET%^
.EI
.SI 5
void check_family_name()
.EI
.SP 7 5

This method checks to make sure the players current family name
is still valid.  This will be called on startup and at various
other times.
.EP

.SI 3
* %^BOLD%^query_all_player_titles%^RESET%^
.EI
.SI 5
string * query_all_player_titles()
.EI
.SP 7 5

This method returns all the player titles as a nice list.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the player titles

.EP

.SI 3
* %^BOLD%^query_family_name%^RESET%^
.EI
.SI 5
string query_family_name()
.EI
.SP 7 5

This method queries the current family name of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the family, 0 if none set
.EP

.SI 3
* %^BOLD%^query_forced_title%^RESET%^
.EI
.SI 5
string query_forced_title()
.EI
.SP 7 5

This method returns the title that has been forced upon you.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the forced title

.EP

.SI 3
* %^BOLD%^query_player_clubs%^RESET%^
.EI
.SI 5
string * query_player_clubs()
.EI
.SP 7 5

This method returns the current know clubs the player is a meber of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of known clubs

.EP

.SI 3
* %^BOLD%^query_player_title%^RESET%^
.EI
.SI 5
string query_player_title()
.EI
.SP 7 5

This method returns the current title of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current title of the player, 0 if none set
.EP

.SI 3
* %^BOLD%^remove_player_club%^RESET%^
.EI
.SI 5
void remove_player_club(string club)
.EI
.SP 7 5

This method removes a club from the list of know clubs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the club to remove

.EP

.SI 3
* %^BOLD%^remove_player_title%^RESET%^
.EI
.SI 5
void remove_player_title(string title)
.EI
.SP 7 5

This method removes the players title from the list of allowed titles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the title to remove from the list
.EP

.SI 3
* %^BOLD%^set_family_name%^RESET%^
.EI
.SI 5
int set_family_name(string name)
.EI
.SP 7 5

This method sets the current family name of the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the new family name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successfuly set

.EP

.SI 3
* %^BOLD%^set_forced_title%^RESET%^
.EI
.SI 5
int set_forced_title(string title, int timeout)
.EI
.SP 7 5

This method sets the forced title for the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
force_title - the new title
.EP
.SP 9 5
timeout - the length of time for it to timeout
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_player_title%^RESET%^
.EI
.SI 5
int set_player_title(string title)
.EI
.SP 7 5

This method sets the current title of the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the new title to set
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 on failure
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^setup_default_titles%^RESET%^
.EI
.SI 5
void setup_default_titles()
.EI
.SP 7 5

This is used by the refresh code.

.EP


