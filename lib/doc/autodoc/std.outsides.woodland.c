.DT
woodland.c
Disk World autodoc help
woodland.c

.SH Description
.SP 5 5

This is a standard woodland room.  It is a specialisation of /std/outside
providing various ready-made add_items for your woodland.


room_chats are also provided, if you don't wish to keep them simply use room_chat to write your own or stop_room_chats to remove them. 

The items and chats are seasonal adding richness to your woodland setting.
.EP
.SP 10 5


Written by Ceres

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/terrain_map/outside_base.c.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/terrain_map.h and /include/weather.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_get_berry%^RESET%^
.EI
.SI 5
mixed do_get_berry(string verb, object * dest)
.EI

.SI 3
* %^BOLD%^do_get_flower%^RESET%^
.EI
.SI 5
mixed do_get_flower(string verb, object * dest)
.EI

.SI 3
* %^BOLD%^do_get_leaf%^RESET%^
.EI
.SI 5
mixed do_get_leaf(string verb, object * dest)
.EI

.SI 3
* %^BOLD%^do_get_rock%^RESET%^
.EI
.SI 5
mixed do_get_rock(string verb, object * dest)
.EI

.SI 3
* %^BOLD%^extra_look%^RESET%^
.EI
.SI 5
string extra_look()
.EI

.SI 3
* %^BOLD%^query_terrain_map_colour%^RESET%^
.EI
.SI 5
string query_terrain_map_colour()
.EI


