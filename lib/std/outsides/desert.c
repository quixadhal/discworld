/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: desert.c,v 1.9 2003/04/16 23:12:26 ceres Exp $
 * 
*/
#define CREATOR "Ceres"
/**
 * This is a standard desert room.  It is a specialisation of /std/outside
 * providing various ready-made add_items for your desert.
 *
 * room_chats are also provided, if you don't wish to keep them simply use
 * room_chat to write your own or stop_room_chats to remove them.
 * @author Ceres
 */
#include <weather.h>
#include <move_failures.h>
#include <terrain_map.h>

inherit TERRAIN_MAP_OUTSIDE_BASE;

void create() {
  ::create();
  set_climate("Bw"); // arid desert.

  add_item("sand", "There is a lot of sand here, tons of it just lying "
     "about the place.");
  add_item("rock", "There are small rocks here and there in the sand.");
    
  room_chat(({120, 240, ({"Grains of sand are blown about in the slight "
          "breeze."})}));
  add_property("rock size", 2+random(5));
  add_property("track type", "desert");
  add_property("sand object", "/obj/media/sand");
}

string query_terrain_map_colour() { return "%^ORANGE%^"; }

int do_get_rock(object *indirs, string dir, string indir, mixed *args,
    string pattern)
{
  int rock_size;
  string rock_type;
  object rock;
  if ( query_property( "rock object" ) )
    rock = clone_object(query_property("rock object"));
  else {
    if ( undefinedp( rock_size = (int)query_property( "rock size" ) ) )
      rock_size = 3;
    if ( !rock_size ) return 0;
    rock = clone_object( "/obj/mineral" );
    if ( undefinedp( rock_type = (string)query_property( "rock type" ) ) )
      rock_type = ({ "cinnabar", "coal", "rosy-quartz", "chalk", "flint",
                     "slate" })[ random( 6 ) ];
    rock->make_mineral(rock_type, 1 + random(rock_size));
  }
  if (!(rock->move(this_player() ) ) ) return 1;
  rock->dest_me();
  return 0;
}

int do_get_sand(object *indirs, string dir, string indir, mixed *args,
    string pattern)
{
  object sand;
  sand = clone_object(query_property("sand object"));
  if (!sand) return 0;
  if (sand->move(this_player()) == MOVE_OK) {
    this_player()->add_succeeded_mess(sand, "$N $V $D.\n", ({ }) );
    return 1;
  }
  sand->dest_me();
  return 0;
}
