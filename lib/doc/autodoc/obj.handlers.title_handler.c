.DT
title_handler.c
Disk World autodoc help
title_handler.c

.SH Description
.SP 5 5

This handler will keep track of special player titles.  It will allow
them to be added or removed based on specific events.  Use the
include <player.h> to get the path to this file.
.EP
.SP 10 5


Written by Pinkfish

Started Wed Sep 27 14:00:31 PDT 2000

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/login_handler.h and /include/login.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^blob%^RESET%^
class blob {
string ob;
string func;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_controlled_title%^RESET%^
.EI
.SI 5
void add_controlled_title(string title, string ob, string func)
.EI
.SP 7 5

Add a method to be called to check if the person is allowed to have
the specified title.  This should be added to control things like
special titles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the title to control
.EP
.SP 9 5
ob - the object to call the method on
.EP
.SP 9 5
function - the function to call

.EP

.SI 3
* %^BOLD%^add_title_to_player%^RESET%^
.EI
.SI 5
void add_title_to_player(string person, string title)
.EI
.SP 7 5

This method adds a person to the pending list for gaining a specific
title.  If the person is currently on then they will get the title
immediately.  If they are not on then it will be added to them when they
next log on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to give the title too
.EP
.SP 9 5
title - the title to give them

.EP

.SI 3
* %^BOLD%^check_player%^RESET%^
.EI
.SI 5
void check_player(object player)
.EI
.SP 7 5

This method checks a player when they logon for titles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to check

.EP

.SI 3
* %^BOLD%^is_allowed_multiple_title%^RESET%^
.EI
.SI 5
int is_allowed_multiple_title(string title)
.EI
.SP 7 5

This method checks to see if the title is an allowed multiple title.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the title to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is allowed, 0 if not

.EP

.SI 3
* %^BOLD%^query_allowed_multiple_titles%^RESET%^
.EI
.SI 5
string * query_allowed_multiple_titles()
.EI
.SP 7 5

This method returns the list of allowed multiple titles.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of allowed multiple titles

.EP

.SI 3
* %^BOLD%^query_pending_titles%^RESET%^
.EI
.SI 5
string * query_pending_titles(string person)
.EI
.SP 7 5

This method returns the list of titles pending for the specific person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to get the titles for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of pending titles

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^query_controlled_title%^RESET%^
.EI
.SI 5
class blob query_controlled_title(string title)
.EI
.SP 7 5

This method returns the blob for the controlled title.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the title which is controlled
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the blob for the controlled title

.EP


