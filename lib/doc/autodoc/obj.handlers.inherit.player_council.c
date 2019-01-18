.DT
player_council.c
Disk World autodoc help
player_council.c

.SH Description
.SP 5 5

This handler is for the control of player houses.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Jun 23 22:50:02 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/handlers/inherit/case_control, /obj/handlers/inherit/nomic_rules and /obj/handlers/inherit/citizen_elections.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nomic_system.h, /include/player.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_amend_rule_motion%^RESET%^
.EI
.SI 5
int add_amend_rule_motion(string area, int rule_no, string amendment, string creator)
.EI
.SP 7 5

This method adds in a request to amend a rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to put the amendment in
.EP
.SP 9 5
rule_no - the rule to amend
.EP
.SP 9 5
amendment - the amendment text
.EP
.SP 9 5
creator - the person doing the amendment
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^add_create_rule_motion%^RESET%^
.EI
.SI 5
int add_create_rule_motion(string area, int type, string text, string creator)
.EI
.SP 7 5

This method puts a new rule up for voting.  It is first placed into
the hands of the magistrates, then into the hands of the players.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area for the rule to be voted on in
.EP
.SP 9 5
type - the type of the rules
.EP
.SP 9 5
text - the text associated with the new rule
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^add_move_rule_type_motion%^RESET%^
.EI
.SI 5
int add_move_rule_type_motion(string area, int rule_no, int type, string creator)
.EI
.SP 7 5

This method puts up a request to move a rule from one type to another.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to do the move in
.EP
.SP 9 5
rule_no - the rule number to move
.EP
.SP 9 5
type - the type to move the rule to
.EP
.SP 9 5
creator - the creator of this request
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^amend_motion%^RESET%^
.EI
.SI 5
int amend_motion(string area, int id, string amend_text, string amender)
.EI
.SP 7 5

This method adds an amendment to the new rule/amendment/whatever, this
rewrites the text of the ammendment of whatever it is.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the amendmend is to go in
.EP
.SP 9 5
id - the rule update id
.EP
.SP 9 5
amendmend - the amendmend to make
.EP
.SP 9 5
amendmender - the person amendmending
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^check_for_completions%^RESET%^
.EI
.SI 5
void check_for_completions()
.EI
.SP 7 5

This method figures out all the current things and when they completed.

.EP

.SI 3
* %^BOLD%^comment_on_motion%^RESET%^
.EI
.SI 5
int comment_on_motion(string area, int id, string comment_text, string commenter)
.EI
.SP 7 5

This method adds a comment to the new rule/amendment/whatever.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the comment is to go in
.EP
.SP 9 5
id - the rule update id
.EP
.SP 9 5
comment - the comment to make
.EP
.SP 9 5
commenter - the person commenting
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^complete_motion%^RESET%^
.EI
.SI 5
int complete_motion(string area, int id)
.EI
.SP 7 5

This method completes the vote on the specified new rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to complete the rule in
.EP
.SP 9 5
id - the id to complete
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^create_area%^RESET%^
.EI
.SI 5
void create_area(string area)
.EI
.SP 7 5

This method creates a new area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the new area

.EP

.SI 3
* %^BOLD%^find_motion%^RESET%^
.EI
.SI 5
class nomic_motion find_motion(string area, int rule_no, int type)
.EI
.SP 7 5

This method finds the specified motion.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to look in
.EP
.SP 9 5
rule_no - the rule number
.EP
.SP 9 5
type - the type of the motion
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the motion if successful, 0 if not

.EP

.SI 3
* %^BOLD%^has_voted_for_motion%^RESET%^
.EI
.SI 5
int has_voted_for_motion(string area, int id, string voter)
.EI
.SP 7 5

This method checks to see if the person has voted for the specified
new rule/amendment/whatever
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SP 9 5
id - the id to check
.EP
.SP 9 5
voter - the person to check for voting
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^motion_as_html%^RESET%^
.EI
.SI 5
string motion_as_html(string area, class nomic_motion motion)
.EI
.SP 7 5

This method returns the new motion as a string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the motion is defined in
.EP
.SP 9 5
motion - the new motion to turn into a styring
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string version of the motion

.EP

.SI 3
* %^BOLD%^motion_as_string%^RESET%^
.EI
.SI 5
string motion_as_string(int indent, string area, class nomic_motion motion, int brief)
.EI
.SP 7 5

This method returns the new motion as a string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
indent - the indentation to putin front of all the lines
.EP
.SP 9 5
area - the area the motion is defined in
.EP
.SP 9 5
motion - the new motion to turn into a styring
.EP
.SP 9 5
brief - disable comments
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string version of the motion

.EP

.SI 3
* %^BOLD%^query_all_motions%^RESET%^
.EI
.SI 5
class nomic_motion * query_all_motions(string area)
.EI
.SP 7 5

This method returns all the new rules for the specified area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to return the new rules in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of new rules

.EP

.SI 3
* %^BOLD%^query_completion_time%^RESET%^
.EI
.SI 5
int query_completion_time(string area, class nomic_motion motion)
.EI
.SP 7 5

This method figures out the compleion time for the specified motion.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to check
.EP
.SP 9 5
the - motion in the area
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the completion time

.EP

.SI 3
* %^BOLD%^query_motion%^RESET%^
.EI
.SI 5
class nomic_motion query_motion(string area, int id)
.EI
.SP 7 5

This method returns a nice new rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the areat to look in
.EP
.SP 9 5
id - the id of the new rule to find
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^query_votes_for_motion%^RESET%^
.EI
.SI 5
int * query_votes_for_motion(string area, int id)
.EI
.SP 7 5

This method returns the current set of votes for the specied new rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to lookup the votes in
.EP
.SP 9 5
id - the idea to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
({ yes, no, abstain })

.EP

.SI 3
* %^BOLD%^remove_rule_motion%^RESET%^
.EI
.SI 5
int remove_rule_motion(string area, int rule_no, string creator)
.EI
.SP 7 5

This method remove a request to amend a rule.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to put the amendment in
.EP
.SP 9 5
rule_no - the rule to amend
.EP
.SP 9 5
amendment - the amendment text
.EP
.SP 9 5
creator - the person doing the amendment
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^rule_as_html%^RESET%^
.EI
.SI 5
string rule_as_html(class nomic_rule rule)
.EI
.SP 7 5

This method turns a rule into html.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
rule - the rule to turn into html
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the rule as a string

.EP

.SI 3
* %^BOLD%^send_council_inform%^RESET%^
.EI
.SI 5
void send_council_inform(string area, int only_magistrates, string mess)
.EI
.SP 7 5

This method sends informs to all the specified council of some sort of
nifty council event.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the council is for
.EP
.SP 9 5
only_magistrates - only send it to the magistrates?
.EP
.SP 9 5
mess - the message to send

.EP

.SI 3
* %^BOLD%^update_immutables%^RESET%^
.EI
.SI 5
void update_immutables(string area)
.EI
.SP 7 5

This method updates the immutable laws with the new set of general rules.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to update.

.EP

.SI 3
* %^BOLD%^vote_for_motion%^RESET%^
.EI
.SI 5
int vote_for_motion(string area, int id, int vote_type, string voter)
.EI
.SP 7 5

This method votes for the specified ammentment/whatever.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area to vote in
.EP
.SP 9 5
id - the id to vote for
.EP
.SP 9 5
vote_type - the type of the vote
.EP
.SP 9 5
voter - the person is voting
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP


