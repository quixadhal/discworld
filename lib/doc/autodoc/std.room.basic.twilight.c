.DT
twilight.c
Disk World autodoc help
twilight.c

.SH Includes
.SP 5 5
This class includes the following files /include/twilight.h and /include/climate.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_day_item%^RESET%^
.EI
.SI 5
varargs int add_day_item(mixed shorts, mixed desc, mixed no_plural)
.EI

.SI 3
* %^BOLD%^add_night_item%^RESET%^
.EI
.SI 5
varargs int add_night_item(mixed shorts, mixed desc, mixed no_plural)
.EI

.SI 3
* %^BOLD%^change%^RESET%^
.EI
.SI 5
void change(int time_now)
.EI

.SI 3
* %^BOLD%^destruct_shadow%^RESET%^
.EI
.SI 5
void destruct_shadow(object thing)
.EI

.SI 3
* %^BOLD%^event_weather%^RESET%^
.EI
.SI 5
void event_weather(int whats_changed)
.EI

.SI 3
* %^BOLD%^query_roomchats%^RESET%^
.EI
.SI 5
mixed query_roomchats()
.EI

.SI 3
* %^BOLD%^query_roomitems%^RESET%^
.EI
.SI 5
mixed query_roomitems()
.EI

.SI 3
* %^BOLD%^query_roomlong%^RESET%^
.EI
.SI 5
string * query_roomlong()
.EI

.SI 3
* %^BOLD%^query_time%^RESET%^
.EI
.SI 5
int query_time()
.EI

.SI 3
* %^BOLD%^return_long%^RESET%^
.EI
.SI 5
string return_long(mixed desc)
.EI

.SI 3
* %^BOLD%^room_day_chat%^RESET%^
.EI
.SI 5
void room_day_chat(mixed * args)
.EI

.SI 3
* %^BOLD%^room_night_chat%^RESET%^
.EI
.SI 5
void room_night_chat(mixed * args)
.EI

.SI 3
* %^BOLD%^set_current_long%^RESET%^
.EI
.SI 5
void set_current_long()
.EI

.SI 3
* %^BOLD%^set_day_long%^RESET%^
.EI
.SI 5
void set_day_long(string str)
.EI

.SI 3
* %^BOLD%^set_night_long%^RESET%^
.EI
.SI 5
void set_night_long(string str)
.EI

.SI 3
* %^BOLD%^setup_shadow%^RESET%^
.EI
.SI 5
void setup_shadow(object this_room)
.EI


