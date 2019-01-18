.DT
friends.c
Disk World autodoc help
friends.c

.SH Description
.SP 5 5

A nice clump of friends to take you out and buy you drinks.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Mar 12 22:21:23 PST 2001

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/cmds/friends.h, /include/player.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_friend%^RESET%^
.EI
.SI 5
void add_friend(string person, string tag)
.EI
.SP 7 5

This method adds a friend to the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the friend to add
.EP
.SP 9 5
tag - the tag to add

.EP

.SI 3
* %^BOLD%^is_friend%^RESET%^
.EI
.SI 5
int is_friend(string person)
.EI
.SP 7 5

This method checks to see if the specified person is a friend or not.
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
1 if they are a friend, 0 if not

.EP

.SI 3
* %^BOLD%^query_friend_tag%^RESET%^
.EI
.SI 5
string query_friend_tag(string person)
.EI
.SP 7 5

This method returns the tag associated with the friend.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to get the information on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string of their friendship tag

.EP

.SI 3
* %^BOLD%^query_friends%^RESET%^
.EI
.SI 5
string * query_friends()
.EI
.SP 7 5

This method returns the current bunch of friends for this player.

.EP

.SI 3
* %^BOLD%^remove_friend%^RESET%^
.EI
.SI 5
void remove_friend(string person)
.EI
.SP 7 5

This method removes a friend from the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to remove

.EP


