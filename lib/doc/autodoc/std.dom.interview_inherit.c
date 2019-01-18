.DT
interview_inherit.c
Disk World autodoc help
interview_inherit.c

.SH Description
.SP 5 5

 Domain Interview Room
 @author Drakkos
 @started on 02/12/2000
 

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h and /include/mail.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_candidate%^RESET%^
.EI
.SI 5
int add_candidate(string name)
.EI
.SP 7 5

Add a candidate to the list.  Will not send part two of the application
to them.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - name of the player to add.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^add_confirmed%^RESET%^
.EI
.SI 5
int add_confirmed(string name)
.EI

.SI 3
* %^BOLD%^add_creator%^RESET%^
.EI
.SI 5
int add_creator(string name)
.EI
.SP 7 5

Entry point to the add creator command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - name of the creator to allow
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^add_interviewed%^RESET%^
.EI
.SI 5
int add_interviewed(string name)
.EI

.SI 3
* %^BOLD%^clear_candidates%^RESET%^
.EI
.SI 5
int clear_candidates()
.EI
.SP 7 5

Clear the list of player candidates.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^clear_interviewed%^RESET%^
.EI
.SI 5
int clear_interviewed()
.EI
.SP 7 5

Clear the list of interviewed players.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^do_add%^RESET%^
.EI
.SI 5
int do_add(string candidate)
.EI
.SP 7 5

Start off a creator application by sending them out the second part of
the application procedure.  
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
candidate - the name to send the second part of the application to.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^do_confirm%^RESET%^
.EI
.SI 5
int do_confirm(string candidate)
.EI
.SP 7 5

Confirm receipt of a candidate's submission.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
candidate - name of the player to add.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^do_end%^RESET%^
.EI
.SI 5
int do_end()
.EI
.SP 7 5

The function to end the interview.  Will write an entry in the interview
text containing who the interview was with and when the interview was
ended.  It will then add the subject to the list of interviewed names and
save the object.  Finally, it will reset the subject of the room. 
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^do_flag%^RESET%^
.EI
.SI 5
int do_flag(string name, string flag)
.EI
.SP 7 5

Entry point for the flag command.  Sets an application as accepted or 
rejected, and sends out the appropriate form mail.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - name of candidate.
.EP
.SP 9 5
flag - "accepted" or "rejected"
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^do_interview%^RESET%^
.EI
.SI 5
varargs int do_interview(object ob, string names)
.EI
.SP 7 5

This function is the main entry point for the interview code.  It 
will set the subject of the room, and write an entry in the interview
text containing who the interview is with, when it was started, and
who conducted the interview.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to interview
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI

.SI 3
* %^BOLD%^do_query%^RESET%^
.EI
.SI 5
int do_query()
.EI
.SP 7 5

Query the current state of applications.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^do_query_text%^RESET%^
.EI
.SI 5
int do_query_text(string flag)
.EI
.SP 7 5

Entry point for the query command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^do_read%^RESET%^
.EI
.SI 5
int do_read(string name)
.EI
.SP 7 5

This function will take the name of a player and if the interview document
for that player exists, print it to the screen of this_player().
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^do_show%^RESET%^
.EI
.SI 5
int do_show()
.EI
.SP 7 5

This function will show the embedded 'movie' text at regular intervals.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
returns 0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^do_stop%^RESET%^
.EI
.SI 5
int do_stop()
.EI

.SI 3
* %^BOLD%^event_emote%^RESET%^
.EI
.SI 5
void event_emote(object ob, string mess)
.EI

.SI 3
* %^BOLD%^flush%^RESET%^
.EI
.SI 5
int flush()
.EI
.SP 7 5

Flush the candidate, interviewed and submitted lists.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^load%^RESET%^
.EI
.SI 5
void load()
.EI

.SI 3
* %^BOLD%^play_movie%^RESET%^
.EI
.SI 5
void play_movie(int i)
.EI
.SP 7 5

This function does a person_say event on the environmen.  The 
content of the event is the relevant movie text to be said.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the index of the movie to be be played.

.EP

.SI 3
* %^BOLD%^query_acceptance%^RESET%^
.EI
.SI 5
string query_acceptance()
.EI
.SP 7 5

Query the acceptance mail text.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the rejection text

.EP

.SI 3
* %^BOLD%^query_all_applicants%^RESET%^
.EI
.SI 5
string * query_all_applicants()
.EI

.SI 3
* %^BOLD%^query_allowed_creators%^RESET%^
.EI
.SI 5
int query_allowed_creators()
.EI
.SP 7 5

Entry point to the query allowed creators command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^query_applicants_text%^RESET%^
.EI
.SI 5
string query_applicants_text()
.EI
.SP 7 5

Return which candidates have been mailed, which have sent in their 
submission, and which are left to be interviewed or replied to.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the text to be displayed

.EP

.SI 3
* %^BOLD%^query_cc%^RESET%^
.EI
.SI 5
string query_cc()
.EI
.SP 7 5

Query the names cc'd by default.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an imploded string of cc'd names

.EP

.SI 3
* %^BOLD%^query_confirmation%^RESET%^
.EI
.SI 5
string query_confirmation()
.EI
.SP 7 5

Returns the confirmation mail text.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The confirmation text

.EP

.SI 3
* %^BOLD%^query_domain%^RESET%^
.EI
.SI 5
string query_domain()
.EI
.SP 7 5

Return the domain the room exists in.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The name of the domain, or 'unset' if none is set.

.EP

.SI 3
* %^BOLD%^query_interview%^RESET%^
.EI
.SI 5
int query_interview()
.EI
.SP 7 5

Return whether creator candidates require an interview for this domain.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if an interview is not required.  Anything else if it is.

.EP

.SI 3
* %^BOLD%^query_interview_dir%^RESET%^
.EI
.SI 5
string query_interview_dir()
.EI
.SP 7 5

Return the directory where interviews are to be stored.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
interview directory.

.EP

.SI 3
* %^BOLD%^query_interviewed%^RESET%^
.EI
.SI 5
string * query_interviewed()
.EI

.SI 3
* %^BOLD%^query_interviewer%^RESET%^
.EI
.SI 5
string * query_interviewer()
.EI

.SI 3
* %^BOLD%^query_is_interviewer%^RESET%^
.EI
.SI 5
int query_is_interviewer(string name)
.EI

.SI 3
* %^BOLD%^query_lord%^RESET%^
.EI
.SI 5
string query_lord()
.EI
.SP 7 5

Return the domain's lord.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the capitalised name of the lord.

.EP

.SI 3
* %^BOLD%^query_part_two%^RESET%^
.EI
.SI 5
string query_part_two()
.EI
.SP 7 5

Return the text for the second part of the application.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
application text.

.EP

.SI 3
* %^BOLD%^query_part_two_required%^RESET%^
.EI
.SI 5
int query_part_two_required()
.EI

.SI 3
* %^BOLD%^query_permitted%^RESET%^
.EI
.SI 5
int query_permitted(string name)
.EI

.SI 3
* %^BOLD%^query_permitted_creators%^RESET%^
.EI
.SI 5
string * query_permitted_creators()
.EI
.SP 7 5

Query the creators permitted to use this room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an empty array if no creators are permitted.  The array of creators 
otherwise.

.EP

.SI 3
* %^BOLD%^query_rejection%^RESET%^
.EI
.SI 5
string query_rejection()
.EI
.SP 7 5

Query the rejection mail text.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the rejection text

.EP

.SI 3
* %^BOLD%^query_subject%^RESET%^
.EI
.SI 5
string query_subject()
.EI
.SP 7 5

Query the subject of an interview if one is ongoing.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if there is no subject.  The name of the subject if there is.

.EP

.SI 3
* %^BOLD%^query_submitted%^RESET%^
.EI
.SI 5
string * query_submitted()
.EI

.SI 3
* %^BOLD%^remove_candidate%^RESET%^
.EI
.SI 5
int remove_candidate(string candidate)
.EI
.SP 7 5

Delete a player from the list.  Will search through all three lists for
an occurance of the name before deleting it where found.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
candidate - the name of the player to remove.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^remove_creator%^RESET%^
.EI
.SI 5
int remove_creator(string name)
.EI
.SP 7 5

The entry point to the remove creator command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - name of the creator to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^remove_interviewed%^RESET%^
.EI
.SI 5
int remove_interviewed(string candidate)
.EI
.SP 7 5

Remove a player from the interviewed list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
candidate - name of the player to add.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^save%^RESET%^
.EI
.SI 5
void save()
.EI

.SI 3
* %^BOLD%^set_acceptance%^RESET%^
.EI
.SI 5
void set_acceptance(string text)
.EI
.SP 7 5

Set the acceptance mail text.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
text - the acceptance mail text

.EP

.SI 3
* %^BOLD%^set_cc%^RESET%^
.EI
.SI 5
void set_cc(string * i)
.EI
.SP 7 5

Set the names cc'd by default.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - an array of cc'd names.

.EP

.SI 3
* %^BOLD%^set_confirmation%^RESET%^
.EI
.SI 5
void set_confirmation(string text)
.EI
.SP 7 5

Set the text of the confirmation mail.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dom - name of the domain.

.EP

.SI 3
* %^BOLD%^set_domain%^RESET%^
.EI
.SI 5
void set_domain(string dom)
.EI
.SP 7 5

Set the domain the room exists in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dom - name of the domain.

.EP

.SI 3
* %^BOLD%^set_interview%^RESET%^
.EI
.SI 5
void set_interview(int i)
.EI
.SP 7 5

Set whether an interview is required of creator candidates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the interview state.

.EP

.SI 3
* %^BOLD%^set_interview_dir%^RESET%^
.EI
.SI 5
int set_interview_dir(string dir)
.EI
.SP 7 5

Set the directory where interviews are to be stored.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - interview directory.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^set_movie%^RESET%^
.EI
.SI 5
void set_movie(string * m)
.EI
.SP 7 5

Tests to see if a particular creator is permitted to make use of the 
interview room.  Domain lords and deputies are always permitted.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the object attempting to use the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the creator is allowed, 0 if they are not.

.EP

.SI 3
* %^BOLD%^set_part_two%^RESET%^
.EI
.SI 5
int set_part_two(string file)
.EI
.SP 7 5

Set the filename for the second part of the application.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - filename of the second part.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success.

.EP

.SI 3
* %^BOLD%^set_part_two_required%^RESET%^
.EI
.SI 5
void set_part_two_required(int i)
.EI

.SI 3
* %^BOLD%^set_rejection%^RESET%^
.EI
.SI 5
void set_rejection(string text)
.EI
.SP 7 5

Set the rejection mail text.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
text - the rejection mail text

.EP


