.DT
multiplayer.c
Disk World autodoc help
multiplayer.c

.SH Description
.SP 5 5

Keeps track of the marked multi players.
Called by the login handler, checking people against a list of
previous multiplayers, and if they are on the list, check if
they are already on.

.EP
.SP 5 5
vars are:
.EP
.SP 5 5
 string *gits: an array of names of suspected multiplayers
.EP
.SP 5 5
 string *denied_parcels: an array of names of multiplayers prevented from
.EP
.SP 5 5
                         using the parcel system for sending parcels
.EP
.SP 5 5
 mapping allowed: the keys are the names of verified non-multiplayers,
.EP
.SP 5 5
                  the values are the names of people they are allowed to
.EP
.SP 5 5
                  play with.
.EP
.SP 5 5

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/playerinfo.h and /include/login_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_allowed%^RESET%^
.EI
.SI 5
int add_allowed(string user, string other)
.EI
.SP 7 5

Adds a player to the allowed multi IP list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
user - the name of the player
.EP
.SP 9 5
other - the name of another user which is not an alt
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if they are not a user, 1 if they are

.EP

.SI 3
* %^BOLD%^add_denied_parcel%^RESET%^
.EI
.SI 5
int add_denied_parcel(string user)
.EI
.SP 7 5

Adds a user to the list of users not allowed to use the parcel system
Only Lords and Liaison deputies are allowed to add a user to this array
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
user - the name of the player

.EP

.SI 3
* %^BOLD%^add_git%^RESET%^
.EI
.SI 5
int add_git(string git)
.EI
.SP 7 5

Adds a player to the multiplayer list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
git - the name of the suspected multiplayer
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if they are not a user, 1 if they are

.EP

.SI 3
* %^BOLD%^check_allowed%^RESET%^
.EI
.SI 5
string * check_allowed(mixed user, mixed * others)
.EI
.SP 7 5

Called by the player object to see if a logon inform should be given
about duplicate players from the same IP address.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
user - the player being checked
.EP
.SP 9 5
others - the others logged on from the same IP
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
array of not allowed dups

.EP

.SI 3
* %^BOLD%^check_multiplayers%^RESET%^
.EI
.SI 5
int check_multiplayers(string action, object player, object item)
.EI

.SI 3
* %^BOLD%^delete_allowed%^RESET%^
.EI
.SI 5
int delete_allowed(string user)
.EI
.SP 7 5

Remove a person from allowed multi IP list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
user - the name of the player to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if they are not in the list, 1 if they are

.EP

.SI 3
* %^BOLD%^member_denied_parcel%^RESET%^
.EI
.SI 5
int member_denied_parcel(string user)
.EI
.SP 7 5

Checks user against the parcels array
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
user - the name of the player
.EP

.SI 3
* %^BOLD%^query_allowed%^RESET%^
.EI
.SI 5
mapping query_allowed()
.EI
.SP 7 5

Returns the current allowed multi player IP list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of allowed multi players

.EP

.SI 3
* %^BOLD%^query_denied_parcels%^RESET%^
.EI
.SI 5
string * query_denied_parcels()
.EI
.SP 7 5

Returns the current list of players denied use of the parcel system.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of denied parcel users

.EP

.SI 3
* %^BOLD%^record_object%^RESET%^
.EI
.SI 5
void record_object(string action, object player, object item)
.EI

.SI 3
* %^BOLD%^remove_denied_parcel%^RESET%^
.EI
.SI 5
int remove_denied_parcel(string user)
.EI
.SP 7 5

Removes a user from the list of users not allowed to use the parcel system
Only Lords and Liaison deputies are allowed to remove a user from this array
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
user - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if they are not in the list, 1 if they are

.EP


