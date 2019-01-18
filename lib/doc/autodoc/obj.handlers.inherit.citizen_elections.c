.DT
citizen_elections.c
Disk World autodoc help
citizen_elections.c

.SH Description
.SP 5 5

This is the file used to do elections for magistrate positions.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Oct 23 22:30:19 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/handlers/inherit/citizens.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nomic_system.h, /include/player_handler.h, /include/player.h, /include/board.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^election_data%^RESET%^
class election_data {
int last_election;
int phase;
int phase_start;
mapping nominations;
mapping votes;
string * voted;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^disable_citizen_election%^RESET%^
.EI
.SI 5
int disable_citizen_election(string area)
.EI
.SP 7 5

This method sets the specifed election to "disabled", meaning it will
sit indefinitely with no election.

.EP

.SI 3
* %^BOLD%^has_voted%^RESET%^
.EI
.SI 5
int has_voted(string area, string person)
.EI
.SP 7 5

This method checks to see if the specified person has voted.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check in
.EP
.SP 9 5
person - the person the check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they have voted, 0 if not

.EP

.SI 3
* %^BOLD%^is_election_phase%^RESET%^
.EI
.SI 5
int is_election_phase(string area)
.EI
.SP 7 5

This method checks to see if we are in the election phase.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if we are, 0 if not

.EP

.SI 3
* %^BOLD%^is_eligible_to_vote_for%^RESET%^
.EI
.SI 5
int is_eligible_to_vote_for(string area, string person)
.EI
.SP 7 5

This method checks to see if the specified person is an eligible
person to vote for.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to checlk
.EP
.SP 9 5
person - the person to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are, 0 if not

.EP

.SI 3
* %^BOLD%^is_grace_phase%^RESET%^
.EI
.SI 5
int is_grace_phase(string area)
.EI
.SP 7 5

This method checks to see if we are in the grace phase.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if we are, 0 if not

.EP

.SI 3
* %^BOLD%^is_nomination_accepted%^RESET%^
.EI
.SI 5
int is_nomination_accepted(string area, string person)
.EI
.SP 7 5

This method checks to see if the specifiedf person has accepted their
nomination or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check in
.EP
.SP 9 5
person - the person to check to see if they have accepted
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are accepted, 0 if not

.EP

.SI 3
* %^BOLD%^is_nomination_phase%^RESET%^
.EI
.SI 5
int is_nomination_phase(string area)
.EI
.SP 7 5

This method checks to see if we are in the nomination phase.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if we are, 0 if not

.EP

.SI 3
* %^BOLD%^nominate_magistrate%^RESET%^
.EI
.SI 5
int nominate_magistrate(string area, string person, string nominator)
.EI
.SP 7 5

This method nominates a person for a position.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to nominate in
.EP
.SP 9 5
person - the being nominated
.EP
.SP 9 5
nominator - the person doing the nomination
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^post_citizen_message%^RESET%^
.EI
.SI 5
int post_citizen_message(string area, string subject, string message)
.EI
.SP 7 5

This method will post a message to the citizen board from the
administration.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to post the message to 
.EP
.SP 9 5
subject - the subject it has
.EP
.SP 9 5
message - the message to post

.EP

.SI 3
* %^BOLD%^post_magistrate_message%^RESET%^
.EI
.SI 5
int post_magistrate_message(string area, string subject, string message)
.EI
.SP 7 5

This method will post a message to the magistrate board from the
administration.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to post the message to 
.EP
.SP 9 5
subject - the subject it has
.EP
.SP 9 5
message - the message to post

.EP

.SI 3
* %^BOLD%^query_can_vote_for%^RESET%^
.EI
.SI 5
string * query_can_vote_for(string area)
.EI
.SP 7 5

This method retuns the list of people that can be voted for.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of people that can be voted for

.EP

.SI 3
* %^BOLD%^query_citizen_board%^RESET%^
.EI
.SI 5
string query_citizen_board(string area)
.EI
.SP 7 5

This method returns the board for the specified area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to look for the board on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the board name for the area, 0 for none

.EP

.SI 3
* %^BOLD%^query_end_of_phase_time%^RESET%^
.EI
.SI 5
int query_end_of_phase_time(string area)
.EI
.SP 7 5

This method returns the end of the phase time.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the end of the phase time, 0 on failure

.EP

.SI 3
* %^BOLD%^query_magistrate_board%^RESET%^
.EI
.SI 5
string query_magistrate_board(string area)
.EI
.SP 7 5

This method returns the board for the specified area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to look for the board on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the board name for the area, 0 for none

.EP

.SI 3
* %^BOLD%^query_next_election_time%^RESET%^
.EI
.SI 5
int query_next_election_time(string area)
.EI
.SP 7 5

This method returns the time of the next election.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time of the next election, 0 if on is in progress

.EP

.SI 3
* %^BOLD%^query_nominated_by%^RESET%^
.EI
.SI 5
string * query_nominated_by(string area, string person)
.EI
.SP 7 5

This method checks to see who has nominated a specified person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SP 9 5
person - the person to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of people who have nominated the person

.EP

.SI 3
* %^BOLD%^query_people_nominated%^RESET%^
.EI
.SI 5
string * query_people_nominated(string area)
.EI
.SP 7 5

This method returns the list of people who have been nominated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to get the list of people who are nominated
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of people who have been nominated

.EP

.SI 3
* %^BOLD%^set_citizen_board%^RESET%^
.EI
.SI 5
void set_citizen_board(string area, string name)
.EI
.SP 7 5

This method sets the board for the specified area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to set the board for
.EP
.SP 9 5
name - the name of the board

.EP

.SI 3
* %^BOLD%^set_magistrate_board%^RESET%^
.EI
.SI 5
void set_magistrate_board(string area, string name)
.EI
.SP 7 5

This method sets the board for the specified area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to set the board for
.EP
.SP 9 5
name - the name of the board

.EP

.SI 3
* %^BOLD%^vote_for_people%^RESET%^
.EI
.SI 5
int vote_for_people(string area, string * people, string voter)
.EI
.SP 7 5

This method votes for a specified person in the area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to vote for someone in
.EP
.SP 9 5
person - the person to vote for
.EP
.SP 9 5
voter - the person voting
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
* %^BOLD%^finish_election_phase%^RESET%^
.EI
.SI 5
void finish_election_phase(string area)
.EI
.SP 7 5

This method finishes the election phase.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to finish the phase for

.EP

.SI 3
* %^BOLD%^finish_nomination_phase%^RESET%^
.EI
.SI 5
void finish_nomination_phase(string area)
.EI
.SP 7 5

This method finishes the nomination phase.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to finish the phase for

.EP

.SI 3
* %^BOLD%^query_election_phase%^RESET%^
.EI
.SI 5
int query_election_phase(string area)
.EI
.SP 7 5

This method returns the phase of the election.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the election phase

.EP

.SI 3
* %^BOLD%^start_election%^RESET%^
.EI
.SI 5
void start_election(string area)
.EI
.SP 7 5

This method starts the election.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to do the election for

.EP


