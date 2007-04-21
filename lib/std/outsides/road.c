/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: woodland.c,v 1.8 2003/04/16 23:12:26 ceres Exp $
*/
#define CREATOR "Ceres"
/**
 * This is a standard road. It hasn't been setup yet.
 */
#include <weather.h>
#include <terrain_map.h>

inherit TERRAIN_MAP_OUTSIDE_BASE;

void create() {
  do_setup++;
  ::create();
  do_setup--;
  add_property("track type", "road");
  add_zone("road");
  
  if(!do_setup) {
    this_object()->setup();
    this_object()->reset();
  }
}

string query_terrain_map_colour() { return "%^RED%^"; }
string query_terrain_map_character() { return "#"; }
