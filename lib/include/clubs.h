/**
 * The main include file for clubs.  This contains all the defines and things
 * needed to interface to the club handler.
 * @author Pinkfish
 * @started Sat Oct  3 22:30:30 EDT 1998
 */
#ifndef __CLUBS_H
/** @ignore yes */
#define __CLUBS_H

#include <am_time.h>

/**
 * The type to use to create a personal club type.  In a personal club it
 * is run by one specific person, if that person leaves the mud for too
 * long or the dues are not paid then the club will cease to exist.
 * @see /obj/handlers/club_handler->create_club()
 */
#define CLUB_PERSONAL  0
/**
 * The type to use to create a elected club type.  In an elected club all
 * the officals are elected now and then.
 * @see /obj/handlers/club_handler->create_club()
 */
#define CLUB_ELECTED   1
/**
 * The type to use to create a family.  A family is a special type of a
 * club.
 * @see /obj/handlers/club_handler->create_club()
 */
#define CLUB_FAMILY    2

/**
 * String array to give the club type name.
 */
#define CLUB_ARRAY ({"Personal", "Elected", "Family"})

/**
 * The default name for the default account.
 */
#define CLUB_DEFAULT_ACCOUNT_NAME "default"

/**
 * This is the mask to get the type from the type attribute.
 */
#define CLUB_TYPE_MASK (15)
/**
 * This is the mask to get the flags from the type attribute.
 */
#define CLUB_FLAGS_MASK (~CLUB_TYPE_MASK)
/**
 * This is the flag to determine if the club is secret or not.
 */
#define CLUB_SECRET_FLAG 16

/**
 * This amount of money a club will cost to run each dw year.
 */
#define CLUB_COST_PER_YEAR (400 * 50)
/**
 * The amount it costs to startup a club.
 */
#define CLUB_STARTUP_COST (400 * 100)
/**
 * The amount it costs to startup a family.
 */
#define FAMILY_STARTUP_COST (400 * 300)
/**
 * This amount of money a family will cost to run each dw year.
 */
#define FAMILY_COST_PER_YEAR (400 * 300)
/**
 * The amount of money a club costs per member per year.  10 am pence.
 */
#define CLUB_COST_PER_MEMBER_PER_YEAR 100
/**
 * The amount of money a family costs per member per year.  10 am pence.
 */
#define FAMILY_COST_PER_MEMBER_PER_YEAR 200
/**
 * This is the cost of the inisgnia.
 */
#define CLUB_COST_INSIGNIA (400)

/**
 * This is the minimum number of members a club must have before it can
 * become elected.
 */
#define CLUB_MINIMUM_ELECTED_MEMBERS 10
/**
 * This is the maximum number of members a club can have before it
 * must turn into an elected club.
 */
#define CLUB_MAXIMUM_ELECTED_MEMBERS 30

/**
 * The location of the club handler.
 */
#define CLUB_HANDLER "/obj/handlers/club_handler"
/**
 * The location of the club badge.
 */
#define CLUB_BADGE "/obj/misc/club_badge"

/**
 * This is an invalid time for the clubs election to be set to.
 */
#define CLUB_INVALID_TIME 0

/**
 * This defines the club timeout for the clubs.  8 weeks.
 */
#define CLUB_TIMEOUT (8 * 7 * 24 * 60 * 60)
/**
 * This defines the amount of time for each pay period.
 */
#define CLUB_PAY_PERIOD (AM_SECONDS_PER_HALF_YEAR / 4)
/**
 * This defines the amount of time between each election.  About 6 real
 * months and 2 am years.
 */
#define CLUB_ELECTION_PERIOD (AM_SECONDS_PER_HALF_YEAR * 2)
/**
 * This defines the minimum amount of time for an election.
 */
#define CLUB_MINIMUM_ELECTION_PERIOD (AM_SECONDS_PER_HALF_YEAR / 2)
/**
 * This defines the amount of time to call for nominations.
 */
#define CLUB_NOMINATION_TIME (7 * 24 * 60 * 60)
/**
 * This defines the amount of time for the actuall election.
 */
#define CLUB_ELECTION_TIME (7 * 24 * 60 * 60)

/**
 * This defines the type to use for clubs in the add_respond_command
 * section of the club handling.
 * @see /std/living/respond_cmd.c
 */
#define CLUB_RESPOND_TYPE "club"
/**
 * This defines the type to use for families in the add_respond_command
 * section of club handling.
 * @see /std/living/respond_cmd.c
 */
#define CLUB_FAMILY_RESPOND_TYPE "family"

#ifndef __CLUBS_H_NO_CLASSES
/**
 * This defines the class to use for accepting into families.
 * @member family the family name
 * @member relationship the relationship type
 */
class family_response_data {
   string family;
   string relationship;
}

/**
 * This class defines the stuff which shows the relationships between the
 * two people.
 * @member related_to who they are related to
 * @member relationship the relationship between the two people
 */
class relationship_data {
   string related_to;
   string relationship;
}
#endif

/**
 * Transaction type when someone requests some insignias.
 */
#define CLUB_TRANSACTION_INSIGNIA 1
/**
 * Transaction type when someone deposits some money.
 */
#define CLUB_TRANSACTION_DEPOSIT 2
/**
 * Transaction type for when the club has its yearly charges.
 */
#define CLUB_TRANSACTION_UPKEEP 3
/**
 * Transaction type for starting balance of the club.
 */
#define CLUB_TRANSACTION_START_BALANCE 4
/**
 * Transaction type for withdrawing some money from the club.
 */
#define CLUB_TRANSACTION_WITHDRAWL 5
/**
 * Transaction type for transfering money from one account to another.
 */
#define CLUB_TRANSACTION_TRANSFER 6

#ifndef __CLUBS_H_NO_CLASSES
/**
 * This class is used to keep track of financial transactions for
 * each club.
 * @member who who made the transaction
 * @member amount Amount of the transaction
 * @member transaction_type type of the transaction
 * @member date the date at which the transaction occured
 * @member information the information about the transaction
 */
class club_transaction {
   string who;
   int amount;
   int transaction_type;
   int date;
   string information;
}

/**
 * This class is used for the archives in elected clubs.
 * @memver archive_date the date at which the archived element was added
 * @member subject the subject of the archive
 * @member matter the actual matter of the archive
 */
class club_archive {
   int archive_date;
   string subject;
   string matter;
}
#endif

/** 
 * This defines the index of the emote flag in the message array.
 */
#define CLUB_MESSAGE_EMOTE_INDEX 1
/**
 * This defines the index of the message itself in the message array.
 */
#define CLUB_MESSAGE_MESSAGE_INDEX 0

/**
 * This is the maximum length of a club name.
 */
#define CLUB_MAX_NAME_LEN 40
/**
 * The maximum length of a family name, with spaces.
 */
#define MAX_FAMILY_NAME_LEN 15

/**
 * This makes the club control room a club or family room.
 * @see /std/room/club_control.c
 */
#define CLUB_ROOM_CLUB_OR_FAMILY 0
/**
 * This makes the club control room a club only room, only club manipulations can
 * be done.
 * @see /std/room/club_control.c
 */
#define CLUB_ROOM_CLUB_ONLY   1
/**
 * This makes the club control room a family only room, only family manipulations can
 * be done.
 * @see /std/room/club_control.c
 */
#define CLUB_ROOM_FAMILY_ONLY 2

#ifndef __CLUBS_H_NO_CLASSES
/**
 * This class is for a discussion idea associated with a club.
 * @member id the unique id of the discusion idea
 * @member time_added the time the discussion item was added
 * @member discuss_type the type of the item up for discusion
 * @member added_by the name of the member who added the item
 * @member information entered by the person when they added the discussion
 * @member name the name of the person who added the item
 * @member choices the choices and votes for the discussion
 * @member voted who has voted for the item
 * @member optional_data the extra data associated with the idea
 */
class discussion_idea {
   int id;
   int time_added;
   int time_finish;
   int discuss_type;
   string added_by;
   string name;
   string information;
   mapping choices;
   string *voted;
   mixed optional_data;
}
#endif

/**
 * Special flag for the type giving the open or closed status of the
 * discussion idea.
 */
#define CLUB_DISCUSS_OPEN 0x800000
/**
 * Special flag for the type giving the quorum, or no quorum required
 * status of the discussion idea.
 */
#define CLUB_DISCUSS_NO_QUORUM 0x400000
/**
 * Mask to only get the real type of the discussion item.
 */
#define CLUB_DISCUSS_TYPE_MASK 0xff

/**
 * This type of discussion item is used when a position become vacant
 * before an full election is due.  It is a by-election.  The
 * name field contains the position being elected and the choices contains
 * the list of people to choose the election from.
 */
#define CLUB_DISCUSS_ELECTION      1
/**
 * This type is a discussion item to the change the clubs description.  The
 * information field will contain the new description and the choices field
 * should only contain 'yes' and 'no'.
 */
#define CLUB_DISCUSS_DESCRIPTION   2
/**
 * This type is a discussion item for a memo of some kind.  Means that it
 * is about whatever the players wish it to be about and results are mailed
 * to the people involed (or posted on their buliten board if they have one).
 */
#define CLUB_DISCUSS_MEMO          3
/**
 * This type is a no-confidence vote in the current committee, or in a
 * specific comittee member.  The name field contains the position the
 * no-confidence vote is in.
 */
#define CLUB_DISCUSS_NO_CONFIDENCE 4
/**
 * This type of item will be a discussion on adding in a new positon to
 * the committee.  The choices in this case should only contain 'yes' and
 * 'no' and the name field will be the name of the new position.
 */
#define CLUB_DISCUSS_NEW_POSITION 5
/**
 * This type of item will be for changing the name of a position in
 * the committee.  The choices will be yes, no, abstain.  The
 * optional_data field will contain the new name and the name field will contain
 * the old name.
 */
#define CLUB_DISCUSS_POSITION_NAME 6
/**
 * This type of item will be for discussing changing the club to a
 * secret type.
 */
#define CLUB_DISCUSS_SECRET_TYPE 7
/**
 * This type of item will be for discussing removing a member from the
 * club.  The name field contains the person being removed.
 */
#define CLUB_DISCUSS_MEMBER_REMOVE 8
/**
 * This type of item will allow a position to be removed from the club.
 * The name field will contain the position being removed.
 */
#define CLUB_DISCUSS_POSITION_REMOVE 9
/**
 * This type of item will allow someone to be nominated for a position.
 * The choices element will be used to keep track of who has nominated
 * who for what.
 */
#define CLUB_DISCUSS_NOMINATION 10
/**
 * This type of item will do calls onto a specified object to handle
 * all of the data and stuff.  The way this works is that the
 * entire discussion item will be passed onto the function, the function
 * is specified as a two element array in the name field of the discussion
 * item.
 */
#define CLUB_DISCUSS_FUNCTION 11
/**
 * This type of item will add a new account to the club.
 * The name field will contain the name of the new account.
 */
#define CLUB_DISCUSS_CREATE_ACCOUNT 12
/**
 * This type of item will add a new account to the club.
 * The name field will contain the name of the new account.
 */
#define CLUB_DISCUSS_REMOVE_ACCOUNT 13

/**
 * This is the choice which means do it!
 */
#define CLUB_DISCUSS_YES_CHOICE "yes"
/**
 * This is the choice which means no no no!
 */
#define CLUB_DISCUSS_NO_CHOICE "no"
/**
 * This is the choice to abstain from the vote.
 */
#define CLUB_DISCUSS_ABSTAIN_CHOICE "abstain"

/**
 * The message sent to the discussion item functions when the item
 * is initialy added.  If this returns a non-zero value then it is
 * properly verified and the item will be added.
 */
#define CLUB_DISCUSS_MESSAGE_VERIFY            1
/**
 * The message sent to the discussion item functions when the item
 * is completed.  This will send the mail to committee and all that
 * fun stuff.  In the case a CLUB_DISCUSS_FUNCTION if this returns
 * a string then the club_handler will deal with mailing the result
 * onto the committee and posting it and archiving it.
 */
#define CLUB_DISCUSS_MESSAGE_COMPLETE          2
/**
 * The message sent to the discussion item functions when a short
 * description is requested.  This should not have any new lines in
 * it and should be very short summary.
 */
#define CLUB_DISCUSS_MESSAGE_SHORT_DESCRIPTION 3
/**
 * The message sent to the discussion item functions when a long
 * description is requested.  This should be a detailed description
 * of the item, the choices are not neeeded to be included since they'
 * will be displayed automatically.
 */
#define CLUB_DISCUSS_MESSAGE_LONG_DESCRIPTION  4
/**
 * The message sent to the discussion item function when a nomination
 * request is done.  The first argument will contain the person
 * doing the nomination and the second will contain the person being
 * nominated.
 */
#define CLUB_DISCUSS_MESSAGE_NOMINATION        5
/**
 * This message is sent to determine the type of the discussion item.
 * Currently there are only two types, nomination and voting.  The
 * voting type means the 'vote' command is used for the discussion
 * item and the 'nomination' means the nominate type is used.
 * @see CLUB_DISCUSS_MESSAGE_TYPE_VOTE
 * @see CLUB_DISCUSS_MESSAGE_TYPE_NOMINATE
 */
#define CLUB_DISCUSS_MESSAGE_TYPE              6
/**
 * This message is sent when a member is removed from the club.
 * The first argument contrains the member being removed.
 */
#define CLUB_DISCUSS_MESSAGE_REMOVE_MEMBER     7
/**
 * This message is sent when a position in the club is changed name.
 * The first arguement is the position being changed and the
 * second arguement is the position it is changed to.
 */
#define CLUB_DISCUSS_MESSAGE_CHANGE_POSITION   8
/**
 * This message is sent to return the nomination mapping.  It must be
 * in the same format as descriped in /obj/handlers/inherit/club_discuss.c
 * @see /obj/handlers/inherit/club_discuss.c
 */
#define CLUB_DISCUSS_MESSAGE_NOMINATION_QUERY  9
/**
 * This message is sent when the position is removed from the club.
 * The first arguement contains the position being removed.
 */
#define CLUB_DISCUSS_MESSAGE_REMOVE_POSITION   10
/**
 * This message is sent when someone is withdrawing from a position in 
 * a nomination discussion item.
 */
#define CLUB_DISCUSS_MESSAGE_WITHDRAW          11

// Make the type default to votable.
/**
 * This is the return type to use to specify that the discusion item
 * is a voting type.
 */
#define CLUB_DISCUSS_MESSAGE_TYPE_VOTE         0
/**
 * This is the return type to use to specify that the discussion item
 * is a nomination type.
 */
#define CLUB_DISCUSS_MESSAGE_TYPE_NOMINATE     1

/**
 * This is the index of the object in the name field when doing a
 * function discussion type.
 */
#define CLUB_DISCUSS_FUNCTION_OBJECT     0
/**
 * This is the index of the function in the name field when doing a
 * function discussion type.
 */
#define CLUB_DISCUSS_FUNCTION_FUNCTION   1

/**
 * This is the maximum size that a position name can be.
 */
#define CLUB_MAX_POSITION_NAME 30

/**
 * The name given to members that have been remove from positions.
 */
#define CLUB_UNKNOWN_MEMBER "unknown"
/**
 * The name of the club controller, when used as a member.
 */
#define CLUB_CONTROLLER_MEMBER "Club Controller"

#endif
