.DT
situations.h
Disk World autodoc help
situations.h

.SH Description
.SP 5 5

This contains the definitions of the situation class
and the when defines for situations.
The when defines consist of (1>>hour) combined with or
where hour is an hour of the day during which a 
situation may occur (25 hour clock, AM time).
For example, WHEN_WEE_HOURS allows the situation
to occur between 12 midnite and 4 am.

.EP
.SP 10 5

.EP

.SH Classes

.SI 3
* %^BOLD%^situation%^RESET%^
class situation {
string start_mess;
string end_mess;
string extra_look;
function start_func;
function end_func;
int * chat_rate;
string * chats;
mixed * add_items;
mixed * random_words;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^WHEN_AFTERNOON%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_ANY_TIME%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_DAY%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_EARLY_MORNING%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_EVENING%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_LATE_MORNING%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_LATENIGHT%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_MIDDAY%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_MORNING%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_NIGHT%^RESET%^
.EI

.SI 3
* %^BOLD%^WHEN_WEE_HOURS%^RESET%^
.EI

