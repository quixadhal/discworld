.DT
library.c
Disk World autodoc help
library.c

.SH Description
.SP 5 5

The library is a sort of information center.
Currently it supports only the recording on stories about players and
recording the quests that they have done.
.EP
.SP 10 5


Written by Furball
.EP



.SH See also
.SP 5 5
/include/library.h

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h and /include/quest_handler.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^qinfo%^RESET%^
class qinfo {
int cached;
string player_name;
string * quests;
int * times;
string text_file;
mapping player_quest_info;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^clean_cache%^RESET%^
.EI
.SI 5
void clean_cache()
.EI

.SI 3
* %^BOLD%^flush_cache%^RESET%^
.EI
.SI 5
int flush_cache(string name)
.EI

.SI 3
* %^BOLD%^get_most_recent_quest%^RESET%^
.EI
.SI 5
string get_most_recent_quest(string name)
.EI
.SP 7 5

This method returns the most recently completed quest by the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the most recently completed quest

.EP

.SI 3
* %^BOLD%^get_most_recent_time%^RESET%^
.EI
.SI 5
int get_most_recent_time(string name)
.EI
.SP 7 5

This method get sthe most recent time a quest was complete by the
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
the time of the most recently completed quest

.EP

.SI 3
* %^BOLD%^query_all_player_quest_info%^RESET%^
.EI
.SI 5
mapping query_all_player_quest_info(string pl_name)
.EI
.SP 7 5

This method returns all the quest information for a player.
This information is used for quests which have several parts to them
and information needs to be stored about the player as they attempt
to complete it.  The keys of the mapping are the quest names and the
values are the information associated with the quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping containing all the quest info

.EP

.SI 3
* %^BOLD%^query_player_quest_info%^RESET%^
.EI
.SI 5
mixed * query_player_quest_info(string pl_name, string qu_info)
.EI
.SP 7 5

This method returns the quest info for a specific quest.
This information is used for quests which have several parts to them
and information needs to be stored about the player as they attempt
to complete it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name - the player name
.EP
.SP 9 5
qu_info - the quest name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the information associated with the quest

.EP

.SI 3
* %^BOLD%^query_quest_done%^RESET%^
.EI
.SI 5
int query_quest_done(string player, string quest)
.EI
.SP 7 5

This method tells us if the player has completed the quest.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the name of the player
.EP
.SP 9 5
quest - the quest name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the quest has been done, 0 if it has not

.EP

.SI 3
* %^BOLD%^query_quest_points%^RESET%^
.EI
.SI 5
int query_quest_points(string name)
.EI
.SP 7 5

This method returns the current number of quest points gathered
by the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current number of quest pointds
.EP

.SI 3
* %^BOLD%^query_quest_time%^RESET%^
.EI
.SI 5
int query_quest_time(string name, string qu_name)
.EI
.SP 7 5

This method returns the time at which a quest is completed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player to get the time for
.EP
.SP 9 5
qu_name - the name of the quest
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time at which it is completed

.EP

.SI 3
* %^BOLD%^query_quests%^RESET%^
.EI
.SI 5
string * query_quests(string name)
.EI
.SP 7 5

This method returns the set of currently completed quests by the
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
the array of completed quests

.EP

.SI 3
* %^BOLD%^query_story%^RESET%^
.EI
.SI 5
string query_story(string name)
.EI
.SP 7 5

This method returns the story associated with the player.   This is
created from all the information about the quests they have done
joined together into a neato story.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the story of the player
.EP

.SI 3
* %^BOLD%^query_title%^RESET%^
.EI
.SI 5
string query_title(string name)
.EI
.SP 7 5

This method returns the players current title.
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
their current title, 0 if no title
.EP

.SI 3
* %^BOLD%^restart%^RESET%^
.EI
.SI 5
int restart(mixed name)
.EI
.SP 7 5

This method is called when a player refreshes totaly so they can
start again from scratch.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if they do not exists, 1 if they do

.EP

.SI 3
* %^BOLD%^save_them%^RESET%^
.EI
.SI 5
void save_them(object thing)
.EI
.SP 7 5

This method causes the player to be saved.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the player to save

.EP

.SI 3
* %^BOLD%^set_player_quest_info%^RESET%^
.EI
.SI 5
void set_player_quest_info(string pl_name, string qu_info, mixed * details)
.EI
.SP 7 5

This method  sets the information related to the players quest.
This information is used for quests which have several parts to them
and information needs to be stored about the player as they attempt
to complete it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name - the name of the player
.EP
.SP 9 5
qu_info - the quest name
.EP
.SP 9 5
details - the information associated with the quest.

.EP

.SI 3
* %^BOLD%^set_quest%^RESET%^
.EI
.SI 5
int set_quest(string pl_name, string qu_name, int no_xp)
.EI
.SP 7 5

This method sets the player as having done the quest and if we
should give them xp for it
This function should be called when a quest is finished.  It will then
call the quest_completed function on the quest handler and do all
assorted modifications to the player object etc.  The name should be the
players name and the quest should be the name of the quest that is
stored in the quest handler.


The include file <library.h> should be used for calls to this handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name - name of the player
.EP
.SP 9 5
qu_name - name of the quest
.EP
.SP 9 5
no_xp - do not give out xp
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the quest is already completed
.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI

.SI 3
* %^BOLD%^unset_quest%^RESET%^
.EI
.SI 5
int unset_quest(string pl_name, string qu_name)
.EI
.SP 7 5

This method removes a quest from the players list of completed quests.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pl_name - the player name
.EP
.SP 9 5
qu_name - the quest name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if they have not done the quest, 1 if they habe
.EP


