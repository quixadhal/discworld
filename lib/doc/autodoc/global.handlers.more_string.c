.DT
more_string.c
Disk World autodoc help
more_string.c

.SH Description
.SP 5 5

This file will handle the real moreing of a string to the player.  The
player object will call out to this, so the all the code and the
variables are not stored in the player object.
.EP
.SP 10 5


Written by Pinkfish

.EP



.SH See also
.SP 5 5
/global/more_file.c

.EP
.SH Classes

.SI 3
* %^BOLD%^more_string_info%^RESET%^
class more_string_info {
int fsize;
int topl;
int botl;
string last_search;
string * the_bit;
string finish_func;
string more_bit;
string stat_line;
object finish_ob;
}

.EI
.SP 7 5

Information on the string being mored.
.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^more_string%^RESET%^
.EI
.SI 5
varargs int more_string(string text, string bity, int noreline)
.EI
.SP 7 5

Puts a long string through a more function.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
text - the text to place through the pager
.EP
.SP 9 5
bity - the title of the text
.EP
.SP 9 5
noreline - no idea...
.EP

.SI 3
* %^BOLD%^set_finish_func%^RESET%^
.EI
.SI 5
varargs void set_finish_func(string str, object ob)
.EI
.SP 7 5

Sets the finish function.  This is called when the more_string
command exits.  If the ob is set to 0 then previous_object() is used
for it.  The str can be a function pointer as well.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the function name or function pointer to use
.EP
.SP 9 5
ob - the object to call it on (ignore for function pointers)
.EP


