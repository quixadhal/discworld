.DT
club_discuss.c
Disk World autodoc help
club_discuss.c

.SH Description
.SP 5 5

This file is for handling discussion items and clubs.
.EP
.SP 10 5


Written by Pinkfish

Started April 21st 1999
.EP



.SH See also
.SP 5 5
/obj/handlers/club_handler.c

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/am_time.h and /include/clubs.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^discussion_item%^RESET%^
.EI
.SI 5
string discussion_item(int id, string player)
.EI
.SP 7 5

This method returns information on the specified discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the discussion item to get information on
.EP
.SP 9 5
player - the player retreiving the information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string which is the information on the discussion item

.EP

.SI 3
* %^BOLD%^discussion_item_list%^RESET%^
.EI
.SI 5
string discussion_item_list(string player)
.EI
.SP 7 5

This method will return the listing of discussion items as a nice
string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player who is doing the listing
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the discussion items as a listing

.EP

.SI 3
* %^BOLD%^do_archive%^RESET%^
.EI
.SI 5
int do_archive(int num)
.EI
.SP 7 5

This method prints out the specific archive in more detail.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the number of the archive to print out
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_list_archives%^RESET%^
.EI
.SI 5
int do_list_archives()
.EI
.SP 7 5

This method prints out the archives.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_nominate%^RESET%^
.EI
.SI 5
int do_nominate(int id, string person)
.EI
.SP 7 5

A special entry point for nominating people to discussion items, this
only works with the nomination discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the discussion item
.EP
.SP 9 5
person - the person to nominate
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^do_withdraw%^RESET%^
.EI
.SI 5
int do_withdraw(int id)
.EI
.SP 7 5

A special entry point for withdrawing from a nominated discussion item, this
only works with the nomination discussion item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the discussion item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP


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

.SI 3
* %^BOLD%^add_elected_commands%^RESET%^
.EI
.SI 5
void add_elected_commands(object per)
.EI
.SP 7 5

This method adds the elected club specific commands to the
player.

.EP


