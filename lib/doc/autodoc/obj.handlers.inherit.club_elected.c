.DT
club_elected.c
Disk World autodoc help
club_elected.c

.SH Description
.SP 5 5

This is the handler for all things clubby, a club being a group of
players.  Each club must have a unique name.  It also handles elections
for various club positions.
.EP
.SP 10 5


Written by Pinkfish

Started Sun Sep 27 03:35:42 EDT 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/handlers/inherit/club_family.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/board.h, /include/am_time.h, /include/mail.h and /include/clubs.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^account_info%^RESET%^
class account_info {
class club_transaction * transactions;
string * allowed_people;
string * allowed_positions;
}

.EI
.SP 7 5

This class keeps track of the accounts, it has the transaction
info for the account and other stuff.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
transactions - the transactions for the account
.EP
.SP 9 5
allowed_people - the people that can use the account
.EP
.SP 9 5
allowed_positions - the positions that can use the account

.EP

.SI 3
* %^BOLD%^election_data%^RESET%^
class election_data {
int time_of_event;
int state;
mapping people;
mapping nominated;
mapping voted_for;
string election_status;
}

.EI
.SP 7 5

This class which keeps track of a currently running election.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
time_of_event - the time the election started
.EP
.SP 9 5
state - the current state of the election
.EP
.SP 9 5
people - the current votes for the specified person.


The format is: ([ position : ([ person : votes, ... ]) ... ])
.EP
.SP 9 5
voted_for - which positions everyone has voted for.


The format is: ([ person : ({ position, position, ... }) ])
.EP
.SP 9 5
nominated - the people who have been nominated for each position
and who nominated them.


This is in the format: ([ position: ([ person : ({ nominations }) ]) ])
A person must be seconded before they are actually added as
a potential for a position.
.EP
.SP 9 5
election_status - the status of the election, this is used with
defines.

.EP

.SI 3
* %^BOLD%^election_info%^RESET%^
class election_info {
string treasurer;
string president;
string vice_president;
string secretary;
mapping account_info;
mapping positions;
int last_election;
int election_period;
mapping options;
class discussion_idea * discussion;
class club_archive * archives;
string announcement;
int announce_time;
}

.EI
.SP 7 5

This is for an elected club, elected clubs ellect their officals and
stuff.  The treasurer etc are actually the indexes to be used into the
positions mapping to determine the actual member which holds the
position.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
treasurer - the name of the treasurer of the club's position
.EP
.SP 9 5
president - the name of the president of the club's position
.EP
.SP 9 5
vice_president - the name of the vice president of the club's position
.EP
.SP 9 5
secretary - the name of the secretary of the club's position
.EP
.SP 9 5
account_info - information about the accounts
.EP
.SP 9 5
positions - the special extra positions available in the club
.EP
.SP 9 5
last_election - when the last election was held
.EP
.SP 9 5
discussion - the current items up for discussion

.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_account_allowed_person%^RESET%^
.EI
.SI 5
int add_account_allowed_person(string club_name, string account, string person)
.EI
.SP 7 5

This method adds a person to the allowed people array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
account - the name of the account
.EP
.SP 9 5
person - the person to add to the array
.EP

.SI 3
* %^BOLD%^add_account_allowed_position%^RESET%^
.EI
.SI 5
int add_account_allowed_position(string club_name, string account, string position)
.EI
.SP 7 5

This method adds a position to the allowed positions array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
account - the name of the account
.EP
.SP 9 5
position - the position to add to the array
.EP

.SI 3
* %^BOLD%^add_money%^RESET%^
.EI
.SI 5
int add_money(string name, int amount, int type, string person, string account, string information)
.EI
.SP 7 5

This method will pay a certain amount of money to club.  This will be
how long the club is payed until.  The club will cost a certain
amount for each member as well as a base cost.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club
.EP
.SP 9 5
amount - the amount to add
.EP
.SP 9 5
type - the type of transaction
.EP
.SP 9 5
person - the person doing the transaction
.EP
.SP 9 5
account - the account being changed
.EP
.SP 9 5
information - the information about the deposit
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount of money not able to be placed in the account
.EP

.SI 3
* %^BOLD%^change_position_name_in_club%^RESET%^
.EI
.SI 5
int change_position_name_in_club(string name, string position, string new_position)
.EI
.SP 7 5

This method changes the name of the position in the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to create the position in
.EP
.SP 9 5
position - the name of the position to change
.EP
.SP 9 5
new_position - the new name of the position
.EP

.SI 3
* %^BOLD%^check_board_post%^RESET%^
.EI
.SI 5
int check_board_post(int type, string board, string previous, string name, int num)
.EI
.SP 7 5

This method checks to see what access the person has to the board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of operation
.EP
.SP 9 5
board - the name of the board
.EP
.SP 9 5
previous - the previous object
.EP
.SP 9 5
name - the name of the person reading the message
.EP
.SP 9 5
num - the number of posts blasted when posting
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^complete_election%^RESET%^
.EI
.SI 5
int complete_election(string club_name)
.EI
.SP 7 5

This method completes the election.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club the election is being completed for
.EP

.SI 3
* %^BOLD%^create_club_board%^RESET%^
.EI
.SI 5
int create_club_board(string club_name)
.EI
.SP 7 5

This method creates a board for the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to create the board for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^create_position_in_club%^RESET%^
.EI
.SI 5
int create_position_in_club(string name, string position)
.EI
.SP 7 5

This method creates a position in the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to create the position in
.EP
.SP 9 5
position - the name of the position
.EP

.SI 3
* %^BOLD%^finish_nomination%^RESET%^
.EI
.SI 5
mixed finish_nomination(string club_name)
.EI
.SP 7 5

This method finishes the nomination stuff.  The return will be 0 if the
election is not valid for this club.  If the election was valid it will
return an array of positions that do not have enough nominated people to
complete the election for.  If the return array is 0 in size then the
next phase of the election is setup.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to finish the nomination for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
see description for return values
.EP

.SI 3
* %^BOLD%^has_nominated_person%^RESET%^
.EI
.SI 5
int has_nominated_person(string club_name, string position, string nominated, string by)
.EI
.SP 7 5

This method checks to see if the person has already nominated
someone for this position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
position - the position they have voted in
.EP
.SP 9 5
person - the person to check to see if they have voted
.EP

.SI 3
* %^BOLD%^has_person_voted%^RESET%^
.EI
.SI 5
int has_person_voted(string club_name, string position, string person)
.EI
.SP 7 5

This method checks to see if the person has already voted in the election.
If the position is not being voted on then, then it will always assume
they have voted for that position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
position - the position they have voted in
.EP
.SP 9 5
person - the person to check to see if they have voted
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they have voted, 0 if not
.EP

.SI 3
* %^BOLD%^holds_any_position_of%^RESET%^
.EI
.SI 5
int holds_any_position_of(string name, string person)
.EI
.SP 7 5

This method will determine if the specified person holds any 
position in the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check for the positions
.EP
.SP 9 5
person - the name of the person to check in the positions
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are in the position, 0 if not

.EP

.SI 3
* %^BOLD%^holds_position_of%^RESET%^
.EI
.SI 5
int holds_position_of(string name, string position, string person)
.EI
.SP 7 5

This method will determine if the specified person holds a
position in the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check for the position
.EP
.SP 9 5
position - the name of the position to check
.EP
.SP 9 5
person - the name of the person to check in the position
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are in the position, 0 if not
.EP

.SI 3
* %^BOLD%^is_allowed_to_modify_account%^RESET%^
.EI
.SI 5
int is_allowed_to_modify_account(string club_name, string account, string person)
.EI
.SP 7 5

This method checks to see if the specified person is allowed to
access the account.  It checks based on both position and name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
account - the name of the account
.EP
.SP 9 5
person - the person's name to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they can access the account, 0 if they cannot
.EP

.SI 3
* %^BOLD%^is_basic_position%^RESET%^
.EI
.SI 5
int is_basic_position(string club_name, string position)
.EI
.SP 7 5

This method determines if the position is one of the 4 basic positions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to check
.EP
.SP 9 5
position - the position to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is, 0 if not
.EP

.SI 3
* %^BOLD%^is_elected_option%^RESET%^
.EI
.SI 5
int is_elected_option(string club_name, string option)
.EI
.SP 7 5

This method checks to see if the specified option exists in this
club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
option - the name of the option to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the option exists, 0 if not
.EP

.SI 3
* %^BOLD%^is_election_in_progress%^RESET%^
.EI
.SI 5
int is_election_in_progress(string club)
.EI
.SP 7 5

This method checks to see if there is currently an election in progress.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to check for an election
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is an election in progress, 0 if not
.EP

.SI 3
* %^BOLD%^is_nominated_for%^RESET%^
.EI
.SI 5
int is_nominated_for(string club_name, string position, string person)
.EI
.SP 7 5

This method checks to see if the specified person has been nominated for
the position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
position - the position they have been nominated for
.EP
.SP 9 5
person - the person who is nominated to that position
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they have been nominated, 0 if not
.EP

.SI 3
* %^BOLD%^is_nomination_accepted%^RESET%^
.EI
.SI 5
int is_nomination_accepted(string club_name, string position, string person)
.EI
.SP 7 5

This method checks to see if the nomination has been accepted.  Currently
a nomination is accepted if there is more than 2 people nominated and one
of them is the person who is nominated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to check
.EP
.SP 9 5
position - the position they are nominated for
.EP
.SP 9 5
person - the person who has been nominated
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are nominated, 0 if not
.EP

.SI 3
* %^BOLD%^is_nomination_in_progress%^RESET%^
.EI
.SI 5
int is_nomination_in_progress(string club)
.EI
.SP 7 5

This method checks to see if the election is in the nomination phase.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the name of the club to check
.EP

.SI 3
* %^BOLD%^is_president_of%^RESET%^
.EI
.SI 5
int is_president_of(string name, string president)
.EI
.SP 7 5

This method will determine if the specified person is the president of
the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check the president of
.EP
.SP 9 5
president - the person to check for being the president
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are in the position, 0 if not

.EP

.SI 3
* %^BOLD%^is_secretary_of%^RESET%^
.EI
.SI 5
int is_secretary_of(string name, string secretary)
.EI
.SP 7 5

This method will determine if the specified person is the secretary of
the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check the secretary of
.EP
.SP 9 5
secretary - the person to check to see if they are the secretary
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are in the position, 0 if not

.EP

.SI 3
* %^BOLD%^is_treasurer_of%^RESET%^
.EI
.SI 5
int is_treasurer_of(string name, string treasurer)
.EI
.SP 7 5

This method will determine if the specified person is the treasurer of
the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check the treasurer of
.EP
.SP 9 5
treasurer - the person to check to see if they are the treasurer
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are in the position, 0 if not

.EP

.SI 3
* %^BOLD%^is_valid_position%^RESET%^
.EI
.SI 5
int is_valid_position(string club_name, string position)
.EI
.SP 7 5

This method determines if this is a valid position for the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
position - the position to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the position is ok, 0 if not
.EP

.SI 3
* %^BOLD%^is_vice_president_of%^RESET%^
.EI
.SI 5
int is_vice_president_of(string name, string vice_president)
.EI
.SP 7 5

This method will determine if the specified person is the vice president of
the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check the vice president of
.EP
.SP 9 5
vice_president - the person to check for being the vice president
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are in the position, 0 if not

.EP

.SI 3
* %^BOLD%^is_voting_in_progress%^RESET%^
.EI
.SI 5
int is_voting_in_progress(string club)
.EI
.SP 7 5

This method checks to see if the election is in the voting phase.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the name of the club to check
.EP

.SI 3
* %^BOLD%^nominate_person%^RESET%^
.EI
.SI 5
int nominate_person(string club_name, string position, string person, string nominating)
.EI
.SP 7 5

This method nominates someone for a certain position in the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to nominate someone in
.EP
.SP 9 5
position - the position to nominate someone for
.EP
.SP 9 5
person - the person to nominate
.EP
.SP 9 5
nominating - the person doing the nominating
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successfuly nominated
.EP

.SI 3
* %^BOLD%^query_account_allowed_people%^RESET%^
.EI
.SI 5
string * query_account_allowed_people(string club_name, string account)
.EI
.SP 7 5

This method returns the people which allowed to manipulate the
account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
account - the name of the account
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of people which can manipulate the account
.EP

.SI 3
* %^BOLD%^query_account_allowed_positions%^RESET%^
.EI
.SI 5
string * query_account_allowed_positions(string club_name, string account)
.EI
.SP 7 5

This method returns the positions which are allowed to manipulate the
account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
account - the name of the account
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of people which can manipulate the account

.EP

.SI 3
* %^BOLD%^query_announcement%^RESET%^
.EI
.SI 5
string query_announcement(string club_name)
.EI
.SP 7 5

This method returns the current announcement string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the announcement string

.EP

.SI 3
* %^BOLD%^query_announcement_time%^RESET%^
.EI
.SI 5
int query_announcement_time(string club_name)
.EI
.SP 7 5

This method returns the time at which the current announcement
was added.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time the announcement was added

.EP

.SI 3
* %^BOLD%^query_archives%^RESET%^
.EI
.SI 5
class club_archive * query_archives(string club_name)
.EI
.SP 7 5

This method returns the archives for this club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to get archives from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the archives for the club

.EP

.SI 3
* %^BOLD%^query_boards%^RESET%^
.EI
.SI 5
string * query_boards()
.EI
.SP 7 5

This method returns all the clubs which currently have boards.
This is their normalised names.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of boards

.EP

.SI 3
* %^BOLD%^query_club_board%^RESET%^
.EI
.SI 5
int query_club_board(string club_name)
.EI
.SP 7 5

This method checks to see if the club has a board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^query_club_board_name%^RESET%^
.EI
.SI 5
string query_club_board_name(string club_name)
.EI
.SP 7 5

This method returns the name of the board for the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to get the board for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the club, 0 if no board

.EP

.SI 3
* %^BOLD%^query_committee%^RESET%^
.EI
.SI 5
string * query_committee(string club_name)
.EI
.SP 7 5

This method returns the committee of the club.  The committee is anyone
who holds a position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of people who hold positions

.EP

.SI 3
* %^BOLD%^query_elected_option%^RESET%^
.EI
.SI 5
mixed query_elected_option(string club_name, string option)
.EI
.SP 7 5

This method queries the option in the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
option - the name of the option to query
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value of the option
.EP

.SI 3
* %^BOLD%^query_election_status%^RESET%^
.EI
.SI 5
string query_election_status(string club_name)
.EI
.SP 7 5

This method queries the status associated with the election.  The
status will say things like when certain positions have not been
voted for and other such stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the club name to get the status of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the status string of the club
.EP

.SI 3
* %^BOLD%^query_last_election%^RESET%^
.EI
.SI 5
int query_last_election(string name)
.EI
.SP 7 5

This method returns the time of the last election.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to get the time for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time of the last election

.EP

.SI 3
* %^BOLD%^query_minimum_next_election%^RESET%^
.EI
.SI 5
int query_minimum_next_election(string name)
.EI
.SP 7 5

This method returns the minimum time between elections.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to get the time for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the minimum time for the next election

.EP

.SI 3
* %^BOLD%^query_next_election%^RESET%^
.EI
.SI 5
int query_next_election(string name)
.EI
.SP 7 5

This method returns the time of the next election.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to get the time for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time of the next election

.EP

.SI 3
* %^BOLD%^query_next_event_time%^RESET%^
.EI
.SI 5
int query_next_event_time(string club_name)
.EI
.SP 7 5

This method returns the time the next event will occur.  The next event
will be either the end of the nomination phase or the end of the
voting phase.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to get the event of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time of the next event
.EP

.SI 3
* %^BOLD%^query_nominated_for%^RESET%^
.EI
.SI 5
string * query_nominated_for(string club_name, string position)
.EI
.SP 7 5

This method returns the list of people that have been nominated for
the specified position in the specified club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
position - the position to get the names of
.EP

.SI 3
* %^BOLD%^query_nominators_for%^RESET%^
.EI
.SI 5
string * query_nominators_for(string club_name, string position, string nominated)
.EI
.SP 7 5

This method returns the people who nominated the specified person
to the position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
position - the position they were nominated for
.EP
.SP 9 5
nominated - the person who was nominated
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
who nominated them
.EP

.SI 3
* %^BOLD%^query_position_members%^RESET%^
.EI
.SI 5
string query_position_members(string club_name, string position)
.EI
.SP 7 5

This method returns the members of the particular position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
position - the name of the position
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of members of the position
.EP

.SI 3
* %^BOLD%^query_position_name%^RESET%^
.EI
.SI 5
string query_position_name(string club_name, string position)
.EI
.SP 7 5

This method figures out the actualy index of the position in the mappings
and stuff associated with the position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
position - the position to create the index for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the position index
.EP

.SI 3
* %^BOLD%^query_transaction_type_name%^RESET%^
.EI
.SI 5
string query_transaction_type_name(int type)
.EI
.SP 7 5

This method returns the string name of the transaction type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type to return the transaction type for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nice transaction type
.EP

.SI 3
* %^BOLD%^query_transactions%^RESET%^
.EI
.SI 5
class club_transaction * query_transactions(string name, string account)
.EI
.SP 7 5

This method returns the transaction history for the club.  This will only
be kept track of for elected clubs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club
.EP
.SP 9 5
account - the name of the account
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of transactions
.EP

.SI 3
* %^BOLD%^query_valid_positions%^RESET%^
.EI
.SI 5
string * query_valid_positions(string club_name)
.EI
.SP 7 5

This method returns the list of available positions for the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of available positions

.EP

.SI 3
* %^BOLD%^remove_account_allowed_person%^RESET%^
.EI
.SI 5
int remove_account_allowed_person(string club_name, string account, string person)
.EI
.SP 7 5

This method removes a person from the allowed people array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
account - the name of the account
.EP
.SP 9 5
person - the person to remove from the array
.EP

.SI 3
* %^BOLD%^remove_account_allowed_position%^RESET%^
.EI
.SI 5
int remove_account_allowed_position(string club_name, string account, string position)
.EI
.SP 7 5

This method removes a position from the allowed positions array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
account - the name of the account
.EP
.SP 9 5
position - the position to remove from the array
.EP

.SI 3
* %^BOLD%^remove_club_board%^RESET%^
.EI
.SI 5
int remove_club_board(string club_name)
.EI
.SP 7 5

This method removes the club's board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to remove the board from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^remove_elected_option%^RESET%^
.EI
.SI 5
int remove_elected_option(string club_name, string option)
.EI
.SP 7 5

This method removes an option in the club.  Options are use for things
like setting if the club has a board associated with it, or other
optional elements.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to remove the option in
.EP
.SP 9 5
option - the name of the option to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^remove_money%^RESET%^
.EI
.SI 5
int remove_money(string name, int amount, int type, string person, string account, string information)
.EI
.SP 7 5

This method removes money from the account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club
.EP
.SP 9 5
amount - the amount to change the balance by
.EP
.SP 9 5
type - the type of transaction
.EP
.SP 9 5
person - the person doing the transaction
.EP
.SP 9 5
account - the account being changed
.EP
.SP 9 5
information - the information about the deposit
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the removal is a success
.EP

.SI 3
* %^BOLD%^remove_position_in_club%^RESET%^
.EI
.SI 5
int remove_position_in_club(string name, string position)
.EI
.SP 7 5

This method removes a position from a club.  The 4 basic positions cannot
be removed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to remove the position from
.EP
.SP 9 5
position - the position to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^set_announcement%^RESET%^
.EI
.SI 5
int set_announcement(string club_name, string announce)
.EI
.SP 7 5

This sets the current announcement.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
announce - the new announcement
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_elected_option%^RESET%^
.EI
.SI 5
int set_elected_option(string club_name, string option, mixed value)
.EI
.SP 7 5

This method sets an option in the club.  Options are use for things
like setting if the club has a board associated with it, or other
optional elements.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to set the option in
.EP
.SP 9 5
option - the name of the option to set
.EP
.SP 9 5
value - the new value of the option
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^set_position_holder%^RESET%^
.EI
.SI 5
int set_position_holder(string club_name, string position, string person)
.EI
.SP 7 5

This method sets the specified person as the holder of the position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to set the position in
.EP
.SP 9 5
position - the name of the position
.EP
.SP 9 5
person - the person who will hold it
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^start_election%^RESET%^
.EI
.SI 5
int start_election(string club_name)
.EI
.SP 7 5

This method starts an election for the specified club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to start an election for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the election is successful started
.EP

.SI 3
* %^BOLD%^vote_for_person%^RESET%^
.EI
.SI 5
int vote_for_person(string club_name, string position, string person, string voter)
.EI
.SP 7 5

This method sets up a vote for a specific person for a position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to make the vote in
.EP
.SP 9 5
position - the position you are voting for
.EP
.SP 9 5
person - the person to vote for
.EP
.SP 9 5
voter - the person doing the voting
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
* %^BOLD%^add_archive%^RESET%^
.EI
.SI 5
int add_archive(string club_name, string subject, string thingy)
.EI
.SP 7 5

This method adds to the archives of things for the current club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
thingy - the thing to archive
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^check_elected_information%^RESET%^
.EI
.SI 5
void check_elected_information(string name, string member, int startup)
.EI
.SP 7 5

This method checks the extra information for the club.  Please note
this does *not* save the information.  You need to do this yourself.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check
.EP

.SI 3
* %^BOLD%^do_mail_committee%^RESET%^
.EI
.SI 5
void do_mail_committee(string club_name, string subject, string body)
.EI
.SP 7 5

This method will mail a messsage to the committee.  Useful for
informing members of problems and things.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to mail
.EP
.SP 9 5
subject - the subject to use
.EP
.SP 9 5
body - the body of the message

.EP

.SI 3
* %^BOLD%^query_account_info%^RESET%^
.EI
.SI 5
class account_info query_account_info(string name, string account)
.EI
.SP 7 5

This method returns the information about the specified account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club
.EP
.SP 9 5
account - the name of the account
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the account info

.EP

.SI 3
* %^BOLD%^query_elected_info%^RESET%^
.EI
.SI 5
class election_info query_elected_info(string name)
.EI
.SP 7 5

This method returns the elected info associated with the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to get the info for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the elected club class, 0 if no club or not elected type

.EP


