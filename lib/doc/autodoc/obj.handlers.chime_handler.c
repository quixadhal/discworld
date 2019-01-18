.DT
chime_handler.c
Disk World autodoc help
chime_handler.c

.SH Description
.SP 5 5

This handler deals with clocks which chime. It uses a single 15 minute
callout to make all clocks on Discworld which choose to chime chime the
quarter, half or hours.

.EP
.SP 10 5


Written by Ceres

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h and /include/chime.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^chime%^RESET%^
class chime {
object ob;
int type;
string mess;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^register_for_chimes%^RESET%^
.EI
.SI 5
int register_for_chimes(object ob, int num, string mess)
.EI
.SP 7 5

Register a clock to do chimes. The num parameter specifes when to
chime.  If it is 1 then the clock chimes every hour, if it is
2 the clock chimes ever half hour, if it is 4 the
clock chimes every quarter hour.  Anything else and
the clock never chimes.  NB: if you set it to chime the quarter hours
it will only chime the quarter hours, it will not chime the hours
or the half hours.  You need to call this function three times to
get your clock to chime the hour, half and quarter hours.


The function "chime" will be called on the clock when the chime occurs. The message will be passed in as the first argument, the second will be the type of chime (CHIME_HOUR, CHIME_HALF_HOUR, CHIME_QUARTER_HOUR). 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object being registered
.EP
.SP 9 5
num - when to chime
.EP
.SP 9 5
mess - the message to chime with

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure.

.EP


