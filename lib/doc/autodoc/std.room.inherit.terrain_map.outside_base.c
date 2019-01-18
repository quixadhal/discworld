.DT
outside_base.c
Disk World autodoc help
outside_base.c

.SH Description
.SP 5 5
 Base inherit for an outside room using the terrain map system.


This file should be inherited by any outside room within a terrain mapped virtual area. 

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
This class inherits the following classes /std/room/inherit/terrain_map/terrain_room.c and /std/room/outside.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/terrain_map.h, /include/login_handler.h and /include/room.h.
.EP

