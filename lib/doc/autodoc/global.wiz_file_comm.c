.DT
wiz_file_comm.c
Disk World autodoc help
wiz_file_comm.c

.SH Description
.SP 5 5

This file contains all the file related commands and information
for the creators.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/wiz_object_comm, /global/wiz_channels, /global/playtester, /global/le, /global/wiz_info_comm and /global/wiz_inter_comm.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/ls.h, /include/player_handler.h, /include/cmds/friends.h, /include/login_handler.h, /include/playtesters.h and /include/creator.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_allowed%^RESET%^
.EI
.SI 5
nomask string * query_allowed()
.EI
.SP 7 5

This method returns the current list of people in the allowed array for
the creator.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current list of allowed people

.EP

.SI 3
* %^BOLD%^query_ed_setup%^RESET%^
.EI
.SI 5
int query_ed_setup()
.EI
.SP 7 5

This method returns the saved setup for the inbuild ed command.  THis
allows certain flag settings to be saved between sessions.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current ed setup flags
.EP

.SI 3
* %^BOLD%^query_in_editor%^RESET%^
.EI
.SI 5
mixed query_in_editor()
.EI
.SP 7 5

This method returns the value of the in editor flag.
It will return non-zero if the player is in an editor.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
non-zero in editor, 0 not in an editor
.EP

.SI 3
* %^BOLD%^query_invis%^RESET%^
.EI
.SI 5
nomask int query_invis()
.EI
.SP 7 5

This method returns the current invisibility level of the object.
This will return 0 if the object is not invisible, 1 for creator
invisible, 2 for director invisible and 3 for trustee invisible.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current invisibility level

.EP

.SI 3
* %^BOLD%^query_last_location%^RESET%^
.EI
.SI 5
mixed query_last_location()
.EI
.SP 7 5

This method returns the last location of the creator for use by goback.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
string last location.

.EP

.SI 3
* %^BOLD%^query_path%^RESET%^
.EI
.SI 5
string query_path()
.EI
.SP 7 5

This method returns the creators current working directory.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current working directory

.EP

.SI 3
* %^BOLD%^query_visible%^RESET%^
.EI
.SI 5
int query_visible(object thing)
.EI
.SP 7 5

This method returns the current visibility status of this creator
in respect to the other object.  This does the allow level checking
and all sorts of exciting things.


It will return 1 for creator invisible, 2 for director invisible, 3 for trustee invisible.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object to test the visibility against
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if not invisible, non-zero if invisible

.EP

.SI 3
* %^BOLD%^review%^RESET%^
.EI
.SI 5
int review()
.EI
.SP 7 5

This method prints out any interesting bits of reviewable information
available on the creator.  This is used by the review command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^set_ed_setup%^RESET%^
.EI
.SI 5
void set_ed_setup(int i)
.EI
.SP 7 5

This method sets the current flags for the inbuild ed command.  THis
allows certain flag settings to be saved between sessions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new flags for the ed command
.EP

.SI 3
* %^BOLD%^set_in_editor%^RESET%^
.EI
.SI 5
void set_in_editor(mixed what)
.EI
.SP 7 5

This method sets the current in editor flag.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
what - the new value of the in editor flag
.EP

.SI 3
* %^BOLD%^set_last_location%^RESET%^
.EI
.SI 5
void set_last_location(mixed loc)
.EI
.SP 7 5

This method stores the last location of the creator for use by goback.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - location

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^command%^RESET%^
.EI
.SI 5
mixed command(string txt)
.EI

.SI 3
* %^BOLD%^process_input%^RESET%^
.EI
.SI 5
string process_input(string inp)
.EI

.SI 3
* %^BOLD%^wiz_commands%^RESET%^
.EI
.SI 5
void wiz_commands()
.EI
.SP 7 5

This method adds in all the creator commands to the player.  This
will be called when the play initialy logs onto the game.

.EP


