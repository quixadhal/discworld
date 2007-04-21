/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: inside_base.c,v 1.7 2003/04/10 18:10:22 pinkfish Exp $
 * 
*/
/**
* @main
* Base inherit for an inside room using the terrain map system.
* <p>
* This file should be inherited by any outside room within a terrain mapped
* virtual area, and by any file with an exit to one of said areas.
* <p>
* When inheriting from this file, it is essential that the function
* query_handler_path() be overriden to return the full pathname of
* the handler associated with this area.
* <p>
* To inherit from this file, use the following lines:
* <ul>
* <li>#include <terrain_map.h>
* <li>
* <li>inherit TERRAIN_MAP_INSIDE_BASE
* </ul>
* 
* @index terrain_map_inside
* @see terrain_map_handler
* @see terrain_map_intro
* @see terrain_map_example
* @author Dek
* @started Thurs April 12 2001 Too Damn Early BST
**/

#include <room.h>
#include <login_handler.h>
#include <terrain_map.h>

inherit "/std/room/basic_room";
inherit TERRAIN_MAP_ROOM_BASE;

void create() {
   set_not_replaceable(1);
   terrain_room::create();
   basic_room::create();
}

void init() {
   terrain_room::init();
   basic_room::init();
} /* init() */

/* @ignore yes */
varargs mixed* query_dest_other(string exit) {
   mixed* data;

   data = basic_room::query_dest_other(exit);
   return terrain_room::query_dest_other(exit, data);
}

/* @ignore yes */
mixed* query_dest_dir(object thing) {
   mixed* data;

   data = basic_room::query_dest_dir(thing);
   return terrain_room::query_dest_dir();
}

mixed stats() {
   return basic_room::stats() + terrain_room::stats();
}
