/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: water_outside_base.c,v 1.9 2003/07/14 21:56:10 pinkfish Exp $
 * 
*/
/**
* @main
* Base inherit for an outside water room using the terrain map system.
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

inherit "/std/room/water_outside";
inherit TERRAIN_MAP_ROOM_BASE;

void create() {
   set_not_replaceable(1);
   terrain_room::create();
   water_outside::create();
}

void init() {
   terrain_room::init();
   water_outside::init();
} /* init() */

/* @ignore yes */
varargs mixed* query_dest_other(string exit) {
   mixed* data;

   data = water_outside::query_dest_other(exit);
   return terrain_room::query_dest_other(exit, data);
}

/* @ignore yes */
mixed* query_dest_dir(object thing) {
   mixed* data;

   data = water_outside::query_dest_dir(thing);
   return terrain_room::query_dest_dir(data);
}

/** @ignore yes */
mixed stats() {
   return water_outside::stats() + terrain_room::stats();
}

/** @ignore yes */
string query_terrain_map_walk_exit() {
   return "swim ";
}

/** @ignore yes */
object query_surface_room() {
  string dest_path;
  object destination;
  if (query_surface()) {
    return this_object();
  }
  dest_path = file_name(this_object());
  dest_path = implode(explode(dest_path, ":")[0..<2], ":")+":0";
  destination = load_object(dest_path);
  if (destination && destination->query_water()) {
    return destination->query_surface_room();
  }
  return ::query_surface_room();
}

/** @ignore yes */
int get_water_surface_light() {
  float their_light, their_clarity, our_depth;
  string above_path = file_name(this_object());
  object above;
  above_path = implode(explode(above_path, ":")[0..<2], ":")+":0";
  above = load_object(above_path);
  if (!above) {
    return 0;
  }
  if (function_exists("query_water_surface_light", above)) {
    their_light = to_float(above->query_water_surface_light());
  } else {
    their_light = to_float(above->query_my_light());
  }
  their_clarity = to_float(above->query_clarity())/100.0;
  our_depth = to_float(-query_terrain_coords()[2]);
  return to_int(their_light * pow(their_clarity, our_depth));
}

/**
 * Make it so that anyone attempting to enter a water room from a non-water
 * room has to specifically wade into it.
 * @ignore yes
 */
string query_enter_exit_direction(string key, string key_here, string direct,
                                  object room, string base_path) {
   if (!room->query_water()) {
      return "wade " + direct;
   }
   return 0;
}

/** @ignore yes */
int can_move_between(string my_type, string direc,
                     string first_type, string second_type, string new_base_path)

{
   return first_type->query_water() && second_type->query_water() &&
          new_base_path->query_water() ||
          (!new_base_path->query_water() &&
           (!first_type->query_water() || !second_type->query_water()));
}
