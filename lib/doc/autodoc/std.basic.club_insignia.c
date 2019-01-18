.DT
club_insignia.c
Disk World autodoc help
club_insignia.c

.SH Description
.SP 5 5

The club insignia inheritable thingy.
.EP
.SP 10 5


Written by Pinkfish

Started Sun Oct  4 04:23:52 EDT 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/club_listing and /std/basic/club_discuss.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h, /include/broadcaster.h and /include/clubs.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^event_channel_message%^RESET%^
.EI
.SI 5
void event_channel_message(object ob, string channel, mixed * data)
.EI
.SP 7 5

The method is called when the channel is triggered.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the person who talked
.EP
.SP 9 5
channel - the name of the channel
.EP
.SP 9 5
message - the pmessage top print
.EP

.SI 3
* %^BOLD%^event_club_changed%^RESET%^
.EI
.SI 5
void event_club_changed(object person, string club_name)
.EI
.SP 7 5

This method is called when the club type is changed.  It will add on
the elected club commands, if that is what we have changed too.

.EP

.SI 3
* %^BOLD%^event_joined_club%^RESET%^
.EI
.SI 5
void event_joined_club(object person, string club_name)
.EI
.SP 7 5

This is the event generated when the person joins the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person doing the joining
.EP
.SP 9 5
club_name - the name of the club being joined

.EP

.SI 3
* %^BOLD%^query_brief_mode%^RESET%^
.EI
.SI 5
int query_brief_mode()
.EI
.SP 7 5

This method tells us if the badge is in brief mode or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current brief mode

.EP

.SI 3
* %^BOLD%^query_brief_string%^RESET%^
.EI
.SI 5
string query_brief_string()
.EI
.SP 7 5

This method returns the short name for the channel string.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short name for the channel

.EP

.SI 3
* %^BOLD%^query_channel_off%^RESET%^
.EI
.SI 5
int query_channel_off()
.EI
.SP 7 5

This method returns the status of the channel off flag.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current value of the channel off flag
.EP

.SI 3
* %^BOLD%^query_club%^RESET%^
.EI
.SI 5
string query_club()
.EI
.SP 7 5

This method returns the club associated with the badge.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the club associated with the badge
.EP

.SI 3
* %^BOLD%^set_club%^RESET%^
.EI
.SI 5
void set_club(string club)
.EI
.SP 7 5

This method sets the club associated with the badge.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the name of the club
.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^add_club_commands%^RESET%^
.EI
.SI 5
void add_club_commands(object per)
.EI
.SP 7 5

This method will add the club commands from the player.

.EP

.SI 3
* %^BOLD%^add_elected_commands%^RESET%^
.EI
.SI 5
void add_elected_commands(object per)
.EI
.SP 7 5

This method will add the club commands from the player.

.EP

.SI 3
* %^BOLD%^club_chat%^RESET%^
.EI
.SI 5
int club_chat(string str, int emote)
.EI
.SP 7 5

The says something wonderful and exciting to all the other club members.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the exciting comment I know they will say

.EP

.SI 3
* %^BOLD%^confirm_disbar%^RESET%^
.EI
.SI 5
void confirm_disbar(string str, string name)
.EI
.SP 7 5

THis method confirms the disbar request from the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the response string
.EP
.SP 9 5
name - the name of the person to disbar
.EP

.SI 3
* %^BOLD%^confirm_resign%^RESET%^
.EI
.SI 5
void confirm_resign(string str, string position)
.EI
.SP 7 5

This method checks to make sure they really wish to resign.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the response
.EP

.SI 3
* %^BOLD%^do_announcement%^RESET%^
.EI
.SI 5
int do_announcement()
.EI
.SP 7 5

This method will print out any announcements associated with the club.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success

.EP

.SI 3
* %^BOLD%^do_brief_mode%^RESET%^
.EI
.SI 5
int do_brief_mode(int mode)
.EI
.SP 7 5

This method sets the brief mode of the badge.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mode - the brief mode]
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_brief_string%^RESET%^
.EI
.SI 5
int do_brief_string(string str)
.EI
.SP 7 5

This method sets the brief message to use for the channel messages.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the channel message short
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_channel_off%^RESET%^
.EI
.SI 5
int do_channel_off(int flag)
.EI
.SP 7 5

This method handles setting the flag and printing all the happy success
messages.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - the new flag

.EP

.SI 3
* %^BOLD%^do_disbar%^RESET%^
.EI
.SI 5
int do_disbar(string name)
.EI
.SP 7 5

This method allows the founders and presidents/vice presidents to disbar
people from the club.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the person to disbar

.EP

.SI 3
* %^BOLD%^do_history%^RESET%^
.EI
.SI 5
int do_history()
.EI
.SP 7 5

This method prints out the history of the channel.
.EP

.SI 3
* %^BOLD%^do_recruit%^RESET%^
.EI
.SI 5
int do_recruit(object * obs, string club)
.EI
.SP 7 5

This is the method to use when recruiting people.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the people to recruit
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_resign%^RESET%^
.EI
.SI 5
int do_resign()
.EI
.SP 7 5

This method starts the resignation process.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure
.EP

.SI 3
* %^BOLD%^do_resign_position%^RESET%^
.EI
.SI 5
int do_resign_position(string position)
.EI
.SP 7 5

This method resigns as a position in the club.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure
.EP


