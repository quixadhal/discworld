.DT
team.c
Disk World autodoc help
team.c

.SH Description
.SP 5 5

This handler deals with grouping players into teams. Teams are entirely
optional.  The handler deals with creating, joining, leaving and ending
teams.  It also provides functions to find out who's a member of which
team etc.

Commands will be available to do group things (group chatting, group health etc.) By default team members will protect each other and follow the leader. 
.EP
.SP 10 5


Written by Ceres

Started June 96

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h and /include/login_handler.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^group%^RESET%^
class group {
object leader;
int policy;
object * members;
int locked;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^end_group%^RESET%^
.EI
.SI 5
int end_group(string g_name)
.EI
.SP 7 5

This method ends a group.  Closes it, finishes it, done, finito.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the group to close
.EP

.SI 3
* %^BOLD%^join_group%^RESET%^
.EI
.SI 5
int join_group(string g_name, object player)
.EI
.SP 7 5

This method joins a player into an existing group.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the group to join
.EP
.SP 9 5
player - the player to join to the group
.EP

.SI 3
* %^BOLD%^leave_group%^RESET%^
.EI
.SI 5
int leave_group(string g_name, object player)
.EI
.SP 7 5

This method removes a player from the group.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the group to leave
.EP
.SP 9 5
player - the player to leave the group
.EP

.SI 3
* %^BOLD%^leaving%^RESET%^
.EI
.SI 5
void leaving(string p_name, string type)
.EI
.SP 7 5

When a player leaves the game automatically remove them from their team.
this function is called automatically by the login handler for every
player who leaves the game
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
p_name - the player name who is leaving
.EP
.SP 9 5
type - the type of action being preformed
.EP

.SI 3
* %^BOLD%^list_groups%^RESET%^
.EI
.SI 5
string * list_groups()
.EI
.SP 7 5

This methor returns the list all the current teams.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of all the current team

.EP

.SI 3
* %^BOLD%^new_group%^RESET%^
.EI
.SI 5
int new_group(string g_name, object leader, int policy)
.EI
.SP 7 5

This method creates a new team.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the group
.EP
.SP 9 5
leader - the leader of the group (object)
.EP
.SP 9 5
policy - the policy of the group
.EP

.SI 3
* %^BOLD%^query_full%^RESET%^
.EI
.SI 5
int query_full(string g_name)
.EI
.SP 7 5

This method checks if its ok to join a group.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the group
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the team is full, 0 if not
.EP

.SI 3
* %^BOLD%^query_group%^RESET%^
.EI
.SI 5
string query_group(object player)
.EI
.SP 7 5

This method finds out which team player is a member of.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to find the team of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the team they are a member of

.EP

.SI 3
* %^BOLD%^query_locked%^RESET%^
.EI
.SI 5
int query_locked(string g_name)
.EI
.SP 7 5

This method determines if the team is locked.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the team to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if locked, 0 if unlocked
.EP

.SI 3
* %^BOLD%^query_members%^RESET%^
.EI
.SI 5
object * query_members(string g_name)
.EI
.SP 7 5

This method returns the members of the team.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the group to get the member of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the members names as strings
.EP

.SI 3
* %^BOLD%^query_owner%^RESET%^
.EI
.SI 5
object query_owner(string g_name)
.EI
.SP 7 5

This method determine the owner (leader)  of a team.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the group to get the owner for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object refering to the owner
.EP

.SI 3
* %^BOLD%^query_policy%^RESET%^
.EI
.SI 5
int query_policy(string g_name)
.EI
.SP 7 5

This method returns the policy of the team
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the team
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the policy of the team
.EP

.SI 3
* %^BOLD%^set_locked%^RESET%^
.EI
.SI 5
int set_locked(string g_name, int lock)
.EI
.SP 7 5

This method sets the lock flag on the team.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the team
.EP
.SP 9 5
lock - the new lock flag
.EP

.SI 3
* %^BOLD%^test_group%^RESET%^
.EI
.SI 5
int test_group(string g_name)
.EI
.SP 7 5

This method determine if a team exists.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
g_name - the name of the team
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the team exists, 0 if not
.EP


