.DT
club_basic.c
Disk World autodoc help
club_basic.c

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

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/am_time.h, /include/broadcaster.h and /include/clubs.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^club_info%^RESET%^
class club_info {
string actual_name;
string founder;
string * recruiters;
string * members;
int type;
mapping accounts;
int last_paid;
int last_touched;
mixed extra_data;
string description;
string region;
}

.EI
.SP 7 5

The basic club information class.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
actual_name - the actual name of the club
.EP
.SP 9 5
founder - the founder of the club
.EP
.SP 9 5
recruiters - the people who can recruit for the club
.EP
.SP 9 5
members - the members of the club
.EP
.SP 9 5
type - the type of the club
.EP
.SP 9 5
accounts - the accounts in the club
.EP
.SP 9 5
last_paid - when the balance was last paid
.EP
.SP 9 5
last_touched - when the club was last touched
.EP
.SP 9 5
extra_data - specific data for each type of club
.EP
.SP 9 5
region - the region the club is in

.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_member%^RESET%^
.EI
.SI 5
int add_member(string name, string member)
.EI
.SP 7 5

This method adds a member to the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to add the recruiter to
.EP
.SP 9 5
member - the member of the club to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^add_money%^RESET%^
.EI
.SI 5
int add_money(string name, int amount, int type, string person, string account)
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
amount - the amount to change the balance by
.EP
.SP 9 5
type - the tyope of the transaction
.EP
.SP 9 5
person - the person removeing the money
.EP
.SP 9 5
account - the account the money is coming from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount of money not able to be placed in the account
.EP

.SI 3
* %^BOLD%^add_observer%^RESET%^
.EI
.SI 5
int add_observer(string obs)
.EI
.SP 7 5

Adds an objec to the list to be informed of changes about the
clubs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the name of the object to inform of changes
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^add_recruiter%^RESET%^
.EI
.SI 5
int add_recruiter(string name, string recruiter)
.EI
.SP 7 5

This method adds a recruiter to the club.  A recruiter can only be added if
they are already a member.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the club name to add the recruiter to
.EP
.SP 9 5
recruiter - the recruiter of the club to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^change_club_type%^RESET%^
.EI
.SI 5
int change_club_type(string name, int type)
.EI
.SP 7 5

This method changes the type of the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to change
.EP
.SP 9 5
type - the new type of the club
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^create_account%^RESET%^
.EI
.SI 5
int create_account(string name, string account)
.EI
.SP 7 5

This method creates an account in the club.
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

.SI 3
* %^BOLD%^create_club%^RESET%^
.EI
.SI 5
int create_club(string name, string founder, int type, string region)
.EI
.SP 7 5

This method creates a club.  The founder and the recruiter set is
initialy set to the founder.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club
.EP
.SP 9 5
founder - the founder of the club
.EP
.SP 9 5
region - the region of the club
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 was able to create the club, 0 if unable to create the club
.EP

.SI 3
* %^BOLD%^disband_club%^RESET%^
.EI
.SI 5
int disband_club(string name)
.EI
.SP 7 5

This method disbands the club.  The club will be totaly zapped and
everything about it efficently munched.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to disband
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^is_account_of%^RESET%^
.EI
.SI 5
int is_account_of(string club_name, string account)
.EI
.SP 7 5

This method checks to see if the account exists for the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the name
.EP
.SP 9 5
account - the name of the account to checlk
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it exists, 0 if it does not

.EP

.SI 3
* %^BOLD%^is_club%^RESET%^
.EI
.SI 5
int is_club(string name)
.EI
.SP 7 5

This method checks to see if the specified club exists.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check for existance
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is a club, 0 if not
.EP

.SI 3
* %^BOLD%^is_creator_club%^RESET%^
.EI
.SI 5
int is_creator_club(string club_name)
.EI
.SP 7 5

This method determines if the club is a creator club or not.  A
club is considered a creator club if the founder is a creator.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if is a creator club, 0 if not

.EP

.SI 3
* %^BOLD%^is_elected_club%^RESET%^
.EI
.SI 5
int is_elected_club(string name)
.EI
.SP 7 5

This method checks to see if the specified club exists and is an
elected club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check to see for an elected type
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the club is an elected type

.EP

.SI 3
* %^BOLD%^is_family%^RESET%^
.EI
.SI 5
int is_family(string name)
.EI
.SP 7 5

This method checks to see if the club type is actually a family.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the family to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the club is a family

.EP

.SI 3
* %^BOLD%^is_founder_of%^RESET%^
.EI
.SI 5
int is_founder_of(string name, string founder)
.EI
.SP 7 5

This method will determine if the specified person is the founder of
the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check the founder of
.EP
.SP 9 5
founder - the person to check for being the founder
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are in the position, 0 if not
.EP

.SI 3
* %^BOLD%^is_member_of%^RESET%^
.EI
.SI 5
int is_member_of(string name, string member)
.EI
.SP 7 5

This method will determine if the specified person is a member of the
club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to find the member of
.EP
.SP 9 5
member - the member to check for the existance of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a member, 0 if they are not

.EP

.SI 3
* %^BOLD%^is_observer%^RESET%^
.EI
.SI 5
int is_observer(string obs)
.EI
.SP 7 5

This method checks to see if the specified thingy is an observer.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the observer to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^is_personal_club%^RESET%^
.EI
.SI 5
int is_personal_club(string name)
.EI
.SP 7 5

This method checks to see if the specified club exists and is an
personal club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check to see for an personal type
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the club is an personal type

.EP

.SI 3
* %^BOLD%^is_recruiter_of%^RESET%^
.EI
.SI 5
int is_recruiter_of(string name, string recruiter)
.EI
.SP 7 5

This method will determine if the specified person is a recruiter of the
club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to find the recruiter of
.EP
.SP 9 5
recruiter - the person is check for the recruiter
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a recruiter, 0 if they are not
.EP

.SI 3
* %^BOLD%^normalise_name%^RESET%^
.EI
.SI 5
string normalise_name(string name)
.EI
.SP 7 5

This method normalises the name for lookups so that we don't
get names too confused.  Thanks to Terano for this idea.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name to normalise
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the normalised name

.EP

.SI 3
* %^BOLD%^query_account_names%^RESET%^
.EI
.SI 5
string * query_account_names(string name)
.EI
.SP 7 5

This method returns the names of all the accounts in the club.
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
the names of all the accounts

.EP

.SI 3
* %^BOLD%^query_balance%^RESET%^
.EI
.SI 5
int query_balance(string name, string account)
.EI
.SP 7 5

This method returns the balance of the club.
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
the current balance of the club
.EP

.SI 3
* %^BOLD%^query_club_cost_per_period%^RESET%^
.EI
.SI 5
int query_club_cost_per_period(string club_name)
.EI
.SP 7 5

This method determines how much the club will cost to run each
pay period.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to get the fees for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount the club will cost in the next pay period
.EP

.SI 3
* %^BOLD%^query_club_description%^RESET%^
.EI
.SI 5
string query_club_description(string club_name)
.EI
.SP 7 5

This method returns the description of the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to get the description of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the club description, 0 if the club is not found
.EP

.SI 3
* %^BOLD%^query_club_name%^RESET%^
.EI
.SI 5
string query_club_name(string club_name)
.EI
.SP 7 5

This method returns the capitalised and un messed name of the club.
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
the un messed name of the club
.EP

.SI 3
* %^BOLD%^query_club_region%^RESET%^
.EI
.SI 5
string query_club_region(string name)
.EI
.SP 7 5

This method returns the region of the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the club name

.EP

.SI 3
* %^BOLD%^query_club_secret%^RESET%^
.EI
.SI 5
int query_club_secret(string name)
.EI
.SP 7 5

This method checks to see if the club is secret or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check for secrecy
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the club is secret, 0 if not
.EP

.SI 3
* %^BOLD%^query_club_type%^RESET%^
.EI
.SI 5
int query_club_type(string name)
.EI
.SP 7 5

This method returns the club type of the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to get the type of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the type of the club
.EP

.SI 3
* %^BOLD%^query_clubs%^RESET%^
.EI
.SI 5
string * query_clubs()
.EI
.SP 7 5

This method returns the names of all the clubs currently in the list
of clubs.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of current clubs
.EP

.SI 3
* %^BOLD%^query_founder%^RESET%^
.EI
.SI 5
string query_founder(string name)
.EI
.SP 7 5

This method returns the founder of the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the club name to get the founder of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the founder of the club
.EP

.SI 3
* %^BOLD%^query_insignia_path%^RESET%^
.EI
.SI 5
string query_insignia_path(string name)
.EI
.SP 7 5

This method returns the insignia object associated with the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club for the insignia object
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the path of the club insignia object
.EP

.SI 3
* %^BOLD%^query_members%^RESET%^
.EI
.SI 5
string * query_members(string name)
.EI
.SP 7 5

This method returns the members of the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the members of the club
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the members of the club
.EP

.SI 3
* %^BOLD%^query_observers%^RESET%^
.EI
.SI 5
string * query_observers()
.EI
.SP 7 5

This method returns the current list of observers.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current list of observers

.EP

.SI 3
* %^BOLD%^query_recruiters%^RESET%^
.EI
.SI 5
string * query_recruiters(string name)
.EI
.SP 7 5

This method returns the recruiters of the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the club name to get the recruiters of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the recruiters of the club
.EP

.SI 3
* %^BOLD%^query_time_fees_due%^RESET%^
.EI
.SI 5
int query_time_fees_due(string club_name)
.EI
.SP 7 5

This method returns the time at which the club dues are again due.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to get the date for
.EP

.SI 3
* %^BOLD%^remove_member%^RESET%^
.EI
.SI 5
int remove_member(string name, string member)
.EI
.SP 7 5

This method removes a member from the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to remove a member from
.EP
.SP 9 5
member - the members name to remove
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
int remove_money(string name, int amount, int type, string person, string account)
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
type - the tyope of the transaction
.EP
.SP 9 5
person - the person removeing the money
.EP
.SP 9 5
account - the account the money is coming from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the removal is a success
.EP

.SI 3
* %^BOLD%^remove_observer%^RESET%^
.EI
.SI 5
int remove_observer(string obs)
.EI
.SP 7 5

This method removes an observer.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the obeserver to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^remove_recruiter%^RESET%^
.EI
.SI 5
int remove_recruiter(string name, string recruiter)
.EI
.SP 7 5

This method removes a recruiter from the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to remove the member from
.EP
.SP 9 5
recruiter - the recruiter to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^reset_club_secret%^RESET%^
.EI
.SI 5
int reset_club_secret(string name)
.EI
.SP 7 5

This method makes a clubs membership open.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to make open
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^set_club_description%^RESET%^
.EI
.SI 5
int set_club_description(string club_name, string description)
.EI
.SP 7 5

This method sets the description of the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to set the description of
.EP
.SP 9 5
description - the new description of the club
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^set_club_secret%^RESET%^
.EI
.SI 5
int set_club_secret(string name)
.EI
.SP 7 5

This method makes a clubs membership secret.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to make secret
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^touch_club%^RESET%^
.EI
.SI 5
void touch_club(string name)
.EI
.SP 7 5

This method touches the club and resets the timeout date.  This should
be done now and then by the club to make sure it does not timeout.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to reset the timeout for
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^create_extra_data%^RESET%^
.EI
.SI 5
void create_extra_data(string name)
.EI

.SI 3
* %^BOLD%^load_main%^RESET%^
.EI
.SI 5
void load_main()
.EI
.SP 7 5

This method loads the data from the disk.

.EP

.SI 3
* %^BOLD%^query_club_info%^RESET%^
.EI
.SI 5
class club_info query_club_info(string name)
.EI
.SP 7 5

This method either loads the data into the cache or it
reads it from the cache.

.EP

.SI 3
* %^BOLD%^save_cache%^RESET%^
.EI
.SI 5
void save_cache()
.EI
.SP 7 5

This method checks the cache and then saves anything changed to the
disk...

.EP

.SI 3
* %^BOLD%^save_club%^RESET%^
.EI
.SI 5
void save_club(string name)
.EI
.SP 7 5

This method saves the data to the disk.

.EP

.SI 3
* %^BOLD%^save_main%^RESET%^
.EI
.SI 5
void save_main()
.EI
.SP 7 5

This method loads the data from the disk.

.EP

.SI 3
* %^BOLD%^send_broadcast_message%^RESET%^
.EI
.SI 5
void send_broadcast_message(string club, string message)
.EI
.SP 7 5

This method sends a broadcast to the clubs talker channel.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the name of the club to send the message to
.EP
.SP 9 5
mess - the message to send

.EP

.SI 3
* %^BOLD%^send_observer_event%^RESET%^
.EI
.SI 5
void send_observer_event(string event_name, string * args ...)
.EI
.SP 7 5

This method calls a function on all the observers to tell them
when an event has taken place.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
event_name - the name of the event
.EP
.SP 9 5
args - the arguments to the event

.EP

.SI 3
* %^BOLD%^set_club_changed%^RESET%^
.EI
.SI 5
void set_club_changed(string name)
.EI
.SP 7 5

This method marks the club as being changed.

.EP


