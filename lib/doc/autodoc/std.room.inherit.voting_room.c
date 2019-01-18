.DT
voting_room.c
Disk World autodoc help
voting_room.c

.SH Description
.SP 5 5

Generalised voting room intended to be inherited into rooms in the mud that
will setup the parameters and settings for votes.


The room does not inherantly support player initiated votes, however it is a fairly simple matter to add this to your own room and have your room add votes when players initiate them. 
.EP
.SP 10 5


Written by Ceres

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/board.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^election%^RESET%^
class election {
mixed * candidates;
int closes;
}

.EI
.SP 7 5

 This class stores the essential information about an election during the
 setup phase. It is only used internally to the voting room.

.EP

.SI 3
* %^BOLD%^vote%^RESET%^
class vote {
string type;
string desc;
string * choices;
mixed * votes;
int ending;
string * voted;
}

.EI
.SP 7 5

 This class stores the essential information about a vote. It is only used
 internally to the voting room.
.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_vote%^RESET%^
.EI
.SI 5
void add_vote(string type, string description, string * choices, int ending)
.EI
.SP 7 5

This function is called to add a vote to the system.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - Freeform string giving the type of vote. This is typically used
by your completion function so that it can perform different actions for
different types of votes.

.EP
.SP 9 5
description - The description of the vote as shown to the player.

.EP
.SP 9 5
choices - An array of vote options eg. ({"Yes", "No" })

.EP
.SP 9 5
ending - An integer time of when the vote should be terminated.

.EP

.SI 3
* %^BOLD%^cast_vote%^RESET%^
.EI
.SI 5
int cast_vote(string which_str, int vote_id)
.EI
.SP 7 5

This function is called when a player votes. The syntax is
"vote <choice> on <vote>". 
.EP

.SI 3
* %^BOLD%^delete_election%^RESET%^
.EI
.SI 5
int delete_election(string election)
.EI
.SP 7 5

This is an administrative function to allow the removal/cancellation
of an election.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
election - The name of the election to be cancelled
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Returns 1 for success, 0 for failure.

.EP

.SI 3
* %^BOLD%^delete_vote%^RESET%^
.EI
.SI 5
int delete_vote(int vote_id)
.EI
.SP 7 5

This is an administrative function to allow the removal/cancellation
of a vote.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
vote_id - The id number of the vote to be cancelled.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Returns 1 for success, 0 for failure.

.EP

.SI 3
* %^BOLD%^do_create_vote%^RESET%^
.EI
.SI 5
int do_create_vote(string choices)
.EI
.SP 7 5

This is an option function which can be defined in upper level inherits.
It allows the players to create their own votes with their own choices.

.EP

.SI 3
* %^BOLD%^end_vote%^RESET%^
.EI
.SI 5
void end_vote(int which)
.EI
.SP 7 5

This function is called to terminate a vote. It calls your completion
action.

.EP

.SI 3
* %^BOLD%^init_add_vote%^RESET%^
.EI
.SI 5
void init_add_vote()
.EI
.SP 7 5

This is the init() which should be called as well as the default
init() if you wish to have player added votes.

.EP

.SI 3
* %^BOLD%^initiate_election%^RESET%^
.EI
.SI 5
void initiate_election(string position)
.EI
.SP 7 5

This function is used to initiate an election.
It sets up the election class and adds it to the mapping.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
position - The name of the position the election is for.

.EP

.SI 3
* %^BOLD%^list_elections%^RESET%^
.EI
.SI 5
int list_elections()
.EI
.SP 7 5

This function is called when a player types 'elections'. It lists the
elections currently accepting candidates.

.EP

.SI 3
* %^BOLD%^list_votes%^RESET%^
.EI
.SI 5
int list_votes()
.EI
.SP 7 5

This function is called when a player types 'list'. It lists the currently
open votes.

.EP

.SI 3
* %^BOLD%^make_announcement%^RESET%^
.EI
.SI 5
void make_announcement(string board, string name, string subject, string message)
.EI
.SP 7 5

This function is provided for convenience to make it easy for your vote
end function to post to a board.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - The name of the board to post to.
.EP
.SP 9 5
name - The name of the person to post as.
.EP
.SP 9 5
subject - The subject line to use.
.EP
.SP 9 5
message - The message to post.

.EP

.SI 3
* %^BOLD%^query_election_in_progress%^RESET%^
.EI
.SI 5
int query_election_in_progress(string position)
.EI
.SP 7 5

This method determines if there is already an election of the
specified type in progress.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
position - the name of the election in progress
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is an election in progress

.EP

.SI 3
* %^BOLD%^query_open_voting%^RESET%^
.EI
.SI 5
int query_open_voting(int open)
.EI
.SP 7 5

This function queries if the votes should be open or not.  If a vote is
open then you know who voted for each thing.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for an open system 0 for closed

.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This method returns the save file currently used for the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current save file

.EP

.SI 3
* %^BOLD%^query_vote_duration%^RESET%^
.EI
.SI 5
int query_vote_duration()
.EI
.SP 7 5

This function is used to returns the normal vote and election duration.
If it is not set and no duration is
given in add_vote then the default value of DEFAULT_VOTE_DURATION is used.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
duraction - The number of seconds the vote should be open.

.EP

.SI 3
* %^BOLD%^second_candidate%^RESET%^
.EI
.SI 5
int second_candidate(string who, string position)
.EI
.SP 7 5

This function is used by players to second candidates for election. If a
candidates isn't seconded he/she won't be in the election. The syntax is:
"second <player> for <position>". 
.EP

.SI 3
* %^BOLD%^set_completion_action%^RESET%^
.EI
.SI 5
void set_completion_action(mixed cond)
.EI
.SP 7 5

This function defines a function to be called when the vote is complete.
Typically your function will post the vote results somewhere or somesuch.
The parameters passedinto the function are:
.EP
.SP 7 5
( string type, string description, string* choices, mapping votes, string* voted) 
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cond - This will usually be a mixed array of an object and a function.
It could also be a function pointer.

.EP

.SI 3
* %^BOLD%^set_election_announcement%^RESET%^
.EI
.SI 5
void set_election_announcement(string board, string person, string subject, string prefix, string suffix)
.EI
.SP 7 5

This function defines the board, person and subject for announcements
of elections.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - This is the board to post to.
.EP
.SP 9 5
person - This is who to post as.
.EP
.SP 9 5
subject - This is the subject line to use.
.EP
.SP 9 5
prefix - The text to preceed the message.  The default is
              "All eligible persons are requested to vote for the position "
              "of "
.EP
.SP 9 5
suffix - The text to follow the message.

.EP

.SI 3
* %^BOLD%^set_no_elections%^RESET%^
.EI
.SI 5
void set_no_elections(int no_elections)
.EI
.SP 7 5

This method sets the room to not allow elections and disable all the
election commands.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
no_elections - the flag

.EP

.SI 3
* %^BOLD%^set_open_voting%^RESET%^
.EI
.SI 5
void set_open_voting(int open)
.EI
.SP 7 5

This function setups if the votes should be open or not.  If a vote is
open then you know who voted for each thing.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
open - 1 for an open system 0 for closed

.EP

.SI 3
* %^BOLD%^set_proxy%^RESET%^
.EI
.SI 5
void set_proxy(string proxy)
.EI
.SP 7 5

This method sets up a proxy for the room.  A proxy is somewhere else
to get all the voting information from.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
proxy - the proxy tpo setup

.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string file)
.EI
.SP 7 5

This method is called by the inheriting object to determine which save file
to use for votes.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - The name of the file you want the voting info saved to
(without the .o)

.EP

.SI 3
* %^BOLD%^set_second_conditions%^RESET%^
.EI
.SI 5
void set_second_conditions(mixed cond)
.EI
.SP 7 5

This function defines a function to be called to determine if a player
is eligible to second a canditate for election.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cond - This will usually be a mixed array of an object and a function.
It could also be a function pointer.

Your function should return 1 if the player is eligible to second or 0 if
they are not.

.EP

.SI 3
* %^BOLD%^set_stand_conditions%^RESET%^
.EI
.SI 5
void set_stand_conditions(mixed cond)
.EI
.SP 7 5

This function defines a function to be called to determine if a player
is eligible to stand for election to a position.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cond - This will usually be a mixed array of an object and a function.
It could also be a function pointer.

Your function should return 1 if the player is eligible to stand or 0 if
they are not.

.EP

.SI 3
* %^BOLD%^set_vote_conditions%^RESET%^
.EI
.SI 5
void set_vote_conditions(mixed cond)
.EI
.SP 7 5

This function defines a function to be called to determine if a player
is eligible to vote.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cond - This will usually be a mixed array of an object and a function.
It could also be a function pointer.

Your function should return 1 if the player is eligible to vote or 0 if
they are not.

.EP

.SI 3
* %^BOLD%^set_vote_duration%^RESET%^
.EI
.SI 5
void set_vote_duration(int duration)
.EI
.SP 7 5

This function is used to set the normal vote and election duration. It can
be overridden when calling add_vote. If it is not set and no duration is
given in add_vote then the default value of DEFAULT_VOTE_DURATION is used.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
duraction - The number of seconds the vote should be open.

.EP

.SI 3
* %^BOLD%^setup_after_load%^RESET%^
.EI
.SI 5
void setup_after_load()
.EI
.SP 7 5

This setups all the stuff that  needs to be setup after the room has
been loaded.  Makes all the callouts for the right amount of time
and so on.

.EP

.SI 3
* %^BOLD%^stand_for_election%^RESET%^
.EI
.SI 5
int stand_for_election(string position)
.EI
.SP 7 5

This function is used by players to stand for election. The syntax is:
"stand for <position>". 
.EP

.SI 3
* %^BOLD%^start_election_vote%^RESET%^
.EI
.SI 5
void start_election_vote(string post)
.EI
.SP 7 5

Once the candidacy phase is over this function starts the election vote
itself by taking all eligible candidates, setting them as choices in the
election and then posting an announcemment.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^add_our_election%^RESET%^
.EI
.SI 5
void add_our_election(string name, class election data)
.EI
.SP 7 5

This method adds in a election.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the election to add
.EP
.SP 9 5
data - the election data

.EP

.SI 3
* %^BOLD%^add_our_vote%^RESET%^
.EI
.SI 5
void add_our_vote(int id, class vote data)
.EI
.SP 7 5

This method adds in a vote.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the vote
.EP
.SP 9 5
data - the vote data

.EP

.SI 3
* %^BOLD%^query_our_elections%^RESET%^
.EI
.SI 5
mapping query_our_elections()
.EI
.SP 7 5

This method is used to return the current elections.  This can be
overridden in hgiher inherits to control things better.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the elections

.EP

.SI 3
* %^BOLD%^query_our_votes%^RESET%^
.EI
.SI 5
mapping query_our_votes()
.EI
.SP 7 5

This method is used to return the current votes.  This can be overridden
in higher functions to get the votes from somewhere else if nessessary.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the votes

.EP

.SI 3
* %^BOLD%^save_room%^RESET%^
.EI
.SI 5
void save_room()
.EI
.SP 7 5

Private function to save the rooms data file.

.EP


