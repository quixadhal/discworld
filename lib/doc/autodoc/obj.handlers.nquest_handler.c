.DT
nquest_handler.c
Disk World autodoc help
nquest_handler.c

.SH Description
.SP 5 5

This class keeps track of the current set of quests availabe in the
game.
.EP
.SP 10 5


Written by Furball

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_quest%^RESET%^
.EI
.SI 5
int add_quest(string name, int level, string title, string story)
.EI
.SP 7 5

This method adds a new quest into the system.  You only need to call
this ONCE.  The name of the quest must be unique.  The story is
what can be seen in the players books about the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the quest
.EP
.SP 9 5
level - the level of the quest 0-100
.EP
.SP 9 5
title - the title for finishing the quest, 0 for none
.EP
.SP 9 5
story - the story about the quest
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if not
.EP

.SI 3
* %^BOLD%^change_quest_status%^RESET%^
.EI
.SI 5
int change_quest_status(string name)
.EI
.SP 7 5

Change the status of a quest from active to inactive
and vice versa.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the - name of a quest.

.EP

.SI 3
* %^BOLD%^delete_quest%^RESET%^
.EI
.SI 5
int delete_quest(string name)
.EI
.SP 7 5

This method removes the given quest from the system.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success
.EP

.SI 3
* %^BOLD%^load_quests%^RESET%^
.EI
.SI 5
void load_quests()
.EI
.SP 7 5

This method reloads the quests from the save file.
.EP

.SI 3
* %^BOLD%^print_some_stats%^RESET%^
.EI
.SI 5
void print_some_stats()
.EI
.SP 7 5

This method prints out a lot of quest stats. For debugging purpsoes
only.

.EP

.SI 3
* %^BOLD%^query_fame_str%^RESET%^
.EI
.SI 5
string query_fame_str(string name)
.EI
.SP 7 5

This method returns a string associated with the fame level of the
player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the fame string
.EP

.SI 3
* %^BOLD%^query_player_fame%^RESET%^
.EI
.SI 5
int query_player_fame(string name)
.EI
.SP 7 5

This method returns the fame of the player.
Gets the fame percentage of the player. Quest points / total quest points
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the fame as a percentage (0-100)
.EP

.SI 3
* %^BOLD%^query_player_story%^RESET%^
.EI
.SI 5
string * query_player_story(string name)
.EI
.SP 7 5

This method lists the stories for the quests the player has done.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the story of the player
.EP

.SI 3
* %^BOLD%^query_quest_done%^RESET%^
.EI
.SI 5
mixed query_quest_done(string name)
.EI
.SP 7 5

This method returns the name of the last person to complete the
quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - thre name of the quest
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the last person to complete the quest
.EP

.SI 3
* %^BOLD%^query_quest_level%^RESET%^
.EI
.SI 5
int query_quest_level(string name)
.EI
.SP 7 5

This method returns the level of the quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the quest
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the level of the quest, -1 on failure
.EP

.SI 3
* %^BOLD%^query_quest_levels%^RESET%^
.EI
.SI 5
int * query_quest_levels()
.EI
.SP 7 5

This method returns the levels of all the quests.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the levels of all the quests
.EP

.SI 3
* %^BOLD%^query_quest_names%^RESET%^
.EI
.SI 5
string * query_quest_names()
.EI
.SP 7 5

This method returns the names of all the quests.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the names of all the quests
.EP

.SI 3
* %^BOLD%^query_quest_status%^RESET%^
.EI
.SI 5
int query_quest_status(string name)
.EI
.SP 7 5

This method returns the status of a quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the - name of a quest
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for active, 0 for inactive.

.EP

.SI 3
* %^BOLD%^query_quest_stories%^RESET%^
.EI
.SI 5
string * query_quest_stories()
.EI
.SP 7 5

This method returns the stories of all the quests.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the stories of all the quests
.EP

.SI 3
* %^BOLD%^query_quest_story%^RESET%^
.EI
.SI 5
string query_quest_story(string name)
.EI
.SP 7 5

This method returns the story associated with the quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - of the quest
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the story of the quest
.EP

.SI 3
* %^BOLD%^query_quest_times%^RESET%^
.EI
.SI 5
int query_quest_times(string name)
.EI
.SP 7 5

This method returns the number of times the quest has been
completed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - thre name of the quest
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of times completed
.EP

.SI 3
* %^BOLD%^query_quest_title%^RESET%^
.EI
.SI 5
string query_quest_title(string name)
.EI
.SP 7 5

This method returns the title associated with the quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - of the quest
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the title of the quest
.EP

.SI 3
* %^BOLD%^query_quest_titles%^RESET%^
.EI
.SI 5
string * query_quest_titles()
.EI
.SP 7 5

This method returns the titles of all the quests.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the titles of all the quests
.EP

.SI 3
* %^BOLD%^query_total_qp%^RESET%^
.EI
.SI 5
int query_total_qp()
.EI
.SP 7 5

This returns the current total gp for all of the current quests.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the total gp

.EP

.SI 3
* %^BOLD%^quest_completed%^RESET%^
.EI
.SI 5
void quest_completed(string name, string quest, object prev_ob)
.EI
.SP 7 5

This method should be called in the code when a quest is complete.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - name of the player
.EP
.SP 9 5
quest - the quest completed
.EP
.SP 9 5
prev_ob - the object which completed the quest
.EP

.SI 3
* %^BOLD%^save_quests%^RESET%^
.EI
.SI 5
void save_quests()
.EI
.SP 7 5

This method saves the current set of quests to the save file.
.EP

.SI 3
* %^BOLD%^set_quest_level%^RESET%^
.EI
.SI 5
int set_quest_level(string name, int level)
.EI
.SP 7 5

This method sets the level of the quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the quest
.EP
.SP 9 5
level - the level of the quest
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success
.EP

.SI 3
* %^BOLD%^set_quest_story%^RESET%^
.EI
.SI 5
int set_quest_story(string name, string story)
.EI
.SP 7 5

This method sets the story associated with the quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - of the quest
.EP
.SP 9 5
story - the new story of the quest
.EP

.SI 3
* %^BOLD%^set_quest_title%^RESET%^
.EI
.SI 5
int set_quest_title(string name, string title)
.EI
.SP 7 5

This method sets the title associated with the quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - of the quest
.EP
.SP 9 5
title - the new title of the quest
.EP


