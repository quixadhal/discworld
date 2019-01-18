.DT
force.c
Disk World autodoc help
force.c

.SH Description
.SP 5 5

This file contains the code to handle forcing.  
The method used will be to ask the object to
do the command.  While this is similar to the original method of doing this
it will be used to be more in the style of asking things to do things,
rather than the making things do things without asking them first.
.EP
.SP 10 5


Written by Furball

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_force_on_me%^RESET%^
.EI
.SI 5
nomask int do_force_on_me(string str)
.EI
.SP 7 5

This is the method called to do the actual force.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string to force us to do
.EP

.SI 3
* %^BOLD%^force_commands%^RESET%^
.EI
.SI 5
void force_commands()
.EI
.SP 7 5

These are the commands which will allow people to force others.
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^do_force%^RESET%^
.EI
.SI 5
int do_force(object * obs, string str)
.EI
.SP 7 5

This is the function which forces another player to do something.
.EP

.SI 3
* %^BOLD%^no_force%^RESET%^
.EI
.SI 5
int no_force(string str)
.EI
.SP 7 5

This method is used to set the ability to let yourself be forced on and
off.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string which is on, off or 0
.EP


