.DT
line_ed.c
Disk World autodoc help
line_ed.c

.SH Description
.SP 5 5

This allows a piece of text to be placed into the players own
line editor (which ever one they wish to use).


Expansion of the editor. We will now allow the player to use any of three different editors. One of them is the default (the old one) one of them is ed and the other I am going to write in a few secs.... 

cute line editor thing. Which even a player could use!
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h and /include/ed.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^append_signature%^RESET%^
.EI
.SI 5
string append_signature()
.EI
.SP 7 5

This method will create the signature to append to whatever needs
a signature.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the players signature

.EP

.SI 3
* %^BOLD%^do_edit%^RESET%^
.EI
.SI 5
varargs int do_edit(string str, mixed end_f, object end_o, string fname, mixed extra)
.EI
.SP 7 5

Edit a string.  This will edit the string or file name and then
call the end function and end object at the end.  It will pas in
the string if the edit is successful, or 0 if it is not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string to edit
.EP
.SP 9 5
end_f - the function to call
.EP
.SP 9 5
end_o - the object to call it on (defaul: previous_object())
.EP
.SP 9 5
fname - a specific filename to edit
.EP
.SP 9 5
extra - an array of any parameters you need to carry into the end function
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure.

.EP

.SI 3
* %^BOLD%^query_editor%^RESET%^
.EI
.SI 5
string query_editor()
.EI
.SP 7 5

This method will return the players current editor setting.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current editor setting.

.EP

.SI 3
* %^BOLD%^query_in_editor%^RESET%^
.EI
.SI 5
int query_in_editor()
.EI
.SP 7 5

This method returns true if the player is currently in the editor.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the player is in the editor

.EP


