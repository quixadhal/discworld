/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: outside_base.c,v 1.10 2003/04/18 21:08:46 ceres Exp $
 * 
*/
/**
* @main
* Base inherit for an outside room using the terrain map system.
* <p>
* This file should be inherited by any outside room within a terrain mapped
* virtual area.
* <p>
* To inherit from this file, use the following lines:
* <ul>
* <li>#include <terrain_map.h>
* <li>
* <li>inherit TERRAIN_MAP_OUTSIDE_BASE
* </ul>
* 
* @index terrain_map_outside
* @see terrain_map_handler
* @see terrain_map_intro
* @see terrain_map_example
* @author Dek
* @started Thurs April 12 2001 Too Damn Early BST
**/

#include <room.h>
#include <login_handler.h>
#include <terrain_map.h>

inherit "/std/room/outside";
inherit TERRAIN_MAP_ROOM_BASE;

void create() {
   set_not_replaceable(1);
   do_setup++;
   terrain_room::create();
   outside::create();
   do_setup--;

   if ( !do_setup ) {
     this_object()->setup();
     this_object()->reset();
   }
}

void init() {
   terrain_room::init();
   outside::init();
} /* init() */

/** @ignore yes */
varargs mixed* query_dest_other(string exit) {
   mixed* data;

   data = outside::query_dest_other(exit);
   return terrain_room::query_dest_other(exit, data);
}

/** @ignore yes */
mixed* query_dest_dir(object thing) {
   mixed* data;

   data = outside::query_dest_dir(thing);
   return terrain_room::query_dest_dir(data);
}

/** @ignore yes */
mixed stats() {
   return outside::stats() + terrain_room::stats();
}

void dest_me() {
  terrain_room::dest_me();
  outside::dest_me();
}
