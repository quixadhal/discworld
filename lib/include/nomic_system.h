/**
 * This is the include file to deal with the nomic system for the
 * player council.
 * @author Pinkfish
 * @started Fri Jun 23 20:10:40 PDT 2000
 */
#ifndef __NOMIC_SYSTEM__
/** @ignore yes */
#define __NOMIC_SYSTEM__

/**
 * The path to the nomic rule handler.
 */
#define NOMIC_HANDLER "/obj/handlers/player_council"

#ifndef NOMIC_SYSTEM_NO_CLASSES
/**
 * This is the class to handle amendments to the rule
 * @member amender the person doing the amendments
 * @member text the text of the amendment
 * @member date_amended when it was amended
 */
class nomic_amendment {
   string amender;
   string text;
   int date_amended;
}

/**
 * The information on nomic rules.
 * @member id the id of the rule (unique)
 * @member date_create when the rule was created
 * @member creator who created the rule
 * @member text the text of the rule
 * @member amendments the amendments to the rule
 */
class nomic_rule {
   int id;
   int type;
   int date_created;
   string creator;
   string text;
   class nomic_amendment* amendments;
}

/**
 * The information needed for each nomic area.
 * @member new_type_num the next numbers to use in each type area
 * @member rules the rules for the area
 */
class nomic_area {
   int* new_type_num;
   class nomic_rule* rules;
}

/**
 * Comment on this new rule thingy by magistrates.
 * @member text the text of the comment
 * @member commenter the person commenting
 * @member date_commented the date the comment was made
 */
class nomic_comment {
   string text;
   string commenter;
   int date_commented;
}

/**
 * This method sets up a new nomic rule for someone to vote on.
 * @member identifier the id of the rule
 * @member motion_type the type of the new rule
 * @member rule the new rule (or amended rule)
 * @member new_rule_type if the rule typ eis to change types, this is the new
 * type
 * @member comments any comments associated with the rule
 * @member yes_votes the current set of yes votes
 * @member no_votes the current set of no votes
 * @member abstain_votes the current set of abstain votes
 * @member voted the current bunch of people that have voted
 * @member state the state of the current rule
 * @member date_added the date this new rule discussion was added
 * @member date_event the date of the last state change event
 * @member added_by the person who added the rule change
 */
class nomic_motion {
   int identifier;
   int motion_type;
   class nomic_rule rule;
   int new_rule_type;
   class nomic_comment* comments;
   int yes_votes;
   int no_votes;
   int abstain_votes;
   string* voted;
   int state;
   int date_added;
   int date_event;
   string added_by;
}
#endif

#ifdef NOMIC_SYSTEM_CASE_CLASSES

/**
 * This is a note associated with the case.
 * @param id the id of the note
 * @param person the person who made the note
 * @param text the text of the note
 * @param time_created the time the note was created
 */
class nomic_case_note {
   int id;
   string person;
   string text;
   int time_created;
}

/**
 * This is an event associated with the case.
 * @param type the type of the event
 * @param mess the message associated with the event
 * @param event_time the time of the event
 * @param data any extra data associed with the event
 */
class nomic_case_event {
   int type;
   string mess;
   string person;
   int event_time;
   mixed data;
}

/**
 * This is the class for the thing.
 * @param id the id of the case
 * @param opener the person who opened the case
 * @param short the short of the case
 * @param people the people who are involved in the case
 * @param notes the list of ids of the notes
 * @param events the array of events for the case
 * @param time_opened the time the case was opened
 */
class nomic_case {
   int id;
   string opener;
   string short;
   string* people;
   int* notes;
   class nomic_case_event* events;
   int time_opened;
}

#endif

/**
 * This is the default set of types for the nomic system.
 */
#define NOMIC_DEFAULT_TYPE_NUMS ({ 100, 300, 2000 })

/**
 * The immutable rule type.  Effects everyone.
 */
#define NOMIC_TYPE_IMMUTABLE 0
/**
 * The general rule type.  Effects everyone.
 */
#define NOMIC_TYPE_GENERAL   1
/**
 * The citizen rule type.  Only effects citizens.
 */
#define NOMIC_TYPE_CITIZEN   2

/**
 * The error define for nomic functions.
 */
#define NOMIC_ERROR -1

/**
 * This sets up the type of the new rule to be a real new rule.
 */
#define NOMIC_MOTION_TYPE_RULE_ADD     1
/**
 * This changes the text of the rule.  Amends it.
 */
#define NOMIC_MOTION_TYPE_RULE_AMEND   2
/**
 * This moves the rule type from one rule set to another.
 */
#define NOMIC_MOTION_TYPE_RULE_MOVE    3
/**
 * This removes the rule type from one rule set to another.
 */
#define NOMIC_MOTION_TYPE_RULE_REMOVE  3

/**
 * This is the state for when it is being looked at by the council.
 */
#define NOMIC_STATE_COUNCIL_REVIEW 1
/**
 * This is the state for when it is being voted on by the council.
 */
#define NOMIC_STATE_COUNCIL_VOTE   2
/**
 * This is the state for when it is being voted on by the citizens.
 */
#define NOMIC_STATE_CITIZEN_VOTE   3

/**
 * This sets the yes type of the vote.
 */
#define NOMIC_VOTE_YES 1
/**
 * This sets the no type of the vote.
 */
#define NOMIC_VOTE_NO  2
/**
 * This sets the abstain type of the vote.
 */
#define NOMIC_VOTE_ABSTAIN  3

/**
 * This is the event type for a close event on a case.
 */
#define NOMIC_CASE_EVENT_CLOSE  1
/**
 * This is the event type for an open event on a case.
 */
#define NOMIC_CASE_EVENT_APPEAL 2
/**
 * This is the event type for a remove person event.
 */
#define NOMIC_CASE_EVENT_ADD_PERSON 3
/**
 * This is the event type for a add person event.
 */
#define NOMIC_CASE_EVENT_REMOVE_PERSON 4

/**
 * This is the directory to use to get the default rules from.
 */
#define NOMIC_RULE_DEFAULT_DIR "/save/nomic/default/"

/**
 * This is the directory to find all the default nomic punishments from.
 */
#define NOMIC_PUNISHMENT_DIR "/std/room/punishments/"
/**
 * This is the directory to find all the punishment documentation.
 */
#define NOMIC_PUNISHMENT_DOCS_DIR "/doc/punishments/"

/**
 * This is where the system saves it's nomic rules.
 */
#define NOMIC_SYSTEM_SAVE_FILE "/save/nomic/main.o"

/**
 * This is the place where we will save the nomic information about
 * cases.
 */
#define NOMIC_CASE_SAVE "/save/nomic/"

/**
 * This is the amount of time some has to appeal the decision.
 * 2 weeks.
 */
#define NOMIC_CASE_APPEAL_TIME (14 * 60 * 60 * 24)

/**
 * This is the name of the poster used to post messages to the boards.
 */
#define NOMIC_SYSTEM_ADMINISTRATOR_NAME "Administration"

#endif
