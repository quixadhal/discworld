.DT
club_discuss.c
Disk World autodoc help
club_discuss.c

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
This class inherits the following classes /obj/handlers/inherit/club_elected.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/am_time.h, /include/broadcaster.h and /include/clubs.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_discussion_item%^RESET%^
.EI
.SI 5
int add_discussion_item(string club, int type, string added_by, string name, string info, string * choices, int time_to_finish, mixed extra_data)
.EI
.SP 7 5

This method will add a discussion item to the table to be voted on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the club the item is being added to
.EP
.SP 9 5
type - the type of the discussion item to be added
.EP
.SP 9 5
added_by - who added the discussion item
.EP
.SP 9 5
name - the name of the discussion item
.EP
.SP 9 5
choices - the choices for the discussion item
.EP
.SP 9 5
time_to_finish - the amount of time the election has to run
.EP
.SP 9 5
extra_data - any optional data associated with the discussion idea
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successfully added, 0 if not

.EP

.SI 3
* %^BOLD%^can_reference_discussion_item%^RESET%^
.EI
.SI 5
int can_reference_discussion_item(string club_name, int id, string person)
.EI
.SP 7 5

This method checks to see if the specified person can see the
discussion idea. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion idea
.EP
.SP 9 5
person - the name of the person
.EP

.SI 3
* %^BOLD%^complete_discussion_item%^RESET%^
.EI
.SI 5
int complete_discussion_item(string club_name, int id)
.EI
.SP 7 5

This method completes the discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if not
.EP

.SI 3
* %^BOLD%^has_voted_for_discussion_item%^RESET%^
.EI
.SI 5
int has_voted_for_discussion_item(string club_name, int id, string person)
.EI
.SP 7 5

This method checks to see if the specified person has already voted for
this discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SP 9 5
person - the name of the person to check

.EP

.SI 3
* %^BOLD%^is_committee_discussion_item%^RESET%^
.EI
.SI 5
int is_committee_discussion_item(string club_name, int id)
.EI
.SP 7 5

This method checks to see if the discussion item is a committee only
item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is a committee only item, 0 if it is open

.EP

.SI 3
* %^BOLD%^is_discuss_nominateable%^RESET%^
.EI
.SI 5
int is_discuss_nominateable(string club_name, int id)
.EI
.SP 7 5

This method checks to see if the discussion item is a votable
item or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is votable, 0 if not

.EP

.SI 3
* %^BOLD%^is_discuss_person_nominated%^RESET%^
.EI
.SI 5
int is_discuss_person_nominated(string club_name, int id, string nominator, string nominated)
.EI
.SP 7 5

This method checks to see if the given nomination has already been 
done.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion ite,
.EP
.SP 9 5
nominator - the person doing the nomination
.EP
.SP 9 5
nominated - the person being nominated
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are already nominated, 0 if not

.EP

.SI 3
* %^BOLD%^is_discuss_votable%^RESET%^
.EI
.SI 5
int is_discuss_votable(string club_name, int id)
.EI
.SP 7 5

This method checks to see if the discussion item is a votable
item or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is votable, 0 if not

.EP

.SI 3
* %^BOLD%^is_discussion_item_finished%^RESET%^
.EI
.SI 5
int is_discussion_item_finished(string club_name, int id)
.EI
.SP 7 5

This method checks to see if the discussion item is completed yet.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the discussion item is finished
.EP

.SI 3
* %^BOLD%^is_discussion_item_quorum%^RESET%^
.EI
.SI 5
int is_discussion_item_quorum(string club_name, int id)
.EI
.SP 7 5

This method checks to see if quorum has been reached or not
for the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to check for quorum
.EP
.SP 9 5
id - the discussion item id
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if quorum is reached, 0 if not

.EP

.SI 3
* %^BOLD%^is_discussion_position_valid%^RESET%^
.EI
.SI 5
int is_discussion_position_valid(string club_name, string position)
.EI
.SP 7 5

This is a special function to be used with the discuss items to
make sure that there is not another discussion item checking the
same name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to check
.EP
.SP 9 5
position - the position to check for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the position is being discussed as an item

.EP

.SI 3
* %^BOLD%^is_no_quorum_discussion_item%^RESET%^
.EI
.SI 5
int is_no_quorum_discussion_item(string club_name, int id)
.EI
.SP 7 5

This method checks to see if the discussion item has no quorum
associated with it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it has no quorum, 0 if it is does

.EP

.SI 3
* %^BOLD%^is_valid_discussion_choice%^RESET%^
.EI
.SI 5
int is_valid_discussion_choice(string club_name, int id, string choice)
.EI
.SP 7 5

This checks to see if the specified choice is a real discussion item
choice.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SP 9 5
choice - the choice to check

.EP

.SI 3
* %^BOLD%^is_valid_discussion_item_type%^RESET%^
.EI
.SI 5
int is_valid_discussion_item_type(int type)
.EI
.SP 7 5

This method checks to make sure the type of the discussion item
is valid.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is valid, 0 if not

.EP

.SI 3
* %^BOLD%^nominate_discussion_item%^RESET%^
.EI
.SI 5
int nominate_discussion_item(string club_name, int id, string nominator, string nominated)
.EI
.SP 7 5

This message does the nomination thing for those discussion items
which accept nominations.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SP 9 5
nominator - the person doing the nomination
.EP
.SP 9 5
nominated - the person nominated

.EP

.SI 3
* %^BOLD%^query_all_discussion_items%^RESET%^
.EI
.SI 5
class discussion_idea * query_all_discussion_items(string club_name)
.EI
.SP 7 5

This method returns all the current discussion items for this
club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to query the discussion items for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of current discussion items

.EP

.SI 3
* %^BOLD%^query_discussion_added_by%^RESET%^
.EI
.SI 5
string query_discussion_added_by(string club_name, int id)
.EI
.SP 7 5

This method returns the person who added the discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the person who added the discussion item

.EP

.SI 3
* %^BOLD%^query_discussion_choices%^RESET%^
.EI
.SI 5
string * query_discussion_choices(string club_name, int id)
.EI
.SP 7 5

This method returns all the choices for this discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of the valid set of choices, empty array on failure

.EP

.SI 3
* %^BOLD%^query_discussion_item%^RESET%^
.EI
.SI 5
class discussion_idea query_discussion_item(string club_name, int id)
.EI
.SP 7 5

This method figures out the discussion item for the club with the
given id.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the found discussion item
.EP

.SI 3
* %^BOLD%^query_discussion_item_by_type%^RESET%^
.EI
.SI 5
class discussion_idea query_discussion_item_by_type(string club_name, int type, class discussion_idea idea)
.EI
.SP 7 5

This method figures out the discussion item for the club with the
given type.  It will also continue on a search from a last found item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
type - the type of the discussion item to find
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the found discussion item
.EP

.SI 3
* %^BOLD%^query_discussion_item_long_description%^RESET%^
.EI
.SI 5
string query_discussion_item_long_description(string club_name, int id)
.EI
.SP 7 5

This method returns a nice little description of the discussion item.
This is a longer description, useful in more places.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a long description of the item

.EP

.SI 3
* %^BOLD%^query_discussion_item_number_voted%^RESET%^
.EI
.SI 5
int query_discussion_item_number_voted(string club_name, int id)
.EI
.SP 7 5

This method returns the number of people who have voted for the
discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to check for quorum
.EP
.SP 9 5
id - the discussion item id
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of people who have voted

.EP

.SI 3
* %^BOLD%^query_discussion_item_quorum_number%^RESET%^
.EI
.SI 5
int query_discussion_item_quorum_number(string club_name, int id)
.EI
.SP 7 5

This method returns the number of people needed for quorum
in this discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to check for quorum
.EP
.SP 9 5
id - the discussion item id
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of people needed for quorum

.EP

.SI 3
* %^BOLD%^query_discussion_item_short_description%^RESET%^
.EI
.SI 5
string query_discussion_item_short_description(string club_name, int id)
.EI
.SP 7 5

This method returns a nice little description of the discussion item.
This is a very short description usable in lists and stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a short description of the item

.EP

.SI 3
* %^BOLD%^query_discussion_nomination%^RESET%^
.EI
.SI 5
mapping query_discussion_nomination(string club_name, int id)
.EI
.SP 7 5

This method returns the nomination mapping for the discussion item
if it exists.  The nomination mapping is of the format:
.EP
.SP 7 5

.EP
.SP 7 5
([
.EP
.SP 7 5
   name : ({ nomated_by, nominated_by, ... })
.EP
.SP 7 5
   ..
.EP
.SP 7 5
 ])
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping as described above

.EP

.SI 3
* %^BOLD%^query_discussion_optional_data%^RESET%^
.EI
.SI 5
mixed query_discussion_optional_data(string club_name, int id)
.EI
.SP 7 5

This method returns the optional data of the discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the optional data in the club

.EP

.SI 3
* %^BOLD%^query_discussion_time_added%^RESET%^
.EI
.SI 5
int query_discussion_time_added(string club_name, int id)
.EI
.SP 7 5

This method returns the timeout time of the current discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time at which the dicussion item will finish

.EP

.SI 3
* %^BOLD%^query_discussion_time_finish%^RESET%^
.EI
.SI 5
int query_discussion_time_finish(string club_name, int id)
.EI
.SP 7 5

This method returns the timeout time of the current discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the time at which the dicussion item will finish

.EP

.SI 3
* %^BOLD%^setup_by_election%^RESET%^
.EI
.SI 5
int setup_by_election(string club, string position, string info)
.EI
.SP 7 5

This method setups up a by-election for the specified position in the
club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the club to make a byelection in
.EP
.SP 9 5
position - the position the byelection is for
.EP
.SP 9 5
info - the information about the vote
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^vote_for_discussion_item%^RESET%^
.EI
.SI 5
int vote_for_discussion_item(string club_name, int id, string person, string choice)
.EI
.SP 7 5

This method sets up the discussion item as being voted by the
person specified.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SP 9 5
person - the person doing the voting
.EP
.SP 9 5
choice - the choices there are to vote on
.EP

.SI 3
* %^BOLD%^withdraw_discussion_item%^RESET%^
.EI
.SI 5
int withdraw_discussion_item(string club_name, int id, string withdraw)
.EI
.SP 7 5

This message does the withdraw thing for those discussion items
which accept nominations.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
id - the id of the discussion item
.EP
.SP 9 5
withdraw - the person being withdrawn

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^archive_discussion_item%^RESET%^
.EI
.SI 5
int archive_discussion_item(string club_name, string subject, string item)
.EI
.SP 7 5

This method is called when a discussion item is finished.  It then
archives it or whatever else it wishes to do with the item
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
item - the text string of the finished item

.EP

.SI 3
* %^BOLD%^broadcast_discuss_message%^RESET%^
.EI
.SI 5
void broadcast_discuss_message(string club_name, int message, string arg1, string arg2)
.EI
.SP 7 5

This method sends a message to all the discussion items in the club
and then marks the club as changed if any of them return a non-zero
value.  This is used for events like a member being removed of a
position being changed in name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
message - the message number
.EP
.SP 9 5
arg1 - the first arguement
.EP
.SP 9 5
arg2 - the second arguement

.EP

.SI 3
* %^BOLD%^remove_discussion_item%^RESET%^
.EI
.SI 5
int remove_discussion_item(string club_name, class discussion_idea item)
.EI
.SP 7 5

This method removes a discussion item from the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to remove the discussion item
.EP
.SP 9 5
item - the discusion item to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^send_discuss_message%^RESET%^
.EI
.SI 5
mixed send_discuss_message(string club_name, int message, class discussion_idea item, string arg1, string arg2)
.EI
.SP 7 5

This method sends a message to all the discussion items in the club
and then marks the club as changed if any of them return a non-zero
value.  This is used for events like a member being removed of a
position being changed in name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
message - the message number
.EP
.SP 9 5
arg1 - the first arguement
.EP
.SP 9 5
arg2 - the second arguement

.EP


