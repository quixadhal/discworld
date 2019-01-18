.DT
clubs.h
Disk World autodoc help
clubs.h

.SH Description
.SP 5 5

The main include file for clubs.  This contains all the defines and things
needed to interface to the club handler.
.EP
.SP 10 5


Written by Pinkfish

Started Sat Oct  3 22:30:30 EDT 1998

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^club_archive%^RESET%^
class club_archive {
int archive_date;
string subject;
string matter;
}

.EI
.SP 7 5

This class is used for the archives in elected clubs.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
subject - the subject of the archive
.EP
.SP 9 5
matter - the actual matter of the archive

.EP

.SI 3
* %^BOLD%^club_transaction%^RESET%^
class club_transaction {
string who;
int amount;
int transaction_type;
int date;
string information;
}

.EI
.SP 7 5

This class is used to keep track of financial transactions for
each club.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
who - who made the transaction
.EP
.SP 9 5
amount - Amount of the transaction
.EP
.SP 9 5
transaction_type - type of the transaction
.EP
.SP 9 5
date - the date at which the transaction occured
.EP
.SP 9 5
information - the information about the transaction

.EP

.SI 3
* %^BOLD%^discussion_idea%^RESET%^
class discussion_idea {
int id;
int time_added;
int time_finish;
int discuss_type;
string added_by;
string name;
string information;
mapping choices;
string * voted;
mixed optional_data;
}

.EI
.SP 7 5

This class is for a discussion idea associated with a club.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
id - the unique id of the discusion idea
.EP
.SP 9 5
time_added - the time the discussion item was added
.EP
.SP 9 5
discuss_type - the type of the item up for discusion
.EP
.SP 9 5
added_by - the name of the member who added the item
.EP
.SP 9 5
information - entered by the person when they added the discussion
.EP
.SP 9 5
name - the name of the person who added the item
.EP
.SP 9 5
choices - the choices and votes for the discussion
.EP
.SP 9 5
voted - who has voted for the item
.EP
.SP 9 5
optional_data - the extra data associated with the idea

.EP

.SI 3
* %^BOLD%^family_response_data%^RESET%^
class family_response_data {
string family;
string relationship;
}

.EI
.SP 7 5

This defines the class to use for accepting into families.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
family - the family name
.EP
.SP 9 5
relationship - the relationship type

.EP

.SI 3
* %^BOLD%^relationship_data%^RESET%^
class relationship_data {
string related_to;
string relationship;
}

.EI
.SP 7 5

This class defines the stuff which shows the relationships between the
two people.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
related_to - who they are related to
.EP
.SP 9 5
relationship - the relationship between the two people

.EP


.SH Defines
.SI 3
* %^BOLD%^CLUB_ARRAY%^RESET%^
.EI
.SP 7 5

String array to give the club type name.

.EP

.SI 3
* %^BOLD%^CLUB_BADGE%^RESET%^
.EI
.SP 7 5

The location of the club badge.

.EP

.SI 3
* %^BOLD%^CLUB_CONTROLLER_MEMBER%^RESET%^
.EI
.SP 7 5

The name of the club controller, when used as a member.

.EP

.SI 3
* %^BOLD%^CLUB_COST_INSIGNIA%^RESET%^
.EI
.SP 7 5

This is the cost of the inisgnia.

.EP

.SI 3
* %^BOLD%^CLUB_COST_PER_MEMBER_PER_YEAR%^RESET%^
.EI
.SP 7 5

The amount of money a club costs per member per year.  10 am pence.

.EP

.SI 3
* %^BOLD%^CLUB_COST_PER_YEAR%^RESET%^
.EI
.SP 7 5

This amount of money a club will cost to run each dw year.

.EP

.SI 3
* %^BOLD%^CLUB_DEFAULT_ACCOUNT_NAME%^RESET%^
.EI
.SP 7 5

The default name for the default account.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_ABSTAIN_CHOICE%^RESET%^
.EI
.SP 7 5

This is the choice to abstain from the vote.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_CREATE_ACCOUNT%^RESET%^
.EI
.SP 7 5

This type of item will add a new account to the club.
The name field will contain the name of the new account.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_DESCRIPTION%^RESET%^
.EI
.SP 7 5

This type is a discussion item to the change the clubs description.  The
information field will contain the new description and the choices field
should only contain 'yes' and 'no'.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_ELECTION%^RESET%^
.EI
.SP 7 5

This type of discussion item is used when a position become vacant
before an full election is due.  It is a by-election.  The
name field contains the position being elected and the choices contains
the list of people to choose the election from.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_FUNCTION%^RESET%^
.EI
.SP 7 5

This type of item will do calls onto a specified object to handle
all of the data and stuff.  The way this works is that the
entire discussion item will be passed onto the function, the function
is specified as a two element array in the name field of the discussion
item.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_FUNCTION_FUNCTION%^RESET%^
.EI
.SP 7 5

This is the index of the function in the name field when doing a
function discussion type.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_FUNCTION_OBJECT%^RESET%^
.EI
.SP 7 5

This is the index of the object in the name field when doing a
function discussion type.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MEMBER_REMOVE%^RESET%^
.EI
.SP 7 5

This type of item will be for discussing removing a member from the
club.  The name field contains the person being removed.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MEMO%^RESET%^
.EI
.SP 7 5

This type is a discussion item for a memo of some kind.  Means that it
is about whatever the players wish it to be about and results are mailed
to the people involed (or posted on their buliten board if they have one).

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_CHANGE_POSITION%^RESET%^
.EI
.SP 7 5

This message is sent when a position in the club is changed name.
The first arguement is the position being changed and the
second arguement is the position it is changed to.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_COMPLETE%^RESET%^
.EI
.SP 7 5

The message sent to the discussion item functions when the item
is completed.  This will send the mail to committee and all that
fun stuff.  In the case a CLUB_DISCUSS_FUNCTION if this returns
a string then the club_handler will deal with mailing the result
onto the committee and posting it and archiving it.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_LONG_DESCRIPTION%^RESET%^
.EI
.SP 7 5

The message sent to the discussion item functions when a long
description is requested.  This should be a detailed description
of the item, the choices are not neeeded to be included since they'
will be displayed automatically.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_NOMINATION%^RESET%^
.EI
.SP 7 5

The message sent to the discussion item function when a nomination
request is done.  The first argument will contain the person
doing the nomination and the second will contain the person being
nominated.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_NOMINATION_QUERY%^RESET%^
.EI
.SP 7 5

This message is sent to return the nomination mapping.  It must be
in the same format as descriped in /obj/handlers/inherit/club_discuss.c
.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_REMOVE_MEMBER%^RESET%^
.EI
.SP 7 5

This message is sent when a member is removed from the club.
The first argument contrains the member being removed.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_REMOVE_POSITION%^RESET%^
.EI
.SP 7 5

This message is sent when the position is removed from the club.
The first arguement contains the position being removed.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_SHORT_DESCRIPTION%^RESET%^
.EI
.SP 7 5

The message sent to the discussion item functions when a short
description is requested.  This should not have any new lines in
it and should be very short summary.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_TYPE%^RESET%^
.EI
.SP 7 5

This message is sent to determine the type of the discussion item.
Currently there are only two types, nomination and voting.  The
voting type means the 'vote' command is used for the discussion
item and the 'nomination' means the nominate type is used.
.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_TYPE_NOMINATE%^RESET%^
.EI
.SP 7 5

This is the return type to use to specify that the discussion item
is a nomination type.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_TYPE_VOTE%^RESET%^
.EI
.SP 7 5

This is the return type to use to specify that the discusion item
is a voting type.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_VERIFY%^RESET%^
.EI
.SP 7 5

The message sent to the discussion item functions when the item
is initialy added.  If this returns a non-zero value then it is
properly verified and the item will be added.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_MESSAGE_WITHDRAW%^RESET%^
.EI
.SP 7 5

This message is sent when someone is withdrawing from a position in 
a nomination discussion item.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_NEW_POSITION%^RESET%^
.EI
.SP 7 5

This type of item will be a discussion on adding in a new positon to
the committee.  The choices in this case should only contain 'yes' and
'no' and the name field will be the name of the new position.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_NO_CHOICE%^RESET%^
.EI
.SP 7 5

This is the choice which means no no no!

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_NO_CONFIDENCE%^RESET%^
.EI
.SP 7 5

This type is a no-confidence vote in the current committee, or in a
specific comittee member.  The name field contains the position the
no-confidence vote is in.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_NO_QUORUM%^RESET%^
.EI
.SP 7 5

Special flag for the type giving the quorum, or no quorum required
status of the discussion idea.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_NOMINATION%^RESET%^
.EI
.SP 7 5

This type of item will allow someone to be nominated for a position.
The choices element will be used to keep track of who has nominated
who for what.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_OPEN%^RESET%^
.EI
.SP 7 5

Special flag for the type giving the open or closed status of the
discussion idea.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_POSITION_NAME%^RESET%^
.EI
.SP 7 5

This type of item will be for changing the name of a position in
the committee.  The choices will be yes, no, abstain.  The
optional_data field will contain the new name and the name field will contain
the old name.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_POSITION_REMOVE%^RESET%^
.EI
.SP 7 5

This type of item will allow a position to be removed from the club.
The name field will contain the position being removed.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_REMOVE_ACCOUNT%^RESET%^
.EI
.SP 7 5

This type of item will add a new account to the club.
The name field will contain the name of the new account.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_SECRET_TYPE%^RESET%^
.EI
.SP 7 5

This type of item will be for discussing changing the club to a
secret type.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_TYPE_MASK%^RESET%^
.EI
.SP 7 5

Mask to only get the real type of the discussion item.

.EP

.SI 3
* %^BOLD%^CLUB_DISCUSS_YES_CHOICE%^RESET%^
.EI
.SP 7 5

This is the choice which means do it!

.EP

.SI 3
* %^BOLD%^CLUB_ELECTED%^RESET%^
.EI
.SP 7 5

The type to use to create a elected club type.  In an elected club all
the officals are elected now and then.
.EP

.SI 3
* %^BOLD%^CLUB_ELECTION_PERIOD%^RESET%^
.EI
.SP 7 5

This defines the amount of time between each election.  About 6 real
months and 2 am years.

.EP

.SI 3
* %^BOLD%^CLUB_ELECTION_TIME%^RESET%^
.EI
.SP 7 5

This defines the amount of time for the actuall election.

.EP

.SI 3
* %^BOLD%^CLUB_FAMILY%^RESET%^
.EI
.SP 7 5

The type to use to create a family.  A family is a special type of a
club.
.EP

.SI 3
* %^BOLD%^CLUB_FAMILY_RESPOND_TYPE%^RESET%^
.EI
.SP 7 5

This defines the type to use for families in the add_respond_command
section of club handling.
.EP

.SI 3
* %^BOLD%^CLUB_FLAGS_MASK%^RESET%^
.EI
.SP 7 5

This is the mask to get the flags from the type attribute.

.EP

.SI 3
* %^BOLD%^CLUB_HANDLER%^RESET%^
.EI
.SP 7 5

The location of the club handler.

.EP

.SI 3
* %^BOLD%^CLUB_INVALID_TIME%^RESET%^
.EI
.SP 7 5

This is an invalid time for the clubs election to be set to.

.EP

.SI 3
* %^BOLD%^CLUB_MAX_NAME_LEN%^RESET%^
.EI
.SP 7 5

This is the maximum length of a club name.

.EP

.SI 3
* %^BOLD%^CLUB_MAX_POSITION_NAME%^RESET%^
.EI
.SP 7 5

This is the maximum size that a position name can be.

.EP

.SI 3
* %^BOLD%^CLUB_MAXIMUM_ELECTED_MEMBERS%^RESET%^
.EI
.SP 7 5

This is the maximum number of members a club can have before it
must turn into an elected club.

.EP

.SI 3
* %^BOLD%^CLUB_MESSAGE_EMOTE_INDEX%^RESET%^
.EI
.SP 7 5

This defines the index of the emote flag in the message array.

.EP

.SI 3
* %^BOLD%^CLUB_MESSAGE_MESSAGE_INDEX%^RESET%^
.EI
.SP 7 5

This defines the index of the message itself in the message array.

.EP

.SI 3
* %^BOLD%^CLUB_MINIMUM_ELECTED_MEMBERS%^RESET%^
.EI
.SP 7 5

This is the minimum number of members a club must have before it can
become elected.

.EP

.SI 3
* %^BOLD%^CLUB_MINIMUM_ELECTION_PERIOD%^RESET%^
.EI
.SP 7 5

This defines the minimum amount of time for an election.

.EP

.SI 3
* %^BOLD%^CLUB_NOMINATION_TIME%^RESET%^
.EI
.SP 7 5

This defines the amount of time to call for nominations.

.EP

.SI 3
* %^BOLD%^CLUB_PAY_PERIOD%^RESET%^
.EI
.SP 7 5

This defines the amount of time for each pay period.

.EP

.SI 3
* %^BOLD%^CLUB_PERSONAL%^RESET%^
.EI
.SP 7 5

The type to use to create a personal club type.  In a personal club it
is run by one specific person, if that person leaves the mud for too
long or the dues are not paid then the club will cease to exist.
.EP

.SI 3
* %^BOLD%^CLUB_RESPOND_TYPE%^RESET%^
.EI
.SP 7 5

This defines the type to use for clubs in the add_respond_command
section of the club handling.
.EP

.SI 3
* %^BOLD%^CLUB_ROOM_CLUB_ONLY%^RESET%^
.EI
.SP 7 5

This makes the club control room a club only room, only club manipulations can
be done.
.EP

.SI 3
* %^BOLD%^CLUB_ROOM_CLUB_OR_FAMILY%^RESET%^
.EI
.SP 7 5

This makes the club control room a club or family room.
.EP

.SI 3
* %^BOLD%^CLUB_ROOM_FAMILY_ONLY%^RESET%^
.EI
.SP 7 5

This makes the club control room a family only room, only family manipulations can
be done.
.EP

.SI 3
* %^BOLD%^CLUB_SECRET_FLAG%^RESET%^
.EI
.SP 7 5

This is the flag to determine if the club is secret or not.

.EP

.SI 3
* %^BOLD%^CLUB_STARTUP_COST%^RESET%^
.EI
.SP 7 5

The amount it costs to startup a club.

.EP

.SI 3
* %^BOLD%^CLUB_TIMEOUT%^RESET%^
.EI
.SP 7 5

This defines the club timeout for the clubs.  8 weeks.

.EP

.SI 3
* %^BOLD%^CLUB_TRANSACTION_DEPOSIT%^RESET%^
.EI
.SP 7 5

Transaction type when someone deposits some money.

.EP

.SI 3
* %^BOLD%^CLUB_TRANSACTION_INSIGNIA%^RESET%^
.EI
.SP 7 5

Transaction type when someone requests some insignias.

.EP

.SI 3
* %^BOLD%^CLUB_TRANSACTION_START_BALANCE%^RESET%^
.EI
.SP 7 5

Transaction type for starting balance of the club.

.EP

.SI 3
* %^BOLD%^CLUB_TRANSACTION_TRANSFER%^RESET%^
.EI
.SP 7 5

Transaction type for transfering money from one account to another.

.EP

.SI 3
* %^BOLD%^CLUB_TRANSACTION_UPKEEP%^RESET%^
.EI
.SP 7 5

Transaction type for when the club has its yearly charges.

.EP

.SI 3
* %^BOLD%^CLUB_TRANSACTION_WITHDRAWL%^RESET%^
.EI
.SP 7 5

Transaction type for withdrawing some money from the club.

.EP

.SI 3
* %^BOLD%^CLUB_TYPE_MASK%^RESET%^
.EI
.SP 7 5

This is the mask to get the type from the type attribute.

.EP

.SI 3
* %^BOLD%^CLUB_UNKNOWN_MEMBER%^RESET%^
.EI
.SP 7 5

The name given to members that have been remove from positions.

.EP

.SI 3
* %^BOLD%^FAMILY_COST_PER_MEMBER_PER_YEAR%^RESET%^
.EI
.SP 7 5

The amount of money a family costs per member per year.  10 am pence.

.EP

.SI 3
* %^BOLD%^FAMILY_COST_PER_YEAR%^RESET%^
.EI
.SP 7 5

This amount of money a family will cost to run each dw year.

.EP

.SI 3
* %^BOLD%^FAMILY_STARTUP_COST%^RESET%^
.EI
.SP 7 5

The amount it costs to startup a family.

.EP

.SI 3
* %^BOLD%^MAX_FAMILY_NAME_LEN%^RESET%^
.EI
.SP 7 5

The maximum length of a family name, with spaces.

.EP

