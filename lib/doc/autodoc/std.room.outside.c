.DT
outside.c
Disk World autodoc help
outside.c

.SH Description
.SP 5 5

This sets the weather handler that the room will use.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/weather.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^cloud_look%^RESET%^
.EI
.SI 5
string cloud_look()
.EI
.SP 7 5

Returns the cloud look for the cloud item.

.EP

.SI 3
* %^BOLD%^query_climate%^RESET%^
.EI
.SI 5
string query_climate()
.EI
.SP 7 5

This method returns the climate type for this room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
climate string
.EP

.SI 3
* %^BOLD%^query_day_light%^RESET%^
.EI
.SI 5
int query_day_light()
.EI
.SP 7 5

This method returns the current level of day light in the room.  The
day light is the light level the room is set to if there was full sun
and no darkness at all.  Mostly a percentage of this is calculated
in the query_light() method.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current daylight level

.EP

.SI 3
* %^BOLD%^query_fast_clean_up%^RESET%^
.EI
.SI 5
int query_fast_clean_up()
.EI
.SP 7 5

This function allows you to check whether or not fast clean-up
is currently allowed.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if it is disabled, 1 if it is allowed

.EP

.SI 3
* %^BOLD%^query_light_change%^RESET%^
.EI
.SI 5
string query_light_change()
.EI

.SI 3
* %^BOLD%^query_visibility%^RESET%^
.EI
.SI 5
int query_visibility()
.EI

.SI 3
* %^BOLD%^query_weather_handler%^RESET%^
.EI
.SI 5
string query_weather_handler()
.EI
.SP 7 5

This returns the path to the weather handler currently being used.

.EP

.SI 3
* %^BOLD%^rain_look%^RESET%^
.EI
.SI 5
string rain_look()
.EI
.SP 7 5

Returns the rain look for the rain item.

.EP

.SI 3
* %^BOLD%^set_climate%^RESET%^
.EI
.SI 5
int set_climate(string climate)
.EI
.SP 7 5

This method sets the climate for this room from the standard
climate types.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
a - climate type string
.EP

.SI 3
* %^BOLD%^set_fast_clean_up%^RESET%^
.EI
.SI 5
void set_fast_clean_up(int number)
.EI
.SP 7 5

This function allows you to disable the fast clean-up of outside
rooms.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - 1 to allow fast clean-up, 0 to disable it

.EP

.SI 3
* %^BOLD%^set_light_change%^RESET%^
.EI
.SI 5
void set_light_change(string s)
.EI

.SI 3
* %^BOLD%^set_weather_handler%^RESET%^
.EI
.SI 5
void set_weather_handler(string handler)
.EI

.SI 3
* %^BOLD%^sleet_look%^RESET%^
.EI
.SI 5
string sleet_look()
.EI
.SP 7 5

Returns the sleet look for the sleet item.

.EP

.SI 3
* %^BOLD%^snow_look%^RESET%^
.EI
.SI 5
string snow_look()
.EI
.SP 7 5

Returns the snow look for the snow item.

.EP


