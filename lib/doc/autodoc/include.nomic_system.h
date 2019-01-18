.DT
nomic_system.h
Disk World autodoc help
nomic_system.h

.SH Description
.SP 5 5

This is the include file to deal with the nomic system for the
player council.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Jun 23 20:10:40 PDT 2000

.EP

.SH Classes

.SI 3
* %^BOLD%^nomic_amendment%^RESET%^
class nomic_amendment {
string amender;
string text;
int date_amended;
}

.EI
.SP 7 5

This is the class to handle amendments to the rule
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
amender - the person doing the amendments
.EP
.SP 9 5
text - the text of the amendment
.EP
.SP 9 5
date_amended - when it was amended

.EP

.SI 3
* %^BOLD%^nomic_area%^RESET%^
class nomic_area {
int * new_type_num;
class nomic_rule * rules;
}

.EI
.SP 7 5

The information needed for each nomic area.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
new_type_num - the next numbers to use in each type area
.EP
.SP 9 5
rules - the rules for the area

.EP

.SI 3
* %^BOLD%^nomic_case%^RESET%^
class nomic_case {
int id;
string opener;
string short;
string * people;
int * notes;
class nomic_case_event * events;
int time_opened;
}

.EI
.SP 7 5

This is the class for the thing.
.EP

.SI 3
* %^BOLD%^nomic_case_event%^RESET%^
class nomic_case_event {
int type;
string mess;
string person;
int event_time;
mixed data;
}

.EI
.SP 7 5

This is an event associated with the case.
.EP

.SI 3
* %^BOLD%^nomic_case_note%^RESET%^
class nomic_case_note {
int id;
string person;
string text;
int time_created;
}

.EI
.SP 7 5

This is a note associated with the case.
.EP

.SI 3
* %^BOLD%^nomic_comment%^RESET%^
class nomic_comment {
string text;
string commenter;
int date_commented;
}

.EI
.SP 7 5

Comment on this new rule thingy by magistrates.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
text - the text of the comment
.EP
.SP 9 5
commenter - the person commenting
.EP
.SP 9 5
date_commented - the date the comment was made

.EP

.SI 3
* %^BOLD%^nomic_motion%^RESET%^
class nomic_motion {
int identifier;
int motion_type;
class nomic_rule rule;
int new_rule_type;
class nomic_comment * comments;
int yes_votes;
int no_votes;
int abstain_votes;
string * voted;
int state;
int date_added;
int date_event;
string added_by;
}

.EI
.SP 7 5

This method sets up a new nomic rule for someone to vote on.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
identifier - the id of the rule
.EP
.SP 9 5
motion_type - the type of the new rule
.EP
.SP 9 5
rule - the new rule (or amended rule)
.EP
.SP 9 5
new_rule_type - if the rule typ eis to change types, this is the new
type
.EP
.SP 9 5
comments - any comments associated with the rule
.EP
.SP 9 5
yes_votes - the current set of yes votes
.EP
.SP 9 5
no_votes - the current set of no votes
.EP
.SP 9 5
abstain_votes - the current set of abstain votes
.EP
.SP 9 5
voted - the current bunch of people that have voted
.EP
.SP 9 5
state - the state of the current rule
.EP
.SP 9 5
date_added - the date this new rule discussion was added
.EP
.SP 9 5
date_event - the date of the last state change event
.EP
.SP 9 5
added_by - the person who added the rule change

.EP

.SI 3
* %^BOLD%^nomic_rule%^RESET%^
class nomic_rule {
int id;
int type;
int date_created;
string creator;
string text;
class nomic_amendment * amendments;
}

.EI
.SP 7 5

The information on nomic rules.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
id - the id of the rule (unique)
.EP
.SP 9 5
date_create - when the rule was created
.EP
.SP 9 5
creator - who created the rule
.EP
.SP 9 5
text - the text of the rule
.EP
.SP 9 5
amendments - the amendments to the rule

.EP


.SH Defines
.SI 3
* %^BOLD%^NOMIC_CASE_APPEAL_TIME%^RESET%^
.EI
.SP 7 5

This is the amount of time some has to appeal the decision.
2 weeks.

.EP

.SI 3
* %^BOLD%^NOMIC_CASE_EVENT_ADD_PERSON%^RESET%^
.EI
.SP 7 5

This is the event type for a remove person event.

.EP

.SI 3
* %^BOLD%^NOMIC_CASE_EVENT_APPEAL%^RESET%^
.EI
.SP 7 5

This is the event type for an open event on a case.

.EP

.SI 3
* %^BOLD%^NOMIC_CASE_EVENT_CLOSE%^RESET%^
.EI
.SP 7 5

This is the event type for a close event on a case.

.EP

.SI 3
* %^BOLD%^NOMIC_CASE_EVENT_REMOVE_PERSON%^RESET%^
.EI
.SP 7 5

This is the event type for a add person event.

.EP

.SI 3
* %^BOLD%^NOMIC_CASE_SAVE%^RESET%^
.EI
.SP 7 5

This is the place where we will save the nomic information about
cases.

.EP

.SI 3
* %^BOLD%^NOMIC_DEFAULT_TYPE_NUMS%^RESET%^
.EI
.SP 7 5

This is the default set of types for the nomic system.

.EP

.SI 3
* %^BOLD%^NOMIC_ERROR%^RESET%^
.EI
.SP 7 5

The error define for nomic functions.

.EP

.SI 3
* %^BOLD%^NOMIC_HANDLER%^RESET%^
.EI
.SP 7 5

The path to the nomic rule handler.

.EP

.SI 3
* %^BOLD%^NOMIC_MOTION_TYPE_RULE_ADD%^RESET%^
.EI
.SP 7 5

This sets up the type of the new rule to be a real new rule.

.EP

.SI 3
* %^BOLD%^NOMIC_MOTION_TYPE_RULE_AMEND%^RESET%^
.EI
.SP 7 5

This changes the text of the rule.  Amends it.

.EP

.SI 3
* %^BOLD%^NOMIC_MOTION_TYPE_RULE_MOVE%^RESET%^
.EI
.SP 7 5

This moves the rule type from one rule set to another.

.EP

.SI 3
* %^BOLD%^NOMIC_MOTION_TYPE_RULE_REMOVE%^RESET%^
.EI
.SP 7 5

This removes the rule type from one rule set to another.

.EP

.SI 3
* %^BOLD%^NOMIC_PUNISHMENT_DIR%^RESET%^
.EI
.SP 7 5

This is the directory to find all the default nomic punishments from.

.EP

.SI 3
* %^BOLD%^NOMIC_PUNISHMENT_DOCS_DIR%^RESET%^
.EI
.SP 7 5

This is the directory to find all the punishment documentation.

.EP

.SI 3
* %^BOLD%^NOMIC_RULE_DEFAULT_DIR%^RESET%^
.EI
.SP 7 5

This is the directory to use to get the default rules from.

.EP

.SI 3
* %^BOLD%^NOMIC_STATE_CITIZEN_VOTE%^RESET%^
.EI
.SP 7 5

This is the state for when it is being voted on by the citizens.

.EP

.SI 3
* %^BOLD%^NOMIC_STATE_COUNCIL_REVIEW%^RESET%^
.EI
.SP 7 5

This is the state for when it is being looked at by the council.

.EP

.SI 3
* %^BOLD%^NOMIC_STATE_COUNCIL_VOTE%^RESET%^
.EI
.SP 7 5

This is the state for when it is being voted on by the council.

.EP

.SI 3
* %^BOLD%^NOMIC_SYSTEM_ADMINISTRATOR_NAME%^RESET%^
.EI
.SP 7 5

This is the name of the poster used to post messages to the boards.

.EP

.SI 3
* %^BOLD%^NOMIC_SYSTEM_SAVE_FILE%^RESET%^
.EI
.SP 7 5

This is where the system saves it's nomic rules.

.EP

.SI 3
* %^BOLD%^NOMIC_TYPE_CITIZEN%^RESET%^
.EI
.SP 7 5

The citizen rule type.  Only effects citizens.

.EP

.SI 3
* %^BOLD%^NOMIC_TYPE_GENERAL%^RESET%^
.EI
.SP 7 5

The general rule type.  Effects everyone.

.EP

.SI 3
* %^BOLD%^NOMIC_TYPE_IMMUTABLE%^RESET%^
.EI
.SP 7 5

The immutable rule type.  Effects everyone.

.EP

.SI 3
* %^BOLD%^NOMIC_VOTE_ABSTAIN%^RESET%^
.EI
.SP 7 5

This sets the abstain type of the vote.

.EP

.SI 3
* %^BOLD%^NOMIC_VOTE_NO%^RESET%^
.EI
.SP 7 5

This sets the no type of the vote.

.EP

.SI 3
* %^BOLD%^NOMIC_VOTE_YES%^RESET%^
.EI
.SP 7 5

This sets the yes type of the vote.

.EP

