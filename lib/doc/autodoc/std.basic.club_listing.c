.DT
club_listing.c
Disk World autodoc help
club_listing.c

.SH Description
.SP 5 5

This is an inheritable for printing out different sorts of club
listings.
.EP
.SP 10 5


Written by Pinkfish

Started Sat Oct 24 02:28:04 EDT 1998
.EP



.SH See also
.SP 5 5
/obj/handlers/club_handler.c and /std/room/club_control_room.c

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
* %^BOLD%^player_relationships_string%^RESET%^
.EI
.SI 5
string player_relationships_string(string player)
.EI
.SP 7 5

This method returns the relations that the specified player
has as a string.  It will return a blank string if the player
has no relationships.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to check relationships for

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^club_info%^RESET%^
.EI
.SI 5
int club_info(string name, int show_election, string person)
.EI
.SP 7 5

This method prints out the information associated with the club.
The stuff printed out here is directly lifted from the club control room.
This method is designed to work with add_command().
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to show the information about
.EP
.SP 9 5
show_election - if non-zero show the election information
.EP
.SP 9 5
person - the person who is showing the information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^club_info_string%^RESET%^
.EI
.SI 5
string club_info_string(string name, int show_election, string person)
.EI
.SP 7 5

This method returns the club information as a string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to show the information about
.EP
.SP 9 5
show_election - if non-zero show the election information
.EP
.SP 9 5
person - the person who is showing the information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the club information string, 0 if no information
.EP

.SI 3
* %^BOLD%^club_list%^RESET%^
.EI
.SI 5
int club_list(string person, int print_families, function check_list)
.EI
.SP 7 5

This method prints out a list of the current clubs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person who is doing the listing
.EP
.SP 9 5
print_families - 1 print the families, 0 print the clubs
.EP
.SP 9 5
check_list - this function is called to see if the club should be listed
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 for success
.EP

.SI 3
* %^BOLD%^club_members%^RESET%^
.EI
.SI 5
int club_members(string club, int online, string person)
.EI
.SP 7 5

This method prints out all the members of the club, or all the members
of the club online.  This method is designed to work with
add_command().
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_name - the name of the club to print the members of
.EP
.SP 9 5
online - print only the online members, 0 not online, 1 online, 2 online
 and listening to the club channel
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if not

.EP

.SI 3
* %^BOLD%^family_info%^RESET%^
.EI
.SI 5
int family_info(string name, string person)
.EI
.SP 7 5

This method prints out the information associated with the family.
The stuff printed out here is directly lifted from the family control room.
This method is designed to work with add_command().
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the family to show the information about
.EP
.SP 9 5
show_election - if non-zero show the election information
.EP
.SP 9 5
person - the person who is showing the information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^family_info_string%^RESET%^
.EI
.SI 5
string family_info_string(string name, string person)
.EI
.SP 7 5

This method returns the family information as a string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the family to show the information about
.EP
.SP 9 5
show_election - if non-zero show the election information
.EP
.SP 9 5
person - the person who is showing the information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the family information string, 0 if no information
.EP

.SI 3
* %^BOLD%^person_in_control%^RESET%^
.EI
.SI 5
object person_in_control(object ob)
.EI
.SP 7 5

This method returns the player in control of the item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the player in control
.EP

.SI 3
* %^BOLD%^sort_and_capitalise%^RESET%^
.EI
.SI 5
string sort_and_capitalise(string * arr)
.EI
.SP 7 5

This method will sort and capitalise the array of strings sent into
the method.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arr - the array to sort and capitalise
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the sorted and capitalised string
.EP

.SI 3
* %^BOLD%^www_club_info_string%^RESET%^
.EI
.SI 5
string www_club_info_string(string name, int show_election, string person)
.EI
.SP 7 5

This method returns the club information as a html string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the club to show the information about
.EP
.SP 9 5
show_election - if non-zero show the election information
.EP
.SP 9 5
person - the person who is showing the information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the club information string, 0 if no information
.EP

.SI 3
* %^BOLD%^www_club_list%^RESET%^
.EI
.SI 5
string www_club_list(string person, int print_families, int start, int show)
.EI
.SP 7 5

This method prints out a list of the current clubs into html.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person who is doing the listing
.EP
.SP 9 5
print_families - 1 print the families, 0 print the clubs
.EP
.SP 9 5
start - the index to start from
.EP
.SP 9 5
show - how many to show
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the club listing string
.EP

.SI 3
* %^BOLD%^www_family_info_string%^RESET%^
.EI
.SI 5
string www_family_info_string(string name, string person)
.EI
.SP 7 5

This method returns the family information as a string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the family to show the information about
.EP
.SP 9 5
show_election - if non-zero show the election information
.EP
.SP 9 5
person - the person who is showing the information
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the family information string, 0 if no information
.EP

.SI 3
* %^BOLD%^www_finger_name%^RESET%^
.EI
.SI 5
string www_finger_name(string name)
.EI
.SP 7 5

This method turns a name into a fingerable name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name to make fingerable
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the htmlised name string

.EP


