/*  -*- LPC -*-  */
/*
 * $Locker: ceres $
 * $Id: mountain.c,v 1.4 2002/03/26 01:41:57 pinkfish Exp ceres $
 * $Log: mountain.c,v $
 * 
 */
#define CREATOR "Ceres"
/**
 * This is a standard woodland room.  It is a specialisation of /std/outside
 * providing various ready-made add_items for your woodland.
 * <p>
 * room_chats are also provided, if you don't wish to keep them simply use
 * room_chat to write your own or stop_room_chats to remove them. 
 * <p>
 * The items and chats are seasonal adding richness to your woodland setting.
 * @author Ceres
 */
#include <weather.h>
#include <terrain_map.h>

inherit TERRAIN_MAP_OUTSIDE_BASE;

void create() {
  ::create();
  set_climate("H"); // highland.
  add_zone("mountain");
  
  switch((string)WEATHER->query_season()) {
  case "spring":
    add_item("flower", ({"long", "Very occasionally there are beautiful "+
                           "snowdrops nestled in the shadow of a boulder.",
                         "get", ({this_object(), "do_get_flower"}),
                         "pick", ({this_object(), "do_get_flower"}),
                         "smell", "The flowers smell wonderful."
                         }));
    add_item("tree", "There are only a few stunted trees here clinging "+
             "to life on the steep slopes.  New leaves poke cautiously "+
             "from their gnarled branches");
    add_item(({"floor", "ground", "earth"}), "The ground is mostly covered "+