.DT
club_inherit.c
Disk World autodoc help
club_inherit.c

.SH Description
.SP 5 5

This is the handler for all things clubby, a club being a group of
players.  Each club must have a unique name.  It also handles elections
for various club positions.


This is broken up into this bit because LPC deals with classes and inheritance in extremely odd ways.
.EP
.SP 10 5


Written by Pinkfish

Started Sun Sep 27 03:35:42 EDT 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/handlers/inherit/club_discuss.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/mail.h, /include/library.h, /include/top_ten_tables.h, /include/player_handler.h, /include/player.h, /include/am_time.h, /include/broadcaster.h, /include/clubs.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^top_ten_tables%^RESET%^
class top_ten_tables {
int num;
int qp;
int age;
int start_time;
int gender;
int single_gender;
int rels;
int no_pks;
mapping guild;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_clubs%^RESET%^
.EI
.SI 5
void check_clubs(int startup)
.EI
.SP 7 5

This method checks all the clubs to make sure they are currently valid
and takes money out of the accounts of those that need to pay.  It
will notify those whose accounts are deliquent.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^check_club_members%^RESET%^
.EI
.SI 5
void check_club_members(string club_name, string * members, class top_ten_tables top_info)
.EI
.SP 7 5

This method checks the members of the club and makes sure they have
not been up to anything naughty, like getting deleted.  It also
updates the top ten information on the top ten handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
members - the members left to check
.EP
.SP 9 5
top_info - the top ten information

.EP

.SI 3
* %^BOLD%^check_extra_information%^RESET%^
.EI
.SI 5
void check_extra_information(string name, string member, int startup)
.EI
.SP 7 5

This method checks the extra information for the club.  Please note
this does *not* save the information.  You need to do this yourself.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to check
.EP

.SI 3
* %^BOLD%^check_specific_club%^RESET%^
.EI
.SI 5
void check_specific_club(string club_name, int extra_space, int startup)
.EI

.SI 3
* %^BOLD%^check_specific_clubs%^RESET%^
.EI
.SI 5
void check_specific_clubs(int startup, int pos)
.EI


