.DT
clock.c
Disk World autodoc help
clock.c

.SH Description
.SP 5 5

An inheritable clock object. This is used for proper clocks, if you need to
create a watch use the watch object instead.

.EP
.SP 10 5


Written by Ceres

.EP



.SH See also
.SP 5 5
/obj/watch

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^chime%^RESET%^
.EI
.SI 5
void chime(string chime_message, int type)
.EI
.SP 7 5

Function to chime. Overload this if you wish to have your own chime
code.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
chime_message - the message set with set_chimes.
.EP

.SI 3
* %^BOLD%^extra_look%^RESET%^
.EI
.SI 5
string extra_look(object thing)
.EI

.SI 3
* %^BOLD%^set_chimes%^RESET%^
.EI
.SI 5
void set_chimes(int num, string mess)
.EI
.SP 7 5

setup chimes for this clock.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - 1 = every hour, 2 = every half hour, 3 = every quarter hour.
.EP
.SP 9 5
mess - The message to be displayed when the clock chimes.

.EP

.SI 3
* %^BOLD%^set_show_date%^RESET%^
.EI
.SI 5
void set_show_date()
.EI
.SP 7 5

Sets this clock to show the date as well as the tme.

.EP


