/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mountain.c,v 1.4 2002/03/26 01:41:57 pinkfish Exp $
 * $Log: mountain.c,v $
 * Revision 1.4  2002/03/26 01:41:57  pinkfish
 * Give them all basic inherit stuff so they can work with terrain rooms.
 *
 * Revision 1.3  2000/06/22 01:19:30  pinkfish
 * Make it use autodoc and fix up the inherit paths.
 *
 * Revision 1.2  1999/08/16 23:34:00  rywfol
 * Fixed pluralisation of flowers.
 * Rywfol 990817
 *
 * Revision 1.1  1998/01/06 04:29:46  ceres
 * Initial revision
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
  add_property("climate", ({-10, 0, 0 }) );
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
             "by hardy grass interspersed by small shrubs, however there "+
             "are large patches of bare rock or gravel.  "+
             "Here and there larger rocks protrude from the ground.");
    
    room_chat(({120, 240, ({"A bird of prey swoops overhead.",
                            "A small insect flies past your face."})}));
    add_property("flower type", "snowdrop");
    break;
  case "summer":
  case "autumn":
    add_item(({"floor", "ground", "earth"}), "The ground is largely "+
             "covered with a hardy grass interspersed with small shrubs, "+
             "however there are large patches of bare rock and gravel.  Here "+
             "and there larger rocks protrude from the ground.");
    add_item("tree", "There are a few small trees clinging to life in this "+
             "harsh environment.");
    room_chat(({120, 240, ({"The wind whistles through the mountain passes.",
                            "A bird of prey swoops overhead.",
                            "An insect brushes against your face."})}));
    break;
  case "winter":
    add_item(({"floor", "ground", "earth"}), "The ground is bare earth "+
             "apart from the occasional hardy shrub.");
    add_item("tree", "There are a few trees dotted around their bare "+
             "branches making them look like ghouhlish skeletons.");
    room_chat(({120, 240, ({"The cold wind whistles over the mountains."
                            })}));
    break;
  }
  add_item("rock", ({ "long", "A nice rock looking thingy.",
                      "get",  ({ this_object(), "do_get_rock" }) }) );
  add_item("shrub", "Dotted here and there these hardy plants struggle "+
           "to survive on the thin soil.");
  add_item("grass", "A particularly wiry kind of grass obviously suited "+
           "to the harsh conditions and thins soil.");
  add_property("rock size", 5+random(20));
  add_property("track type", "mountain");
}

/** @ignore yres */
mixed do_get_rock(string verb, object *dest ) {
  int rock_size;
  string rock_type;
  object rock;

  if ( query_property( "rock object" ) )
    return ( rock = clone_object( (string)query_property( "rock object" ) ) );
  if ( undefinedp( rock_size = (int)query_property( "rock size" ) ) )
    rock_size = 3;
  if ( !rock_size ) return 0;
  rock = clone_object( "/obj/mineral" );
  if ( undefinedp( rock_type = (string)query_property( "rock type" ) ) )
    rock_type = ({ "cinnabar", "coal", "rosy-quartz", "chalk", "flint",
                   "slate" })[ random( 6 ) ];
  rock->make_mineral(rock_type, 1 + random(rock_size));
  if (!(rock->move(this_player() ) ) ) return 1;
  rock->dest_me();
  return 0;
}

/** @ignore yres */
mixed do_get_flower( string verb, object *dest ) {
  string flower_type;
  object flower;

  printf("Verb: %s\n", verb);
  if ( query_property( "flower object" ) )
    return (flower = clone_object((string)query_property("flower object")));
  if ((int)query_property("noflowers"))
    return 0;
  flower = clone_object("/std/plant");
  
  if(undefinedp(flower_type = (string)query_property("flower type")))
    flower_type = ({"forgetmenot", "violet"})[ random(2) ];
  flower->set_plant(flower_type);
  flower->set_plant_type(flower_type);
  flower->set_plant_desc("A beautiful "+flower_type+" picked before its "+
                         "time.\n");
  flower->add_plural( capitalize( flower_type ) );
  
  if (!(flower->move(this_player()))) return 1;
  flower->dest_me();
  return 0;
}
