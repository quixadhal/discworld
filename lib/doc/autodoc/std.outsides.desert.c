.DT
desert.c
Disk World autodoc help
desert.c

.SH Description
.SP 5 5

This is a standard desert room.  It is a specialisation of /std/outside
providing various ready-made add_items for your desert.

room_chats are also provided, if you don't wish to keep them simply use
room_chat to write your own or stop_room_chats to remove them.
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
This class includes the following files /include/move_failures.h, /include/terrain_map.h and /include/weather.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_get_rock%^RESET%^
.EI
.SI 5
int do_get_rock(object * indirs, string dir, string indir, mixed * args, string pattern)
.EI

.SI 3
* %^BOLD%^do_get_sand%^RESET%^
.EI
.SI 5
int do_get_sand(object * indirs, string dir, string indir, mixed * args, string pattern)
.EI

.SI 3
* %^BOLD%^query_terrain_map_colour%^RESET%^
.EI
.SI 5
string query_terrain_map_colour()
.EI


