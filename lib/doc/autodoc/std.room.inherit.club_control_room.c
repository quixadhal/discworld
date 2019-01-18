.DT
club_control_room.c
Disk World autodoc help
club_control_room.c

.SH Description
.SP 5 5

This room allows people to create clubs and do fluffy club stuff if
they need to.
.EP
.SP 10 5


Written by Pinkfish

Started Sun Oct  4 02:24:46 EDT 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/club_discuss and /std/basic/club_listing.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/move_failures.h, /include/shops/bank.h, /include/am_time.h, /include/clubs.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^is_valid_club_name%^RESET%^
.EI
.SI 5
int is_valid_club_name(string name)
.EI

.SI 3
* %^BOLD%^is_valid_family_name%^RESET%^
.EI
.SI 5
int is_valid_family_name(string name)
.EI
.SP 7 5

This method is called to check the validity of a family name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the family.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if valid, 0 if invalid.

.EP

.SI 3
* %^BOLD%^query_club_control_type%^RESET%^
.EI
.SI 5
int query_club_control_type()
.EI
.SP 7 5

This method queries the type of the room.  This controls if the room is to 
be used for clubs, families or both.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the control type of the room
.EP

.SI 3
* %^BOLD%^query_club_region%^RESET%^
.EI
.SI 5
string query_club_region()
.EI
.SP 7 5

This method returns the region for the club control room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the region the clubs are created in

.EP

.SI 3
* %^BOLD%^query_club_warning_message%^RESET%^
.EI
.SI 5
string query_club_warning_message()
.EI
.SP 7 5

This method creates the warning message for the club, saying how much
it costs per year and how everything works.  This should probably
be used in a sign or something in the main room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the warning message

.EP

.SI 3
* %^BOLD%^query_percentage_cut%^RESET%^
.EI
.SI 5
int query_percentage_cut()
.EI
.SP 7 5

This method returns the percentage cut the room takes for deposits
to the club account.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current percentage cut
.EP

.SI 3
* %^BOLD%^set_club_control_type%^RESET%^
.EI
.SI 5
void set_club_control_type(int room_type)
.EI
.SP 7 5

This method sets the type of the room.  This controls if the room is to 
be used for clubs, families or both.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of the club
.EP

.SI 3
* %^BOLD%^set_club_region%^RESET%^
.EI
.SI 5
void set_club_region(string region)
.EI
.SP 7 5

This method sets the region for the club control room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
region - the region for the clubs created here

.EP

.SI 3
* %^BOLD%^set_percentage_cut%^RESET%^
.EI
.SI 5
void set_percentage_cut(int value)
.EI
.SP 7 5

This method sets the percentage cut the room takes for deposits to the
club account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cut - the nice new percentage cut
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^confirm_create%^RESET%^
.EI
.SI 5
void confirm_create(string str, string name)
.EI
.SP 7 5

This method will confirm the creation of a club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the yes or no string
.EP
.SP 9 5
name - the name of the club to create

.EP

.SI 3
* %^BOLD%^confirm_create_family%^RESET%^
.EI
.SI 5
void confirm_create_family(string str, string name)
.EI
.SP 7 5

This method will confirm the creation of a family.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the yes or no string
.EP
.SP 9 5
name - the name of the family to create

.EP

.SI 3
* %^BOLD%^disband_confirm%^RESET%^
.EI
.SI 5
void disband_confirm(string str, string name)
.EI
.SP 7 5

This method is called to confirm the disbanding of the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the response the user type
.EP
.SP 9 5
name - the name of the club to disband
.EP

.SI 3
* %^BOLD%^do_announcement%^RESET%^
.EI
.SI 5
int do_announcement(string name)
.EI
.SP 7 5

This method sets an announcement for the club.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_balance%^RESET%^
.EI
.SI 5
int do_balance(string name)
.EI
.SP 7 5

This method prints the balance of this club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to get the balance of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_change%^RESET%^
.EI
.SI 5
int do_change(string name, string type)
.EI
.SP 7 5

This allows the club to change into a new type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to change
.EP
.SP 9 5
type - the type to change the club into
.EP

.SI 3
* %^BOLD%^do_change_option%^RESET%^
.EI
.SI 5
int do_change_option(string name, string option)
.EI
.SP 7 5

This allows the club to change its options.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to change
.EP
.SP 9 5
option - the option to change on the club
.EP

.SI 3
* %^BOLD%^do_club_list_member%^RESET%^
.EI
.SI 5
int do_club_list_member(string name)
.EI
.SP 7 5

This method does a listing of all the clubs that a certain person is
in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the person whose name to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_create%^RESET%^
.EI
.SI 5
int do_create(string name)
.EI
.SP 7 5

This method is called to try and create a club of the specified name.
All clubs when they first start are personal clubs.  Once they have
a certain number of members, they can convert to being an elected
type of club if they wish.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to create
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_create_family%^RESET%^
.EI
.SI 5
int do_create_family(string name)
.EI
.SP 7 5

This method is called to try and create a family of the specified name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the family to create
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_deposit%^RESET%^
.EI
.SI 5
int do_deposit(string mon, string name, string account, string reason)
.EI
.SP 7 5

This method allows to you deposit money into the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the money objects to deposit
.EP
.SP 9 5
name - the name of the club to deposit them to
.EP
.SP 9 5
reason - the reason of the deposit
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_describe%^RESET%^
.EI
.SI 5
int do_describe(string name)
.EI
.SP 7 5

This method sets the club description.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to describe
.EP

.SI 3
* %^BOLD%^do_disband%^RESET%^
.EI
.SI 5
int do_disband(string name)
.EI
.SP 7 5

This method will forcibly disband the club.  This can only be done
for founder type clubs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to disband
.EP

.SI 3
* %^BOLD%^do_financials%^RESET%^
.EI
.SI 5
int do_financials(string name, string account)
.EI
.SP 7 5

This method prints out financial reports for the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to get financials on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_force_election%^RESET%^
.EI
.SI 5
int do_force_election(string name)
.EI

.SI 3
* %^BOLD%^do_insignia%^RESET%^
.EI
.SI 5
int do_insignia(string name, int num)
.EI
.SP 7 5

This method will create a bunch of insignia for the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the number of insignia to create
.EP
.SP 9 5
name - the name of the club to create insignia for

.EP

.SI 3
* %^BOLD%^do_nominate%^RESET%^
.EI
.SI 5
int do_nominate(string person, string position, string club_name)
.EI
.SP 7 5

This method nominates a person into a position in a club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to nominate
.EP
.SP 9 5
position - the position to nominate them too
.EP
.SP 9 5
club_name - the name of the club
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_recruiter_add%^RESET%^
.EI
.SI 5
int do_recruiter_add(string name, string recruit)
.EI
.SP 7 5

This method attempts to add a recruiter to the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club
.EP
.SP 9 5
recruit - the person to add as a recruiter

.EP

.SI 3
* %^BOLD%^do_recruiter_remove%^RESET%^
.EI
.SI 5
int do_recruiter_remove(string name, string recruit)
.EI
.SP 7 5

This method attempts to add a recruiter to the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club
.EP
.SP 9 5
recruit - the person to add as a recruiter

.EP

.SI 3
* %^BOLD%^do_transfer%^RESET%^
.EI
.SI 5
int do_transfer(string money, string name, string from_account, string to_account, string reason)
.EI
.SP 7 5

This method is used when someone wishes to withdraw some money from the
club account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
money - the amount of money to withdraw
.EP
.SP 9 5
club - the club to withdraw the money from
.EP
.SP 9 5
from_account - the account to withdraw money from
.EP
.SP 9 5
to_account - the account to deposit the money to
.EP
.SP 9 5
reason - the reason to withdraw money from the account
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if suecceded, 0 if failed.

.EP

.SI 3
* %^BOLD%^do_vote%^RESET%^
.EI
.SI 5
int do_vote(string person, string position, string club_name)
.EI
.SP 7 5

This method attempts to vote for someone in a club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to vote for
.EP
.SP 9 5
position - the position to vote for them in
.EP
.SP 9 5
club_name - the name of the club to vote for them in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_withdrawl%^RESET%^
.EI
.SI 5
int do_withdrawl(string money, string name, string account, string reason)
.EI
.SP 7 5

This method is used when someone wishes to withdraw some money from the
club account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
money - the amount of money to withdraw
.EP
.SP 9 5
club - the club to withdraw the money from
.EP
.SP 9 5
account - the account to withdraw money from
.EP
.SP 9 5
reason - the reason to withdraw money from the account
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if suecceded, 0 if failed.

.EP

.SI 3
* %^BOLD%^finish_description%^RESET%^
.EI
.SI 5
void finish_description(string describe, string name)
.EI
.SP 7 5

This is the method that is called when the club description has been
finished edited.  It will set the description on the club handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
describe - the new description
.EP
.SP 9 5
name - the name of the club to describe
.EP


