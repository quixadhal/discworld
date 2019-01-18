.DT
water_inside_base.c
Disk World autodoc help
water_inside_base.c

.SH Description
.SP 5 5
 Base inherit for an inside water room using the terrain map system.


This file should be inherited by any inside room within a terrain mapped virtual area, and by any file with an exit to one of said areas. 

When inheriting from this file, it is essential that the function query_handler_path() be overriden to return the full pathname of the handler associated with this area. 

To inherit from this file, use the following lines: 
.EP
.SO 6 2 -10

	*	#include <terrain_map.h> 
	*	
	*	inherit TERRAIN_MAP_OUTSIDE_BASE 
.EO
.SP 5 5

.EP
.SP 10 5


Written by Dek

Started Thurs April 12 2001 Too Damn Early BST

.EP



.SH See also
.SP 5 5
terrain_map_handler, terrain_map_intro and terrain_map_example
.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/water_inside and /std/room/inherit/terrain_map/terrain_room.c.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/terrain_map.h, /include/login_handler.h and /include/room.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_dest_dir%^RESET%^
.EI
.SI 5
mixed * query_dest_dir(object thing)
.EI

.SI 3
* %^BOLD%^query_dest_other%^RESET%^
.EI
.SI 5
varargs mixed * query_dest_other(string exit)
.EI


