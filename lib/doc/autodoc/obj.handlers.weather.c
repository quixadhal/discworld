.DT
weather.c
Disk World autodoc help
weather.c

.SH Description
.SP 5 5

An attempt at a new weather handler for Discworld.
Ceres@Discworld 14/4/96

Having thought about the way weather works (and Discworlds interesting
meteorology) I decided not to try and implement some model of realworld
weather. Instead regional variations in weather are handled by
regional climate properties. This system will handle diurnal and seasonal
variations and that's about it. Hopefully that will make it more
understandable, usable and efficient.

Basic Operation
.EP
.SP 5 5
---------------
.EP
.SP 5 5
The weather is based on weather 'pattern's. The pattern lasts for several hours and is calculated from the 'seasonal' norm and the 'variance'. The 'current' weather will tend towards the pattern weather.

The pattern depends on the climate. The climates are taken from the Koppen Climate Classification system. The possible climates are:
.EP
.SP 5 5
tropical
.EP
.SP 5 5
- rainforest 'Af' eg. amazon, congo, malaysia
.EP
.SP 5 5
- monsoon 'Am' 
.EP
.SP 5 5
- savanna 'Aw' eg. caribbean, tropical africa, central america
.EP
.SP 5 5
desert
.EP
.SP 5 5
- arid 'Bw'
.EP
.SP 5 5
- semi-arid 'Bs' 
.EP
.SP 5 5
mediterranean (aka Moist Subtropical Mid-Latitude Climates)
.EP
.SP 5 5
- humid subtropical 'Cfa' eg. Souteast US (florida)
.EP
.SP 5 5
- mediterranean 'Cs' eg. california, south africa, western/central australia
.EP
.SP 5 5
- marine 'Cfb' eg. US west coast, nw europe
.EP
.SP 5 5
continental
.EP
.SP 5 5
- dry winters 'Dw' 
.EP
.SP 5 5
- dry summers 'Ds'
.EP
.SP 5 5
- wet 'Df'
.EP
.SP 5 5
polar
.EP
.SP 5 5
- tundra 'Et'
.EP
.SP 5 5
- frozen 'Ef'
.EP
.SP 5 5
highland 'H' 

The default for a room is mediterranean marine (Cfb).

The actual temparture in a room will depend on the current weather for that climate, local modifiers (climate property), diurnal variations and variance.

Thus update_patttern() is called once every hour (CYCLE_SPEED) and update_weather() is called every 5 minutes (UPDATE_SPEED).

Decisions
.EP
.SP 5 5
---------
.EP
.SP 5 5
Calculation of the general weather has been moved into update_weather. Rather than doing it on a room by room basis everytime someone looks! Instead, local climate controls will be added verbatim to the current global weather. This will improve efficiency massively, but will reduce the overall weather variations.

Principles
.EP
.SP 5 5
----------
.EP
.SP 5 5
Rain/sleet/snow depends on cloud density and temperature.
.EP
.SP 5 5
If it's windy and lots of cloud density we might get a thunder storm. Cloud cover should lower temperature as should wind.

Usage
.EP
.SP 5 5
-----
.EP
.SP 5 5
To alter the weather in a given room the easiest approach is to set the climate. Individual room or area variations can be done by adding a climate property.
.EP
.SP 5 5
The property should contain three integers, temperature, cloud cover and wind speed. These modify the default values for the room.

For example:
.EP
.SP 5 5
add_property("climate", ({10, -50, 0});
.EP
.SP 5 5
This will increase the temperature by 10C, reduce the cloud cover by 50% and make no change to the wind speed.

To completely alter the weather in a room you need to define the function room_weather() as follows:
.EP
.SP 5 5
int *room_weather(int temp, int cloud, int wind, int rain);
.EP
.SP 5 5
Your room_weather function will be called with the values the room would ordinarily receive and should return the new temperature, cloud cover, wind speed and rain type values that you desire.

If you need to make your room react to changes in the weather you can use the weather_notify() system to be told when the weather changes.
.EP
.SP 5 5
This should only be done for a small number of rooms or the lag will be excessive.

Changed Jun 3 1996 by Grampa to include weather notifications for rooms.


.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h, /include/climate.h and /include/weather.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_umbrella%^RESET%^
.EI
.SI 5
int check_umbrella(object ob)
.EI

.SI 3
* %^BOLD%^cloud_string%^RESET%^
.EI
.SI 5
string cloud_string(object env)
.EI
.SP 7 5

A string giving a nice description of the cloud types.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the environment to find the clouds for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current clouds

.EP

.SI 3
* %^BOLD%^day_number%^RESET%^
.EI
.SI 5
int day_number()
.EI
.SP 7 5

Find out which day of the Disc (common) year it is.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the day of the year.

.EP

.SI 3
* %^BOLD%^minute_number%^RESET%^
.EI
.SI 5
int minute_number()
.EI
.SP 7 5

Find out which minute of the day (0 - 1439) it is.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the minute.

.EP

.SI 3
* %^BOLD%^query_cloud%^RESET%^
.EI
.SI 5
int query_cloud(object env)
.EI
.SP 7 5

Find out the cloud cover of a given room

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an open ended percentage. Greater than 100 means heavier cover.

.EP

.SI 3
* %^BOLD%^query_darkness%^RESET%^
.EI
.SI 5
int query_darkness(object env)
.EI
.SP 7 5

return percentage of light
should be dependant on the base amount of light, day or night time and
cloud cover.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the - room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a percentage

.EP

.SI 3
* %^BOLD%^query_moon_phase%^RESET%^
.EI
.SI 5
string query_moon_phase()
.EI
.SP 7 5

Find out which phase of the moon we're currently in.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string for the phase of the moon.

.EP

.SI 3
* %^BOLD%^query_moon_state%^RESET%^
.EI
.SI 5
int query_moon_state()
.EI
.SP 7 5

Query the state of the moon.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an integer indicating the state of the moon.
0 - no moon, 1 - crescent, 2 - quarter, 3 - half, 4 - three quarter,
5 - gibbous, 6 - full.

.EP

.SI 3
* %^BOLD%^query_moon_string%^RESET%^
.EI
.SI 5
string query_moon_string(object env)
.EI
.SP 7 5

Get the appropriate moon string. This is almost completely from the old
system. I don't understand the moon.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the room to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the moon string.

.EP

.SI 3
* %^BOLD%^query_raining%^RESET%^
.EI
.SI 5
int query_raining(object env)
.EI
.SP 7 5

Find out if its raining in a given room.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 or 0 for true or false.

.EP

.SI 3
* %^BOLD%^query_season%^RESET%^
.EI
.SI 5
string query_season()
.EI
.SP 7 5

Return which of the 4 seasons we are in.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
spring, summer, autumn or winter.

.EP

.SI 3
* %^BOLD%^query_snowing%^RESET%^
.EI
.SI 5
int query_snowing(object env)
.EI
.SP 7 5

Find out if its snowing in a given room.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 or 0 for true or false.

.EP

.SI 3
* %^BOLD%^query_star_string%^RESET%^
.EI
.SI 5
string query_star_string(object env)
.EI
.SP 7 5

Returns a string describing the current visible stars.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string for the visible stars.

.EP

.SI 3
* %^BOLD%^query_stars%^RESET%^
.EI
.SI 5
int query_stars(object env)
.EI
.SP 7 5

Return the number of currently visible stars as a percentage.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A number 0-100, 0 being no stars.

.EP

.SI 3
* %^BOLD%^query_sunrise%^RESET%^
.EI
.SI 5
int query_sunrise(int doy)
.EI
.SP 7 5

Find out when sunrise will be on a given day of the year.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
doy - Day of the Discworld common year.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
sunrise time (in seconds past midnite).
.EP

.SI 3
* %^BOLD%^query_sunset%^RESET%^
.EI
.SI 5
int query_sunset(int doy)
.EI
.SP 7 5

Find out when sunset will be on a given day of the year.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
doy - Day of the Discworld common year.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
sunset time (in seconds past midnite).

.EP

.SI 3
* %^BOLD%^query_temperature%^RESET%^
.EI
.SI 5
int query_temperature(object env)
.EI
.SP 7 5

Find out the temperature of a given room

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a temperature in Celcius

.EP

.SI 3
* %^BOLD%^query_tod%^RESET%^
.EI
.SI 5
string query_tod()
.EI
.SP 7 5

Return a string for the period of the day

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
night, morning, midday, afternoon or evening.

.EP

.SI 3
* %^BOLD%^query_visibility%^RESET%^
.EI
.SI 5
int query_visibility(object env)
.EI
.SP 7 5

return percentage of visibility.
It should also depend on things like rain/snow etc, anything which
will effect visibility.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the - room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a percentage

.EP

.SI 3
* %^BOLD%^query_windsp%^RESET%^
.EI
.SI 5
int query_windsp(object env)
.EI
.SP 7 5

Find out the wind speed in a given room

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the wind speed in miles per hour.

.EP

.SI 3
* %^BOLD%^rain_string%^RESET%^
.EI
.SI 5
string rain_string(object env)
.EI
.SP 7 5

Creates a string showing how heavily it is raining.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the room room to check in

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the rain string
.EP

.SI 3
* %^BOLD%^sleet_string%^RESET%^
.EI
.SI 5
string sleet_string(object env)
.EI
.SP 7 5

Creates a string showing how heavily it is sleeting.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the room room to check in

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the sleet string
.EP

.SI 3
* %^BOLD%^snow_string%^RESET%^
.EI
.SI 5
string snow_string(object env)
.EI
.SP 7 5

Creates a string showing how heavily it is snowing.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
env - the room room to check in

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the snow string
.EP

.SI 3
* %^BOLD%^temp_string%^RESET%^
.EI
.SI 5
string temp_string(int temp)
.EI
.SP 7 5

Return a temperature string equating to the temperature passed
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
int - The temperature in Celcius
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
string a temperature string.

.EP

.SI 3
* %^BOLD%^weather_notify%^RESET%^
.EI
.SI 5
int weather_notify(object which_room, int notifications)
.EI
.SP 7 5

Add a room to the list of rooms to notify about weather.  Notifications
are an ORed (|) combination of the #defines in /include/climate.h
IE: To inform about temperature and rain changes, call
weather_notify( room, (NOTIFY_TEMPERATURE | NOTIFY_RAIN) );

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
which_room - the room to be notified
.EP
.SP 9 5
notifications - a bitmap of notifications
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
success or failure (1 or 0)

.EP

.SI 3
* %^BOLD%^weather_string%^RESET%^
.EI
.SI 5
varargs string weather_string(object env, string obscured)
.EI
.SP 7 5

Creates the weather string which is put into room descs.

.EP


